#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>

// result prend la valeur a^h mod n
int square_and_multiply(mpz_t result, mpz_t a, mpz_t n, mpz_t h);


// Renvoi 0 si n est composé et 1 si n potentiellement premier
int test_fermat(mpz_t n, int k);

// Renvoi 0 si n est composé et 1 si n potentiellement premier
int test_miller_rabin(mpz_t n, int k);
