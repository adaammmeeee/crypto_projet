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

    test_fermat(n, 10);

    mpz_clears(a,n,h,r,NULL);


    return 0;
}
