
#include "header.hpp"

/*
Bibliotecas que poderão ser úteis
#include <stack>
#include <queue>
*/
using namespace std;

//nt *i = (int*) malloc (sizeof(int));

//struct Trabalho *ListaTrabalhosProntos, *ListaTrabalhosTerminados;
//Lista TrabalhoProntos e trabalhos terminados
struct deque<struct Trabalho *> listaTrabalhosProntos, listaTrabalhosTerminados;
deque<struct Trabalho *>::iterator iteratorList;

//Lista de PVs:
static pthread_t *pvs;

// Fim
static bool Fim = false;
pthread_mutex_t lock, lock2, lock3 = PTHREAD_MUTEX_INITIALIZER;

void armazenaResultados(Trabalho *t)
{
  listaTrabalhosTerminados.push_front(t);
}

void *MeuPV(void *dta)
{
  int *recebeResultadoFuncao;
  Trabalho *t;
  pthread_t tread_aux;

  pthread_mutex_lock(&lock);
  while (Fim == false && !listaTrabalhosProntos.empty())
  {
    t = listaTrabalhosProntos.front();

    listaTrabalhosProntos.pop_front();

    recebeResultadoFuncao = (int *)t->f((void *)&(t->numFib));

    t->res = *recebeResultadoFuncao;

    listaTrabalhosTerminados.push_front(t);
  }

  pthread_mutex_unlock(&lock);

  return NULL;
}

int start(int qtdpvs)
{
  int thread_status;
  pvs = (pthread_t *)malloc(qtdpvs * sizeof(pthread_t));
  Fim = false;
  for (int i = 0; i < qtdpvs; i++)
  {
    thread_status = pthread_create(&(pvs[i]), NULL, MeuPV, NULL);
    if (thread_status != 0)
    {
      // Seguindo documentação do trabalho, se ocorrer erro na criação retorna 0;
      return 0;
    }
  }
  return 1; // Criação bem-sucedida
}

void finish()
{
  int thread_status;
  Fim = true;
  for (int i = 0; i < qtdPVs; i++)
  {
    thread_status = pthread_join(pvs[i], NULL);
    if (thread_status != 0)
    {
      // Caso ocorra erro no join da threads;
      printf("Erro ao finalizar Processadores Virtuais.\n");
      exit(EXIT_FAILURE);
    }
  }
  free(pvs);
}

int idTarefa = -1;

int spawn(struct Atrib *atrib, void *(*t)(void *), void *dta)
{
  int *x = (int *)dta;
  idTarefa++;

  struct Trabalho *trab;
  trab = (Trabalho *)malloc(sizeof(Trabalho));
  if (trab == NULL)
  {
    return 0;
  }
  trab->tid = idTarefa;
  trab->f = t;
  trab->numFib = *x;
  trab->res = 0;
  listaTrabalhosProntos.push_front(trab);

  return 1;
}

int wait;

int sync(int tId, int **res)
{
  bool tarefaEstaEmListaTrabalhosProntos = false;
  bool tarefaEstaEmListaTrabalhosTerminados = false;
  int resp, ResultadoTarefa;
  int x = 1;
  int *n;
  void *result;
  Trabalho *aux, *trabalhoPronto;
  for (int i = 0; i < listaTrabalhosProntos.size(); i++)
  { //caso 1
    if (tId == listaTrabalhosProntos.at(i)->tid)
    {
      //tarefa na lista de prontas
      tarefaEstaEmListaTrabalhosProntos = true;
      trabalhoPronto = listaTrabalhosProntos.at(i);
      listaTrabalhosProntos.erase(listaTrabalhosProntos.begin() + i);
      result = (int *)trabalhoPronto->f((void *)&(trabalhoPronto->numFib));
      *res = (int *)result;
      trabalhoPronto->res = **res;

      return **res;
    }
  }
  if (!tarefaEstaEmListaTrabalhosProntos)
  {
    //caso 2: verifica se está em listaTrabalhosTerminados e a retira

    for (int i = 0; i < listaTrabalhosTerminados.size(); i++)
    {
      if (tId == listaTrabalhosTerminados.at(i)->tid)
      {
        ResultadoTarefa = listaTrabalhosTerminados.at(i)->res;

        tarefaEstaEmListaTrabalhosTerminados = true;
        listaTrabalhosTerminados.erase(listaTrabalhosTerminados.begin() + i);

        return ResultadoTarefa;
      }
    }
  }

  if (!tarefaEstaEmListaTrabalhosTerminados && !tarefaEstaEmListaTrabalhosProntos)
  {
    //caso 3: tarefa esta sendo executada, esperar um pouco;
    for (wait = 0; wait < 3; wait++)
      ;
  }
  
  return 0;
}
