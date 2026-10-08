#ifndef P17_JSON_TOKENIZER_H
#define P17_JSON_TOKENIZER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P17_NOINLINE __attribute__((noinline))

typedef enum {
    P17_TOK_INVALID = 0,
    P17_TOK_START_OBJ,   // {
    P17_TOK_END_OBJ,     // }
    P17_TOK_START_ARR,   // [
    P17_TOK_END_ARR,     // ]
    P17_TOK_COLON,       // :
    P17_TOK_COMMA,       // ,
    P17_TOK_STRING,      // "str"
    P17_TOK_NUMBER,      // 123.45
    P17_TOK_TRUE,        // true
    P17_TOK_FALSE,       // false
    P17_TOK_NULL,        // null
    P17_TOK_EOF
} p17_token_type_t;

typedef struct {
    p17_token_type_t type;
    const char *start;
    size_t length;
} p17_token_t;

typedef struct {
    const char *src;
    size_t pos;
    size_t len;
} p17_tokenizer_t;

P17_NOINLINE void             p17_f01_tok_init(p17_tokenizer_t *t, const char *src, size_t len);
P17_NOINLINE void             p17_f02_skip_whitespace(p17_tokenizer_t *t);
P17_NOINLINE bool             p17_f03_parse_string(p17_tokenizer_t *t, p17_token_t *tok);
P17_NOINLINE bool             p17_f04_parse_number(p17_tokenizer_t *t, p17_token_t *tok);
P17_NOINLINE bool             p17_f05_parse_literal(p17_tokenizer_t *t, const char *lit, p17_token_type_t type, p17_token_t *tok);
P17_NOINLINE p17_token_type_t p17_f06_next_token(p17_tokenizer_t *t, p17_token_t *tok);
P17_NOINLINE p17_token_type_t p17_f07_peek_token(p17_tokenizer_t *t, p17_token_t *tok);
P17_NOINLINE bool             p17_f08_consume_expected(p17_tokenizer_t *t, p17_token_type_t expected);
P17_NOINLINE bool             p17_f09_validate_brackets(const char *json, size_t len);
P17_NOINLINE size_t           p17_f10_minify_json(const char *src, size_t len, char *dst, size_t dst_cap);
P17_NOINLINE size_t           p17_f11_count_tokens_by_type(p17_tokenizer_t *t, p17_token_type_t target_type);
P17_NOINLINE bool             p17_f12_find_key_in_object(const char *json, size_t len, const char *key, p17_token_t *val_tok);
P17_NOINLINE size_t           p17_f13_extract_string_value(const p17_token_t *tok, char *dst, size_t dst_cap);
P17_NOINLINE bool             p17_f14_is_valid_number(const char *s, size_t len);
P17_NOINLINE int64_t          p17_f15_parse_integer(const p17_token_t *tok);
P17_NOINLINE void             p17_f16_tok_reset(p17_tokenizer_t *t);

#endif
