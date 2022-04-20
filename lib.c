#include "lib.h"
#include <string.h>
#include <ctype.h>

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

    for (int i = 0; i < k; i++)
    {

        mpz_urandomm(alea, state, n_sub_3); // On assigne à alea une valeur aléatoire entre 0 et n-3-1 inclu donc 0 <= alea <= n-4 ou 0 <= alea < n-3
        mpz_add_ui(alea, alea, 2);          // On ajoute 2 a aléa pour avoir 1 < alea < n-1
        gmp_printf("le nb aléatoire tiré est %Zd\n", alea);

        square_and_multiply(test, alea, n, n_sub_1);
        gmp_printf("le résultat de %Zd^%Zd mod %Zd est %Zd \n", alea, n_sub_1, n, test);

        if (mpz_cmp_ui(test, 1) != 0) // si test et 1 sont différent
        {
            gmp_printf("%Zd est composé\n", n);
            mpz_clears(test, alea, n_sub_1, n_sub_3, NULL);
            gmp_randclear(state);

            return 0;
        }
    }
    gmp_printf("%Zd est potentiellement premier\n", n);
    mpz_clears(test, alea, n_sub_1, n_sub_3, NULL);
    gmp_randclear(state);

    return 1;
}

int test_miller_rabin(mpz_t n, int k)
{
    // Première étape on décompose n-1 = (2^s)*t ou t est impair
    mpz_t n_sub_1;
    mpz_t t;
    mpz_t s;
    mpz_t s_sub_1;

    mpz_t reste;

    mpz_inits(s, n_sub_1, t, reste, s_sub_1, NULL);
    mpz_sub_ui(n_sub_1, n, 1); // n-1
    mpz_set(t, n_sub_1);       // t := n-1
    mpz_set_ui(s, 0);          // s := 0
    mpz_set_ui(reste, 0);      // reste := 0

    while (mpz_cmp_ui(reste, 0) == 0) // Tant que le reste est égal à 0
    {
        mpz_add_ui(s, s, 1);     // s:= s+1
        mpz_div_ui(t, t, 2);     // t:= t/2
        mpz_mod_ui(reste, t, 2); // on place le reste de la division t/2 dans reste
    }
    mpz_sub_ui(s_sub_1, s, 1);

    // On a fini la décomposition
    // On peut faire la boucle for maintenant

    mpz_t alea;
    mpz_init(alea);
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    mpz_t y;
    mpz_init(y);

    for (int i = 1; i < k; i++)
    {
        mpz_urandomm(alea, state, n_sub_1); // On assigne à alea une valeur aléatoire entre 0 et n-2 inclu donc 0 <= alea <= n-2
        mpz_add_ui(alea, alea, 1);          // On a ajoute 1 à aléa on a 1 <= alea <= n-1  ou 0 < alea < n
        //gmp_printf("a = %Zd\n", alea);

        square_and_multiply(y, alea, n, t); // y := alea^t mod n

        if ( (mpz_cmp_ui(y, 1) != 0) && (mpz_cmp(y, n_sub_1) != 0) ) // y = n-1 mod n = -1 mod n
        {
            mpz_t j;
            mpz_init_set_str(j, "1", 10);

            while (mpz_cmp(j, s_sub_1) <= 0) // Tant que j<s-1
            {
                mpz_t two;
                mpz_init_set_str(two, "2", 10);
                square_and_multiply(y, y, n, two);
                if (mpz_cmp_ui(y, 1) == 0)
                {
                    printf("composé\n");
                    mpz_clears(s, alea, y, j, n_sub_1, t, reste, s_sub_1, NULL);
                    gmp_randclear(state);
                    return 0;
                }
                if (mpz_cmp(y, n_sub_1) == 0) //y = n-1 mod n = -1 mod n
                {
                    goto continuer;
                }
                mpz_add_ui(j, j, 1);
            }
            mpz_clears(s, alea, y, j, n_sub_1, t, reste, s_sub_1, NULL);
            gmp_randclear(state);
            printf("composé\n");
            return 0;
        }
    continuer:;
    }
    mpz_clears(s, alea, y, n_sub_1, t, reste, s_sub_1, NULL);
    gmp_randclear(state);
    printf("premier\n");
    return 1;
}