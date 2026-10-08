#include "query_component.h"
#include <string.h>
#include <math.h>

P17_NOINLINE void p17_f01_tok_init(p17_tokenizer_t *t, const char *src, size_t len) {
    if (!t) return;
    t->src = src;
    t->len = len;
    t->pos = 0;
}

P17_NOINLINE void p17_f02_skip_whitespace(p17_tokenizer_t *t) {
    if (!t) return;
    while (t->pos < t->len) {
        char c = t->src[t->pos];
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            t->pos++;
        } else {
            break;
        }
    }
}

P17_NOINLINE bool p17_f03_parse_string(p17_tokenizer_t *t, p17_token_t *tok) {
    if (!t || t->pos >= t->len || t->src[t->pos] != '"') return false;
    size_t start = t->pos;
    t->pos++; // skip opening quote
    bool escape = false;
    while (t->pos < t->len) {
        char c = t->src[t->pos++];
        if (escape) {
            escape = false;
        } else if (c == '\\') {
            escape = true;
        } else if (c == '"') {
            if (tok) {
                tok->type = P17_TOK_STRING;
                tok->start = &t->src[start];
                tok->length = t->pos - start;
            }
            return true;
        }
    }
    return false;
}

P17_NOINLINE bool p17_f04_parse_number(p17_tokenizer_t *t, p17_token_t *tok) {
    if (!t || t->pos >= t->len) return false;
    size_t start = t->pos;
    char c = t->src[t->pos];
    if (c == '-') t->pos++;
    while (t->pos < t->len && (t->src[t->pos] >= '0' && t->src[t->pos] <= '9')) {
        t->pos++;
    }
    if (t->pos < t->len && t->src[t->pos] == '.') {
        t->pos++;
        while (t->pos < t->len && (t->src[t->pos] >= '0' && t->src[t->pos] <= '9')) {
            t->pos++;
        }
    }
    if (t->pos == start || (t->pos == start + 1 && t->src[start] == '-')) return false;
    if (tok) {
        tok->type = P17_TOK_NUMBER;
        tok->start = &t->src[start];
        tok->length = t->pos - start;
    }
    return true;
}

P17_NOINLINE bool p17_f05_parse_literal(p17_tokenizer_t *t, const char *lit, p17_token_type_t type, p17_token_t *tok) {
    if (!t || !lit) return false;
    size_t lit_len = strlen(lit);
    if (t->pos + lit_len > t->len) return false;
    if (strncmp(&t->src[t->pos], lit, lit_len) == 0) {
        if (tok) {
            tok->type = type;
            tok->start = &t->src[t->pos];
            tok->length = lit_len;
        }
        t->pos += lit_len;
        return true;
    }
    return false;
}

P17_NOINLINE p17_token_type_t p17_f06_next_token(p17_tokenizer_t *t, p17_token_t *tok) {
    if (!t) return P17_TOK_INVALID;
    p17_f02_skip_whitespace(t);
    if (t->pos >= t->len) {
        if (tok) { tok->type = P17_TOK_EOF; tok->start = NULL; tok->length = 0; }
        return P17_TOK_EOF;
    }
    char c = t->src[t->pos];
    size_t cur = t->pos;

    if (c == '{') { t->pos++; if (tok) { tok->type = P17_TOK_START_OBJ; tok->start = &t->src[cur]; tok->length = 1; } return P17_TOK_START_OBJ; }
    if (c == '}') { t->pos++; if (tok) { tok->type = P17_TOK_END_OBJ;   tok->start = &t->src[cur]; tok->length = 1; } return P17_TOK_END_OBJ; }
    if (c == '[') { t->pos++; if (tok) { tok->type = P17_TOK_START_ARR; tok->start = &t->src[cur]; tok->length = 1; } return P17_TOK_START_ARR; }
    if (c == ']') { t->pos++; if (tok) { tok->type = P17_TOK_END_ARR;   tok->start = &t->src[cur]; tok->length = 1; } return P17_TOK_END_ARR; }
    if (c == ':') { t->pos++; if (tok) { tok->type = P17_TOK_COLON;     tok->start = &t->src[cur]; tok->length = 1; } return P17_TOK_COLON; }
    if (c == ',') { t->pos++; if (tok) { tok->type = P17_TOK_COMMA;     tok->start = &t->src[cur]; tok->length = 1; } return P17_TOK_COMMA; }

    if (c == '"') {
        if (p17_f03_parse_string(t, tok)) return P17_TOK_STRING;
        return P17_TOK_INVALID;
    }
    if ((c >= '0' && c <= '9') || c == '-') {
        if (p17_f04_parse_number(t, tok)) return P17_TOK_NUMBER;
        return P17_TOK_INVALID;
    }
    if (p17_f05_parse_literal(t, "true", P17_TOK_TRUE, tok)) return P17_TOK_TRUE;
    if (p17_f05_parse_literal(t, "false", P17_TOK_FALSE, tok)) return P17_TOK_FALSE;
    if (p17_f05_parse_literal(t, "null", P17_TOK_NULL, tok)) return P17_TOK_NULL;

    t->pos++;
    if (tok) { tok->type = P17_TOK_INVALID; tok->start = &t->src[cur]; tok->length = 1; }
    return P17_TOK_INVALID;
}

