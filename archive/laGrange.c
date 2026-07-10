#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define EPSILON 1e-9

typedef struct {
	double x;
	double y;
} Share;

bool lagrange_interpolate(const Share* shares, size_t n, double x_target, double* out_result) {

	if (!shares || !out_result || n == 0) {
		return false;
	}
	
	double total = 0.0;

	for (size_t i = 0; i < n; ++i) {
		double term = shares[i].y;
      		
      		for (size_t j = 0; j < n; ++j) {
      			if (i == j) continue;
      			
      			double denominator = shares[i].x - shares[j].x;

      			if (fabs(denominator) < EPSILON) {
      				return false;
      			}
      			
      			term *= (x_target - shares[j].x) / denominator;
      		}
		total += term;
	}

	*out_result = total;
	return true;
}

double evaluate_polynomial(double x) {
	const double coeffs[] = {42.0, 15.0, 3.0};
	const size_t degree = sizeof(coeffs) / sizeof(coeffs[0]);

	double result = 0.0;
	double x_pow = 1.0;

	for (size_t i = 0; i < degree; ++i) {
      		result += coeffs[i] * x_pow;
		x_pow *= x;
	}
	return result;
}

int main(void) {
	 
	 const size_t TOTAL_SHARES = 5;
	 const size_t REQUIRED_SHARES = 3;

	 Share tx_shares[TOTAL_SHARES];
	 for (size_t i = 0; i < TOTAL_SHARES; ++i) {
	 	tx_shares[i].x = (double)(i + 1);
		tx_shares[i].y = evaluate_polynomial(tx_shares[i].x);
	 }

	Share rx_shares[REQUIRED_SHARES];
	rx_shares[0] = tx_shares[0];
	rx_shares[1] = tx_shares[2];
	rx_shares[2] = tx_shares[4];


	double recovered_secret = 0.0;

	if (lagrange_interpolate(rx_shares, REQUIRED_SHARES, 0.0, &recovered_secret)) {
	 	printf("Recovered Secret: %.0f\n", recovered_secret);
		return EXIT_SUCCESS;
	} else {
	 	fprintf(stderr, "Error: Interpolation failed due to invalid data.\n");
		return EXIT_FAILURE;

	}
}
