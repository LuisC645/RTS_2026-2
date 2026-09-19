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

#include <sys/time.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//#define DEBUG_MODE

struct periodic_task {
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
		
	printf("El tiempo actual en segundos es: %ld.%s%ld\n\n", sec, zeros, msec);	
}


struct periodic_task* start_periodic_timer(uint64_t offs, int period)
{
	int res;
	struct periodic_task *p;

	const int signotif = SIGRTMAX - 3;

	p = malloc(sizeof(struct periodic_task));
	if (p == NULL) {
		return NULL;
	}

	memset(&p->t, 0, sizeof(p->t));
	memset(&p->sigev, 0, sizeof(p->sigev));

	sigemptyset(&p->sigset);
	sigaddset(&p->sigset, signotif);

	p->sigev.sigev_notify = SIGEV_SIGNAL;
	p->sigev.sigev_signo = signotif;

	sigprocmask(SIG_BLOCK, &p->sigset, NULL);

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
	printf("\nAntes de armar el temporizador: \n");
	current_time();
#endif

	printf("\nEste proceso tiene un periodo esperado de : %d us. \n", period);
	printf("Si el offset fue de %ld us, entonces cuente aqui %ld seg. antes de comenzar la ejecuciòn del temporizador.\n", offs, offs/1000000);

	res = timer_settime(p->timer, 0, &p->t, NULL);

	if (res < 0) {
		perror("Timer_settime");
		exit(-1);
	}

	p->period = period;

	return p;
}


static void wait_next_activation(struct periodic_task *t)
{
	int sig;

	int result = sigwait(&t->sigset, &sig);
	if (result > 0) {
		perror("Sigwait");
		exit(-1);
	}

#ifdef DEBUG_MODE
	else if(result == 0)
		printf("El System Call sigwait obtuvo la señal: %d\n\n", sig);
#endif
}


static void job_body(void)
{
	static int cnt;			//	Static int inicializa en cero, y se mantiene su valor de manera global, 
							//	inclusive al ejecutar otra instancia de la misma funciòn.
	static uint64_t start;	// Igual al caso anterior.
	uint64_t t;
	struct timeval tv;

	if (start == 0) {		//	Dado que start es static uint64_t, en cada nueva ejecuciòn de esta funciòn 
							//	se continua con el valor de start que tenìa en la ejecuciòn previa. 
							//	Solo en la primera ejecuciòn de job_body(), start = 0;
		gettimeofday(&tv, NULL);
		start = tv.tv_sec * 1000ULL + tv.tv_usec / 1000ULL;
	}
        
	gettimeofday(&tv, NULL);
	t = tv.tv_sec * 1000ULL + tv.tv_usec / 1000ULL;

#ifdef DEBUG_MODE
	printf("cnt = %d \n", cnt);
	printf("cnt mod 100 = %d \n", cnt%100);
#endif
	if (cnt && (cnt % 100) == 0) {	// Se define este condicional para contar de 1 a 100 y evitar divisiòn por cero.
		printf("\n***** Luego de tomar 100 muestras periódicas, en promedio el periodo de ejecuciòn en ms fue de: %f ***** \n", (double)(t - start) / (double)cnt); 
	}
	cnt++;	//	Dado que cnt es static int, en cada nueva ejecuciòn de esta funciòn 
			//	se continua en el valor de cnt que tenìa en la ejecuciòn previa.
}


int main(int argc, char *argv[])
{
	printf("\n\n********* Tarea periódica usando POSIX Timers *********\n\n");
	
	struct periodic_task *pt;

	pt = start_periodic_timer(4000000, 5420);  // Tiempo ingresado en microsegundos (4000ms, 5.42ms).
	if (pt == NULL) {
		perror("Start Periodic Timer");
		return -1;
	}

	while(1) {
#ifdef DEBUG_MODE
		printf("\nNueva ejecuciòn del While(1).\n");
		current_time();
#endif

		wait_next_activation(pt); // activando este wait, entre disparo y disparo se ejecuta la funcion job_body(), 
					// la cual toma 100 muestras de tiempo y muestra el promedio del tiempo en ms.
		job_body();	//	Funciòn (o tarea) que serà ejecutada periodicamente de la mano de la funciòn 
					//	wait_next_activation(pt) llamada en la instrucciòn anterior.
	}

	return 0;
}
