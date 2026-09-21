CC = gcc 
CFLAGS1 = -g -Og -std=gnu99
CFLAGS2 = -ggdb3 -O0 

SOURCE1 = AllocMemory.c
SOURCE2 = fibonacci2026_v1.c
SOURCE3 = fibonacci2026_v2.c
SOURCE4 = multifunctions_v1.c
SOURCE5 = multifunctions_v2.c
SOURCE6 = multifunctions_v3.c

all: AllocMemory fibonacci2026_v1 fibonacci2026_v2 multifunctions_v1 multifunctions_v2 multifunctions_v3


AllocMemory: $(SOURCE1)
	$(CC) $(CFLAGS2) $(SOURCE1) -o $@

fibonacci2026_v1: $(SOURCE2)
	$(CC) $(CFLAGS2) $(SOURCE2) -o $@

fibonacci2026_v2: $(SOURCE3)
	$(CC) $(CFLAGS2) $(SOURCE3) -o $@

multifunctions_v1: $(SOURCE4)
	$(CC) $(CFLAGS2) $(SOURCE4) -o $@

multifunctions_v2: $(SOURCE5)
	$(CC) $(CFLAGS2) $(SOURCE5) -o $@

multifunctions_v3: $(SOURCE6)
	$(CC) $(CFLAGS2) $(SOURCE6) -o $@

valgrind: AllocMemory fibonacci2026_v1 fibonacci2026_v2 multifunctions_v1 multifunctions_v2 multifunctions_v3
	valgrind --tool=callgrind --dump-instr=yes --instr-atstart=no ./AllocMemory
	valgrind --tool=callgrind --dump-instr=yes --instr-atstart=no ./fibonacci2026_v1
	valgrind --tool=callgrind --dump-instr=yes --instr-atstart=no ./fibonacci2026_v2
	valgrind --tool=callgrind --dump-instr=yes --instr-atstart=no ./multifunctions_v1
	valgrind --tool=callgrind --dump-instr=yes --instr-atstart=no ./multifunctions_v2
	valgrind --tool=callgrind --dump-instr=yes --instr-atstart=no ./multifunctions_v3

clean:
	$(RM) AllocMemory fibonacci2026_v1 fibonacci2026_v2 multifunctions_v1 multifunctions_v2 multifunctions_v3 *.out*
