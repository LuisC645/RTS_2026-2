# Makefile to compile the signal and timer examples
# Written by Gustavo Patino
# UdeA - Sept/2026

CC = gcc
CFLAGS = -Wall -g 

PROGS = example7_timer_create example8_periodic-process example9_periodic-task example10_periodic-threads example11_periodic-task

all: $(PROGS)

example7_timer_create: example7_timer_create.c
	$(CC) $(CFLAGS) -o $@ $^

example8_periodic-process: example8_periodic-process.c
	$(CC) $(CFLAGS) -o $@ $^

example9_periodic-task: example9_periodic-task.c
	$(CC) $(CFLAGS) -o $@ $^

example10_periodic-threads: example10_periodic-threads.c
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

example11_periodic-task: example11_periodic-task.c
	$(CC) $(CFLAGS) -o $@ $^


clean:
	rm -f *.o
	rm -f $(PROGS)
	rm -f *.txt
	
