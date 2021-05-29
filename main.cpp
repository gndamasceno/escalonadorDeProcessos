#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <iostream>
#include <unistd.h>
#include "header.hpp"
#include <list>
#include <deque>

/*
Bibliotecas que poderão ser úteis
#include <stack>
#include <queue>
*/
using namespace std;
/*
struct Atrib: descreve atributos a serem considerados para escalonamento da tarefa. O exemplo abaixo é uma configuração mínima, podendo ser adicionados outros atributos.O campo p representa a prioridade da tarefa e o campo c o seu custo computacional. Note: o custo computacional deverá ser indicado pelo programador.
*/
struct Atrib {
  int p;
  int c;
}typedef Atrib;

struct Trabalho {
   int tid;
   void* (*f)(void*); 
   int numFib; // Parâmetro de entrada para f, no caso o número que é  fibonacci
   int res; // Retorno de f
}typedef Trabalho;

//struct Trabalho *ListaTrabalhosProntos, *ListaTrabalhosTerminados;
//Lista TrabalhoProntos e trabalhos terminados
struct deque<struct Trabalho *> listaTrabalhosProntos, listaTrabalhosTerminados;
deque<struct Trabalho *>::iterator iteratorList;


//Lista de PVs:
static pthread_t *pvs;
static int qtdPVs = 50;
// Fim
static bool Fim = false;
pthread_mutex_t lock, lock2, lock3 =  PTHREAD_MUTEX_INITIALIZER;
//nt *i = (int*) malloc (sizeof(int));

void printIdLista(deque<struct Trabalho *> x){
    for (int i = 0; i < x.size(); i++){
      cout << x.at(i)->tid << " ";
      
    }
    cout << endl;
}

