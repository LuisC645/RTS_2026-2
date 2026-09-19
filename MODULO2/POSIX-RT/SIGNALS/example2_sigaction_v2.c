/*************************************************/
//  - Code modified by Prof. Gustavo Patino  -
//    - Real-Time Systems (IEO 2547027) -
//     - University of Antioquia (UdeA) -
//      - Medellin, Colombia 2026 -
/*************************************************/

#include <unistd.h>
#include <signal.h>
#include <stdio.h>


void handler(int num){
	printf(" In this example, some signals have been changed and this execution won't die!  ( *_* )\n\n");
	}

int main()
{
	int ret1, ret2;
	ret1 = ret2 = 1;
	
	struct sigaction sa;
	sa.sa_handler = handler;

	ret1 = sigaction(SIGINT, &sa, NULL);
	ret2 = sigaction(SIGTERM, &sa, NULL);

	if (ret1 != 0 || ret2 != 0) {
		perror("sigaction");
		return -1;
	}
	
		while (1)
	{
		printf("This program is also wasting your cycles (^-^)/ . Process id: %d\n", getpid());
		sleep(1);
	}
}
	
