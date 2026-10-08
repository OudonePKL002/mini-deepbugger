#include "donor_component.h"
#include <string.h>
#include <math.h>

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

P03_NOINLINE size_t p03_f12_str_escape_c(const char *src, char *dst, size_t dst_sz) {
    if (!src || !dst || dst_sz == 0) return 0;
    size_t out = 0;
    for (size_t i = 0; src[i] && out + 2 < dst_sz; ++i) {
        char c = src[i];
        if (c == '\n') { dst[out++] = '\\'; dst[out++] = 'n'; }
        else if (c == '\t') { dst[out++] = '\\'; dst[out++] = 't'; }
        else if (c == '\\') { dst[out++] = '\\'; dst[out++] = '\\'; }
        else { dst[out++] = c; }
    }
    dst[out] = '\0';
    return out;
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
