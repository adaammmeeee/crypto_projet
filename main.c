#include "lib.h"

int main()
{
    mpz_t a, n, h, r;
    mpz_init_set_str(a, "12", 10);
    mpz_init_set_str(h, "24", 10);
    mpz_init_set_str(n, "15", 10);
    mpz_init(r);

    square_and_multiply(a, a, n, h);
    //gmp_printf("%Zd\n", a);
    test_miller_rabin(n, 11);

    mpz_clears(a,n,h,r,NULL);


    return 0;
}
