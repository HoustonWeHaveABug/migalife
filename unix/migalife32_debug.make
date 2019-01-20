MIGALIFE32_DEBUG_STACK_SIZE=16777216
MIGALIFE32_DEBUG_C_FLAGS=-DDEBUG -std=c90 -g -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winline -Wlong-long -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-import -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings

migalife32_debug: migalife32_debug.o
	gcc -g -Wl,--stack,${MIGALIFE32_DEBUG_STACK_SIZE} -o migalife32_debug migalife32_debug.o

migalife32_debug.o: ../migalife.c migalife32_debug.make
	gcc -c ${MIGALIFE32_DEBUG_C_FLAGS} -o migalife32_debug.o ../migalife.c

clean:
	rm -f migalife32_debug migalife32_debug.o
