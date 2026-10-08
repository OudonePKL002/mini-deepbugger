#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "p03_string_token.h"

int main(void) {
    char s1[64] = "  \t hello world \n ";
    assert(p03_f01_str_trim(s1) == 11);
    assert(strcmp(s1, "hello world") == 0);

    char tokens[4][32];
    size_t count = p03_f02_str_split_delim("apple:banana:cherry", ':', tokens, 4);
    assert(count == 3);
    assert(strcmp(tokens[0], "apple") == 0);

    char joined[64];
    p03_f03_str_join(tokens, count, '-', joined, sizeof(joined));
    assert(strcmp(joined, "apple-banana-cherry") == 0);

    assert(p03_f04_str_starts_with("prefix_test", "prefix") == true);
    assert(p03_f05_str_ends_with("test_suffix", "suffix") == true);

    char rep[32] = "a_b_c_d";
    assert(p03_f06_str_replace_char(rep, '_', '-') == 3);
    assert(strcmp(rep, "a-b-c-d") == 0);

    assert(p03_f07_str_count_substr("bananana", "na") == 3);

    char case_s[32] = "Hello-123";
    p03_f08_str_to_lower_ascii(case_s);
    assert(strcmp(case_s, "hello-123") == 0);
    p03_f09_str_to_upper_ascii(case_s);
    assert(strcmp(case_s, "HELLO-123") == 0);

    p03_f10_str_reverse(case_s);
    assert(strcmp(case_s, "321-OLLEH") == 0);

    assert(p03_f11_str_levenshtein("kitten", "sitting") == 3);

    char esc[32];
    p03_f12_str_escape_c("a\nb", esc, sizeof(esc));
    assert(strcmp(esc, "a\\nb") == 0);

    char unesc[32];
    p03_f13_str_unescape_c(esc, unesc, sizeof(unesc));
    assert(strcmp(unesc, "a\nb") == 0);

    int32_t val = 0;
    assert(p03_f14_str_parse_int("-12345", &val) == true && val == -12345);

    uint8_t raw[3] = {0x12, 0xAB, 0xEF};
    char hex_buf[16];
    p03_f15_str_format_hex(raw, 3, hex_buf, sizeof(hex_buf));
    assert(strcmp(hex_buf, "12abef") == 0);

    uint8_t parsed[4];
    assert(p03_f16_str_parse_hex("12abef", parsed, 4) == 3);
    assert(parsed[0] == 0x12 && parsed[1] == 0xAB && parsed[2] == 0xEF);

    printf("PASS: p03_string_token unit tests passed.\n");
    return 0;
}
