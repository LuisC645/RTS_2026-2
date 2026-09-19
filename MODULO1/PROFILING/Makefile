CC = gcc 
CFLAGS1 = -g -Og -std=gnu99
CFLAGS2 = -ggdb3 -O0 

SOURCE1 = ex1.c 
SOURCE2 = memoryleak.c
SOURCE3 = fibonacci_2025.c
SOURCE4 = prueba.c

all: ex1 memoryleak fibonacci prueba


ex1: $(SOURCE1)
	$(CC) $(CFLAGS2) $(SOURCE1) -o $@

memoryleak: $(SOURCE2)
	$(CC) $(CFLAGS2) $(SOURCE2) -o $@

fibonacci: $(SOURCE3)
	$(CC) $(CFLAGS2) $(SOURCE3) -o $@

prueba: $(SOURCE4)
	$(CC) $(CFLAGS2) $(SOURCE4) -o $@


valgrind: ex1 memoryleak fibonacci prueba
	valgrind --tool=callgrind --dump-instr=yes --instr-atstart=no ./ex1
	valgrind --tool=callgrind --dump-instr=yes --instr-atstart=no ./memoryleak
	valgrind --tool=callgrind --dump-instr=yes --instr-atstart=no ./fibonacci
	valgrind --tool=callgrind --dump-instr=yes --instr-atstart=no ./prueba

clean:
	$(RM) ex1 memoryleak fibonacci prueba *.out*
