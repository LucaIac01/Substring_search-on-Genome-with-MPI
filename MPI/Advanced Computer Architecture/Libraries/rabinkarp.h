
#ifndef RABIN_K
#define RABIN_K

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <stddef.h>
#include "utilities.h"

void rabin_karp(char *txt, char *pattern, size_t lentxt, size_t lenpat, long long int *occurrences);
#endif