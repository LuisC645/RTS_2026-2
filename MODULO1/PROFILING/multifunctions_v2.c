#include <valgrind/callgrind.h>

int add() {
	int val = 0;
	for(int i = 0; i < 800; ++i)
	val += i;
	return val;
}

 int mult() {
	 int val = 1;
	 for(int i = 0; i < 800; ++i)
	 val *= i;
	 return val;
 }

 int foo() {
	 return add() + add() + mult();
 }

 int main(void) {
	 int dum = 0;

	 CALLGRIND_START_INSTRUMENTATION;
		 for(int i = 0; i < 80000; ++i)
		 dum += foo();
	 CALLGRIND_STOP_INSTRUMENTATION;

	 // other computation
	 return 0;
 }
