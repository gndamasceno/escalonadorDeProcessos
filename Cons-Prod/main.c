#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>


typedef struct {
   int *buffer;
   int bsize, ppos, cpos;
   
   pthread_mutex_t m;
   sem_t plivre, pocupada;
} Input;


void *cons(void *arg);
void *prod(void *arg);

int main(int argc, char **argv){
  Input dta;
  pthread_t *vcons, *vprod;
  int i, nprod = 2, ncons = 2;

  dta.bsize = atoi(argv[1]);
  nprod     = atoi(argv[2]);
  ncons     = atoi(argv[3]);

  if( dta.bsize > 10000 || dta.bsize < 100 ) dta.bsize = 1000;
  if( nprod > 10 || nprod < 2 ) nprod = 2;
  if( ncons > 10 || ncons < 2 ) ncons = 2;

  printf("Usando: Buffer[%d], Produtores[%d], Consumidores [%d]\n",
          dta.bsize, nprod, ncons );
  
  vcons = (pthread_t*) malloc(ncons*sizeof(pthread_t));
  vprod = (pthread_t*) malloc(nprod*sizeof(pthread_t));
  dta.buffer = (int*) malloc(dta.bsize*sizeof(int));
  dta.ppos = dta.cpos = 0;

  srand((unsigned) time(NULL));
  
  pthread_mutex_init(&dta.m, NULL);
  sem_init(&dta.plivre, 0, dta.bsize);
  sem_init(&dta.pocupada, 0, 0);
  
  for(i = 0; i < ncons; ++i )
    pthread_create(&(vcons[i]), NULL, cons, (void*)&dta);
  for(i = 0; i < nprod; ++i )
    pthread_create(&(vprod[i]), NULL, prod, (void*)&dta);
  for(i = 0; i < ncons; ++i )
    pthread_join(vcons[i], (void*)&dta);
  for(i = 0; i < nprod; ++i )
    pthread_join(vprod[i], (void*)&dta);
  
  return 0;
}

void *prod(void *arg){
  Input *dta = (Input*) arg;
  int n = (int)(rand() * 1000.0);
  while(1){
    ++n;
    sem_wait(&(dta->plivre));
    pthread_mutex_lock(&(dta->m));
    dta->buffer[dta->ppos] = n;
    dta->ppos = (dta->ppos + 1) % dta->bsize;
    printf("Produzindo numero %d\n", n);
    pthread_mutex_unlock(&(dta->m));
    sem_post(&(dta->pocupada));
    sleep(1);
  }
}

void *cons(void *arg){
  Input *dta = (Input*) arg;
  int n;
  while(1){
    sem_wait(&(dta->pocupada));
    pthread_mutex_lock(&(dta->m));
    n = dta->buffer[dta->cpos];
    dta->cpos = (dta->cpos+1) % dta->bsize;
    printf("Consumindo numero %d\n", n);
    pthread_mutex_unlock(&(dta->m));
    sem_post(&(dta->plivre));
    sleep(1);
  }
}
