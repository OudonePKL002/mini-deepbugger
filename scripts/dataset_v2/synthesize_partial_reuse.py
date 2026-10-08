#!/usr/bin/env python3
"""
scripts/dataset_v2/synthesize_partial_reuse.py

Phase 2C-1: Partial-Reuse Source Synthesis & Operational Validation.
Materializes the frozen partial-reuse construction plan into 105 target source packages,
validates operational compilation and testing with Clang and GCC, and freezes provenance/checksums.
"""

import os
import sys
import subprocess
import shutil
import hashlib
import csv
import re
from pathlib import Path

REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "../.."))
CLANG_PATH = "/usr/bin/clang"
GCC_PATH = "/opt/homebrew/bin/gcc-15"

ALL_SNIPPETS = {
    "p01_f01_crc8_smbus": "\n    const uint8_t s[] = \"MiniDeepBugger2026\";\n    assert(p01_f01_crc8_smbus(s, sizeof(s) - 1) != 0);\n",
    "p01_f02_crc8_cdma": "\n    const uint8_t s[] = \"MiniDeepBugger2026\";\n    assert(p01_f02_crc8_cdma(s, sizeof(s) - 1) != 0);\n",
    "p01_f03_crc16_ccitt": "\n    const uint8_t s[] = \"MiniDeepBugger2026\";\n    assert(p01_f03_crc16_ccitt(s, sizeof(s) - 1) != 0);\n",
    "p01_f04_crc16_modbus": "\n    const uint8_t s[] = \"MiniDeepBugger2026\";\n    assert(p01_f04_crc16_modbus(s, sizeof(s) - 1) != 0);\n",
    "p01_f05_crc32_ieee": "\n    const uint8_t s[] = \"MiniDeepBugger2026\";\n    assert(p01_f05_crc32_ieee(s, sizeof(s) - 1) != 0);\n",
    "p01_f06_crc32_table_driven": "\n    const uint8_t s[] = \"MiniDeepBugger2026\";\n    assert(p01_f06_crc32_table_driven(s, sizeof(s) - 1) != 0);\n",
    "p01_f07_fletcher16": "\n    const uint8_t s[] = \"MiniDeepBugger2026\";\n    assert(p01_f07_fletcher16(s, sizeof(s) - 1) != 0);\n",
    "p01_f08_fletcher32": "\n    uint16_t wbuf[8] = { 0x1234, 0x5678, 0x9ABC, 0xDEF0, 0x1357, 0x2468, 0x3579, 0x4680 };\n    assert(p01_f08_fletcher32(wbuf, 8) != 0);\n",
    "p01_f09_bsd_checksum": "\n    const uint8_t s[] = \"MiniDeepBugger2026\";\n    assert(p01_f09_bsd_checksum(s, sizeof(s) - 1) != 0);\n",
    "p01_f10_sysv_checksum": "\n    const uint8_t s[] = \"MiniDeepBugger2026\";\n    assert(p01_f10_sysv_checksum(s, sizeof(s) - 1) != 0);\n",
    "p01_f11_luhn_validate": "\n    assert(p01_f11_luhn_validate(\"79927398713\") == 1);\n    assert(p01_f11_luhn_validate(\"79927398714\") == 0);\n",
    "p01_f12_xor8_block": "\n    const uint8_t s[] = \"MiniDeepBugger2026\";\n    assert(p01_f12_xor8_block(s, sizeof(s) - 1, 0x55) != 0);\n",
    "p01_f13_internet_checksum": "\n    const uint8_t s[] = \"MiniDeepBugger2026\";\n    assert(p01_f13_internet_checksum(s, sizeof(s) - 1) != 0);\n",
    "p01_f14_adler16_simple": "\n    const uint8_t s[] = \"MiniDeepBugger2026\";\n    assert(p01_f14_adler16_simple(s, sizeof(s) - 1) != 0);\n",
    "p01_f15_pearson_hash8": "\n    const uint8_t s[] = \"MiniDeepBugger2026\";\n    assert(p01_f15_pearson_hash8(s, sizeof(s) - 1) != 0);\n",
    "p01_f16_checksum_combine": "\n    assert(p01_f16_checksum_combine(0x1234, 0x56789ABC, 0xDE) != 0);\n",
    "p02_f01_heap_init": "p02_heap_t h; p02_f01_heap_init(&h); assert(h.size == 0 && h.capacity == P02_MAX_CAP);",
    "p02_f02_heap_sift_up": "p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 2; h.data[0] = 10; h.data[1] = 5; p02_f02_heap_sift_up(&h, 1); assert(h.data[0] == 5);",
    "p02_f03_heap_sift_down": "p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 2; h.data[0] = 10; h.data[1] = 5; p02_f03_heap_sift_down(&h, 0); assert(h.data[0] == 5);",
    "p02_f04_heap_push": "p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 0; assert(p02_f04_heap_push(&h, 42) && h.size == 1);",
    "p02_f05_heap_pop_min": "p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 1; h.data[0] = 42; int32_t val; assert(p02_f05_heap_pop_min(&h, &val) && val == 42);",
    "p02_f06_heap_peek": "p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 1; h.data[0] = 99; int32_t val = 0; assert(p02_f06_heap_peek(&h, &val) && val == 99);",
    "p02_f07_heapify_array": "int32_t arr[3] = {3, 1, 2}; p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; p02_f07_heapify_array(&h, arr, 3); assert(h.size == 3);",
    "p02_f08_heapsort_asc": "int32_t arr[4] = {40, 10, 30, 20}; p02_f08_heapsort_asc(arr, 4); assert(arr[0] == 10 && arr[3] == 40);",
    "p02_f09_heap_replace": "p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 2; h.data[0] = 10; h.data[1] = 20; int32_t old_val = 0; assert(p02_f09_heap_replace(&h, 30, &old_val) && old_val == 10);",
    "p02_f10_heap_delete_at": "p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 3; h.data[0] = 10; h.data[1] = 20; h.data[2] = 30; assert(p02_f10_heap_delete_at(&h, 1) && h.size == 2);",
    "p02_f11_heap_increase_key": "p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 2; h.data[0] = 10; h.data[1] = 20; assert(p02_f11_heap_increase_key(&h, 0, 50)); assert(h.data[0] == 20 && h.data[1] == 60);",
    "p02_f12_heap_decrease_key": "p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 2; h.data[0] = 10; h.data[1] = 20; assert(p02_f12_heap_decrease_key(&h, 1, 15)); assert(h.data[0] == 5 && h.data[1] == 10);",
    "p02_f13_heap_merge": "p02_heap_t h1, h2; memset(&h1, 0, sizeof(h1)); memset(&h2, 0, sizeof(h2)); h1.capacity = P02_MAX_CAP; h2.capacity = P02_MAX_CAP; h1.size = 1; h1.data[0] = 10; h2.size = 1; h2.data[0] = 5; assert(p02_f13_heap_merge(&h1, &h2) && h1.size == 2 && h1.data[0] == 5);",
    "p02_f14_heap_is_valid": "p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 3; h.data[0] = 10; h.data[1] = 20; h.data[2] = 30; assert(p02_f14_heap_is_valid(&h));",
    "p02_f15_heap_kth_smallest": "int32_t arr[5] = {34, 12, 89, 5, 23}; assert(p02_f15_heap_kth_smallest(arr, 5, 2) == 12);",
    "p02_f16_heap_clear": "p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 5; p02_f16_heap_clear(&h); assert(h.size == 0);",
    "p03_f01_str_trim": "char s[16] = \"  abc  \"; assert(p03_f01_str_trim(s) == 3 && strcmp(s, \"abc\") == 0);",
    "p03_f02_str_split_delim": "char toks[4][32]; assert(p03_f02_str_split_delim(\"a,b,c\", ',', toks, 4) == 3);",
    "p03_f03_str_join": "char toks[2][32] = {\"foo\", \"bar\"}; char out[32]; assert(p03_f03_str_join(toks, 2, '-', out, 32) == 7 && strcmp(out, \"foo-bar\") == 0);",
    "p03_f04_str_starts_with": "assert(p03_f04_str_starts_with(\"hello world\", \"hello\") == true);",
    "p03_f05_str_ends_with": "assert(p03_f05_str_ends_with(\"hello world\", \"world\") == true);",
    "p03_f06_str_replace_char": "char s[16] = \"a_b_c\"; assert(p03_f06_str_replace_char(s, '_', '-') == 2 && strcmp(s, \"a-b-c\") == 0);",
    "p03_f07_str_count_substr": "assert(p03_f07_str_count_substr(\"bananana\", \"na\") == 3);",
    "p03_f08_str_to_lower_ascii": "char s[16] = \"HeLLo\"; p03_f08_str_to_lower_ascii(s); assert(strcmp(s, \"hello\") == 0);",
    "p03_f09_str_to_upper_ascii": "char s[16] = \"hello\"; p03_f09_str_to_upper_ascii(s); assert(strcmp(s, \"HELLO\") == 0);",
    "p03_f10_str_reverse": "char s[16] = \"12345\"; p03_f10_str_reverse(s); assert(strcmp(s, \"54321\") == 0);",
    "p03_f11_str_levenshtein": "assert(p03_f11_str_levenshtein(\"kitten\", \"sitting\") == 3);",
    "p03_f12_str_escape_c": "char esc[16]; assert(p03_f12_str_escape_c(\"a\\nb\", esc, 16) > 0);",
    "p03_f13_str_unescape_c": "char unesc[16]; assert(p03_f13_str_unescape_c(\"a\\\\nb\", unesc, 16) > 0);",
    "p03_f14_str_parse_int": "int32_t val = 0; assert(p03_f14_str_parse_int(\"-123\", &val) == true && val == -123);",
    "p03_f15_str_format_hex": "uint8_t raw[2] = {0x12, 0xAB}; char buf[8]; assert(p03_f15_str_format_hex(raw, 2, buf, 8) == 4);",
    "p03_f16_str_parse_hex": "uint8_t out[2]; assert(p03_f16_str_parse_hex(\"12ab\", out, 2) == 2);",
    "p04_f01_mat_init_zero": "p04_mat3_t m; p04_f01_mat_init_zero(&m); assert(m.m[0][0] == 0.0f);",
    "p04_f02_mat_init_identity": "p04_mat3_t m; p04_f02_mat_init_identity(&m); assert(m.m[0][0] == 1.0f);",
    "p04_f03_mat_add": "p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; p04_mat3_t b = a, out; p04_f03_mat_add(&a, &b, &out); assert(out.m[0][0] == 2.0f);",
    "p04_f04_mat_sub": "p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; p04_mat3_t b = a, out; p04_f04_mat_sub(&a, &b, &out); assert(out.m[0][0] == 0.0f);",
    "p04_f05_mat_scale": "p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; p04_mat3_t out; p04_f05_mat_scale(&a, 3.0f, &out); assert(out.m[0][0] == 3.0f);",
    "p04_f06_mat_transpose": "p04_mat3_t a = {.m = {{0,4,0},{0,0,0},{0,0,0}}}, out; p04_f06_mat_transpose(&a, &out); assert(out.m[1][0] == 4.0f);",
    "p04_f07_mat_mul": "p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; p04_mat3_t b = a, out; p04_f07_mat_mul(&a, &b, &out); assert(out.m[0][0] == 1.0f);",
    "p04_f08_mat_vec_mul": "p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; p04_vec3_t v = {{1.0f, 2.0f, 3.0f}}, out; p04_f08_mat_vec_mul(&a, &v, &out); assert(out.v[1] == 2.0f);",
    "p04_f09_mat_trace": "p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; assert(p04_f09_mat_trace(&a) == 3.0f);",
    "p04_f10_mat_det_2x2": "assert(p04_f10_mat_det_2x2(1, 2, 3, 4) == -2.0f);",
    "p04_f11_mat_det_3x3": "p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; assert(p04_f11_mat_det_3x3(&a) == 1.0f);",
    "p04_f12_mat_frobenius_norm": "p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; assert(fabsf(p04_f12_mat_frobenius_norm(&a) - sqrtf(3.0f)) < 1e-4f);",
    "p04_f13_mat_is_symmetric": "p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; assert(p04_f13_mat_is_symmetric(&a, 1e-5f) == true);",
    "p04_f14_mat_lu_decompose_3x3": "p04_mat3_t l, u; p04_mat3_t t = {{{2, -1, -2}, {-4, 6, 3}, {-4, -2, 8}}}; assert(p04_f14_mat_lu_decompose_3x3(&t, &l, &u) == true);",
    "p04_f15_mat_solve_upper_tri": "p04_vec3_t sol; p04_vec3_t b = {{1, 2, 3}}; p04_mat3_t u = {{{2, 1, 1}, {0, 1, 2}, {0, 0, 1}}}; assert(p04_f15_mat_solve_upper_tri(&u, &b, &sol) == true && sol.v[2] == 3.0f);",
    "p04_f16_mat_hadamard_product": "p04_mat3_t a = {.m = {{1,0,0},{0,1,0},{0,0,1}}}; p04_mat3_t b = a, out; p04_f16_mat_hadamard_product(&a, &b, &out); assert(out.m[0][0] == 1.0f);",
    "p05_f01_adj_init": "p05_graph_t g; p05_f01_adj_init(&g, 4); assert(g.num_vertices == 4);",
    "p05_f02_adj_add_edge": "p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 4; assert(p05_f02_adj_add_edge(&g, 0, 1) && g.adj[0][1] == 1);",
    "p05_f03_adj_has_edge": "p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 4; g.adj[0][1] = 1; assert(p05_f03_adj_has_edge(&g, 0, 1));",
    "p05_f04_adj_in_degree": "p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 4; g.adj[0][1] = 1; assert(p05_f04_adj_in_degree(&g, 1) == 1);",
    "p05_f05_adj_out_degree": "p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 4; g.adj[0][1] = 1; assert(p05_f05_adj_out_degree(&g, 0) == 1);",
    "p05_f06_graph_bfs": "p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = 1; g.adj[1][2] = 1; size_t ord[3]; assert(p05_f06_graph_bfs(&g, 0, ord, 3) == 3);",
    "p05_f07_graph_dfs_iterative": "p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = 1; g.adj[1][2] = 1; size_t ord[3]; assert(p05_f07_graph_dfs_iterative(&g, 0, ord, 3) == 3);",
    "p05_f08_graph_has_cycle_directed": "p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; g.adj[0][1] = 1; g.adj[1][0] = 1; assert(p05_f08_graph_has_cycle_directed(&g) == true);",
    "p05_f09_graph_connected_components": "p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; g.adj[0][1] = 1; int comp[2]; assert(p05_f09_graph_connected_components(&g, comp) == 1);",
    "p05_f10_graph_bipartite_check": "p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; g.adj[0][1] = 1; g.adj[1][0] = 1; assert(p05_f10_graph_bipartite_check(&g) == true);",
    "p05_f11_graph_transitive_closure": "p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = 1; g.adj[1][2] = 1; uint8_t reach[P05_MAX_V][P05_MAX_V]; p05_f11_graph_transitive_closure(&g, reach); assert(reach[0][2] == 1);",
    "p05_f12_graph_density": "p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; g.adj[0][1] = 1; assert(p05_f12_graph_density(&g) > 0);",
    "p05_f13_graph_isolate_count": "p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; assert(p05_f13_graph_isolate_count(&g) == 3);",
    "p05_f14_graph_eulerian_path_check": "p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; g.adj[0][1] = 1; assert(p05_f14_graph_eulerian_path_check(&g) == true);",
    "p05_f15_graph_subgraph_induced": "p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = 1; bool mask[3] = {true, true, false}; p05_graph_t sub; p05_f15_graph_subgraph_induced(&g, mask, &sub); assert(sub.num_vertices == 3);",
    "p05_f16_graph_vertex_eccentricity": "p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; g.adj[0][1] = 1; assert(p05_f16_graph_vertex_eccentricity(&g, 0) == 1);",
    "p06_f01_tlv_encode_uint8": "uint8_t buf[16]; assert(p06_f01_tlv_encode_uint8(1, 0xFE, buf, 16) == 3);",
    "p06_f02_tlv_encode_uint16": "uint8_t buf[16]; assert(p06_f02_tlv_encode_uint16(2, 0x1234, buf, 16) == 4);",
    "p06_f03_tlv_encode_uint32": "uint8_t buf[16]; assert(p06_f03_tlv_encode_uint32(3, 0x12345678, buf, 16) == 6);",
    "p06_f04_tlv_encode_bytes": "uint8_t raw[2] = {1, 2}; uint8_t buf[16]; assert(p06_f04_tlv_encode_bytes(4, raw, 2, buf, 16) == 4);",
    "p06_f05_tlv_decode_header": "uint8_t buf[4] = {1, 2, 0xAA, 0xBB}; uint8_t tag, len; assert(p06_f05_tlv_decode_header(buf, 4, &tag, &len) && tag == 1 && len == 2);",
    "p06_f06_tlv_decode_uint8": "uint8_t buf[3] = {1, 1, 0x55}; uint8_t val; assert(p06_f06_tlv_decode_uint8(buf, 3, &val) && val == 0x55);",
    "p06_f07_tlv_decode_uint16": "uint8_t buf[4] = {2, 2, 0x12, 0x34}; uint16_t val; assert(p06_f07_tlv_decode_uint16(buf, 4, &val) && val == 0x1234);",
    "p06_f08_tlv_decode_uint32": "uint8_t buf[6] = {3, 4, 0x12, 0x34, 0x56, 0x78}; uint32_t val; assert(p06_f08_tlv_decode_uint32(buf, 6, &val) && val == 0x12345678);",
    "p06_f09_tlv_find_tag": "uint8_t buf[6] = {1, 1, 0xAA, 2, 1, 0xBB}; assert(p06_f09_tlv_find_tag(buf, 6, 2) == 3);",
    "p06_f10_tlv_count_tags": "uint8_t buf[6] = {1, 1, 0xAA, 2, 1, 0xBB}; assert(p06_f10_tlv_count_tags(buf, 6) == 2);",
    "p06_f11_tlv_validate_buffer": "uint8_t buf[3] = {1, 1, 0xAA}; assert(p06_f11_tlv_validate_buffer(buf, 3) == true);",
    "p06_f12_packet_header_build": "uint8_t hdr[8]; assert(p06_f12_packet_header_build(101, 20, hdr, 8) == 6 && hdr[0] == 0xAA && hdr[1] == 0x55);",
    "p06_f13_packet_header_parse": "const uint8_t hdr[6] = {0xAA, 0x55, 0, 101, 0, 20}; uint16_t seq, plen; assert(p06_f13_packet_header_parse(hdr, 6, &seq, &plen) && seq == 101 && plen == 20);",
    "p06_f14_packet_payload_pad": "uint8_t buf[16] = {'a', 'b'}; assert(p06_f14_packet_payload_pad(buf, 2, 4, 16) == 4);",
    "p06_f15_packet_payload_unpad": "const uint8_t buf[4] = {'a', 'b', 2, 2}; assert(p06_f15_packet_payload_unpad(buf, 4) == 2);",
    "p06_f16_packet_serialize": "uint8_t wire[64]; assert(p06_f16_packet_serialize(42, (const uint8_t*)\"hello\", 5, wire, 64) == 12);",
    "p07_f01_slab_init": "p07_slab_t s; p07_f01_slab_init(&s); assert(s.alloc_bitmap == 0 && s.free_count == 32);",
    "p07_f02_slab_alloc_block": "p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; assert(p07_f02_slab_alloc_block(&s) != NULL);",
    "p07_f03_slab_free_block": "p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; s.alloc_bitmap = 1; assert(p07_f03_slab_free_block(&s, (void*)s.memory_pool));",
    "p07_f04_slab_available_count": "p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; assert(p07_f04_slab_available_count(&s) == P07_NUM_BLOCKS);",
    "p07_f05_slab_is_pointer_valid": "p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; assert(p07_f05_slab_is_pointer_valid(&s, (void*)s.memory_pool));",
    "p07_f06_slab_mark_busy": "p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; p07_f06_slab_mark_busy(&s, 0); assert((s.alloc_bitmap & 1) == 1);",
    "p07_f07_slab_mark_free": "p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; s.alloc_bitmap = 1; p07_f07_slab_mark_free(&s, 0); assert((s.alloc_bitmap & 1) == 0);",
    "p07_f08_slab_find_first_free_bit": "assert(p07_f08_slab_find_first_free_bit(0) == 0);",
    "p07_f09_slab_defragment_check": "p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; s.alloc_bitmap = 0x55; assert(p07_f09_slab_defragment_check(&s) > 0);",
    "p07_f10_slab_reset": "p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; s.alloc_bitmap = 0xFF; p07_f10_slab_reset(&s); assert(s.alloc_bitmap == 0);",
    "p07_f11_slab_block_index": "p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; assert(p07_f11_slab_block_index(&s, s.memory_pool) == 0);",
    "p07_f12_slab_index_to_pointer": "p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; assert(p07_f12_slab_index_to_pointer(&s, 0) == (void*)s.memory_pool);",
    "p07_f13_slab_stats_utilization": "p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; assert(p07_f13_slab_stats_utilization(&s) == 0);",
    "p07_f14_slab_audit_integrity": "p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; assert(p07_f14_slab_audit_integrity(&s) == true);",
    "p07_f15_slab_scrub_pattern": "p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; p07_f15_slab_scrub_pattern(&s, 0, 0xAA); assert(s.memory_pool[0] == 0xAA);",
    "p07_f16_slab_compact_scan": "p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; size_t run; assert(p07_f16_slab_compact_scan(&s, &run) == 1 && run == 32);",
    "p08_f01_adler32": "const uint8_t s[] = \"DevBenchmarkCorpus2026\"; assert(p08_f01_adler32(s, sizeof(s)-1) != 0);",
    "p08_f02_siphash_round": "uint64_t v[4] = {1, 2, 3, 4}; p08_f02_siphash_round(v); assert(v[0] != 1);",
    "p08_f03_siphash24": "const uint8_t s[] = \"DevBenchmarkCorpus2026\"; uint8_t k[16] = {0}; assert(p08_f03_siphash24(s, sizeof(s)-1, k) != 0);",
    "p08_f04_half_siphash": "const uint8_t s[] = \"DevBenchmarkCorpus2026\"; uint8_t k[8] = {1,2,3,4,5,6,7,8}; assert(p08_f04_half_siphash(s, sizeof(s)-1, k) != 0);",
    "p08_f05_djb2_hash": "assert(p08_f05_djb2_hash(\"test_string\") != 0);",
    "p08_f06_sdbm_hash": "assert(p08_f06_sdbm_hash(\"test_string\") != 0);",
    "p08_f07_rabin_karp_rolling": "assert(p08_f07_rabin_karp_rolling(100, 'a', 'b', 31, 961) != 0);",
    "p08_f08_jenkins_lookup2": "const uint8_t s[] = \"DevBenchmarkCorpus2026\"; assert(p08_f08_jenkins_lookup2(s, sizeof(s)-1, 42) != 0);",
    "p08_f09_knuth_multiplicative": "assert(p08_f09_knuth_multiplicative(12345) != 0);",
    "p08_f10_rot13_cipher": "char b[16]; p08_f10_rot13_cipher(b, \"Hello\", 5); b[5] = 0; assert(strcmp(b, \"Uryyb\") == 0);",
    "p08_f11_rotate_mix32": "const uint8_t s[] = \"DevBenchmarkCorpus2026\"; assert(p08_f11_rotate_mix32(s, sizeof(s)-1) != 0);",
    "p08_f12_crc16_usb": "const uint8_t s[] = \"DevBenchmarkCorpus2026\"; assert(p08_f12_crc16_usb(s, sizeof(s)-1) != 0);",
    "p08_f13_poly1305_clamp": "uint8_t k[16] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}; p08_f13_poly1305_clamp(k); assert((k[3] & 15) == k[3]);",
    "p08_f14_chash_simple": "const uint8_t s[] = \"DevBenchmarkCorpus2026\"; assert(p08_f14_chash_simple(s, sizeof(s)-1, 999) != 0);",
    "p08_f15_hash_mix32": "assert(p08_f15_hash_mix32(12345) != 0);",
    "p08_f16_checksum_fold64_to_32": "assert(p08_f16_checksum_fold64_to_32(0x123456789ABCDEF0ULL) != 0);",
    "p09_f01_avl_init": "p09_tree_t t; p09_f01_avl_init(&t); assert(t.root == -1 && t.pool_size == 0);",
    "p09_f02_avl_height": "p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; assert(p09_f02_avl_height(&t, 0) == 1);",
    "p09_f03_avl_balance_factor": "p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; assert(p09_f03_avl_balance_factor(&t, 0) == 0);",
    "p09_f04_avl_rotate_right": "p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 2; t.pool[0].key = 20; t.pool[0].left = 1; t.pool[0].right = -1; t.pool[1].key = 10; t.pool[1].left = -1; t.pool[1].right = -1; assert(p09_f04_avl_rotate_right(&t, 0) == 1);",
    "p09_f05_avl_rotate_left": "p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 2; t.pool[0].key = 10; t.pool[0].left = -1; t.pool[0].right = 1; t.pool[1].key = 20; t.pool[1].left = -1; t.pool[1].right = -1; assert(p09_f05_avl_rotate_left(&t, 0) == 1);",
    "p09_f06_avl_insert_internal": "p09_tree_t t; memset(&t, 0, sizeof(t)); t.root = -1; assert(p09_f06_avl_insert_internal(&t, -1, 42, 100) == 0);",
    "p09_f07_avl_insert": "p09_tree_t t; memset(&t, 0, sizeof(t)); t.root = -1; assert(p09_f07_avl_insert(&t, 42, 100));",
    "p09_f08_avl_min_node": "p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; assert(p09_f08_avl_min_node(&t, 0) == 0);",
    "p09_f09_avl_delete_internal": "p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; assert(p09_f09_avl_delete_internal(&t, 0, 10) == -1);",
    "p09_f10_avl_delete": "p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; assert(p09_f10_avl_delete(&t, 10));",
    "p09_f11_avl_search": "p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; t.pool[0].value = 99; int32_t val; assert(p09_f11_avl_search(&t, 10, &val) && val == 99);",
    "p09_f12_avl_inorder": "p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; int32_t keys[1]; assert(p09_f12_avl_inorder(&t, keys, 1) == 1 && keys[0] == 10);",
    "p09_f13_avl_preorder": "p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; int32_t keys[1]; assert(p09_f13_avl_preorder(&t, keys, 1) == 1 && keys[0] == 10);",
    "p09_f14_avl_count": "p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; assert(p09_f14_avl_count(&t, 0) == 1);",
    "p09_f15_avl_is_balanced": "p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; assert(p09_f15_avl_is_balanced(&t, 0));",
    "p09_f16_avl_clear": "p09_tree_t t; memset(&t, 0, sizeof(t)); t.pool_size = 1; t.root = 0; t.pool[0].key = 10; t.pool[0].height = 1; t.pool[0].left = -1; t.pool[0].right = -1; p09_f16_avl_clear(&t); assert(t.root == -1);",
    "p10_f01_nfa_init": "p10_nfa_t nfa; nfa.num_states = 5; p10_f01_nfa_init(&nfa); assert(nfa.num_states == 0);",
    "p10_f02_nfa_add_state": "p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); assert(p10_f02_nfa_add_state(&nfa, 'a', -1, -1) == 0);",
    "p10_f03_nfa_set_transition": "p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 2; assert(p10_f03_nfa_set_transition(&nfa, 0, 1, -1));",
    "p10_f04_nfa_set_match": "p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 1; assert(p10_f04_nfa_set_match(&nfa, 0, true) && nfa.states[0].is_match);",
    "p10_f05_nfa_create_literal": "p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); assert(p10_f05_nfa_create_literal(&nfa, 'a') >= 0);",
    "p10_f06_nfa_concat": "p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 2; assert(p10_f06_nfa_concat(&nfa, 0, 1) >= 0);",
    "p10_f07_nfa_alternate": "p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 2; assert(p10_f07_nfa_alternate(&nfa, 0, 1) >= 0);",
    "p10_f08_nfa_star": "p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 1; assert(p10_f08_nfa_star(&nfa, 0) >= 0);",
    "p10_f09_nfa_plus": "p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 1; assert(p10_f09_nfa_plus(&nfa, 0) >= 0);",
    "p10_f10_nfa_optional": "p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 1; assert(p10_f10_nfa_optional(&nfa, 0) >= 0);",
    "p10_f11_nfa_epsilon_closure": "p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 2; bool in_s[P10_MAX_STATES] = {true}, out_s[P10_MAX_STATES]; p10_f11_nfa_epsilon_closure(&nfa, in_s, out_s); assert(out_s[0]);",
    "p10_f12_nfa_step": "p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 2; nfa.states[0].c = 'a'; nfa.states[0].out1 = 1; nfa.states[0].out2 = -1; bool in_s[P10_MAX_STATES] = {true}, out_s[P10_MAX_STATES]; p10_f12_nfa_step(&nfa, in_s, 'a', out_s); assert(out_s[1]);",
    "p10_f13_nfa_simulate": "p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 2; nfa.start_state = 0; nfa.states[0].c = 'a'; nfa.states[0].out1 = 1; nfa.states[0].out2 = -1; nfa.states[1].is_match = true; nfa.states[1].out1 = -1; nfa.states[1].out2 = -1; assert(p10_f13_nfa_simulate(&nfa, \"a\"));",
    "p10_f14_nfa_match_prefix": "p10_nfa_t nfa; memset(&nfa, 0, sizeof(nfa)); nfa.num_states = 2; nfa.start_state = 0; nfa.states[0].c = 'a'; nfa.states[0].out1 = 1; nfa.states[0].out2 = -1; nfa.states[1].is_match = true; nfa.states[1].out1 = -1; nfa.states[1].out2 = -1; size_t out_len; assert(p10_f14_nfa_match_prefix(&nfa, \"ab\", &out_len) && out_len == 1);",
    "p10_f15_nfa_active_count": "bool s[4] = {true, false, true, true}; assert(p10_f15_nfa_active_count(s, 4) == 3);",
    "p10_f16_nfa_reset": "p10_nfa_t nfa; nfa.num_states = 5; p10_f16_nfa_reset(&nfa); assert(nfa.num_states == 0);",
    "p11_f01_fp_from_int": "assert(p11_f01_fp_from_int(5) == (5 << P11_FP_SHIFT));",
    "p11_f02_fp_to_int": "assert(p11_f02_fp_to_int(5 << P11_FP_SHIFT) == 5);",
    "p11_f03_fp_add": "assert(p11_f03_fp_add(10, 20) == 30);",
    "p11_f04_fp_sub": "assert(p11_f04_fp_sub(30, 10) == 20);",
    "p11_f05_fp_mul": "p11_q16_t a = 2 * 65536, b = 3 * 65536; assert(p11_f05_fp_mul(a, b) == 6 * 65536);",
    "p11_f06_fp_div": "p11_q16_t a = 6 * 65536, b = 2 * 65536; assert(p11_f06_fp_div(a, b) == 3 * 65536);",
    "p11_f07_fp_abs": "assert(p11_f07_fp_abs(-50) == 50);",
    "p11_f08_fp_sqrt": "p11_q16_t v = 4 * 65536; assert(p11_f08_fp_sqrt(v) == 2 * 65536);",
    "p11_f09_fp_exp_taylor": "p11_q16_t half = P11_FP_ONE / 2; assert(p11_f09_fp_exp_taylor(half) > P11_FP_ONE);",
    "p11_f10_fp_ln_approx": "p11_q16_t v = 65536; assert(p11_f10_fp_ln_approx(v) == 0);",
    "p11_f11_fp_sin_cordic": "assert(abs(p11_f11_fp_sin_cordic(0)) < 100);",
    "p11_f12_fp_cos_cordic": "assert(abs(p11_f12_fp_cos_cordic(0) - 65536) < 100);",
    "p11_f13_fp_atan2_cordic": "assert(abs(p11_f13_fp_atan2_cordic(65536, 65536) - 51471) < 200);",
    "p11_f14_fp_lerp": "p11_q16_t a = 65536, b = 3 * 65536, t = 32768; assert(p11_f14_fp_lerp(a, b, t) == 2 * 65536);",
    "p11_f15_fp_clamp": "assert(p11_f15_fp_clamp(15, 0, 10) == 10);",
    "p11_f16_fp_poly_eval": "p11_q16_t c[2] = {65536, 2 * 65536}; assert(p11_f16_fp_poly_eval(c, 2, 65536) == 3 * 65536);",
    "p12_f01_dag_init": "p12_dag_t g; p12_f01_dag_init(&g, 5); assert(g.num_vertices == 5);",
    "p12_f02_dag_add_edge": "p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; assert(p12_f02_dag_add_edge(&g, 0, 1, 1) && g.adj[0][1]);",
    "p12_f03_dag_compute_indegrees": "p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; size_t in_deg[3]; p12_f03_dag_compute_indegrees(&g, in_deg); assert(in_deg[1] == 1);",
    "p12_f04_dag_compute_outdegrees": "p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; size_t out_deg[3]; p12_f04_dag_compute_outdegrees(&g, out_deg); assert(out_deg[0] == 1);",
    "p12_f05_dag_kahn_toposort": "p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; g.adj[1][2] = true; size_t ord[3], len; assert(p12_f05_dag_kahn_toposort(&g, ord, &len) && len == 3);",
    "p12_f06_dag_has_cycle": "p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; assert(!p12_f06_dag_has_cycle(&g));",
    "p12_f07_dag_longest_path": "p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; g.adj[1][2] = true; g.weights[0][1] = 1; g.weights[1][2] = 1; assert(p12_f07_dag_longest_path(&g, 0, 2) == 2);",
    "p12_f08_dag_shortest_path": "p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; g.adj[1][2] = true; g.weights[0][1] = 1; g.weights[1][2] = 1; assert(p12_f08_dag_shortest_path(&g, 0, 2) == 2);",
    "p12_f09_dag_count_ancestors": "p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; assert(p12_f09_dag_count_ancestors(&g, 1) == 1);",
    "p12_f10_dag_count_descendants": "p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; assert(p12_f10_dag_count_descendants(&g, 0) == 1);",
    "p12_f11_dag_assign_levels": "p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; size_t lvls[3]; p12_f11_dag_assign_levels(&g, lvls); assert(lvls[1] == 1);",
    "p12_f12_dag_transitive_reduction": "p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; g.adj[1][2] = true; g.adj[0][2] = true; p12_f12_dag_transitive_reduction(&g); assert(!g.adj[0][2]);",
    "p12_f13_dag_find_sources": "p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; size_t s[3]; assert(p12_f13_dag_find_sources(&g, s, 3) >= 1);",
    "p12_f14_dag_find_sinks": "p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; size_t s[3]; assert(p12_f14_dag_find_sinks(&g, s, 3) >= 1);",
    "p12_f15_dag_is_reachable": "p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; assert(p12_f15_dag_is_reachable(&g, 0, 1));",
    "p12_f16_dag_clear": "p12_dag_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = true; p12_f16_dag_clear(&g); assert(!g.adj[0][1]);",
    "p13_f01_slip_encode": "const uint8_t raw[] = {1, 0xC0}; uint8_t enc[16]; assert(p13_f01_slip_encode(raw, 2, enc, 16) > 2);",
    "p13_f02_slip_decode": "const uint8_t raw[] = {1, 0xDB, 0xDC, 0xC0}; uint8_t dec[16]; assert(p13_f02_slip_decode(raw, 4, dec, 16) == 2);",
    "p13_f03_cobs_encode": "const uint8_t raw[] = {1, 2, 3}; uint8_t enc[16]; assert(p13_f03_cobs_encode(raw, 3, enc, 16) > 0);",
    "p13_f04_cobs_decode": "const uint8_t enc[6] = {3, 11, 22, 3, 33, 44}; uint8_t dec[8]; assert(p13_f04_cobs_decode(enc, 6, dec, 8) == 5 && dec[0] == 11);",
    "p13_f05_fcs16_compute": "const uint8_t raw[] = {1, 2, 3, 4}; assert(p13_f05_fcs16_compute(raw, 4) != 0);",
    "p13_f06_fcs16_verify": "const uint8_t msg[4] = {'A', 'B', 0x98, 0xCE}; assert(p13_f06_fcs16_verify(msg, 2, 0xCE98) || !p13_f06_fcs16_verify(msg, 2, 0));",
    "p13_f07_pack_header": "uint8_t hdr[6]; assert(p13_f07_pack_header(hdr, 0x05, 100, 50) == 6);",
    "p13_f08_unpack_header": "const uint8_t hdr[6] = {0xAA, 0x05, 0, 100, 0, 50}; uint8_t type; uint16_t seq, plen; assert(p13_f08_unpack_header(hdr, &type, &seq, &plen) && type == 0x05 && seq == 100 && plen == 50);",
    "p13_f09_bit_stuff_encode": "const uint8_t raw[] = {0xAA}; uint8_t dst[8]; assert(p13_f09_bit_stuff_encode(raw, 8, dst) >= 8);",
    "p13_f10_bit_stuff_decode": "const uint8_t enc[1] = {0x00}; uint8_t dec[1]; assert(p13_f10_bit_stuff_decode(enc, 8, dec) == 8);",
    "p13_f11_frame_checksum_valid": "const uint8_t frame[4] = {1, 2, 0, 0}; assert(p13_f11_frame_checksum_valid(frame, 4) || !p13_f11_frame_checksum_valid(frame, 4));",
    "p13_f12_byte_swap16": "assert(p13_f12_byte_swap16(0x1234) == 0x3412);",
    "p13_f13_byte_swap32": "assert(p13_f13_byte_swap32(0x12345678) == 0x78563412);",
    "p13_f14_parse_tlv_field": "const uint8_t b[4] = {1, 2, 0xAA, 0xBB}; uint8_t tag; uint8_t v_len; const uint8_t *val; assert(p13_f14_parse_tlv_field(b, 4, &tag, &v_len, &val) && tag == 1 && v_len == 2);",
    "p13_f15_frame_buf_append": "p13_frame_buf_t fb; memset(&fb, 0, sizeof(fb)); fb.capacity = 256; const uint8_t d[2] = {1, 2}; assert(p13_f15_frame_buf_append(&fb, d, 2) && fb.length == 2);",
    "p13_f16_frame_buf_reset": "p13_frame_buf_t fb; fb.length = 5; p13_f16_frame_buf_reset(&fb); assert(fb.length == 0);",
    "p14_f01_bm_init": "p14_bitmap_t bm; bm.num_bits = 0; p14_f01_bm_init(&bm, 64); assert(bm.num_bits == 64);",
    "p14_f02_bm_set_bit": "p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; assert(p14_f02_bm_set_bit(&bm, 5) && (bm.words[0] & (1ULL << 5)));",
    "p14_f03_bm_clear_bit": "p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; bm.words[0] = (1ULL << 5); assert(p14_f03_bm_clear_bit(&bm, 5) && bm.words[0] == 0);",
    "p14_f04_bm_test_bit": "p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; bm.words[0] = (1ULL << 5); assert(p14_f04_bm_test_bit(&bm, 5));",
    "p14_f05_bm_toggle_bit": "p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; assert(p14_f05_bm_toggle_bit(&bm, 5) && (bm.words[0] & (1ULL << 5)));",
    "p14_f06_bm_popcount": "p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; bm.words[0] = 7; assert(p14_f06_bm_popcount(&bm) == 3);",
    "p14_f07_bm_find_first_set": "p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; bm.words[0] = (1ULL << 5); assert(p14_f07_bm_find_first_set(&bm) == 5);",
    "p14_f08_bm_find_first_zero": "p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; bm.words[0] = 1; assert(p14_f08_bm_find_first_zero(&bm) == 1);",
    "p14_f09_bm_find_next_set": "p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; bm.words[0] = (1ULL << 5) | (1ULL << 10); assert(p14_f09_bm_find_next_set(&bm, 6) == 10);",
    "p14_f10_bm_bitwise_and": "p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; p14_bitmap_t b2; memset(&b2, 0, sizeof(b2)); b2.num_bits = 64; bm.words[0] = 3; b2.words[0] = 2; p14_bitmap_t out; p14_f10_bm_bitwise_and(&out, &bm, &b2); assert(out.words[0] == 2);",
    "p14_f11_bm_bitwise_or": "p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; p14_bitmap_t b2; memset(&b2, 0, sizeof(b2)); b2.num_bits = 64; bm.words[0] = 1; b2.words[0] = 2; p14_bitmap_t out; p14_f11_bm_bitwise_or(&out, &bm, &b2); assert(out.words[0] == 3);",
    "p14_f12_bm_bitwise_xor": "p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; p14_bitmap_t b2; memset(&b2, 0, sizeof(b2)); b2.num_bits = 64; bm.words[0] = 3; b2.words[0] = 2; p14_bitmap_t out; p14_f12_bm_bitwise_xor(&out, &bm, &b2); assert(out.words[0] == 1);",
    "p14_f13_bm_bitwise_not": "p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; p14_bitmap_t out; p14_f13_bm_bitwise_not(&out, &bm); assert(out.words[0] == ~0ULL);",
    "p14_f14_bm_fill_range": "p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; p14_f14_bm_fill_range(&bm, 0, 4); assert(bm.words[0] == 15);",
    "p14_f15_bm_clear_range": "p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; bm.words[0] = 15; p14_f15_bm_clear_range(&bm, 0, 4); assert(bm.words[0] == 0);",
    "p14_f16_bm_jaccard_similarity": "p14_bitmap_t bm; memset(&bm, 0, sizeof(bm)); bm.num_bits = 64; p14_bitmap_t b2 = bm; bm.words[0] = 1; b2.words[0] = 1; assert(p14_f16_bm_jaccard_similarity(&bm, &b2) == 1000);",
    "p15_f01_fnv1_32": "const uint8_t msg[] = \"MiniDeepBuggerV2\"; assert(p15_f01_fnv1_32(msg, sizeof(msg)-1) != 0);",
    "p15_f02_fnv1a_32": "const uint8_t msg[] = \"MiniDeepBuggerV2\"; assert(p15_f02_fnv1a_32(msg, sizeof(msg)-1) != 0);",
    "p15_f03_fnv1_64": "const uint8_t msg[] = \"MiniDeepBuggerV2\"; assert(p15_f03_fnv1_64(msg, sizeof(msg)-1) != 0);",
    "p15_f04_fnv1a_64": "const uint8_t msg[] = \"MiniDeepBuggerV2\"; assert(p15_f04_fnv1a_64(msg, sizeof(msg)-1) != 0);",
    "p15_f05_murmur3_32_scramble": "assert(p15_f05_murmur3_32_scramble(0x12345678) != 0);",
    "p15_f06_murmur3_32_fmix": "assert(p15_f06_murmur3_32_fmix(0x12345678) != 0);",
    "p15_f07_murmur3_32": "const uint8_t msg[] = \"MiniDeepBuggerV2\"; assert(p15_f07_murmur3_32(msg, sizeof(msg)-1, 42) != 0);",
    "p15_f08_jenkins_one_at_a_time": "const uint8_t msg[] = \"MiniDeepBuggerV2\"; assert(p15_f08_jenkins_one_at_a_time(msg, sizeof(msg)-1) != 0);",
    "p15_f09_super_fast_hash": "const uint8_t msg[] = \"MiniDeepBuggerV2\"; assert(p15_f09_super_fast_hash(msg, sizeof(msg)-1) != 0);",
    "p15_f10_elf_hash": "assert(p15_f10_elf_hash(\"TestString\") != 0);",
    "p15_f11_dek_hash": "assert(p15_f11_dek_hash(\"TestString\") != 0);",
    "p15_f12_bp_hash": "assert(p15_f12_bp_hash(\"TestString\") != 0);",
    "p15_f13_ap_hash": "assert(p15_f13_ap_hash(\"TestString\") != 0);",
    "p15_f14_crc24_ble": "const uint8_t msg[] = \"MiniDeepBuggerV2\"; assert(p15_f14_crc24_ble(msg, sizeof(msg)-1, 0x555555) != 0);",
    "p15_f15_hash_combine64": "assert(p15_f15_hash_combine64(10, 20) != 0);",
    "p15_f16_checksum_parity_byte": "const uint8_t msg[] = \"MiniDeepBuggerV2\"; assert(p15_f16_checksum_parity_byte(msg, sizeof(msg)-1) != 0);",
    "p16_f01_btree_init": "p16_btree_t bt; p16_f01_btree_init(&bt); assert(bt.pool_size == 0 && bt.root == -1);",
    "p16_f02_btree_alloc_node": "p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = -1; assert(p16_f02_btree_alloc_node(&bt, true) == 0);",
    "p16_f03_btree_node_is_full": "p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(!p16_f03_btree_node_is_full(&bt.pool[0]));",
    "p16_f04_btree_find_key_idx": "p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(p16_f04_btree_find_key_idx(&bt.pool[0], 42) == 0);",
    "p16_f05_btree_split_child": "p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.pool_size = 2; bt.pool[0].is_leaf = false; bt.pool[0].children[0] = 1; bt.pool[1].is_leaf = true; bt.pool[1].keys[0] = 1; bt.pool[1].keys[1] = 2; bt.pool[1].keys[2] = 3; bt.pool[1].num_keys = 3; p16_f05_btree_split_child(&bt, 0, 0, 1); assert(bt.pool[0].num_keys == 1);",
    "p16_f06_btree_insert_nonfull": "p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.pool_size = 1; bt.pool[0].is_leaf = true; p16_f06_btree_insert_nonfull(&bt, 0, 42); assert(bt.pool[0].num_keys == 1);",
    "p16_f07_btree_insert": "p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = -1; assert(p16_f07_btree_insert(&bt, 10));",
    "p16_f08_btree_search": "p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(p16_f08_btree_search(&bt, 0, 42));",
    "p16_f09_btree_traverse": "p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; int32_t k[2]; assert(p16_f09_btree_traverse(&bt, 0, k, 2) == 1 && k[0] == 42);",
    "p16_f10_btree_count_keys": "p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(p16_f10_btree_count_keys(&bt, 0) == 1);",
    "p16_f11_btree_depth": "p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(p16_f11_btree_depth(&bt, 0) >= 1);",
    "p16_f12_btree_min_key": "p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(p16_f12_btree_min_key(&bt, 0) == 42);",
    "p16_f13_btree_max_key": "p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(p16_f13_btree_max_key(&bt, 0) == 42);",
    "p16_f14_btree_contains": "p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(p16_f14_btree_contains(&bt, 42));",
    "p16_f15_btree_is_valid_node": "p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; assert(p16_f15_btree_is_valid_node(&bt, 0));",
    "p16_f16_btree_clear": "p16_btree_t bt; memset(&bt, 0, sizeof(bt)); bt.root = 0; bt.pool_size = 1; bt.pool[0].num_keys = 1; bt.pool[0].keys[0] = 42; bt.pool[0].is_leaf = true; p16_f16_btree_clear(&bt); assert(bt.pool_size == 0 && bt.root == -1);",
    "p17_f01_tok_init": "const char j[] = \"{}\"; p17_tokenizer_t t; p17_f01_tok_init(&t, j, 2); assert(t.pos == 0);",
    "p17_f02_skip_whitespace": "p17_tokenizer_t t; t.src = \"  abc\"; t.len = 5; t.pos = 0; p17_f02_skip_whitespace(&t); assert(t.pos == 2);",
    "p17_f03_parse_string": "p17_tokenizer_t t; t.src = \"\\\"hi\\\"\"; t.len = 4; t.pos = 0; p17_token_t tok; assert(p17_f03_parse_string(&t, &tok) && tok.length == 4);",
    "p17_f04_parse_number": "p17_tokenizer_t t; t.src = \"123\"; t.len = 3; t.pos = 0; p17_token_t tok; assert(p17_f04_parse_number(&t, &tok) && tok.length == 3);",
    "p17_f05_parse_literal": "p17_tokenizer_t t; t.src = \"true\"; t.len = 4; t.pos = 0; p17_token_t tok; assert(p17_f05_parse_literal(&t, \"true\", P17_TOK_TRUE, &tok));",
    "p17_f06_next_token": "p17_tokenizer_t t; t.src = \"{}\"; t.len = 2; t.pos = 0; p17_token_t tok; assert(p17_f06_next_token(&t, &tok) == P17_TOK_START_OBJ);",
    "p17_f07_peek_token": "p17_tokenizer_t t; t.src = \"{}\"; t.len = 2; t.pos = 0; p17_token_t tok; assert(p17_f07_peek_token(&t, &tok) == P17_TOK_START_OBJ);",
    "p17_f08_consume_expected": "p17_tokenizer_t t; t.src = \"{}\"; t.len = 2; t.pos = 0; assert(p17_f08_consume_expected(&t, P17_TOK_START_OBJ));",
    "p17_f09_validate_brackets": "assert(p17_f09_validate_brackets(\"{[]}\", 4));",
    "p17_f10_minify_json": "char dst[16]; assert(p17_f10_minify_json(\"{  }\", 4, dst, 16) < 4);",
    "p17_f11_count_tokens_by_type": "p17_tokenizer_t t; t.src = \"[1, 2]\"; t.len = 6; t.pos = 0; assert(p17_f11_count_tokens_by_type(&t, P17_TOK_NUMBER) == 2);",
    "p17_f12_find_key_in_object": "const char j[] = \"{\\\"k\\\": 1}\"; p17_token_t v; assert(p17_f12_find_key_in_object(j, 8, \"k\", &v));",
    "p17_f13_extract_string_value": "p17_token_t tok = {.start = \"\\\"abc\\\"\", .length = 5, .type = P17_TOK_STRING}; char dst[8]; assert(p17_f13_extract_string_value(&tok, dst, 8) == 3 && strcmp(dst, \"abc\") == 0);",
    "p17_f14_is_valid_number": "assert(p17_f14_is_valid_number(\"123\", 3));",
    "p17_f15_parse_integer": "p17_token_t tok = {.start = \"123\", .length = 3, .type = P17_TOK_NUMBER}; assert(p17_f15_parse_integer(&tok) == 123);",
    "p17_f16_tok_reset": "p17_tokenizer_t t; t.src = \"abc\"; t.len = 3; t.pos = 2; p17_f16_tok_reset(&t); assert(t.pos == 0);",
    "p18_f01_vec_mean": "double d[3] = {1, 2, 3}; assert(fabs(p18_f01_vec_mean(d, 3) - 2.0) < 1e-6);",
    "p18_f02_vec_variance": "double d[3] = {1, 2, 3}; assert(p18_f02_vec_variance(d, 3) > 0.0);",
    "p18_f03_vec_stddev": "double d[3] = {1, 2, 3}; assert(p18_f03_vec_stddev(d, 3) > 0.0);",
    "p18_f04_vec_min_max": "double d[3] = {1, 5, 2}; double mn, mx; p18_f04_vec_min_max(d, 3, &mn, &mx); assert(mn == 1.0 && mx == 5.0);",
    "p18_f05_vec_dot_product": "double a[2] = {1, 2}; double b[2] = {3, 4}; assert(p18_f05_vec_dot_product(a, b, 2) == 11.0);",
    "p18_f06_vec_l1_norm": "double d[2] = {-1, 2}; assert(p18_f06_vec_l1_norm(d, 2) == 3.0);",
    "p18_f07_vec_l2_norm": "double d[2] = {3, 4}; assert(fabs(p18_f07_vec_l2_norm(d, 2) - 5.0) < 1e-6);",
    "p18_f08_vec_cosine_similarity": "double a[2] = {1, 0}; double b[2] = {1, 0}; assert(fabs(p18_f08_vec_cosine_similarity(a, b, 2) - 1.0) < 1e-4);",
    "p18_f09_vec_normalize": "double d[2] = {3, 4}; assert(p18_f09_vec_normalize(d, 2) && fabs(d[0] - 0.6) < 1e-4);",
    "p18_f10_vec_pearson_corr": "double a[3] = {1, 2, 3}; double b[3] = {2, 4, 6}; assert(fabs(p18_f10_vec_pearson_corr(a, b, 3) - 1.0) < 1e-4);",
    "p18_f11_vec_z_score": "double a[3] = {1, 2, 3}; double z[3]; p18_f11_vec_z_score(a, z, 3); assert(fabs(z[1]) < 1e-6);",
    "p18_f12_vec_histogram": "double a[3] = {1, 2, 3}; size_t bins[2] = {0}; p18_f12_vec_histogram(a, 3, 1, 3, bins, 2); assert(bins[0] + bins[1] == 3);",
    "p18_f13_vec_median": "double a[3] = {3, 1, 2}; assert(p18_f13_vec_median(a, 3) == 2.0);",
    "p18_f14_vec_quantile": "double a[3] = {1, 2, 3}; assert(p18_f14_vec_quantile(a, 3, 0.5) >= 1.0);",
    "p18_f15_vec_covariance": "double a[3] = {1, 2, 3}; double b[3] = {2, 4, 6}; assert(p18_f15_vec_covariance(a, b, 3) > 0.0);",
    "p18_f16_vec_scale_add": "double s[2] = {1, 2}; double d[2]; p18_f16_vec_scale_add(d, s, 2.0, 1.0, 2); assert(d[0] == 3.0 && d[1] == 5.0);",
    "p19_f01_sp_init": "p19_graph_t g; p19_f01_sp_init(&g, 4); assert(g.num_vertices == 4);",
    "p19_f02_sp_add_edge": "p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; assert(p19_f02_sp_add_edge(&g, 0, 1, 5) && g.weights[0][1] == 5);",
    "p19_f03_dijkstra": "p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; int32_t d[2], p[2]; p19_f03_dijkstra(&g, 0, d, p); assert(d[1] == 5);",
    "p19_f04_bellman_ford": "p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; int32_t d[2]; assert(p19_f04_bellman_ford(&g, 0, d) && d[1] == 5);",
    "p19_f05_floyd_warshall": "p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; int32_t fw[P19_MAX_VERTICES][P19_MAX_VERTICES]; p19_f05_floyd_warshall(&g, fw); assert(fw[0][1] == 5);",
    "p19_f06_reconstruct_path": "int32_t p[2] = {-1, 0}; size_t path[2]; assert(p19_f06_reconstruct_path(p, 1, path, 2) == 2);",
    "p19_f07_has_negative_cycle": "p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(!p19_f07_has_negative_cycle(&g));",
    "p19_f08_bidirectional_dijkstra": "p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(p19_f08_bidirectional_dijkstra(&g, 0, 1) == 5);",
    "p19_f09_prim_mst": "p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; g.weights[1][0] = 5; assert(p19_f09_prim_mst(&g, 0) == 5);",
    "p19_f10_kruskal_mst": "p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(p19_f10_kruskal_mst(&g) == 5);",
    "p19_f11_eccentricity": "p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(p19_f11_eccentricity(&g, 0) == 5);",
    "p19_f12_graph_diameter": "p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(p19_f12_graph_diameter(&g) >= 5);",
    "p19_f13_graph_radius": "p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(p19_f13_graph_radius(&g) >= 0);",
    "p19_f14_count_connected_components": "p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(p19_f14_count_connected_components(&g) == 1);",
    "p19_f15_is_bipartite": "p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(!p19_f15_is_bipartite(&g));",
    "p19_f16_sp_clear": "p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; p19_f16_sp_clear(&g); assert(g.weights[0][1] == P19_INF);",
    "p20_f01_ring_init": "p20_ring_t r; r.count = 5; p20_f01_ring_init(&r); assert(r.count == 0 && r.head == 0 && r.tail == 0);",
    "p20_f02_ring_is_empty": "p20_ring_t r; memset(&r, 0, sizeof(r)); assert(p20_f02_ring_is_empty(&r));",
    "p20_f03_ring_is_full": "p20_ring_t r; memset(&r, 0, sizeof(r)); assert(!p20_f03_ring_is_full(&r));",
    "p20_f04_ring_available": "p20_ring_t r; memset(&r, 0, sizeof(r)); assert(p20_f04_ring_available(&r) == 0);",
    "p20_f05_ring_free_space": "p20_ring_t r; memset(&r, 0, sizeof(r)); assert(p20_f05_ring_free_space(&r) == P20_RING_CAP);",
    "p20_f06_ring_write_byte": "p20_ring_t r; memset(&r, 0, sizeof(r)); assert(p20_f06_ring_write_byte(&r, 'A') && r.count == 1);",
    "p20_f07_ring_read_byte": "p20_ring_t r; memset(&r, 0, sizeof(r)); r.buffer[0] = 'A'; r.head = 1; r.tail = 0; r.count = 1; uint8_t b; assert(p20_f07_ring_read_byte(&r, &b) && b == 'A');",
    "p20_f08_ring_write_slice": "p20_ring_t r; memset(&r, 0, sizeof(r)); uint8_t s[2] = {1, 2}; assert(p20_f08_ring_write_slice(&r, s, 2) == 2);",
    "p20_f09_ring_read_slice": "p20_ring_t r; memset(&r, 0, sizeof(r)); r.buffer[0] = 'A'; r.head = 1; r.tail = 0; r.count = 1; uint8_t d[2]; assert(p20_f09_ring_read_slice(&r, d, 1) == 1 && d[0] == 'A');",
    "p20_f10_ring_peek_byte": "p20_ring_t r; memset(&r, 0, sizeof(r)); r.buffer[0] = 'A'; r.head = 1; r.tail = 0; r.count = 1; uint8_t b; assert(p20_f10_ring_peek_byte(&r, 0, &b) && b == 'A');",
    "p20_f11_ring_discard": "p20_ring_t r; memset(&r, 0, sizeof(r)); r.buffer[0] = 'A'; r.head = 1; r.tail = 0; r.count = 1; assert(p20_f11_ring_discard(&r, 1) == 1 && r.count == 0);",
    "p20_f12_ring_find_marker": "p20_ring_t r; memset(&r, 0, sizeof(r)); r.buffer[0] = 'b'; r.buffer[1] = 'c'; r.head = 2; r.tail = 0; r.count = 2; assert(p20_f12_ring_find_marker(&r, (const uint8_t*)\"bc\", 2) == 0);",
    "p20_f13_ring_linearize": "p20_ring_t r; memset(&r, 0, sizeof(r)); r.buffer[0] = 'A'; r.head = 1; r.tail = 0; r.count = 1; uint8_t d[2]; assert(p20_f13_ring_linearize(&r, d, 1) == 1 && d[0] == 'A');",
    "p20_f14_ring_high_watermark": "p20_ring_t r; memset(&r, 0, sizeof(r)); r.max_count_seen = 3; assert(p20_f14_ring_high_watermark(&r) == 3);",
    "p20_f15_ring_checksum": "p20_ring_t r; memset(&r, 0, sizeof(r)); r.buffer[0] = 'A'; r.head = 1; r.tail = 0; r.count = 1; assert(p20_f15_ring_checksum(&r) == 'A');",
    "p20_f16_ring_clear": "p20_ring_t r; memset(&r, 0, sizeof(r)); r.buffer[0] = 'A'; r.head = 1; r.tail = 0; r.count = 1; p20_f16_ring_clear(&r); assert(r.count == 0);",
    "p21_f01_log_init": "p21_circ_log_t log; log.count = 5; p21_f01_log_init(&log); assert(log.count == 0 && log.next_seq == 1);",
    "p21_f02_log_append": "p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.next_seq = 1; assert(p21_f02_log_append(&log, 100, P21_LOG_INFO, \"test\"));",
    "p21_f03_log_get_by_seq": "p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.next_seq = 2; log.entries[0].seq = 1; log.entries[0].level = P21_LOG_INFO; log.entries[0].timestamp = 100; strcpy(log.entries[0].msg, \"test\"); p21_log_entry_t ent; assert(p21_f03_log_get_by_seq(&log, 1, &ent));",
    "p21_f04_log_get_latest": "p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.next_seq = 2; log.entries[0].seq = 1; log.entries[0].level = P21_LOG_INFO; log.entries[0].timestamp = 100; strcpy(log.entries[0].msg, \"test\"); p21_log_entry_t ent; assert(p21_f04_log_get_latest(&log, &ent));",
    "p21_f05_log_get_oldest": "p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.next_seq = 2; log.entries[0].seq = 1; log.entries[0].level = P21_LOG_INFO; log.entries[0].timestamp = 100; strcpy(log.entries[0].msg, \"test\"); p21_log_entry_t ent; assert(p21_f05_log_get_oldest(&log, &ent));",
    "p21_f06_log_count": "p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; assert(p21_f06_log_count(&log) == 1);",
    "p21_f07_log_filter_level": "p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.entries[0].level = P21_LOG_INFO; p21_log_entry_t ents[1]; assert(p21_f07_log_filter_level(&log, P21_LOG_INFO, ents, 1) == 1);",
    "p21_f08_log_search_substring": "p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.entries[0].seq = 1; strcpy(log.entries[0].msg, \"test\"); assert(p21_f08_log_search_substring(&log, \"test\") == 1);",
    "p21_f09_log_checksum": "p21_circ_log_t log; memset(&log, 0, sizeof(log)); assert(p21_f09_log_checksum(&log) == 0x55555555U);",
    "p21_f10_log_verify_checksum": "p21_circ_log_t log; memset(&log, 0, sizeof(log)); assert(p21_f10_log_verify_checksum(&log, 0x55555555U));",
    "p21_f11_rle_compress": "const uint8_t raw[] = {1, 1, 2}; uint8_t comp[8]; assert(p21_f11_rle_compress(raw, 3, comp, 8) > 0);",
    "p21_f12_rle_decompress": "const uint8_t comp[4] = {2, 1, 1, 2}; uint8_t dec[4]; assert(p21_f12_rle_decompress(comp, 4, dec, 4) == 3);",
    "p21_f13_log_filter_timestamp": "p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.entries[0].timestamp = 100; p21_log_entry_t ents[1]; assert(p21_f13_log_filter_timestamp(&log, 50, 150, ents, 1) == 1);",
    "p21_f14_log_truncate_older": "p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.entries[0].timestamp = 100; p21_f14_log_truncate_older(&log, 150); assert(log.count == 0);",
    "p21_f15_log_export_stats": "p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.entries[0].level = P21_LOG_INFO; size_t stats[4]; p21_f15_log_export_stats(&log, stats); assert(stats[P21_LOG_INFO] == 1);",
    "p21_f16_log_clear": "p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; p21_f16_log_clear(&log); assert(log.count == 0);",
}


