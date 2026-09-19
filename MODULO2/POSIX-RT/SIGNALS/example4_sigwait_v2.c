/* 
   Example by https://www.ibm.com/support/knowledgecenter/SSLTBW_2.4.0/com.ibm.zos.v2r4.bpxbd00/sigwait.htm

   This example demonstrates the use of the sigwait() function.
   The program will wait until a SIGRTMIN signal is received from the command line.

   Expected output:
   A Real-Time signal was received.
  
*/

/*************************************************/
//  - Code modified by Prof. Gustavo Patino  -
//    - Real-Time Systems (IEO 2547027) -
//     - University of Antioquia (UdeA) -
//      - Medellin, Colombia 2026 -
/*************************************************/


#include <signal.h>
#include <stdio.h>
#include <errno.h>

int main() {

  sigset_t set;
  int sig;
  int ret_val;

  sigemptyset(&set);

  sigaddset(&set, SIGINT);
  sigaddset(&set, SIGTERM);
  sigaddset(&set, 34); // Real-Time signal No. 0: SIGRTMIN
  sigaddset(&set, SIGRTMIN + 4); // Real-Time signal No. 4: SIGRTMIN + 4

  sigprocmask( SIG_BLOCK, &set, NULL ); // Lo que hace SIG_BLOCK es bloquear la ejecuciòn del handler de la señal.

  printf("\nWaiting for a Real-Time Signal...\n");

  ret_val = sigwait(&set, &sig); // La señal que se reciba dejara de estar "pending" de entregarse, pero seguirà bloqueada.

  if(ret_val > 0)
     perror("sigwait failed\n");
  else {
   if(sig >= 34)
        printf("\nThe Real-Time signal %d was received.\n\n", sig);
      
   else if(sig == 2)
        printf("\nNo Real-Time Signal was received. Instead, signal SIGINT was received\n\n");
   else 
        printf("\nNo Real-Time Signal was received. Instead, sigwait returned with the signal: %d\n", sig);
  }
}
