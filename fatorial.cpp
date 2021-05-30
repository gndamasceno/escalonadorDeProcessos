
#include "header.hpp"

using namespace std;

void *fatorial(void *dta)
{
    int *n = (int *)dta;
    int *n1,
        *r = (int *)malloc(sizeof(int)),
        *r1;
    struct Atrib a1;

    if (*n <= 1)
        *r = 1;
    else
    {
        contador++;
        n1 = (int *)malloc(sizeof(int));
        *n1 = *n - 1;
        a1.p = 0;
        a1.c = *n1;
        spawn(&a1, fatorial, (void *)n1);

        sync(contador, &r1);
        contador++;

        *r = (*n) * (*r1);

        free(r1);
        free(n1);
    }
    return r;
}

int main(int argc, char *argv[])
{

    if (atoi(argv[1]) < 0)
    {
        cout << "Argumento inválido";
        exit(EXIT_FAILURE);
    }

    int t1, *r, resultado;
    struct Atrib a1;
    a1.c = 0;
    a1.p = 0;
    int numeroFatorial = atoi(argv[1]);

    t1 = spawn(&a1, fatorial, &numeroFatorial);

    if (!start(qtdPVs))
    {
        cout << "Criação de Processadores virtuais falhou" << endl;
        exit(EXIT_FAILURE);
    }
    resultado = sync(0, &r);

    finish();
    cout << "Fatorial de " << numeroFatorial << " igual a ";
    cout << resultado << endl;
}