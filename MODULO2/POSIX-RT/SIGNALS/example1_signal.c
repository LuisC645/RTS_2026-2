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
	printf(" Signals have been changed and this execution won't die!  (-_^)\n\n");
	}


int main()
{
	signal(SIGINT, handler);	// Enmascarimiento de la señal #2 (SIGINT), con la funciòn "handler".
	signal(SIGTERM, handler);	// Enmascarimiento de la señal #15 (SIGTERM), con la funciòn "handler".
	signal(SIGKILL, handler);	// Intento infructuoso de enmascar la señal #9 (SIGKILL), con la funciòn "handler".
					// La señal SIGKILL, junto con la señal SIGSTOP, son las ùnicas señales que no se
					// pueden enmarscar (Pàg 68 del libro "Systems Programming in Linux With C and C++
					// de devnaga (Abril del 2020)"). 
					
	while (1)
	{
		printf("This program is wasting your cycles! (^-^)/ . Process id: %d\n", getpid());
		sleep(1);
	}
}
	
