# Makefile to compile the signal and timer examples
# Written by Gustavo Patino
# UdeA - Sept/2026

CC = gcc
CFLAGS = -Wall -g 

PROGS = example1_signal example2_sigaction example4_sigwait example5_gettimeofday 

all: $(PROGS)

example1_signal: example1_signal.c
	$(CC) $(CFLAGS) -o $@ $^

example2_sigaction: example2_sigaction_v2.c
	$(CC) $(CFLAGS) -o $@ $^

example4_sigwait: example4_sigwait_v2.c
	$(CC) $(CFLAGS) -o $@ $^	

example5_gettimeofday: example5_gettimeofday.c
	$(CC) $(CFLAGS) -o $@ $^


clean:
	rm -f *.o
	rm -f $(PROGS)
	
