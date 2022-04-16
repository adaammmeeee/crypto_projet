#include "lib.h"
#include <string.h>

int square_and_multiply(mpz_t result, mpz_t a, mpz_t n, mpz_t h)
{
    mpz_set(result, a);
    char *buffer = malloc(sizeof(char) * 1024);
    mpz_get_str(buffer, 2, h);

    /* La boucle est inversé par rapport à l'algorithme car pour un nb binaire 1100 par exemple,
       L'élément 0 du tableau est 1100
                                  ^
       On parcourt donc en commençant par le bit de poid fort comme dans l'algorithme
    */
    for (int i = 1; i < strlen(buffer); i++)
    {
        mpz_mul(result, result, result);
        mpz_mod(result, result, n);
        if (buffer[i] == '1')
        {
            mpz_mul(result, result, a);
            mpz_mod(result, result, n);
        }
    }
    free(buffer);
    return 0;
}

int test_fermat(mpz_t n, int k)
{
    mpz_t alea, test, n_sub_1, n_sub_3;

    mpz_inits(test, alea, n_sub_3, n_sub_1, NULL);

    mpz_sub_ui(n_sub_3, n, 3);
    mpz_sub_ui(n_sub_1, n, 1);
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    // Initialisation de state

    for (int i =0; i<k;i++)
    {

        mpz_urandomm(alea, state, n_sub_3); // On assigne à alea une valeur aléatoire entre 0 et n-3 inclu donc 0 <= alea <= n-4 ou 0 <= alea < n-3
        mpz_add_ui(alea, alea, 2);          // On ajoute 2 a aléa pour avoir 1 < alea < n-1
        gmp_printf("le nb aléatoire tiré est %Zd\n", alea);

        square_and_multiply(test, alea, n, n_sub_1);
        gmp_printf("le résultat de %Zd^%Zd mod %Zd est %Zd \n", alea, n_sub_1, n, test);

        if (mpz_cmp_ui(test, 1) != 0) // si test et 1 sont différent
        {
            gmp_printf("%Zd est composé\n", n);
            mpz_clears( test, alea, n_sub_1, n_sub_3, NULL);
            gmp_randclear(state);

            return 0;
        }
    }
    gmp_printf("%Zd est potentiellement premier\n", n);
    mpz_clears(test, alea, n_sub_1, n_sub_3, NULL);
    gmp_randclear(state);

    return 1;
}
