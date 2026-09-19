/*************************************************/
//  - Code modified by Prof. Gustavo Patino  -
//    - Real-Time Systems (IEO 2547027) -
//     - University of Antioquia (UdeA) -
//      - Medellin, Colombia 2026 -
/*************************************************/


#include <string.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


//#define DEBUG_MODE
#define TIME2SLEEP 1000000	// Time to sleep in us.

void current_time()
{
	time_t sec;
	suseconds_t	msec;
	
	struct timeval tv;
	char zeros[3];

	gettimeofday(&tv, 0);

	sec = tv.tv_sec;
	msec = tv.tv_usec/1000;
	
	if (msec < 10)
		strcpy(zeros, "00");
	else if (msec < 100)
		strcpy(zeros, "0");
	else 		
		strcpy(zeros, "");
	
	printf("%ld.%s%ld\n", sec, zeros, msec);
}


// callback function when the timer expires.. this will be called
void timer_handle(int sig, siginfo_t *si, void *uc)
{
	// Aqui va el codigo asociado al llamado de la señal SIGRTMIN.
	printf("\n********** A Real-Time Signal have just happened due to a timer! **********\n");
	
#ifdef DEBUG_MODE
	printf("********** The signal came at: ");	
	current_time();
	printf("\n");
#endif
}


int main()
{
	int ret;
	struct sigevent sev;
	struct itimerspec its;
	struct sigaction sa;
	timer_t timerid;
	
	memset(&sev, 0, sizeof(sev));
	memset(&its, 0, sizeof(its));
	memset(&sa, 0, sizeof(sa));
	
	// set signal mask for the timer
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = timer_handle;

	if (sigaction(SIGRTMIN + 1, &sa, NULL) < 0) {
		exit(-1);
	}

	// set signal mask for sigevent
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIGRTMIN + 1;
	sev.sigev_value.sival_ptr = &timerid;

	/**** Timer creation with timer_create: ***/
	ret = timer_create(CLOCK_REALTIME, &sev, &timerid);
	if (ret < 0) {
		printf("There were an error with timer_create.");
		exit(-1);
	}
	
	// set timer to tick 2.023 sec intervals
	its.it_value.tv_sec = 3; // Offset
	its.it_value.tv_nsec = 999999999; // Offset
	its.it_interval.tv_sec = 2;
	its.it_interval.tv_nsec = 23000000;
	
	/**** Arming the timer with timer_settime: ***/
	ret = timer_settime(timerid, 0, &its, NULL);
	if (ret < 0) {
		printf("There were an error with timer_settime.");
		exit(-1);
	}
	
	int count = 0;
	
	// wait indefinitely
	while (1) {
#ifdef DEBUG_MODE
		printf("\nSleeping for %d microseconds at... ", TIME2SLEEP);
		current_time();	
#endif
		ret=usleep(TIME2SLEEP);	//	El conteo en este While infinito "duerme" durante 1 segundo.

#ifdef DEBUG_MODE		
		printf("USLEEP() returned:  %d\n", ret);
#endif
		
		if (ret == 0){
			printf("\n...counting %d seconds. \n", ++count);
#ifdef DEBUG_MODE
			printf("This happens at: ");
			current_time();
#endif
		} else
		printf("\nATTENTION: SLEEP was interrupted, so no counting was done. \n");
	}
}
