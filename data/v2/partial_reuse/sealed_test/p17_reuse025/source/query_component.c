#include "query_component.h"
#include <string.h>
#include <math.h>

P17_NOINLINE void p17_f01_tok_init(p17_tokenizer_t *t, const char *src, size_t len) {
    if (!t) return;
    t->src = src;
    t->len = len;
    t->pos = 0;
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
