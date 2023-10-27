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

struct sigaction action;
struct itimerval timer ;

#define QUANTUM 20
#define DEBUG 1

void task_set_eet (task_t *task, int et)
{
	if (task == NULL)
	{
		taskExec->eet = et;
		taskExec->ret = et;
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
		return taskExec->ret;

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

task_t* scheduler() {
	int isCurrent = 1;
	task_t* menorTarefa = NULL;
	task_t* proximaTarefa = readyQueue->next;

	if(countTasks >= 1) 
	{		
		for(int i=1; i <= countTasks; i++)
		{
			if((proximaTarefa->eet < taskExec->ret && proximaTarefa->state == 'r') 
				|| (taskExec->state == 'e' || taskExec->quantum == 0))
			{
				printf("\nA nova tarefa é menor\n");
				menorTarefa = proximaTarefa;
				isCurrent = 0;
			}
			printf("O atual é menor, vai para o próximo\n");
			proximaTarefa = proximaTarefa->next;
		}
	}
	
	if(isCurrent)
	{
		return taskExec;
	}
	else
	{
		return menorTarefa;
	}
}

void tratador_tempo (int signum)
{
	if(countTasks >= 1)
	{
		taskExec->quantum--;
		taskExec->running_time++;
		
		if(taskExec->quantum == 0)
		{
			printf("\nQuantum zerou\n");
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
	timer.it_value.tv_usec = 200000;			// primeiro disparo, em micro-segundos
	timer.it_interval.tv_usec = 200000;			// disparos subsequentes, em micro-segundos
	
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
	systemTime++;
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
		printf("\ntask_create - AFTER - [%d]", task->id);
	#endif

	systemTime++;
	task_set_eet(task, 99999);
}

void before_task_exit () {
	// put your customization here
	#ifdef DEBUG
		printf("\ntask_exit - BEFORE - [%d]", taskExec->id);
	#endif
	printf("Antes de sair da tarefa\n");
}

void after_task_exit () {
	// put your customization here
	#ifdef DEBUG
		printf("\ntask_exit - AFTER- [%d]", taskExec->id);
	#endif
	printf("Saiu da tarefa\n");
}

void before_task_switch ( task_t *task ) {
	// put your customization here
	#ifdef DEBUG
		printf("\ntask_switch - BEFORE - [%d -> %d]", taskExec->id, task->id);
	#endif
}

void after_task_switch ( task_t *task ) {
	// put your customization here
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
	taskExec->quantum = QUANTUM;
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

