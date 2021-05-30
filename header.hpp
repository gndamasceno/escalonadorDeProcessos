#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <iostream>
#include <unistd.h>
#include <list>
#include <deque>
/*
Esta primitiva lança o núcleo de execução, instanciando m processadores virtuais, indicados pelo parâmetro m. O retorno 0 (zero) indica falha na instanciação dos processadores virtuais. Um valor maior que 0 indica criação bem sucedida.
*/
int start(int m);

/*
Esta primitiva é bloqueante, retornando após todos os processadores virtuais terem finalizado.
*/
void finish();

/*
A primitiva spawn lança a execução da tarefa descrita no ponteiro para função indicada pelo parâmetro t. O parâmetro para a função *t é descrito no parâmetro dta. O parâmetro struct Atrib* atrib descreve os atributos a serem considerados no escalonamento da tarefa. A função retorna 0 (zero) em caso de falha na criação da tarefa ou um valor inteiro positivo maior que 0, considerado o identificador único da tarefa no programa. Caso NULL seja passado como endereço para atrib, devem ser considerados os valores default para os atributos. Usem sempre NULL, desconsiderem este parâmetro.
*/
int spawn(struct Atrib *atrib, void *(*t)(void *), void *dta);

/*
A primitiva sync é bloqueante: a tarefa que invoca a primitiva sync informa qual tarefa, identificada no parâmetro tId, que deve ser sincronizada. O retorno da primitiva  é 0 (zero) em caso de falha ou 1 (um), em caso de sincronização bem sucedida. O parâmetro res contém, como saída, o endereço de memória que contém os resultados de saída. Importante observar: uma tarefa somente pode ser sincroniza uma única vez. Não é permitido múltiplos syncs de uma mesma tarefa.
*/
int sync(int tId, int **res);

void *exemploTarefa(void *p);

/*
Procura por tarefas para executar
*/
void *MeuPV(void *dta);

/*
struct Atrib: descreve atributos a serem considerados para escalonamento da tarefa. O exemplo abaixo é uma configuração mínima, podendo ser adicionados outros atributos.O campo p representa a prioridade da tarefa e o campo c o seu custo computacional. Note: o custo computacional deverá ser indicado pelo programador.
*/
struct Atrib
{
    int p;
    int c;
} typedef Atrib;

struct Trabalho
{
    int tid;
    void *(*f)(void *);
    int numFib; // Parâmetro de entrada para f, no caso o número que é  fibonacci
    int res;    // Retorno de f
} typedef Trabalho;

//Identifica a tarefa
static int contador = 0;

//Quantidade de Processadores virtuais
static int qtdPVs = 100;