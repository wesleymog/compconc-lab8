/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Descricao: implementa  o problema dos leitores/escritores usando variaveis de condicao da biblioteca Pthread
*/

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <semaphore.h>


#define L 4 //numero de threads leitoras
#define E 2 //numero de threads escritoras

//variaveis do problema

//sem_t em_e, em_l, escr, leit; //semaforos
int e=0, l=0; //globais

//variaveis para sincronizacao
pthread_mutex_t mutex;
pthread_cond_t cond_leit, cond_escr;

//thread leitora
void * leitor (void * arg) {
  int *id = (int *) arg;
  while(1) {
    pthread_mutex_lock(&mutex);
    printf("L[%d] quer ler\n", *id);
    while(e > 0) {
        printf("L[%d] bloqueou\n", *id);
        pthread_cond_wait(&cond_leit, &mutex);
        printf("L[%d] desbloqueou\n", *id);
    }
    l++;
    pthread_mutex_unlock(&mutex);
    printf("L[%d] lendo\n", *id);
    sleep(1);
    printf("L[%d] finalizando leitura\n", *id);
    pthread_mutex_lock(&mutex);
    printf("L[%d] terminou de ler\n", *id);
    l--;
    if(l==0) pthread_cond_signal(&cond_escr);
    pthread_mutex_unlock(&mutex);
  } 
  free(arg);
  pthread_exit(NULL);
}

//thread leitora
void * escritor (void * arg) {
  int *id = (int *) arg;
  while(1) {
    pthread_mutex_lock(&mutex);
    printf("E[%d] quer escrever\n", *id);
    while(e>0) {
     printf("E[%d] bloqueou\n", *id);
     pthread_cond_wait(&cond_escr, &mutex);
     printf("E[%d] desbloqueou\n", *id);
   }
   e++;
   printf("E[%d] escrevendo\n", *id);
   sleep(1);
   printf("E[%d] terminou de escrever\n", *id);
   e--;
   pthread_cond_signal(&cond_escr);
   pthread_cond_broadcast(&cond_leit);
   pthread_mutex_unlock(&mutex);
   sleep(5);
  } 
  free(arg);
  pthread_exit(NULL);
}

//funcao principal
int main(void) {
  //identificadores das threads
  pthread_t tid[L+E];
  int id[L+E];

  //inicializa as variaveis de sincronizacao
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond_leit, NULL);
  pthread_cond_init(&cond_escr, NULL);
  /*
  sem_init(&em_e,0,1);
  sem_init(&em_l,0,1);
  sem_init(&leit,0,1);
  sem_init(&escr,0,1);
  */
  //cria as threads leitoras
  for(int i=0; i<L; i++) {
    id[i] = i+1;
    if(pthread_create(&tid[i], NULL, leitor, (void *) &id[i])) exit(-1);
  } 
  
  //cria as threads escritoras
  for(int i=0; i<E; i++) {
    id[i+L] = i+1;
    if(pthread_create(&tid[i+L], NULL, escritor, (void *) &id[i+L])) exit(-1);
  } 

  pthread_exit(NULL);
  return 0;
}
