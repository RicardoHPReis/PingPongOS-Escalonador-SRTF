#include "ppos.h"
#include "ppos-core-globals.h"

// PROFESSOR: Marco Aurélio Wehrmeister
// DISCIPLINA: Sistemas Operacionais - S73
// ALUNOS: Ricardo Henrique Pires dos Reis, Maria Gabriela Rodrigues Policarpo

// ****************************************************************************
// Coloque aqui as suas modificações, p.ex. includes, defines variáveis, 
// estruturas e funções
#include <sys/time.h>
#include <signal.h>

#define QUANTUM 20
// #define DEBUG 1

struct sigaction action;
struct itimerval timer;
int tempo = 0;

void task_set_eet (task_t *task, int et)
{
	if (task == NULL)
	{
		taskExec->eet = et;
	}
	else
	{
		task->eet = et;
		task->ret = et;
	}
}

int task_get_eet(task_t *task) {
    if (task == NULL)
		return taskExec->eet;
	
    return task->eet;
}


int task_get_ret(task_t *task) {
    if (task == NULL)
	{
		return taskExec->ret;
	}

    return task->ret;
}

void verificaStatusTask(task_t *task)
{
	printf("id: %d\n", task->id);
	printf("state: %c\n", task->state);
	printf("exitCode: %d\n", task->exitCode);
	printf("awakeTime: %c\n", task->awakeTime);
	printf("eet: %c\n", task->eet);
	printf("ret: %c\n", task->ret);
	printf("running_time: %c\n\n", task->running_time);
}

void verificaVariaveisGlobais()
{
	printf("NextID: %ld\n", nextid);
	printf("countTasks: %ld\n", countTasks);
	printf("preemption: %c\n", preemption);
	printf("systemTime: %d\n\n", systemTime);
}

task_t *scheduler() {
	task_t * proxima_tarefa = readyQueue;
	task_t * aux = readyQueue; 
  int shortest_time = task_get_ret(taskExec);
	
  	if(readyQueue == NULL) 
		return taskExec;
	else
	{
		do
		{
			if (aux->ret < shortest_time && aux != taskMain) {
				shortest_time = aux->ret;
				proxima_tarefa = aux;
			}
			aux = aux->next;
		}
		while(aux != readyQueue);
	}
	if(proxima_tarefa->execution_time == 0)
		proxima_tarefa->execution_time = systime();
	
	return proxima_tarefa;
}

void tratador_tempo(int signum) {
 	systemTime++;
	if(taskExec != NULL) {
	    taskExec->running_time++;
	    taskExec->ret--;
	}

	if(taskExec != taskDisp && taskExec != taskMain) {
		taskExec->quantum--;
		if(taskExec->quantum == 0)
		{
			task_yield();
		}
	}
}

void temporizador ()
{
	// registra a ação para o sinal de timer SIGALRM
	action.sa_handler = tratador_tempo ;
	sigemptyset (&action.sa_mask) ;
	action.sa_flags = 0 ;
	if (sigaction (SIGALRM, &action, 0) < 0)
	{
		perror ("Erro em sigaction: ") ;
		exit (1) ;
	}

	// ajusta valores do temporizador
	timer.it_value.tv_usec =  1000;
	timer.it_interval.tv_usec = 1000; 
	
	// arma o temporizador ITIMER_REAL (vide man setitimer)
	if (setitimer (ITIMER_REAL, &timer, 0) < 0)
	{
		perror ("Erro em setitimer: ") ;
		exit (1) ;
	}
}

// ****************************************************************************

void before_ppos_init () {
	// put your customization here
	#ifdef DEBUG
		printf("\ninit - BEFORE");
	#endif

	systemTime = 0;
	temporizador();
}

void after_ppos_init () {
	// put your customization here
	#ifdef DEBUG
		printf("\ninit - AFTER");
	#endif
}

void before_task_create (task_t *task) {
	// put your customization here
	#ifdef DEBUG
		printf("\ntask_create - BEFORE - [%d]", task->id);
	#endif
}

void after_task_create (task_t *task ) {
	// put your customization here
	#ifdef DEBUG
		printf("\ntask_create - AFTER - [%d]\n", task->id);
	#endif
	if(task!= NULL) 
	{
	    task_set_eet(task, 99999);
	    task->quantum = QUANTUM;
		task->execution_time = 0;
	    task->activations = 0;
	}
}

void before_task_exit () {
	// put your customization here
	#ifdef DEBUG
		printf("\ntask_exit - BEFORE - [%d]", taskExec->id);
	#endif
}

