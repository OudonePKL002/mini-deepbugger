#include "query_component.h"
#include <string.h>
#include <math.h>

P03_NOINLINE size_t p03_f01_str_trim(char *s) {
    if (!s) return 0;
    size_t len = 0;
    while (s[len]) len++;
    if (len == 0) return 0;
    size_t start = 0;
    while (s[start] == ' ' || s[start] == '\t' || s[start] == '\n' || s[start] == '\r') {
        start++;
    }
    if (start == len) {
        s[0] = '\0';
        return 0;
    }
    size_t end = len - 1;
    while (end > start && (s[end] == ' ' || s[end] == '\t' || s[end] == '\n' || s[end] == '\r')) {
        end--;
    }
    size_t new_len = end - start + 1;
    for (size_t i = 0; i < new_len; ++i) {
        s[i] = s[start + i];
    }
    s[new_len] = '\0';
    return new_len;
}

P03_NOINLINE size_t p03_f02_str_split_delim(const char *s, char delim, char tokens[][32], size_t max_tok) {
    if (!s || max_tok == 0) return 0;
    size_t count = 0;
    size_t idx = 0;
    size_t tok_pos = 0;
    while (s[idx] && count < max_tok) {
        if (s[idx] == delim) {
            tokens[count][tok_pos] = '\0';
            count++;
            tok_pos = 0;
        } else {
            if (tok_pos < 31) {
                tokens[count][tok_pos++] = s[idx];
            }
        }
        idx++;
    }
    if (count < max_tok) {
        tokens[count][tok_pos] = '\0';
        count++;
    }
    return count;
}

P03_NOINLINE size_t p03_f03_str_join(const char tokens[][32], size_t count, char sep, char *out_buf, size_t out_sz) {
    if (!out_buf || out_sz == 0) return 0;
    out_buf[0] = '\0';
    size_t out_pos = 0;
    for (size_t i = 0; i < count; ++i) {
        size_t j = 0;
        while (tokens[i][j] && out_pos + 1 < out_sz) {
            out_buf[out_pos++] = tokens[i][j++];
        }
        if (i + 1 < count && out_pos + 1 < out_sz) {
            out_buf[out_pos++] = sep;
        }
    }
    out_buf[out_pos] = '\0';
    return out_pos;
}

P03_NOINLINE bool p03_f04_str_starts_with(const char *str, const char *prefix) {
    if (!str || !prefix) return false;
    size_t i = 0;
    while (prefix[i]) {
        if (str[i] != prefix[i]) return false;
        i++;
    }
    return true;
}

P03_NOINLINE bool p03_f05_str_ends_with(const char *str, const char *suffix) {
    if (!str || !suffix) return false;
    size_t slen = 0, pfxlen = 0;
    while (str[slen]) slen++;
    while (suffix[pfxlen]) pfxlen++;
    if (pfxlen > slen) return false;
    size_t start = slen - pfxlen;
    for (size_t i = 0; i < pfxlen; ++i) {
        if (str[start + i] != suffix[i]) return false;
    }
    return true;
}

P03_NOINLINE size_t p03_f06_str_replace_char(char *s, char old_c, char new_c) {
    if (!s) return 0;
    size_t count = 0;
    for (size_t i = 0; s[i]; ++i) {
        if (s[i] == old_c) {
            s[i] = new_c;
            count++;
        }
    }
    return count;
}

P03_NOINLINE size_t p03_f07_str_count_substr(const char *haystack, const char *needle) {
    if (!haystack || !needle || !needle[0]) return 0;
    size_t count = 0;
    size_t nlen = 0;
    while (needle[nlen]) nlen++;
    for (size_t i = 0; haystack[i]; ++i) {
        size_t j = 0;
        while (needle[j] && haystack[i + j] == needle[j]) {
            j++;
        }
        if (j == nlen) {
            count++;
            i += (nlen - 1);
        }
    }
    return count;
}

P03_NOINLINE void p03_f08_str_to_lower_ascii(char *s) {
    if (!s) return;
    for (size_t i = 0; s[i]; ++i) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = (char)(s[i] + ('a' - 'A'));
        }
    }
}

P03_NOINLINE void p03_f09_str_to_upper_ascii(char *s) {
    if (!s) return;
    for (size_t i = 0; s[i]; ++i) {
        if (s[i] >= 'a' && s[i] <= 'z') {
            s[i] = (char)(s[i] - ('a' - 'A'));
        }
    }
}

P03_NOINLINE void p03_f10_str_reverse(char *s) {
    if (!s) return;
    size_t len = 0;
    while (s[len]) len++;
    if (len <= 1) return;
    size_t i = 0;
    size_t j = len - 1;
    while (i < j) {
        char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        i++;
        j--;
    }
}

P03_NOINLINE size_t p03_f15_str_format_hex(const uint8_t *bytes, size_t len, char *out_buf, size_t buf_sz) {
    static const char hex[] = "0123456789abcdef";
    if (!bytes || !out_buf || buf_sz < 2 * len + 1) return 0;
    size_t pos = 0;
    for (size_t i = 0; i < len; ++i) {
        out_buf[pos++] = hex[(bytes[i] >> 4) & 0x0F];
        out_buf[pos++] = hex[bytes[i] & 0x0F];
    }
    out_buf[pos] = '\0';
    return pos;
}

P03_NOINLINE size_t p03_f16_str_parse_hex(const char *hex_str, uint8_t *out_bytes, size_t max_bytes) {
    if (!hex_str || !out_bytes) return 0;
    size_t len = 0;
    while (hex_str[len]) len++;
    if (len % 2 != 0) return 0;
    size_t out_len = len / 2;
    if (out_len > max_bytes) out_len = max_bytes;
    for (size_t i = 0; i < out_len; ++i) {
        char hi = hex_str[2 * i];
        char lo = hex_str[2 * i + 1];
        uint8_t h_val = (hi >= '0' && hi <= '9') ? (uint8_t)(hi - '0') :
                        (hi >= 'a' && hi <= 'f') ? (uint8_t)(hi - 'a' + 10) :
                        (hi >= 'A' && hi <= 'F') ? (uint8_t)(hi - 'A' + 10) : 0xFF;
        uint8_t l_val = (lo >= '0' && lo <= '9') ? (uint8_t)(lo - '0') :
                        (lo >= 'a' && lo <= 'f') ? (uint8_t)(lo - 'a' + 10) :
                        (lo >= 'A' && lo <= 'F') ? (uint8_t)(lo - 'A' + 10) : 0xFF;
        if (h_val == 0xFF || l_val == 0xFF) return 0;
        out_bytes[i] = (uint8_t)((h_val << 4) | l_val);
    }
    return out_len;
}
