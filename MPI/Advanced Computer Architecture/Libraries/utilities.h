#ifndef UTIL_RABINKARP
#define UTIL_RABINKARP
#define _LARGEFILE_SOURCE
#define _FILE_OFFSET_BITS 64

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

size_t who_is_active(int *active_flags, size_t text_length, size_t pattern_length, int cores);
char *readFile(char *argv, size_t *len);
char *split_dataset(char *text_data, size_t *chunklen, size_t text_length, size_t pattern_length, size_t chunk_size, int num_workers);
char *receive_dataset(size_t chunk_size, size_t text_length, size_t pattern_length, size_t *chunklen, int worker_rank, int num_workers);
void null_check(void *memory_ptr);

#endif