void after_task_exit () {
	// put your customization here
  	taskExec->execution_time = systime() - taskExec->execution_time;
	tempo = systime();
	#ifdef DEBUG
		printf("\ntask_exit - AFTER- [%d]", taskExec->id);
	#endif
	printf("\nTask %d exit: Execution time: %d ms; Processor time: %d ms; %d activations\n",
		taskExec->id,
    	taskExec->execution_time,
		taskExec->running_time,
		taskExec->activations);
}

void before_task_switch ( task_t *task ) {
	// put your customization here
	#ifdef DEBUG
		printf("\ntask_switch - BEFORE - [%d -> %d]", taskExec->id, task->id);
	#endif
	
	task->activations++;
}

void after_task_switch ( task_t *task ) {
	// put your customization here
	task->quantum = QUANTUM;
	#ifdef DEBUG
		printf("\ntask_switch - AFTER - [%d -> %d]", taskExec->id, task->id);
	#endif
}

void before_task_yield () {
	// put your customization here
	#ifdef DEBUG
		printf("\ntask_yield - BEFORE - [%d]", taskExec->id);
	#endif 
}

void after_task_yield () {
	// put your customization here
	#ifdef DEBUG
		printf("\ntask_yield - AFTER - [%d]", taskExec->id);
	#endif
}

void before_task_suspend( task_t *task ) {
	// put your customization here
	#ifdef DEBUG
		printf("\ntask_suspend - BEFORE - [%d]", task->id);
	#endif
}

void after_task_suspend( task_t *task ) {
	// put your customization here
	#ifdef DEBUG
		printf("\ntask_suspend - AFTER - [%d]", task->id);
	#endif
}

void before_task_resume(task_t *task) {
	// put your customization here
	#ifdef DEBUG
		printf("\ntask_resume - BEFORE - [%d]", task->id);
	#endif
}

void after_task_resume(task_t *task) {
	// put your customization here
	#ifdef DEBUG
		printf("\ntask_resume - AFTER - [%d]", task->id);
	#endif
}

void before_task_sleep () {
	// put your customization here
	#ifdef DEBUG
		printf("\ntask_sleep - BEFORE - [%d]", taskExec->id);
	#endif
}

void after_task_sleep () {
	// put your customization here
	#ifdef DEBUG
		printf("\ntask_sleep - AFTER - [%d]", taskExec->id);
	#endif
}

int before_task_join (task_t *task) {
	// put your customization here
	#ifdef DEBUG
		printf("\ntask_join - BEFORE - [%d]", taskExec->id);
	#endif
	return 0;
}

int after_task_join (task_t *task) {
	// put your customization here
	#ifdef DEBUG
		printf("\ntask_join - AFTER - [%d]", taskExec->id);
	#endif
	return 0;
}

//******************************************************************

int before_sem_create (semaphore_t *s, int value) {
	// put your customization here
	#ifdef DEBUG
		printf("\nsem_create - BEFORE - [%d]", taskExec->id);
	#endif
	return 0;
}

int after_sem_create (semaphore_t *s, int value) {
	// put your customization here
#ifdef DEBUG
	printf("\nsem_create - AFTER - [%d]", taskExec->id);
#endif
	return 0;
}

int before_sem_down (semaphore_t *s) {
	// put your customization here
#ifdef DEBUG
	printf("\nsem_down - BEFORE - [%d]", taskExec->id);
#endif
	return 0;
}

int after_sem_down (semaphore_t *s) {
	// put your customization here
#ifdef DEBUG
	printf("\nsem_down - AFTER - [%d]", taskExec->id);
#endif
	return 0;
}

int before_sem_up (semaphore_t *s) {
	// put your customization here
#ifdef DEBUG
	printf("\nsem_up - BEFORE - [%d]", taskExec->id);
#endif
	return 0;
}

int after_sem_up (semaphore_t *s) {
	// put your customization here
#ifdef DEBUG
	printf("\nsem_up - AFTER - [%d]", taskExec->id);
#endif
	return 0;
}

int before_sem_destroy (semaphore_t *s) {
	// put your customization here
#ifdef DEBUG
	printf("\nsem_destroy - BEFORE - [%d]", taskExec->id);
#endif
	return 0;
}

int after_sem_destroy (semaphore_t *s) {
	// put your customization here
#ifdef DEBUG
	printf("\nsem_destroy - AFTER - [%d]", taskExec->id);
#endif
	return 0;
}

int before_mutex_create (mutex_t *m) {
	// put your customization here
#ifdef DEBUG
	printf("\nmutex_create - BEFORE - [%d]", taskExec->id);
#endif
	return 0;
}

int after_mutex_create (mutex_t *m) {
	// put your customization here
#ifdef DEBUG
	printf("\nmutex_create - AFTER - [%d]", taskExec->id);
#endif
	return 0;
}

