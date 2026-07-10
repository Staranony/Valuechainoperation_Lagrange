#ifndef LAGRANGE_2_H
#define LAGRANGE_2_H


#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
	uint64_t x;
	uint64_t y;

} Share;

bool lagrange_recover_secret(const Share* shares, size_t n, uint64_t* out_secret);

#endif
