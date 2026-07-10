#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include "laGrange_2.h"

#define MOD_PRIME 2305843009213693951ULL

uint64_t get_secure_random() {
	uint64_t rand_val = 0;
	FILE *fp = fopen("/dev/urandom", "rb");
	if (!fp) {
		fprintf(stderr, "Fatal Error: Cannot access /dev/urandom. System is compromised.\n");
		exit(EXIT_FAILURE);
	}
	if (fread(&rand_val, sizeof(rand_val), 1, fp) != 1) {
		fprintf(stderr, "Fatal Error: Entropy pool exhausted.\n");
		fclose(fp);
		exit(EXIT_FAILURE);
	}
	fclose(fp);
	return rand_val;
}

uint64_t evaluate_polynomial(uint64_t x, uint64_t secret) {
	static uint64_t a1 = 0, a2 = 0;
	static bool initialized = false;


	if (!initialized) {
		a1 = get_secure_random() % MOD_PRIME;
		a2 = get_secure_random() % MOD_PRIME;
		initialized = true;


		printf("[Architect's Log] Generated Polynomial: f(x) = %" PRIu64 " + %" PRIu64 "x + %" PRIu64 "x^2 (mod P)\n", secret, a1, a2);
	}

	uint64_t result = secret;
	uint64_t term1 = (uint64_t)(((__int128)a1 * x) % MOD_PRIME);

	uint64_t x_squared = (uint64_t)(((__int128)x * x) % MOD_PRIME);
	uint64_t term2 = (uint64_t)(((__int128)a2 * x_squared) % MOD_PRIME);

	result = (result + term1) % MOD_PRIME;
	result = (result + term2) % MOD_PRIME;

	return result;
}

int main(void) {
	const size_t TOTAL_SHARES = 5;
	const size_t REQUIRED_SHARES = 3;

	Share tx_shares[TOTAL_SHARES];
	for (size_t i = 0; i < TOTAL_SHARES; ++i) {
      		tx_shares[i].x = (uint64_t)(i + 1);
		tx_shares[i].y = evaluate_polynomial(tx_shares[i].x, 42ULL);
	}

	Share rx_shares[REQUIRED_SHARES];
	rx_shares[0] = tx_shares[0];
	rx_shares[1] = tx_shares[2];
	rx_shares[2] = tx_shares[4];
	

	uint64_t recovered_secret = 0;

	if (lagrange_recover_secret(rx_shares, REQUIRED_SHARES, &recovered_secret)) {
		printf("Recovered Secret: %" PRIu64 "\n", recovered_secret);
		

		explicit_bzero(&recovered_secret, sizeof(recovered_secret));

		explicit_bzero(rx_shares, sizeof(rx_shares));

		explicit_bzero(tx_shares, sizeof(tx_shares));

		printf("[Architect's Log} Zeroization complete. All sensitive data securely wiped from RAM.\n");

		return EXIT_SUCCESS;

	} else {
		fprintf(stderr, "Error: Protocol failed.\n");
		return EXIT_FAILURE;
	}
}


	
