// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 1.1 -- Julho de 2016

// Mudanças feitas para o trabalho.
// PROFESSOR: Marco Aurélio Wehrmeister
// DISCIPLINA: Sistemas Operacionais - S73
// ALUNOS: Ricardo Henrique Pires dos Reis, Maria Gabriela Rodrigues Policarpo


// Estruturas de dados internas do sistema operacional

#ifndef __PPOS_DATA__
#define __PPOS_DATA__

#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>		// biblioteca POSIX de trocas de contexto
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "queue.h"			// biblioteca de filas genéricas

// Estrutura que define um Task Control Block (TCB)
typedef struct task_t
{
	struct task_t *prev, *next;		// ponteiros para usar em filas
	int id;				            // identificador da tarefa
	ucontext_t context;			    // contexto armazenado da tarefa
	unsigned char state;            // indica o estado de uma tarefa (ver defines no final do arquivo ppos.h): 
									// n - nova, r - pronta, x - executando, s - suspensa, e - terminada
	struct task_t* queue;
	struct task_t* joinQueue;
	int exitCode;
	unsigned int awakeTime;         // usado para armazenar a hora em que deve ser acordado

	// ...(outros campos deve ser adicionados APÓS esse comentário)
	int eet;						// Tempo estimado de execução da tarefa
	int ret;						// Tempo restante para terminar a execução da tarefa
	int running_time;				// Indica o tempo que a tarefa está rodando
	int quantum;					// Indica o tempo que a tarefa está no processador
	int execution_time;
	int processing_time;
	int activations; 
} task_t;

//**********************************************************

// estrutura que define um semáforo
typedef struct {
	struct task_t *queue;
	int value;

	unsigned char active;
} semaphore_t;

// estrutura que define um mutex
typedef struct {
	struct task_t *queue;
	unsigned char value;

	unsigned char active;
} mutex_t;

// estrutura que define uma barreira
typedef struct {
	struct task_t *queue;
	int maxTasks;
	int countTasks;
	unsigned char active;
	mutex_t mutex;
} barrier_t;

// estrutura que define uma fila de mensagens
typedef struct {
	void* content;
	int messageSize;
	int maxMessages;
	int countMessages;
	
	semaphore_t sBuffer;
	semaphore_t sItem;
	semaphore_t sVaga;
	
	unsigned char active;
} mqueue_t;

#endif