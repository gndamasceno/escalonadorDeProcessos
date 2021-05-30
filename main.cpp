
#include "header.hpp"

using namespace std;


//nt *i = (int*) malloc (sizeof(int));


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

int main()
{
    int t1, *r, resultadoFibonacci;
    struct Atrib a1;
    a1.c = 0;
    a1.p = 0;
    int numeroFibonacci = 9;

    t1 = spawn( &a1, fibo, &numeroFibonacci);
    
   
    if(!start(qtdPVs)){
      cout << "Criação de Processadores virtuais falhou" << endl;
      exit(EXIT_FAILURE);
    }
    resultadoFibonacci = sync (0, &r);
    
  
    finish();
    cout << "fibonacci("<< numeroFibonacci <<") = " << resultadoFibonacci;
   
}

void* MeuPV(void* dta) {
 int *recebeResultadoFuncao;
 Trabalho *t;

 pthread_mutex_lock(&lock);
 while (Fim == false && !listaTrabalhosProntos.empty()){
   t = listaTrabalhosProntos.front();
   cout << "MeuPv Executando tarefa " << t->tid << "..." <<endl;
   listaTrabalhosProntos.pop_front();

   recebeResultadoFuncao = (int*) t->f((void*) &(t->numFib));
   t->res = *recebeResultadoFuncao;

   listaTrabalhosTerminados.push_front(t);
   cout << "MeuPv terminou a execução da tarefa " << t->tid << "! Printando tarefas terminadas: ";
   printIdLista(listaTrabalhosTerminados);
 }
  pthread_mutex_unlock(&lock); 
  return NULL;
}

int start( int qtdpvs ) {
 int thread_status;
 pvs = (pthread_t *) malloc(qtdpvs*sizeof(pthread_t));
 Fim = false;
 for( int i = 0 ; i < qtdpvs ; i++ ){
   thread_status = pthread_create(&(pvs[i]),NULL, MeuPV , NULL);
   if(thread_status != 0)
      return 0;// Seguindo documentação do trabalho, se ocorrer erro na criação retorna 0;  ;
    
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

int sync( int tId, int **res ){
    
    cout << "sync foi chamado para executar a tarefa " << tId <<"..." << endl;
    bool tarefaEstaEmListaTrabalhosProntos = false;
    bool tarefaEstaEmListaTrabalhosTerminados = false;
    int  ResultadoTarefa;
    Trabalho *trabalhoPronto;
    cout << "sync percorrendo lista de prontos..." << endl;
    for(int i = 0; i < listaTrabalhosProntos.size(); i++){ //caso 1   
        if(tId == listaTrabalhosProntos.at(i)->tid){ //tarefa na lista de prontas
          cout << "tarefa " << tId << " está na lista de prontos! Executando..." << endl;
          tarefaEstaEmListaTrabalhosProntos = true;
        
          trabalhoPronto = listaTrabalhosProntos.at(i);
          listaTrabalhosProntos.erase(listaTrabalhosProntos.begin()+i);  
          *res = (int*) trabalhoPronto->f((void*) &(trabalhoPronto->numFib));
          trabalhoPronto->res = **res;
         
          
          cout << "Tarefa "<< tId << " executada com sucesso! Lista de tarefas terminadas: ";
          printIdLista(listaTrabalhosTerminados);
          cout << "Lista de tarefas prontas: ";
          printIdLista(listaTrabalhosProntos);
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
            return ResultadoTarefa;
          }
        }               
    } 
      
    if (!tarefaEstaEmListaTrabalhosTerminados && !tarefaEstaEmListaTrabalhosProntos){//caso 3: tarefa esta sendo executada, esperar um pouco;
        for (wait = 0; wait < 3; wait++)
            cout << "Tarefa " << tId << " pode estar sendo executada por meuPV. Esperando..." << endl;  
    }
    cout << "Chamada de tarefa incorreta!" << endl;
    return 0;
}


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

