/*************************************************/
// Prof. Luca Abeni. The Real-Time Operating Systems and Middleware course
// http://www.dit.unitn.it/~abeni/RTOS/periodic-3.c

/*************************************************/

/*************************************************/
//  - Code modified by Prof. Gustavo Patino  -
//    - Real-Time Systems (IEO 2547027) -
//     - University of Antioquia (UdeA) -
//      - Medellin, Colombia 2026 -
/*************************************************/

#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

//#define DEBUG_MODE
#define SECONDS_TO_RUN 10


static int thread_1_count;
static int thread_2_count;
static int thread_3_count;

pthread_mutex_t lock;


struct periodic_thread {
	sigset_t sigset;
	struct itimerspec t;
	struct sigevent sigev;
	timer_t timer;
	int period;
	};


void current_time()
{
	time_t sec;
	suseconds_t	msec;
	char zeros[3];	
	
	struct timeval tv;
	
	gettimeofday(&tv, 0);

	sec = tv.tv_sec;
	msec = tv.tv_usec/1000;
	
	if (msec < 10)
		strcpy(zeros, "00");
	else if (msec < 100)
		strcpy(zeros, "0");
	else 		
		strcpy(zeros, "");
		
	printf("El tiempo actual en segundos es: %ld.%s%ld\n", sec, zeros, msec);	
}


struct periodic_thread* start_periodic_timer(uint64_t offs, int period, short unsigned sigrt)
{
	int res;
	struct periodic_thread *p;
		
	int signotif = SIGRTMIN + sigrt;	//	No se puede utilizar la señal SIGLARM, 
										//	dado que tendremos un nùmero de hilos igual o mayor a uno.

	p = malloc(sizeof(struct periodic_thread));
	if (p == NULL) {
		return NULL;
	}

	memset(&p->t, 0, sizeof(p->t));
	memset(&p->sigev, 0, sizeof(p->sigev));

	sigemptyset(&p->sigset);
	sigaddset(&p->sigset, signotif);

	p->sigev.sigev_notify = SIGEV_SIGNAL;
	p->sigev.sigev_signo = signotif;

	res = timer_create(CLOCK_REALTIME, &p->sigev, &p->timer);
	if (res < 0) {
		perror("Timer Create");
		exit(-1);
	}

	p->t.it_value.tv_sec = offs / 1000000;
	p->t.it_value.tv_nsec = (offs % 1000000) * 1000;
	p->t.it_interval.tv_sec = period / 1000000;
	p->t.it_interval.tv_nsec = (period % 1000000) * 1000;

#ifdef DEBUG_MODE
	printf("\nAntes de armar el temporizador, el tiempo actual es: \n");
	current_time();
#endif

	printf("Este proceso tiene un periodo esperado de : %d us. \n", period);
	printf("El offset es de %ld us.\n\n", offs);

	res = timer_settime(p->timer, 0, &p->t, NULL);

	if (res < 0) {
		perror("Timer_settime");
		exit(-1);
	}

	p->period = period;
	return p;
}


static void wait_next_activation(struct periodic_thread *t)
{
	int sig;

	int result = sigwait(&t->sigset, &sig);
	if (result < 0) {
		perror("Sigwait");
		exit(-1);
	}
#ifdef DEBUG_MODE
	else if(result == 0)
		printf("El System Call sigwait() obtuvo la señal: %d\n", sig);
#endif
}


static void *thread_1(void *arg)
{
	float period = 115000; // Period of 115ms
	struct periodic_thread *t1;

	pthread_mutex_lock(&lock);

	printf("Hilo 1: Periodo %.2f ms.\n", period/1000);
	t1=start_periodic_timer(100000, period, 1);

	pthread_mutex_unlock(&lock);

	while (1) {
		wait_next_activation(t1);
		++thread_1_count;
	}
	return NULL;
}

static void *thread_2(void *arg)
{
	float period = 22500; // Period of 22.5ms
	struct periodic_thread *t2;

	pthread_mutex_lock(&lock);

	printf("Hilo 2: Periodo %.2f ms.\n", period/1000);
	t2=start_periodic_timer(200000, period, 2);

	pthread_mutex_unlock(&lock);

	while (1) {
		wait_next_activation(t2);
		++thread_2_count;
	}
	return NULL;
}

static void *thread_3(void *arg)
{
	float period = 54300; // Period of 54,3ms
	struct periodic_thread *t3;

	pthread_mutex_lock(&lock);

	printf("Hilo 3: Periodo %.2f ms.\n", period/1000);
	t3=start_periodic_timer(500000, period, 3);

	pthread_mutex_unlock(&lock);

	while (1) {
		wait_next_activation(t3);
		++thread_3_count;
	}
	return NULL;
}


int main(int argc, char *argv[])
{
	pthread_t t_1;
	pthread_t t_2;
	pthread_t t_3;
	sigset_t alarm_sigset;
	int i;

	printf("\n\n********* Hilos periodicos usando POSIX Timers *********\n\n\n");

	printf("Después de crear los hilos, el hilo principal estará suspendido por %d segundos, a fin de permitir que los hilos puedan correr. \n\n", SECONDS_TO_RUN);

	/* Block all real time signals so they can be used for the timers.
	   Note: this has to be done in main() before any threads are created
	   so they all inherit the same mask. Doing it later is subject to
	   race conditions */

	sigemptyset(&alarm_sigset);
	for (i = SIGRTMIN; i <= SIGRTMAX; i++)
		sigaddset(&alarm_sigset, i);
		
	sigprocmask(SIG_BLOCK, &alarm_sigset, NULL);

	pthread_create(&t_1, NULL, thread_1, NULL);
	pthread_create(&t_2, NULL, thread_2, NULL);
	pthread_create(&t_3, NULL, thread_3, NULL);

	sleep(SECONDS_TO_RUN);

	pthread_mutex_lock(&lock);
	printf("En %d segundos ejecutados hubo: \n\n", SECONDS_TO_RUN);
	printf("%d iteraciones del Hilo 1.\n", thread_1_count);
	printf("%d iteraciones del Hilo 2.\n", thread_2_count);
	printf("%d iteraciones del Hilo 3.\n\n\n", thread_3_count);
	pthread_mutex_unlock(&lock);

	pthread_mutex_destroy(&lock);

	return 0;
}

