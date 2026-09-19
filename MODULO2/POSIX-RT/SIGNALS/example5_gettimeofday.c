/*************************************************/
//  - Code modified by Prof. Gustavo Patino  -
//    - Real-Time Systems (IEO 2547027) -
//     - University of Antioquia (UdeA) -
//      - Medellin, Colombia 2026 -
/*************************************************/

#include <time.h>
#include <sys/time.h>
#include <stdio.h>

int main() {

	struct tm *info; // https://www.cplusplus.com/reference/ctime/tm/

	struct timeval tv;	// https://man7.org/linux/man-pages/man2/gettimeofday.2.html
	time_t t;	// https://cplusplus.com/reference/ctime/time_t/

	char buffer[64];
 
	gettimeofday(&tv, NULL);	// https://man7.org/linux/man-pages/man2/gettimeofday.2.html
	t = tv.tv_sec;	// Tiempo en segundo segùn la Epoch de Linux.

	printf("\nIn rude format, the time now is %ld.%ld\n\n", tv.tv_sec, tv.tv_usec / 1000);
	getchar();

	info = localtime(&t); 
	// https://www.cplusplus.com/reference/ctime/localtime/?kw=localtime 
	//	localtime() Uses the value pointed by time_t (variable t) to fill a tm structure (info) 
	//	with the values that represent the corresponding time, expressed for the local timezone.	
	
	printf("In standard format, now the date and time are: %s\n",asctime (info));
	getchar();

	strftime (buffer, sizeof buffer, "%A, %B %d of %G.\n\n", info); 
	// https://www.cplusplus.com/reference/ctime/strftime/?kw=strftime: Format time as string.
	
	printf("Just considering the date, today is: %s",buffer);
	getchar();

	strftime (buffer, sizeof buffer, "%I:%M:%S %p.\n", info); 
	// https://www.cplusplus.com/reference/ctime/strftime/?kw=strftime: Format time as string.
	printf("And the current time is: %s\n\n",buffer);

	return 0;
}
