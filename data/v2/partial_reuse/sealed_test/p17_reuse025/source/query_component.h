#ifndef TARGET_QUERY_COMPONENT_H
#define TARGET_QUERY_COMPONENT_H

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

/* Benchmark Function Prototypes */
P17_NOINLINE void             p17_f01_tok_init(p17_tokenizer_t *t, const char *src, size_t len);
P17_NOINLINE bool             p17_f03_parse_string(p17_tokenizer_t *t, p17_token_t *tok);
P17_NOINLINE bool             p17_f04_parse_number(p17_tokenizer_t *t, p17_token_t *tok);
P17_NOINLINE bool             p17_f09_validate_brackets(const char *json, size_t len);

#endif /* TARGET_QUERY_COMPONENT_H */
