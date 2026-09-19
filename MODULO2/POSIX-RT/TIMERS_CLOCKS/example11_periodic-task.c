/*************************************************/
// Prof. Luca Abeni. The Real-Time Operating Systems and Middleware course
// http://www.dit.unitn.it/~abeni/RTOS/periodic-5.c

/*************************************************/


/*************************************************/
//  - Code modified by Prof. Gustavo Patino  -
//    - Real-Time Systems (IEO 2547027) -
//     - University of Antioquia (UdeA) -
//      - Medellin, Colombia 2026 -
/*************************************************/


#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//#define DEBUG_MODE

#define NSEC_PER_SEC 1000000000ULL
#define SAMPLES 153


struct periodic_task {
	struct timespec arrival;
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

static inline void timespec_add_us(struct timespec *t, uint64_t d)
{
	//	El valor d que llega es en us, entonces este se agrega al timespec *t en nanosegundos, 
	//	multiplicado por 1000. (d*1000 nsec = d usec).
	//	Por esto la funcion se llama "timespec_add_us", o sea, agrega microsegundos al timespec.
	//	El resultado de esta funciòn queda en timespec.

#ifdef DEBUG_MODE
		printf("\ntimespec t, componente sec =  %ld\n", t->tv_sec);
		printf("timespec t, componente nsec =  %ld\n", t->tv_nsec);
#endif

	d *= 1000;
	d += t->tv_nsec;
	while (d >= NSEC_PER_SEC) {
		d -= NSEC_PER_SEC;  // Por cada resta de NSEC_PER_SEC debe agregar un segundo al tv_sec.
		t->tv_sec += 1;
	}
	t->tv_nsec = d;
}

struct periodic_task* start_periodic_timer(uint64_t offs, int t)
{
	int ret;
	struct periodic_task *p;

	p = malloc(sizeof(struct periodic_task));
	if (p == NULL) {
	return NULL;
	}

#ifdef DEBUG_MODE
	printf("\nAntes de armar el temporizador, el tiempo actual es: \n");
	current_time();
#endif

	printf("\nEsta tarea tiene un periodo esperado de : %d us. \n", t);
	printf("El offset es de %ld us, entonces cuente aqui %ld seg. antes de comenzar la ejecuciòn del temporizador.\n", offs, offs/1000000);

	ret = clock_gettime(CLOCK_REALTIME, &p->arrival);
	if (ret < 0) {
		perror("clock_gettime");
		exit(-1);
	}
	timespec_add_us(&p->arrival, offs);	//	Inicializa el tiempo de offset de la tarea periodica en unidades 
									//	de "REAL-TIME" (Epochs) del reloj del sistema. El resultado queda en 
									//	&p->arrival.
	p->period = t;					//	Inicializa la variable del "periodo" de la tarea periodica.

	return p;
}


static void wait_next_activation(struct periodic_task *t)
{
	int ret = clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &t->arrival, NULL);
	if (ret > 0) {
		perror("clock_nanosleep");
		exit(-1);
	}
	timespec_add_us(&t->arrival, t->period);
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
	printf("\nEn este llamado de Job_Body, el tiempo actual es: \n");
	current_time();
#endif	

#ifdef DEBUG_MODE
	printf("cnt = %d \n", cnt);
	printf("cnt mod SAMPLES = %d \n", cnt%SAMPLES);
#endif
	if (cnt && (cnt % SAMPLES) == 0) {	// Se define este condicional para contar de 1 a "SAMPLES" y evitar divisiòn por cero.
		printf("\n***** En promedio, en %d muestras el periodo de ejecuciòn en ms fue de: %f ***** \n\n", SAMPLES, (double)(t - start) / (double)cnt); 
	}
	cnt++;	//	Dado que cnt es static int, en cada nueva ejecuciòn de esta funciòn 
			//	se continua en el valor de cnt que tenìa en la ejecuciòn previa.
}


int main(int argc, char *argv[])
{
	printf("\n\n********* Tarea periódica usando POSIX Clocks *********\n\n");

	struct periodic_task *pt;

	pt = start_periodic_timer(4000000, 3235);  // Tiempo ingresado en microsegundos (4000ms, 3.235ms).
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
					// la cual toma un nùmero de "SAMPLES" (muestras) de tiempo y muestra el promedio del tiempo en ms.
		job_body();	//	Funciòn (o tarea) que serà ejecutada periodicamente de la mano de la funciòn 
					//	wait_next_activation(pt) llamada en la instrucciòn anterior.
	}

	return 0;
}
