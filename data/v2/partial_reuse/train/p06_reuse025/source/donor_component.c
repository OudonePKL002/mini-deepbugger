#include "donor_component.h"
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

P03_NOINLINE size_t p03_f11_str_levenshtein(const char *s1, const char *s2) {
    if (!s1 || !s2) return 0;
    size_t len1 = 0, len2 = 0;
    while (s1[len1]) len1++;
    while (s2[len2]) len2++;
    if (len1 > 32) len1 = 32;
    if (len2 > 32) len2 = 32;
    size_t dp[33][33];
    for (size_t i = 0; i <= len1; ++i) dp[i][0] = i;
    for (size_t j = 0; j <= len2; ++j) dp[0][j] = j;
    for (size_t i = 1; i <= len1; ++i) {
        for (size_t j = 1; j <= len2; ++j) {
            size_t cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            size_t m1 = dp[i - 1][j] + 1;
            size_t m2 = dp[i][j - 1] + 1;
            size_t m3 = dp[i - 1][j - 1] + cost;
            size_t min_v = m1 < m2 ? m1 : m2;
            dp[i][j] = min_v < m3 ? min_v : m3;
        }
    }
    return dp[len1][len2];
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

P03_NOINLINE size_t p03_f13_str_unescape_c(const char *src, char *dst, size_t dst_sz) {
    if (!src || !dst || dst_sz == 0) return 0;
    size_t out = 0;
    for (size_t i = 0; src[i] && out + 1 < dst_sz; ++i) {
        if (src[i] == '\\' && src[i + 1]) {
            i++;
            if (src[i] == 'n') dst[out++] = '\n';
            else if (src[i] == 't') dst[out++] = '\t';
            else dst[out++] = src[i];
        } else {
            dst[out++] = src[i];
        }
    }
    dst[out] = '\0';
    return out;
}

P03_NOINLINE bool p03_f14_str_parse_int(const char *s, int32_t *out_val) {
    if (!s || !s[0]) return false;
    int sign = 1;
    size_t i = 0;
    if (s[0] == '-') { sign = -1; i++; }
    else if (s[0] == '+') { i++; }
    if (!s[i]) return false;
    int64_t acc = 0;
    while (s[i]) {
        if (s[i] < '0' || s[i] > '9') return false;
        acc = acc * 10 + (s[i] - '0');
        if (acc > 2147483647LL && sign == 1) return false;
        if (acc > 2147483648LL && sign == -1) return false;
        i++;
    }
    if (out_val) *out_val = (int32_t)(acc * sign);
    return true;
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