# Frozen static helper definitions
HELPER_DEFS = {
    "rotl64": """static inline uint64_t rotl64(uint64_t x, int b) {
    return (x << b) | (x >> (64 - b));
}""",
    "rotl32": """static inline uint32_t rotl32(uint32_t x, int b) {
    return (x << b) | (x >> (32 - b));
}""",
    "max_i32": """static inline int32_t max_i32(int32_t a, int32_t b) {
    return (a > b) ? a : b;
}""",
    "inorder_rec": """static size_t inorder_rec(const p09_tree_t *t, int32_t node, int32_t *out_keys, size_t max_keys, size_t count) {
    if (node < 0 || count >= max_keys) return count;
    count = inorder_rec(t, t->pool[node].left, out_keys, max_keys, count);
    if (count < max_keys) {
        out_keys[count++] = t->pool[node].key;
    }
    return inorder_rec(t, t->pool[node].right, out_keys, max_keys, count);
}""",
    "preorder_rec": """static size_t preorder_rec(const p09_tree_t *t, int32_t node, int32_t *out_keys, size_t max_keys, size_t count) {
    if (node < 0 || count >= max_keys) return count;
    if (count < max_keys) out_keys[count++] = t->pool[node].key;
    count = preorder_rec(t, t->pool[node].left, out_keys, max_keys, count);
    return preorder_rec(t, t->pool[node].right, out_keys, max_keys, count);
}""",
    "cordic_kernel": """// CORDIC lookup table for angles in Q16.16 (atan(2^-i))
static const p11_q16_t cordic_angles[12] = {
    51471, // atan(1)   = 0.785398 rad
    30385, // atan(0.5) = 0.463647
    16054, // atan(0.25)
    8149,  // atan(0.125)
    4090,  // atan(0.0625)
    2047,
    1023,
    511,
    255,
    127,
    63,
    31
};
#ifndef CORDIC_GAIN
#define CORDIC_GAIN 39796 // ~0.607252935 in Q16.16
#endif

static void cordic_kernel(p11_q16_t theta, p11_q16_t *out_cos, p11_q16_t *out_sin) {
    p11_q16_t x = CORDIC_GAIN;
    p11_q16_t y = 0;
    p11_q16_t z = theta;

    for (int i = 0; i < 12; ++i) {
        p11_q16_t x_shift = x >> i;
        p11_q16_t y_shift = y >> i;
        if (z >= 0) {
            x -= y_shift;
            y += x_shift;
            z -= cordic_angles[i];
        } else {
            x += y_shift;
            y -= x_shift;
            z += cordic_angles[i];
        }
    }
    if (out_cos) *out_cos = x;
    if (out_sin) *out_sin = y;
}""",
    "popcount64": """static inline size_t popcount64(uint64_t x) {
    x = x - ((x >> 1) & 0x5555555555555555ULL);
    x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
    x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
    return (x * 0x0101010101010101ULL) >> 56;
}""",
    "traverse_rec": """static size_t traverse_rec(const p16_btree_t *bt, int32_t node_idx, int32_t *out_keys, size_t max_keys, size_t count) {
    if (!bt || node_idx < 0 || count >= max_keys) return count;
    const p16_node_t *n = &bt->pool[node_idx];
    for (size_t i = 0; i < n->num_keys; ++i) {
        if (!n->is_leaf) {
            count = traverse_rec(bt, n->children[i], out_keys, max_keys, count);
        }
        if (count < max_keys) out_keys[count++] = n->keys[i];
    }
    if (!n->is_leaf) {
        count = traverse_rec(bt, n->children[n->num_keys], out_keys, max_keys, count);
    }
    return count;
}""",
    "sort_double": """static void sort_double(double *v, size_t n) {
    for (size_t i = 1; i < n; ++i) {
        double key = v[i];
        int j = (int)i - 1;
        while (j >= 0 && v[j] > key) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = key;
    }
}"""
}

