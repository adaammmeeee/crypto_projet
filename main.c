#include "lib.h"

int main()
{
    mpz_t a, n, h, r;
    mpz_init_set_str(a, "12", 10);
    mpz_init_set_str(h, "24", 10);
    mpz_init_set_str(n, "4", 10);
    mpz_init(r);

    square_and_multiply(r, a, n, h);
    gmp_printf("%Zd\n", r);

    mpz_t k;
    mpz_init_set_str(k, "10", 10);
    test_fermat(n, k);

    mpz_clears(a,n,h,r,k,NULL);


    return 0;
}
