/*************************************************/
//  - Code by Prof. Gustavo Patino  -
//    - Real-Time Systems (IEO 2547027) -
//     - University of Antioquia (UdeA) -
//      - Medellin, Colombia 2026 -
/*************************************************/


#include "task_definition.h"


void thread_1(int* c)
{
	pthread_mutex_lock(&lock);
	printf("Thread 1 counting: %d\n", (*c)++);
	pthread_mutex_unlock(&lock);
}


void thread_2(int* c)
{
	pthread_mutex_lock(&lock);
	printf("Thread 2 counting: %d\n", (*c)++);
	pthread_mutex_unlock(&lock);
}


void thread_3(int* c)
{
	pthread_mutex_lock(&lock);
	printf("Thread 3 counting: %d\n", (*c)++);
	pthread_mutex_unlock(&lock);
}
