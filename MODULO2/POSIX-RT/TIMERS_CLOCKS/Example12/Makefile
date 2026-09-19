# Makefile to compile the Example 11 about Periodic Threads with POSIX Real-Time Extension
# Written by Gustavo Patino
# UdeA - Sept. 2026


CC = gcc
CFLAGS = -Wall -g
LIBRT = -lrt
LIBPTHREAD = -lpthread

SETTINGS = periodic_settings
TASKS = task_definition
MAIN = example12_periodic-threads


all: $(MAIN)

$(SETTINGS).o: $(SETTINGS).c $(SETTINGS).h
	$(CC) $(CFLAGS) -c $(SETTINGS).c $(LIBRT) $(LIBPTHREAD)

$(TASKS).o: $(TASKS).c $(TASKS).h
	$(CC) $(CFLAGS) -c $(TASKS).c $(LIBRT) $(LIBPTHREAD)

$(MAIN): $(MAIN).c $(SETTINGS).o $(TASKS).o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBRT) $(LIBPTHREAD)



clean:
	rm -f *.o
	rm -f $(MAIN)


