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

int test_fermat(mpz_t n, mpz_t k)
{
    mpz_t cpt, alea, test, n_sub_1, n_sub_2;

    mpz_inits(test, alea, n_sub_2, n_sub_1, NULL);
    mpz_init_set_str(cpt, "1", 10);

    mpz_sub_ui(n_sub_2, n, 2);
    mpz_sub_ui(n_sub_1, n, 1);
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    // Initialisation de state

    while (mpz_cmp(cpt, k))
    {

        mpz_urandomm(alea, state, n_sub_2); // On assigne à alea une valeur aléatoire entre 0 et n-3 inclu donc 0 <= alea <= n-3 ou 0 <= alea < n-2
        mpz_add_ui(alea, alea, 1);          // On ajoute 1 a aléa pour avoir 1 < alea < n-1
        gmp_printf("le nb aléatoire tiré est %Zd\n", alea);

        square_and_multiply(test, alea, n, n_sub_1);
        gmp_printf("le résultat de %Zd^%Zd mod %Zd est %Zd \n", alea, n_sub_1, n, test);

        if (mpz_cmp_ui(test, 1) != 0) // si test et 1 sont différent
        {
            gmp_printf("%Zd est composé\n", n);
            mpz_clears(cpt, test, alea, n_sub_1, n_sub_2, NULL);
            gmp_randclear(state);

            return 0;
        }
        mpz_add_ui(cpt, cpt, 1); // Incrémentation du compteur
    }
    gmp_printf("%Zd est potentiellement premier\n", n);
    mpz_clears(cpt, test, alea, n_sub_1, n_sub_2, NULL);
    gmp_randclear(state);

    return 1;
}
