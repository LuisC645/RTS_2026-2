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

// #define DEBUG_MODE

static sigset_t sigset;

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

int start_periodic_timer(uint64_t offs, int period)
{
	struct itimerspec t;
	struct sigevent sigev;
	timer_t timer;
	int res;

	const int signotif = SIGRTMIN + 5;

	memset(&t, 0, sizeof(t));
	memset(&sigev, 0, sizeof(sigev));

	sigemptyset(&sigset);
	sigaddset(&sigset, signotif);

	sigev.sigev_notify = SIGEV_SIGNAL;
	sigev.sigev_signo = signotif;

	sigprocmask(SIG_BLOCK, &sigset, NULL);

	res = timer_create(CLOCK_REALTIME, &sigev, &timer);
	if (res < 0) {
		perror("Timer Create");
		exit(-1);
	}

	t.it_value.tv_sec = offs / 1000000;
	t.it_value.tv_nsec = (offs % 1000000) * 1000;
	t.it_interval.tv_sec = period / 1000000;
	t.it_interval.tv_nsec = (period % 1000000) * 1000;

#ifdef DEBUG_MODE
	printf("\nAntes de armar el temporizador: \n");
	current_time();
#endif
	printf("\nEste proceso tiene un periodo esperado de : %d us. \n", period);
	printf("Si el offset fue de %ld us, entonces cuente aqui %ld seg. antes de comenzar la ejecuciòn del temporizador.\n", offs, offs/1000000);

	return timer_settime(timer, 0, &t, NULL);
}

static void wait_next_activation(void)
{
	int sig;

	int result = sigwait(&sigset, &sig);
	if (result > 0) {
		perror("Sigwait");
		exit(-1);
	}
#ifdef DEBUG_MODE
	else if(result == 0)
		printf("\nEl System Call sigwait() obtuvo la señal: %d\n", sig);
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
	printf("\ncnt = %d \n", cnt);
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
	int res;

	printf("\n\n********* Proceso periódico usando POSIX Timers *********\n\n");

	res = start_periodic_timer(3000000, 10570); // Tiempo ingresado en microsegundos (3000ms, 10.57ms).
	if (res < 0) {
		perror("Start Periodic Timer");
		return -1;
	}

	while(1) {

#ifdef DEBUG_MODE
		printf("\nNueva ejecución del While(1).\n");
		current_time();
#endif

		wait_next_activation();	//	Activando este wait, entre disparo y disparo se ejecuta la funcion job_body(), 
								//	la cual toma 100 muestras de tiempo y muestra el promedio del tiempo en ms.
		job_body();	//	Funciòn (o tarea) que serà ejecutada periodicamente de la mano de la funciòn 
					//	wait_next_activation() llamada en la instrucciòn anterior.
	}

	return 0;
}
