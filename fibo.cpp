
#include "header.hpp"

using namespace std;

void *fibo(void *dta)
{
    int *n = (int *)dta;
    int *n1, *n2,
        *r = (int *)malloc(sizeof(int)),
        *r1, *r2;
    struct Atrib a1, a2;

    if (*n <= 2)
        *r = 1;
    else
    {
        contador++;
        n1 = (int *)malloc(sizeof(int));
        *n1 = *n - 1;
        a1.p = 0;
        a1.c = *n1;
        spawn(&a1, fibo, (void *)n1);
        n2 = (int *)malloc(sizeof(int));
        *n2 = *n - 2;
        a2.p = 0;
        a1.c = *n2;
        spawn(&a2, fibo, (void *)n2);

        sync(contador, &r1);
        contador++;
        sync(contador, &r2);
        *r = *r1 + *r2;

        free(r1);
        free(r2);
        free(n1);
        free(n2);
    }
    return r;
}

int main()
{
    clock_t begin = clock();
    /* here, do your time-consuming job */

    int t1, *r, resultado;
    struct Atrib a1;
    a1.c = 0;
    a1.p = 0;
    int numeroFibonacci = 9;

    t1 = spawn(&a1, fibo, &numeroFibonacci);

    if (!start(qtdPVs))
    {
        cout << "Criação de Processadores virtuais falhou" << endl;
        exit(EXIT_FAILURE);
    }
    resultado = sync(0, &r);

    finish();
    cout << "Resultado final: ";
    cout << resultado << endl;

    /* here, do your time-consuming job */

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    cout << time_spent << endl;
    return 0;
}