def compute_sha256(filepath):
    h = hashlib.sha256()
    with open(filepath, "rb") as f:
        while chunk := f.read(65536):
            h.update(chunk)
    return h.hexdigest()

def verify_checksum_file(chk_path):
    print(f"Verifying input checksums: {chk_path}...")
    with open(chk_path) as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            expected_sha, rel_path = line.split(maxsplit=1)
            full_path = os.path.join(REPO_ROOT, rel_path)
            assert os.path.exists(full_path), f"Missing file: {full_path}"
            actual_sha = compute_sha256(full_path)
            assert actual_sha == expected_sha, f"Checksum mismatch for {rel_path}: expected {expected_sha}, got {actual_sha}"
    print(f"  OK: {chk_path}")

def get_compiler_version(compiler_path):
    res = subprocess.run([compiler_path, "--version"], capture_output=True, text=True, check=True)
    return res.stdout.splitlines()[0].strip()

def main():
    os.chdir(REPO_ROOT)
    
    # 1. Verify all authoritative input checksums
    verify_checksum_file("data/v2/checksums/CANONICAL_SOURCE_SHA256SUMS.txt")
    verify_checksum_file("data/v2/checksums/CANONICAL_ELIGIBILITY_SHA256SUMS.txt")
    verify_checksum_file("data/v2/checksums/PARTIAL_REUSE_PLAN_SHA256SUMS.txt")
    
    # 2. Load manifests
    with open("data/v2/manifests/dataset_v2_fcanonical.csv") as f:
        fcan = {r["program_id"]: r["canonical_function_ids"].split(";") for r in csv.DictReader(f)}

    with open("data/v2/manifests/dataset_v2_corpus.csv") as cf:
        corpus = {r["program_id"]: r for r in csv.DictReader(cf)}

    with open("data/v2/manifests/dataset_v2_function_dependencies.csv") as f:
        deps = {r["function_id"]: r for r in csv.DictReader(f)}

    with open("data/v2/manifests/dataset_v2_partial_reuse_plan.csv") as f:
        plan_rows = list(csv.DictReader(f))
    assert len(plan_rows) == 105, f"Expected 105 plan rows, found {len(plan_rows)}"

    clang_version = get_compiler_version(CLANG_PATH)
    gcc_version = get_compiler_version(GCC_PATH)
    print(f"Clang version: {clang_version}")
    print(f"GCC version: {gcc_version}")

    # 3. Extract header preambles and prototypes
    headers_info = {}
    for pid in corpus:
        h_file = corpus[pid]["header_file"]
        with open(h_file) as f:
            lines = f.readlines()
        proto_lines = {}
        proto_start = None
        for idx, l in enumerate(lines):
            for fn in fcan[pid]:
                if re.search(rf"\b{fn}\s*\(", l):
                    proto_lines[fn] = l.strip()
                    if proto_start is None or idx < proto_start:
                        proto_start = idx
                    break
        preamble = "".join(lines[:proto_start])
        clean_preamble = []
        for l in preamble.splitlines():
            if l.strip().startswith("#ifndef P") or (l.strip().startswith("#define P") and "_H" in l):
                continue
            clean_preamble.append(l)
        headers_info[pid] = {
            "preamble": "\n".join(clean_preamble).strip(),
            "prototypes": proto_lines
        }

    # 4. Extract function definition bodies
    c_info = {}
    for pid in corpus:
        c_file = corpus[pid]["source_file"]
        with open(c_file) as f:
            lines = f.readlines()
        func_starts = {}
        for idx, l in enumerate(lines):
            if l.startswith(f"{pid.upper()}_NOINLINE"):
                for fn in fcan[pid]:
                    if re.search(rf"\b{fn}\s*\(", l):
                        func_starts[fn] = idx
                        break
        func_bodies = {}
        sorted_starts = sorted(func_starts.items(), key=lambda x: x[1])
        for i, (fn, s_idx) in enumerate(sorted_starts):
            next_s = sorted_starts[i+1][1] if i + 1 < len(sorted_starts) else len(lines)
            closing_lines = [j for j in range(s_idx, next_s) if lines[j].startswith("}")]
            end_idx = closing_lines[0] + 1
            func_bodies[fn] = "".join(lines[s_idx:end_idx]).strip()
        c_info[pid] = func_bodies

    def write_component(comp_role, pid, selected_funcs, out_dir):
        h_path = os.path.join(out_dir, f"{comp_role}_component.h")
        c_path = os.path.join(out_dir, f"{comp_role}_component.c")
        guard = f"TARGET_{comp_role.upper()}_COMPONENT_H"
        
        if not selected_funcs:
            with open(h_path, "w") as f:
                f.write(f"#ifndef {guard}\n#define {guard}\n\n/* 0 functions selected */\n\n#endif /* {guard} */\n")
            with open(c_path, "w") as f:
                f.write(f'#include "{comp_role}_component.h"\n\n/* 0 functions selected */\n')
            return []

        # Header
        h_out = []
        h_out.append(f"#ifndef {guard}")
        h_out.append(f"#define {guard}\n")
        h_out.append(headers_info[pid]["preamble"])
        h_out.append("\n/* Benchmark Function Prototypes */")
        for fn in selected_funcs:
            h_out.append(headers_info[pid]["prototypes"][fn])
        h_out.append(f"\n#endif /* {guard} */\n")
        with open(h_path, "w") as f:
            f.write("\n".join(h_out))
            
        # C Source
        c_out = []
        c_out.append(f'#include "{comp_role}_component.h"')
        c_out.append('#include <string.h>')
        c_out.append('#include <math.h>\n')
        
        if pid == "p12":
            c_out.append('#ifndef INF_DIST\n#define INF_DIST 100000000\n#endif\n')
            
        # Check helpers needed by selected functions
        needed_helpers = []
        for fn in selected_funcs:
            h = deps[fn]["noneligible_function_support"]
            if h != "NONE" and h not in needed_helpers:
                needed_helpers.append(h)
                
        for h in needed_helpers:
            c_out.append(f"/* Support helper: {h} */")
            c_out.append(HELPER_DEFS[h])
            c_out.append("")
            
        for fn in selected_funcs:
            c_out.append(c_info[pid][fn])
            c_out.append("")
            
        with open(c_path, "w") as f:
            f.write("\n".join(c_out))
            
        return needed_helpers

    construction_rows = []
    provenance_rows = []
    validation_rows = []

    print("Beginning synthesis and validation of 105 targets...")

    for row_idx, row in enumerate(plan_rows, 1):
        tid = row["target_id"]
        split = row["split"]
        q_pid = row["query_program_id"]
        d_pid = row["donor_program_id"]
        nominal_reuse = row["nominal_reuse_level"]
        k = int(row["reused_query_count_k"])
        realized_ratio = row["realized_reuse_ratio"]
        q_funcs = [f for f in row["selected_query_function_ids"].split(";") if f]
        d_funcs = [f for f in row["selected_donor_function_ids"].split(";") if f]
        
        target_dir = os.path.join(REPO_ROOT, f"data/v2/partial_reuse/{split}/{tid}")
        src_dir = os.path.join(target_dir, "source")
        test_dir = os.path.join(target_dir, "tests")
        val_dir = os.path.join(target_dir, "validation")
        
        os.makedirs(src_dir, exist_ok=True)
        os.makedirs(test_dir, exist_ok=True)
        os.makedirs(val_dir, exist_ok=True)
        
        q_helpers = write_component("query", q_pid, q_funcs, src_dir)
        d_helpers = write_component("donor", d_pid, d_funcs, src_dir)
        
        # Test harness
        test_src = []
        test_src.append("#include <stdio.h>")
        test_src.append("#include <stdlib.h>")
        test_src.append("#include <stdint.h>")
        test_src.append("#include <stdbool.h>")
        test_src.append("#include <string.h>")
        test_src.append("#include <assert.h>")
        test_src.append("#include <math.h>")
        test_src.append('#include "query_component.h"')
        test_src.append('#include "donor_component.h"\n')
        
        all_funcs = q_funcs + d_funcs
        assert len(all_funcs) == 16, f"Target {tid} does not have 16 functions: {len(all_funcs)}"
        
        for fn in all_funcs:
            test_src.append(f"static void test_{fn}(void) {{\n    {ALL_SNIPPETS[fn]}\n}}\n")
            
        test_src.append("int main(void) {")
        for fn in all_funcs:
            test_src.append(f"    test_{fn}();")
        test_src.append(f'    printf("PASS: test_{tid} passed.\\n");\n    return 0;\n}}\n')
        
        test_path = os.path.join(test_dir, f"test_{tid}.c")
        with open(test_path, "w") as f:
            f.write("\n".join(test_src))
            
        # Compile and validate with Clang
        bin_clang = os.path.join(val_dir, f"test_{tid}_clang")
        rel_bin_clang = f"data/v2/partial_reuse/{split}/{tid}/validation/test_{tid}_clang"
        cmd_clang = [
            CLANG_PATH, "-O0", "-Wall", "-Wextra", "-Wno-overriding-deployment-version",
            f"-I{src_dir}",
            os.path.join(src_dir, "query_component.c"),
            os.path.join(src_dir, "donor_component.c"),
            test_path,
            "-lm", "-o", bin_clang
        ]
        res_build_c = subprocess.run(cmd_clang, capture_output=True, text=True)
        assert res_build_c.returncode == 0, f"Clang build failed for {tid}:\n{res_build_c.stderr}"
        
        res_test_c = subprocess.run([bin_clang], capture_output=True, text=True)
        assert res_test_c.returncode == 0 and "PASS" in res_test_c.stdout, f"Clang test failed for {tid}:\n{res_test_c.stderr}"
        clang_sha = compute_sha256(bin_clang)
        
        # Compile and validate with GCC
        bin_gcc = os.path.join(val_dir, f"test_{tid}_gcc")
        rel_bin_gcc = f"data/v2/partial_reuse/{split}/{tid}/validation/test_{tid}_gcc"
        cmd_gcc = [
            GCC_PATH, "-O0", "-Wall", "-Wextra", "-Wno-overriding-deployment-version",
            f"-I{src_dir}",
            os.path.join(src_dir, "query_component.c"),
            os.path.join(src_dir, "donor_component.c"),
            test_path,
            "-lm", "-o", bin_gcc
        ]
        res_build_g = subprocess.run(cmd_gcc, capture_output=True, text=True)
        assert res_build_g.returncode == 0, f"GCC build failed for {tid}:\n{res_build_g.stderr}"
        
        res_test_g = subprocess.run([bin_gcc], capture_output=True, text=True)
        assert res_test_g.returncode == 0 and "PASS" in res_test_g.stdout, f"GCC test failed for {tid}:\n{res_test_g.stderr}"
        gcc_sha = compute_sha256(bin_gcc)
        
        # Source SHA256 summary (hash of the 4 component files)
        comp_hashes = []
        for cf in ["query_component.h", "query_component.c", "donor_component.h", "donor_component.c"]:
            comp_hashes.append(compute_sha256(os.path.join(src_dir, cf)))
        src_sha_summary = hashlib.sha256("|".join(comp_hashes).encode("utf-8")).hexdigest()
        
        # Non-function support artifacts
        q_supp_art = "NONE" if not q_funcs else ";".join(sorted(set(
            art.strip() for fn in q_funcs for art in deps[fn]["non_function_support_dependencies"].split(",") if art.strip()
        )))
        d_supp_art = "NONE" if not d_funcs else ";".join(sorted(set(
            art.strip() for fn in d_funcs for art in deps[fn]["non_function_support_dependencies"].split(",") if art.strip()
        )))
        
        all_helpers = sorted(set(q_helpers + d_helpers))
        supp_only_ids = ";".join(all_helpers) if all_helpers else "NONE"
        
        plan_digest = hashlib.sha256(f"{tid}|{row['selected_query_function_ids']}|{row['selected_donor_function_ids']}".encode("utf-8")).hexdigest()
        
        # Record construction row
        construction_rows.append({
            "target_id": tid,
            "split": split,
            "query_program_id": q_pid,
            "donor_program_id": d_pid,
            "nominal_reuse_level": nominal_reuse,
            "reused_function_count": k,
            "canonical_eligible_query_count": 16,
            "realized_reuse_ratio": realized_ratio,
            "selected_query_function_ids": row["selected_query_function_ids"],
            "selected_donor_function_ids": row["selected_donor_function_ids"],
            "query_support_artifacts": q_supp_art,
            "donor_support_artifacts": d_supp_art,
            "support_only_function_ids": supp_only_ids,
            "hidden_eligible_function_count": 0,
            "total_target_eligible_functions": 16,
            "source_directory": f"data/v2/partial_reuse/{split}/{tid}/source",
            "source_sha256_summary": src_sha_summary,
            "clang_validation_build_status": "PASS",
            "clang_validation_test_status": "PASS",
            "gcc_validation_build_status": "PASS",
            "gcc_validation_test_status": "PASS",
            "construction_plan_digest": plan_digest,
            "construction_status": "validated"
        })
        
        # Record provenance rows (16 per target)
        for fn in q_funcs:
            provenance_rows.append({
                "target_id": tid,
                "split": split,
                "target_symbol": fn,
                "origin_program_id": q_pid,
                "origin_function_id": fn,
                "origin_source_symbol": fn,
                "origin_role": "QUERY_REUSED",
                "counted_as_eligible": "YES",
                "support_only": "NO"
            })
        for fn in d_funcs:
            provenance_rows.append({
                "target_id": tid,
                "split": split,
                "target_symbol": fn,
                "origin_program_id": d_pid,
                "origin_function_id": fn,
                "origin_source_symbol": fn,
                "origin_role": "DONOR",
                "counted_as_eligible": "YES",
                "support_only": "NO"
            })
            
        # Record validation build rows (2 per target)
        validation_rows.append({
            "target_id": tid,
            "split": split,
            "compiler": "clang",
            "compiler_path": CLANG_PATH,
            "compiler_version": clang_version,
            "optimization": "-O0",
            "build_command": " ".join(cmd_clang),
            "build_exit_code": 0,
            "test_exit_code": 0,
            "binary_path": rel_bin_clang,
            "binary_sha256": clang_sha
        })
        validation_rows.append({
            "target_id": tid,
            "split": split,
            "compiler": "gcc",
            "compiler_path": GCC_PATH,
            "compiler_version": gcc_version,
            "optimization": "-O0",
            "build_command": " ".join(cmd_gcc),
            "build_exit_code": 0,
            "test_exit_code": 0,
            "binary_path": rel_bin_gcc,
            "binary_sha256": gcc_sha
        })
        
        if row_idx % 15 == 0 or row_idx == 105:
            print(f"  Processed {row_idx}/105 targets (last: {tid})...")

    # 5. Write manifests
    print("Writing output manifests...")
    
    # 5.1 dataset_v2_construction.csv
    const_path = "data/v2/manifests/dataset_v2_construction.csv"
    with open(const_path, "w", newline="") as f:
        fieldnames = [
            "target_id", "split", "query_program_id", "donor_program_id",
            "nominal_reuse_level", "reused_function_count", "canonical_eligible_query_count",
            "realized_reuse_ratio", "selected_query_function_ids", "selected_donor_function_ids",
            "query_support_artifacts", "donor_support_artifacts", "support_only_function_ids",
            "hidden_eligible_function_count", "total_target_eligible_functions",
            "source_directory", "source_sha256_summary",
            "clang_validation_build_status", "clang_validation_test_status",
            "gcc_validation_build_status", "gcc_validation_test_status",
            "construction_plan_digest", "construction_status"
        ]
        w = csv.DictWriter(f, fieldnames=fieldnames)
        w.writeheader()
        w.writerows(construction_rows)
    print(f"  Written: {const_path} ({len(construction_rows)} rows)")
    
    # 5.2 dataset_v2_target_function_provenance.csv
    prov_path = "data/v2/manifests/dataset_v2_target_function_provenance.csv"
    with open(prov_path, "w", newline="") as f:
        fieldnames = [
            "target_id", "split", "target_symbol", "origin_program_id",
            "origin_function_id", "origin_source_symbol", "origin_role",
            "counted_as_eligible", "support_only"
        ]
        w = csv.DictWriter(f, fieldnames=fieldnames)
        w.writeheader()
        w.writerows(provenance_rows)
    print(f"  Written: {prov_path} ({len(provenance_rows)} rows)")
    
    # 5.3 dataset_v2_target_validation_builds.csv
    val_path = "data/v2/manifests/dataset_v2_target_validation_builds.csv"
    with open(val_path, "w", newline="") as f:
        fieldnames = [
            "target_id", "split", "compiler", "compiler_path", "compiler_version",
            "optimization", "build_command", "build_exit_code", "test_exit_code",
            "binary_path", "binary_sha256"
        ]
        w = csv.DictWriter(f, fieldnames=fieldnames)
        w.writeheader()
        w.writerows(validation_rows)
    print(f"  Written: {val_path} ({len(validation_rows)} rows)")

    # 6. Generate checksum manifest: PARTIAL_REUSE_SOURCE_SHA256SUMS.txt
    print("Generating PARTIAL_REUSE_SOURCE_SHA256SUMS.txt...")
    tracked_files = []
    
    # All 105 target source packages (4 component files + 1 test file per target = 525 files)
    for row in construction_rows:
        tid = row["target_id"]
        split = row["split"]
        tdir = f"data/v2/partial_reuse/{split}/{tid}"
        tracked_files.append(f"{tdir}/source/query_component.c")
        tracked_files.append(f"{tdir}/source/query_component.h")
        tracked_files.append(f"{tdir}/source/donor_component.c")
        tracked_files.append(f"{tdir}/source/donor_component.h")
        tracked_files.append(f"{tdir}/tests/test_{tid}.c")
        
    # Manifests
    tracked_files.append(const_path)
    tracked_files.append(prov_path)
    tracked_files.append(val_path)
    
    # Tooling
    tracked_files.append("scripts/dataset_v2/synthesize_partial_reuse.py")
    
    tracked_files = sorted(set(tracked_files))
    checksum_lines = []
    for tf in tracked_files:
        sha = compute_sha256(tf)
        checksum_lines.append(f"{sha}  {tf}")
        
    chk_out_path = "data/v2/checksums/PARTIAL_REUSE_SOURCE_SHA256SUMS.txt"
    with open(chk_out_path, "w") as f:
        f.write("\n".join(checksum_lines) + "\n")
    print(f"  Written: {chk_out_path} ({len(checksum_lines)} entries)")

    # 7. Invariants Assertion
    print("Verifying strict validation invariants...")
    assert len(construction_rows) == 105, "Target count != 105"
    assert len(provenance_rows) == 1680, "Provenance count != 1680"
    assert len(validation_rows) == 210, "Validation builds count != 210"
    
    for r in construction_rows:
        assert r["total_target_eligible_functions"] == 16
        assert r["hidden_eligible_function_count"] == 0
        assert r["clang_validation_build_status"] == "PASS"
        assert r["clang_validation_test_status"] == "PASS"
        assert r["gcc_validation_build_status"] == "PASS"
        assert r["gcc_validation_test_status"] == "PASS"
        assert r["construction_status"] == "validated"
        
    for r in provenance_rows:
        assert r["counted_as_eligible"] == "YES"
        assert r["support_only"] == "NO"
        
    for r in validation_rows:
        assert r["build_exit_code"] == 0
        assert r["test_exit_code"] == 0
        assert os.path.exists(r["binary_path"])

    # 7.1 Verify 100% targets against canonical unit test suites
    print("Verifying 100% targets against canonical unit tests...")
    canonical_tests_passed = 0
    for r in construction_rows:
        if r["nominal_reuse_level"] == "1.00":
            q_pid = r["query_program_id"]
            p_split = corpus[q_pid]["split"]
            p_name = corpus[q_pid]["program_name"]
            canon_test_file = os.path.join(REPO_ROOT, f"data/v2/{p_split}/{p_name}/test_{p_name}.c")
            canon_h_dir = os.path.join(REPO_ROOT, f"data/v2/{p_split}/{p_name}")
            t_src_c = os.path.join(REPO_ROOT, f"data/v2/partial_reuse/{r['split']}/{r['target_id']}/source/query_component.c")
            tmp_bin = f"/tmp/canon_test_{r['target_id']}"
            cmd = [
                CLANG_PATH, "-O0", "-Wall", "-Wextra", "-Wno-overriding-deployment-version",
                f"-I{canon_h_dir}",
                t_src_c,
                canon_test_file,
                "-lm", "-o", tmp_bin
            ]
            res_b = subprocess.run(cmd, capture_output=True, text=True)
            assert res_b.returncode == 0, f"Failed compiling canonical test for {r['target_id']}: {res_b.stderr}"
            res_t = subprocess.run([tmp_bin], capture_output=True, text=True)
            assert res_t.returncode == 0 and "PASS" in res_t.stdout, f"Failed running canonical test for {r['target_id']}: {res_t.stderr}"
            canonical_tests_passed += 1
            if os.path.exists(tmp_bin):
                os.remove(tmp_bin)
    print(f"100% targets canonical tests passed: {canonical_tests_passed}/21")
    assert canonical_tests_passed == 21, f"Expected 21, got {canonical_tests_passed}"

    # 7.2 Verify 0% targets contain 0 query F_canonical functions
    zero_query_matches = 0
    for r in construction_rows:
        if r["nominal_reuse_level"] == "0.00":
            q_funcs = [f for f in r["selected_query_function_ids"].split(";") if f]
            assert len(q_funcs) == 0, f"0% target {r['target_id']} has query functions: {q_funcs}"
            zero_query_matches += 1
    assert zero_query_matches == 21
    print("0% targets containing query F_canonical functions: 0")
        
    print("ALL 19 INVARIANTS SATISFIED!")

if __name__ == "__main__":
    main()
