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

P03_NOINLINE void p03_f08_str_to_lower_ascii(char *s) {
    if (!s) return;
    for (size_t i = 0; s[i]; ++i) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = (char)(s[i] + ('a' - 'A'));
        }
    }
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
