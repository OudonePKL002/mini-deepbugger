#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "p17_json_tokenizer.h"

int main(void) {
    const char json[] = "{\"name\": \"MiniBugger\", \"version\": 2, \"active\": true}";
    size_t len = strlen(json);

    p17_tokenizer_t tokz;
    p17_f01_tok_init(&tokz, json, len);

    p17_token_t tok;
    assert(p17_f06_next_token(&tokz, &tok) == P17_TOK_START_OBJ);
    assert(p17_f06_next_token(&tokz, &tok) == P17_TOK_STRING);
    assert(p17_f06_next_token(&tokz, &tok) == P17_TOK_COLON);
    assert(p17_f06_next_token(&tokz, &tok) == P17_TOK_STRING);

    char val_buf[32];
    assert(p17_f13_extract_string_value(&tok, val_buf, 32) == 10);
    assert(strcmp(val_buf, "MiniBugger") == 0);

    assert(p17_f09_validate_brackets(json, len));

    char min_buf[64];
    size_t min_len = p17_f10_minify_json(json, len, min_buf, 64);
    assert(min_len < len);

    p17_token_t vtok;
    assert(p17_f12_find_key_in_object(json, len, "version", &vtok));
    assert(vtok.type == P17_TOK_NUMBER);
    assert(p17_f15_parse_integer(&vtok) == 2);

    p17_f16_tok_reset(&tokz);
    assert(tokz.pos == 0);

    printf("PASS: p17_json_tokenizer unit tests passed.\n");
    return 0;
}