P17_NOINLINE bool p17_f08_consume_expected(p17_tokenizer_t *t, p17_token_type_t expected) {
    p17_token_t tok;
    return p17_f06_next_token(t, &tok) == expected;
}

P17_NOINLINE bool p17_f09_validate_brackets(const char *json, size_t len) {
    if (!json) return false;
    char stack[128];
    size_t top = 0;
    bool in_str = false;

    for (size_t i = 0; i < len; ++i) {
        char c = json[i];
        if (in_str) {
            if (c == '\\') i++;
            else if (c == '"') in_str = false;
            continue;
        }
        if (c == '"') in_str = true;
        else if (c == '{' || c == '[') {
            if (top >= 128) return false;
            stack[top++] = c;
        } else if (c == '}') {
            if (top == 0 || stack[--top] != '{') return false;
        } else if (c == ']') {
            if (top == 0 || stack[--top] != '[') return false;
        }
    }
    return top == 0 && !in_str;
}

P17_NOINLINE size_t p17_f10_minify_json(const char *src, size_t len, char *dst, size_t dst_cap) {
    if (!src || !dst) return 0;
    size_t out = 0;
    bool in_str = false;

    for (size_t i = 0; i < len; ++i) {
        char c = src[i];
        if (in_str) {
            if (out < dst_cap) dst[out++] = c;
            if (c == '\\') {
                if (++i < len && out < dst_cap) dst[out++] = src[i];
            } else if (c == '"') {
                in_str = false;
            }
        } else {
            if (c == '"') {
                in_str = true;
                if (out < dst_cap) dst[out++] = c;
            } else if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
                if (out < dst_cap) dst[out++] = c;
            }
        }
    }
    return out;
}

P17_NOINLINE bool p17_f12_find_key_in_object(const char *json, size_t len, const char *key, p17_token_t *val_tok) {
    if (!json || !key) return false;
    p17_tokenizer_t tokz;
    p17_f01_tok_init(&tokz, json, len);
    p17_token_t tok;

    while (p17_f06_next_token(&tokz, &tok) != P17_TOK_EOF) {
        if (tok.type == P17_TOK_STRING && tok.length >= 2) {
            if (strncmp(tok.start + 1, key, tok.length - 2) == 0 && key[tok.length - 2] == '\0') {
                p17_token_t col;
                if (p17_f06_next_token(&tokz, &col) == P17_TOK_COLON) {
                    if (p17_f06_next_token(&tokz, val_tok) != P17_TOK_EOF) return true;
                }
            }
        }
    }
    return false;
}

P17_NOINLINE bool p17_f14_is_valid_number(const char *s, size_t len) {
    if (!s || len == 0) return false;
    size_t i = 0;
    if (s[i] == '-') i++;
    if (i >= len) return false;
    while (i < len && (s[i] >= '0' && s[i] <= '9')) i++;
    if (i < len && s[i] == '.') {
        i++;
        while (i < len && (s[i] >= '0' && s[i] <= '9')) i++;
    }
    return i == len;
}

P17_NOINLINE void p17_f16_tok_reset(p17_tokenizer_t *t) {
    if (!t) return;
    t->pos = 0;
}
