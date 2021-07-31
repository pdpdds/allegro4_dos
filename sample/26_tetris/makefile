OPTIMIZED_CFLAGS = -O3 -ffast-math
CFLAGS = $(OPTIMIZED_CFLAGS)  -Wpointer-sign -Wdeprecated-declarations -Wunused-but-set-variable \
	-I. -I..\..\allegro\include -I.\include	
CFLAGS += -DUSE_DATAFILE
CPPFLAGS = $(OPTIMIZED_CFLAGS) -Wall  \
	-I. -I..\..\allegro\include -I.\include
CC = gcc.exe
CPP = gpp.exe

.c.o: 
	$(CC) $(CFLAGS) -c $< 2>&1 | tee -a compile.log

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $< 

all: main.exe

main.exe : $(patsubst %.c,%.o,$(wildcard src/*.c))
	gcc -o main.exe ./src/*.o\
		-L../../allegro/lib -lalleg -lstdc++
	strip main.exe

clean:
	rm -f *.o
	rm -f src/*.o	
	rm -f compile.log
	rm -f main.exe