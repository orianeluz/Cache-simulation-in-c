/*326367570 orian eluz */;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cache.h"


cache_t initialize_cache(uchar s, uchar t, uchar b, uchar E) {
    cache_t new_cache;
    new_cache.s = s;
    new_cache.t = t;
    new_cache.b = b;
    new_cache.E = E;

    int num_sets = 1 << s;        // 2^s sets
    int block_size = 1 << b;      // 2^b bytes per block

    // array of cache sets
    new_cache.cache = (cache_line_t**)malloc(num_sets * sizeof(cache_line_t*));
    if (!new_cache.cache) {
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_sets; ++i) {
        // lines for each set
        new_cache.cache[i] = (cache_line_t*)malloc(E * sizeof(cache_line_t));
        if (!new_cache.cache[i]) {
            exit(EXIT_FAILURE);
        }
        // initialize each line in the set
        for (int j = 0; j < E; ++j) {
            new_cache.cache[i][j].valid = 0;
            new_cache.cache[i][j].frequency = 0;
            new_cache.cache[i][j].tag = 0;

            // memory for each block
            new_cache.cache[i][j].block = (uchar*)malloc(block_size * sizeof(uchar));
            if (!new_cache.cache[i][j].block) {
                exit(EXIT_FAILURE);
            }
        }
    }

    return new_cache;
}
void write_byte(cache_t cache, uchar* start, long int off, uchar new) {
    int set_index = (off >> cache.b) & ((1 << cache.s) - 1);
    long int tag = (off >> (cache.b + cache.s)) & ((1L << cache.t) - 1);
    int block_offset = off & ((1 << cache.b) - 1);
    int block_start = off & ~((1 << cache.b) - 1);
    cache_line_t* set = cache.cache[set_index];
    
    // Write to memory
    start[off] = new;
    
    // check if the block already in cache
    for (int i = 0; i < cache.E; ++i) {
        if (set[i].valid && set[i].tag == tag) {
            set[i].block[block_offset] = new;
            set[i].frequency++;
            return;
        }
    }
    
    // Look for empty line first
    for (int i = 0; i < cache.E; ++i) {
        if (!set[i].valid) {
            set[i].valid = 1;
            set[i].frequency = 1;
            set[i].tag = tag;
            int block_size = 1 << cache.b;
            memcpy(set[i].block, start + block_start, block_size);
            set[i].block[block_offset] = new;
            return;
        }
    }
    
    // No empty lines so use LFU
    int lfu_index = 0;
    for (int i = 1; i < cache.E; ++i) {
        if (set[i].frequency < set[lfu_index].frequency) {
            lfu_index = i;
        }
    }
    
    set[lfu_index].frequency = 1;
    set[lfu_index].tag = tag;
    int block_size = 1 << cache.b;
    memcpy(set[lfu_index].block, start + block_start, block_size);
    set[lfu_index].block[block_offset] = new;
}

uchar read_byte(cache_t cache, uchar* start, long int off) {
    int set_index = (off >> cache.b) & ((1 << cache.s) - 1);
    long int tag = (off >> (cache.b + cache.s)) & ((1L << cache.t) - 1);
    int block_offset = off & ((1 << cache.b) - 1);
    cache_line_t* set = cache.cache[set_index];

    // check if the block already in cache
    for (int i = 0; i < cache.E; i++) {
        if (set[i].valid && set[i].tag == tag) {
            set[i].frequency++;
            return set[i].block[block_offset];
        }
    }
    
    // Cache miss so read from memory and write to the cache
    uchar value = start[off];
    write_byte(cache, start, off, value);
    return value;
}

void print_cache(cache_t cache) {
    int S = 1 << cache.s;
    int B = 1 << cache.b;

    for (int i = 0; i < S; i++) {
        printf("Set %d\n", i);
        for (int j = 0; j < cache.E; j++) {
            printf("%1d %d 0x%0*lx ", cache.cache[i][j].valid,
                   cache.cache[i][j].frequency, cache.t, cache.cache[i][j].tag);
            for (int k = 0; k < B; k++) {
                printf("%02x ", cache.cache[i][j].block[k]);
            }
            puts("");
        }
    }
}
