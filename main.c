#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "header.h"

/*
struct Atrib: descreve atributos a serem considerados para escalonamento da tarefa. O exemplo abaixo é uma configuração mínima, podendo ser adicionados outros atributos.O campo p representa a prioridade da tarefa e o campo c o seu custo computacional. Note: o custo computacional deverá ser indicado pelo programador.
*/
struct Atrib {

int p;

int c;

};


