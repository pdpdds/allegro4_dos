OPTIMIZED_CFLAGS = -O3 -ffast-math
CFLAGS = $(OPTIMIZED_CFLAGS) -Wall  \
	-I./src/lib/allegro/include -I./src/lib/coll-c/include \
	-I./src/lib/sxmlc -I./src/lib/fastsqrt \
	-I./src/lib/ppcol/include -I./src
CFLAGS += -DUSE_DATAFILE
CC = gcc.exe
CPP = gpp.exe

.c.o: 
	$(CC) $(CFLAGS) -c $< 2>&1 | tee -a compile.log

src/lib/coll-c/%.o: src/lib/coll-c/%.c
	$(CC) -std=gnu99 $(CFLAGS) -c -o $@ $< 

src/lib/sxmlc/%.o: src/lib/sxmlc/%.c
	$(CC) $(CFLAGS) -c -o $@ $< 

src/lib/fastsqrt/%.o: src/lib/fastsqrt/%.c
	$(CC) $(CFLAGS) -c -o $@ $< 

all: main.exe

objs = src/fox.o src/opening.o src/menu.o src/gradient.o src/tool.o src/intro.o \
	   	src/hanout.o src/hanconv.o src/move.o src/effect.o src/sintbl.o src/game.o src/stage.o src/globals.o 
		
objs2 = fox.o opening.o menu.o gradient.o tool.o intro.o \
	   	hanout.o hanconv.o move.o effect.o sintbl.o game.o stage.o globals.o 		

main.exe : $(patsubst %.c,%.o,$(wildcard src/lib/fastsqrt/*.c)) \
	$(patsubst %.c,%.o,$(wildcard src/lib/coll-c/*.c)) $(objs) \
	$(patsubst %.c,%.o,$(wildcard src/lib/sxmlc/*.c)) $(objs)
	gcc -o main.exe $(objs2) \
		./src/lib/coll-c/*.o ./src/lib/fastsqrt/*.o ./src/lib/sxmlc/*.o \
		-L./src/lib/allegro/lib -lalleg
	strip main.exe

clean:
	rm -f *.o 
	rm -f src/lib/coll-c/*.o
	rm -f src/lib/sxmlc/*.o
	rm -f src/lib/fastsqrt/*.o
	rm -f compile.log
	rm -f main.exe
