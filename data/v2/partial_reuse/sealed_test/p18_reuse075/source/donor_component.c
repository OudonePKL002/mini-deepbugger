#include "donor_component.h"
#include <string.h>
#include <math.h>

P17_NOINLINE void p17_f01_tok_init(p17_tokenizer_t *t, const char *src, size_t len) {
    if (!t) return;
    t->src = src;
    t->len = len;
    t->pos = 0;
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

P17_NOINLINE size_t p17_f13_extract_string_value(const p17_token_t *tok, char *dst, size_t dst_cap) {
    if (!tok || !dst || tok->type != P17_TOK_STRING || tok->length < 2) return 0;
    size_t copy_len = tok->length - 2;
    if (copy_len >= dst_cap) copy_len = dst_cap - 1;
    memcpy(dst, tok->start + 1, copy_len);
    dst[copy_len] = '\0';
    return copy_len;
}

P17_NOINLINE int64_t p17_f15_parse_integer(const p17_token_t *tok) {
    if (!tok || tok->type != P17_TOK_NUMBER || tok->length == 0) return 0;
    int64_t val = 0;
    bool neg = false;
    size_t i = 0;
    if (tok->start[i] == '-') { neg = true; i++; }
    for (; i < tok->length && tok->start[i] >= '0' && tok->start[i] <= '9'; ++i) {
        val = val * 10 + (tok->start[i] - '0');
    }
    return neg ? -val : val;
}
