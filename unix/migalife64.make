MIGALIFE64_STACK_SIZE=16777216
MIGALIFE64_C_FLAGS=-DLONGLONG_PTR -std=c99 -O2 -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winline -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-import -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings

migalife64: migalife64.o
	gcc -Wl,--stack,${MIGALIFE64_STACK_SIZE} -o migalife64 migalife64.o

migalife64.o: ../migalife.c migalife64.make
	gcc -c ${MIGALIFE64_C_FLAGS} -o migalife64.o ../migalife.c

clean:
	rm -f migalife64 migalife64.o