int before_mutex_lock (mutex_t *m) {
	// put your customization here
#ifdef DEBUG
	printf("\nmutex_lock - BEFORE - [%d]", taskExec->id);
#endif
	return 0;
}

int after_mutex_lock (mutex_t *m) {
	// put your customization here
#ifdef DEBUG
	printf("\nmutex_lock - AFTER - [%d]", taskExec->id);
#endif
	return 0;
}

int before_mutex_unlock (mutex_t *m) {
	// put your customization here
#ifdef DEBUG
	printf("\nmutex_unlock - BEFORE - [%d]", taskExec->id);
#endif
	return 0;
}

int after_mutex_unlock (mutex_t *m) {
	// put your customization here
#ifdef DEBUG
	printf("\nmutex_unlock - AFTER - [%d]", taskExec->id);
#endif
	return 0;
}

int before_mutex_destroy (mutex_t *m) {
	// put your customization here
#ifdef DEBUG
	printf("\nmutex_destroy - BEFORE - [%d]", taskExec->id);
#endif
	return 0;
}

int after_mutex_destroy (mutex_t *m) {
	// put your customization here
#ifdef DEBUG
	printf("\nmutex_destroy - AFTER - [%d]", taskExec->id);
#endif
	return 0;
}

int before_barrier_create (barrier_t *b, int N) {
	// put your customization here
#ifdef DEBUG
	printf("\nbarrier_create - BEFORE - [%d]", taskExec->id);
#endif
	return 0;
}

int after_barrier_create (barrier_t *b, int N) {
	// put your customization here
#ifdef DEBUG
	printf("\nbarrier_create - AFTER - [%d]", taskExec->id);
#endif
	return 0;
}

int before_barrier_join (barrier_t *b) {
	// put your customization here
#ifdef DEBUG
	printf("\nbarrier_join - BEFORE - [%d]", taskExec->id);
#endif
	return 0;
}

int after_barrier_join (barrier_t *b) {
	// put your customization here
#ifdef DEBUG
	printf("\nbarrier_join - AFTER - [%d]", taskExec->id);
#endif
	return 0;
}

int before_barrier_destroy (barrier_t *b) {
	// put your customization here
#ifdef DEBUG
	printf("\nbarrier_destroy - BEFORE - [%d]", taskExec->id);
#endif
	return 0;
}

int after_barrier_destroy (barrier_t *b) {
	// put your customization here
#ifdef DEBUG
	printf("\nbarrier_destroy - AFTER - [%d]", taskExec->id);
#endif
	return 0;
}

int before_mqueue_create (mqueue_t *queue, int max, int size) {
	// put your customization here
#ifdef DEBUG
	printf("\nmqueue_create - BEFORE - [%d]", taskExec->id);
#endif
	return 0;
}

int after_mqueue_create (mqueue_t *queue, int max, int size) {
	// put your customization here
#ifdef DEBUG
	printf("\nmqueue_create - AFTER - [%d]", taskExec->id);
#endif
	return 0;
}

int before_mqueue_send (mqueue_t *queue, void *msg) {
	// put your customization here
#ifdef DEBUG
	printf("\nmqueue_send - BEFORE - [%d]", taskExec->id);
#endif
	return 0;
}

int after_mqueue_send (mqueue_t *queue, void *msg) {
	// put your customization here
#ifdef DEBUG
	printf("\nmqueue_send - AFTER - [%d]", taskExec->id);
#endif
	return 0;
}

int before_mqueue_recv (mqueue_t *queue, void *msg) {
	// put your customization here
#ifdef DEBUG
	printf("\nmqueue_recv - BEFORE - [%d]", taskExec->id);
#endif
	return 0;
}

int after_mqueue_recv (mqueue_t *queue, void *msg) {
	// put your customization here
#ifdef DEBUG
	printf("\nmqueue_recv - AFTER - [%d]", taskExec->id);
#endif
	return 0;
}

int before_mqueue_destroy (mqueue_t *queue) {
	// put your customization here
#ifdef DEBUG
	printf("\nmqueue_destroy - BEFORE - [%d]", taskExec->id);
#endif
	return 0;
}

int after_mqueue_destroy (mqueue_t *queue) {
	// put your customization here
#ifdef DEBUG
	printf("\nmqueue_destroy - AFTER - [%d]", taskExec->id);
#endif
	return 0;
}

int before_mqueue_msgs (mqueue_t *queue) {
	// put your customization here
#ifdef DEBUG
	printf("\nmqueue_msgs - BEFORE - [%d]", taskExec->id);
#endif
	return 0;
}

int after_mqueue_msgs (mqueue_t *queue) {
	// put your customization here
#ifdef DEBUG
	printf("\nmqueue_msgs - AFTER - [%d]", taskExec->id);
#endif
	return 0;
}

