MIGALIFE32_STACK_SIZE=16777216
MIGALIFE32_C_FLAGS=-std=c90 -O2 -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winline -Wlong-long -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-import -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings

migalife32: migalife32.o
	gcc -Wl,--stack,${MIGALIFE32_STACK_SIZE} -o migalife32 migalife32.o

migalife32.o: ../migalife.c migalife32.make
	gcc -c ${MIGALIFE32_C_FLAGS} -o migalife32.o ../migalife.c

clean:
	rm -f migalife32 migalife32.o
