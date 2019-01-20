MIGALIFE64_DEBUG_STACK_SIZE=16777216
MIGALIFE64_DEBUG_C_FLAGS=-DDEBUG -DLONGLONG_PTR -std=c99 -g -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winline -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-import -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings

migalife64_debug: migalife64_debug.o
	gcc -g -Wl,--stack,${MIGALIFE64_DEBUG_STACK_SIZE} -o migalife64_debug migalife64_debug.o

migalife64_debug.o: ../migalife.c migalife64_debug.make
	gcc -c ${MIGALIFE64_DEBUG_C_FLAGS} -o migalife64_debug.o ../migalife.c

clean:
	rm -f migalife64_debug migalife64_debug.o