int contador = 0;
void* fibo( void* dta ) {
  int  *n = (int*)dta;
  cout << *n;
  int *n1, *n2,
      *r = (int *) malloc(sizeof(int)),
      *r1, *r2;
  struct Atrib a1, a2;

  if( *n <= 2 ) *r = 1;
  else {
    contador++;
    n1 = (int *) malloc(sizeof(int));
    *n1 = *n - 1;
    a1.p = 0; a1.c = *n1;
    spawn( &a1, fibo, (void*) n1 );
    n2 = (int *) malloc(sizeof(int));
    *n2 = *n - 2;
    a2.p = 0; a1.c = *n2;
    spawn( &a2, fibo, (void*) n2 );
    
    
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

void* exemploTarefa( void* p ){
  struct Atrib a2;
    a2.c = 0;
    a2.p = 0;


  int *n = (int*)p;
  int *n1, *n2, *r = (int *) malloc (sizeof (int)), *r1, *r2;
  if(contador >= 2) {
      cout << "exemploTarefa chamada, mas não foi feito spawn" << endl;
      *r = 1000;
  }
  else{
    cout << "exemploTarefa  chamada, fazendo mais spawn..." << endl;
    contador++;
    n1 = (int *) malloc (sizeof (int));
    *n1 = *n;
    n2 = (int *) malloc (sizeof (int));
    *n2 = *n;
    //cout << *n1 << " teste" << endl;
    
    spawn(&a2, exemploTarefa, (void*) n1);
    //spawn(&a2, exemploTarefa, (void*) n2);
    
 
    sync(contador, &r1);
    *r2 = *r1 + 1000;
    //sync(2, &r2);

    *r = *r1 + *r2; 
    //cout << "r1 tem valor de: " << *r1 << endl;
   
  
  }
  cout << "Resultado: " << *r << endl;
  return r;
}

int main()
{
    int t1, *r, opa;
    struct Atrib a1;
    a1.c = 0;
    a1.p = 0;
    int numeroFibonacci = 8;

 
    /*t1 = spawn( &a1, exemploTarefa, NULL);
    t1 = spawn( &a1, exemploTarefa, NULL);
    t1 = spawn( &a1, exemploTarefa, NULL);*/
    t1 = spawn( &a1, fibo, &numeroFibonacci);
    
   
    if(!start(qtdPVs)){
      cout << "Criação de Processadores virtuais falhou" << endl;
      exit(EXIT_FAILURE);
    }
    opa = sync (0, &r);
    
    //printIdLista(listaTrabalhosTerminados);
    finish();
    cout << opa;
    /*printIdLista(listaTrabalhosTerminados);
    sync(0, 100);*/

  
}

void armazenaResultados (Trabalho *t){
  listaTrabalhosTerminados.push_front(t);
}
void* MeuPV(void* dta) {
 int *n;
 void *result;
 int x = 2;
 Trabalho *t;
 pthread_t tread_aux;

 
 pthread_mutex_lock(&lock);
 while (Fim == false && !listaTrabalhosProntos.empty()){
   t = listaTrabalhosProntos.front();
    cout << "MeuPv Executando tarefa " << t->tid << "..." <<endl;
   listaTrabalhosProntos.pop_front();

   result = (int*) t->f((void*) &(t->numFib));
   n = (int*) result;
   t->res = *n;

   listaTrabalhosTerminados.push_front(t);
   cout << "MeuPv terminou a execução da tarefa " << t->tid << "! Printando tarefas terminadas: ";
   printIdLista(listaTrabalhosTerminados);
}
  
  pthread_mutex_unlock(&lock); 
  
  return NULL;

}

// eu crio os pv executando a função MeuPV, como o fim é false ele vai ficar rodando até ter algum trabalho na lista, qndo tiver um trabalho, ele deveria executar o trabalho. Como eu faria pra ele executar o fibonacci?
int start( int qtdpvs ) {
 int thread_status;
 pvs =(pthread_t *) malloc(qtdpvs*sizeof(pthread_t));
 Fim = false;
 for( int i = 0 ; i < qtdpvs ; i++ ){
   thread_status = pthread_create(&(pvs[i]),NULL, MeuPV , NULL);
   if(thread_status != 0)
    {
      // Seguindo documentação do trabalho, se ocorrer erro na criação retorna 0;
      return 0;
    }
 } 
 return 1; // Criação bem-sucedida
}

void finish() {
  int thread_status;
  Fim = true;
  for( int i = 0 ; i < qtdPVs;i++ ){
   thread_status = pthread_join(pvs[i], NULL);
   if(thread_status != 0)
    {
      // Caso ocorra erro no join da threads;
      printf ("Erro ao finalizar Processadores Virtuais.\n");
      exit(EXIT_FAILURE);
    }
  }
  free(pvs);
}

int idTarefa = -1;

int spawn( struct Atrib* atrib, void *(*t) (void *), void* dta ){
  int *x = (int*) dta;
  idTarefa++;
  cout << "Armazenando tarefa " << idTarefa << " na lista de Trabalhos Prontos..." << endl;
  struct Trabalho *trab;
  trab = (Trabalho *) malloc(sizeof(Trabalho));
  trab->tid = idTarefa;        
  trab->f = t;
  trab->numFib = *x;
  trab->res = 0;
  listaTrabalhosProntos.push_front(trab);
  cout << "Lista de Trabalhos Prontos: ";
  printIdLista(listaTrabalhosProntos);
  return 0;
}

int wait;

int sync( int tId, int **res ){
    //pthread_mutex_lock(&lock2); 
    cout << "sync foi chamado para executar a tarefa " << tId <<"..." << endl;
    bool tarefaEstaEmListaTrabalhosProntos = false;
    bool tarefaEstaEmListaTrabalhosTerminados = false;
    int resp, ResultadoTarefa;
    int x = 1;
    int *n;
    void *result;
    Trabalho *aux, *trabalhoPronto;
      //pthread_mutex_lock(&lock);  
      cout << "sync percorrendo lista de prontos..." << endl;
      for(int i = 0; i < listaTrabalhosProntos.size(); i++){ //caso 1   
        if(tId == listaTrabalhosProntos.at(i)->tid){ //tarefa na lista de prontas
          cout << "tarefa " << tId << " está na lista de prontos! Executando..." << endl;
          tarefaEstaEmListaTrabalhosProntos = true;
          trabalhoPronto = listaTrabalhosProntos.at(i);
          listaTrabalhosProntos.erase(listaTrabalhosProntos.begin()+i);  
          result = (int*) trabalhoPronto->f((void*) &(trabalhoPronto->numFib));
          *res = (int*) result;
          trabalhoPronto->res = **res;
         
          
          cout << "Tarefa "<< tId << " executada com sucesso! Lista de tarefas terminadas: ";
          printIdLista(listaTrabalhosTerminados);
          cout << "Lista de tarefas prontas: ";
          printIdLista(listaTrabalhosProntos);
          cout << "oii";
          //pthread_mutex_unlock(&lock2); 
          return **res;
        }
       
        
      }
      if (!tarefaEstaEmListaTrabalhosProntos){//caso 2: verifica se está em listaTrabalhosTerminados e a retira 
        cout << "Não está em prontos. Verificando se está em trabalhos terminados..." << endl;
        for (int i = 0; i < listaTrabalhosTerminados.size(); i++){
          if(tId == listaTrabalhosTerminados.at(i)->tid){
            ResultadoTarefa = listaTrabalhosTerminados.at(i)->res;
            cout << "Tarefa " << tId << " Está em trabalhos terminados, removendo.." << endl;
            tarefaEstaEmListaTrabalhosTerminados = true;
            listaTrabalhosTerminados.erase(listaTrabalhosTerminados.begin()+i);
            cout <<"Tarefa " << tId << " removida!" << endl;
            pthread_mutex_unlock(&lock2);  
            return ResultadoTarefa;
          }
        }
        
        
      } 
      
       if (!tarefaEstaEmListaTrabalhosTerminados && !tarefaEstaEmListaTrabalhosProntos){//caso 3: tarefa esta sendo executada, esperar um pouco;
          for (wait = 0; wait < 3; wait++)
            cout << "Tarefa " << tId << " pode estar sendo executada por meuPV. Esperando..." << endl;  
          pthread_mutex_unlock(&lock2);             
      }

    cout << "Chamada de tarefa incorreta!" << endl;
    pthread_mutex_unlock(&lock2);  
    return 0;
}


/*
possivel ordem de execução do programa
-main -
      - createPtreads(3)
      -               - MeuPV - id 1(fica rodando esperando algo aparecer)
      -               - spawn(fibo 2)
      -               -               - Coloca na lista de trabalhos prontos (fibo 4)
      -               - MeuPV-1
      -               -               - MeuPV-1 executa fibo 2
      -               -               -                       - MeuPV-1 chama Spawn (fibo 1)
      -               -               -                       -                      - TrabalhosProntos(fibo1)
      -               -               -                       -                      - MeuPV-2 Executa Fibo 1
      -               -               -                       -                      - TrabalhoTerminado(fibo1)
      -               -               -                       - MeuPV-1 chama Spawn (fibo 0)
      -               -               -                       -                      - TrabalhoProntos(fibo0)
      -               -               -                       -                      - MeuPV-3 Executa Fibo 0
      -               -               -                       -                      - TrabalhoTerminado(fibo0)
      -               -               -                       - MeuPV-1 sync fibo 1(res= 1)
      -               -               -                       - MeuPV-1 sync fibo 0(res= 1 - deveria pegar 0)
      -               - Como definir o fim???
-Retorna Resultado
-Exibe Resultado
-finish()
      -               -
Fim poderia ser definido com a propriedade profundidade?
Fim poderia ser definido com tempo ocioso da função MEUPV?
O programa rodaria a função finish?
  - 
função fibonacci do exemplo ta errada> fibo 0 é 0 e não 1
COMO MONTAR OS IDS
  sempre q ser feito uma chamada para um spawn adicionar +1 no id
OQ ACONTECE SE O NÚMERO DE PVS FOR MENOR QUE O NÚMERO DE CHAMADAS DE SPAWNS? (VAI SER NA MAIORIA DOS CASOS, CAUSANDO DEADLOCK)

*/

