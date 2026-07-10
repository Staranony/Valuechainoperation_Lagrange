#include "laGrange_2.h"
#define MOD_PRIME 2305843009213693951ULL

static uint64_t field_reduce(__int128 v) {
	__int128 p = (__int128)MOD_PRIME;
      	return (uint64_t)(v % p);
}

static uint64_t field_add(uint64_t a, uint64_t b) {
	return field_reduce((__int128)a + (__int128)b);
}

static uint64_t field_sub(uint64_t a, uint64_t b) {
	return (a + MOD_PRIME - b) % MOD_PRIME;
}

static uint64_t field_mul(uint64_t a, uint64_t b) {
	return field_reduce((__int128)a * (__int128)b);
}

static uint64_t field_inverse(uint64_t a) {
	uint64_t exponent = MOD_PRIME - 2;
      	uint64_t result = 1;
      	uint64_t base = a;


	for (int i = 0; i < 61; ++i) {
      		uint64_t bit = (exponent >> i) & 1;

      		uint64_t dummy_mul = field_mul(result, base);

      		uint64_t mask = -(uint64_t)bit;
      		result = (result & ~mask) | (dummy_mul & mask);

      		base = field_mul(base, base);

	}
	return result;
}

bool lagrange_recover_secret(const Share* shares, size_t n, uint64_t* out_secret) {
	if (!shares || !out_secret || n == 0) return false;	

	for (size_t i = 0; i < n; ++i) {
      		for (size_t j = i + 1; j < n; ++j) {
      			if (shares[i].x == shares[j].x) return false;
      		}
      	}
	uint64_t total = 0;
      		
	for (size_t i = 0; i < n; ++i) {
      		uint64_t term = shares[i].y;

      		for (size_t j = 0; j < i; ++j) {

      			uint64_t denominator = field_sub(shares[i].x, shares[j].x);
      			uint64_t numerator = field_sub(0, shares[j].x);

      			term = field_mul(term, field_mul(numerator, field_inverse(denominator)));
      		}
		for (size_t j = i + 1; j < n; ++j) {
     			uint64_t denominator = field_sub(shares[i].x, shares[j].x);
     			uint64_t numerator = field_sub(0, shares[j].x);
     			term = field_mul(term, field_mul(numerator, field_inverse(denominator)));
     		}
      		total = field_add(total, term);
	}

	*out_secret = total;
	return true;
}


