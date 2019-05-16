OPTIMIZED_CFLAGS = -O3 -ffast-math
CFLAGS = $(OPTIMIZED_CFLAGS) -Wall  \
	-I./lib/allegro/include -I./lib/coll-c/include \
	-I./lib/sxmlc -I./lib/fastsqrt \
	-I./lib/ppcol/include -I.
CFLAGS += -DUSE_DATAFILE
CC = gcc.exe
CPP = gpp.exe

.c.o: 
	$(CC) $(CFLAGS) -c $< 2>&1 | tee -a compile.log

lib/coll-c/%.o: lib/coll-c/%.c
	$(CC) -std=gnu99 $(CFLAGS) -c -o $@ $< 

lib/sxmlc/%.o: lib/sxmlc/%.c
	$(CC) $(CFLAGS) -c -o $@ $< 

lib/fastsqrt/%.o: lib/fastsqrt/%.c
	$(CC) $(CFLAGS) -c -o $@ $< 

all: a.exe

objs = fox.o opening.o menu.o gradient.o tool.o intro.o \
	   	hanout.o hanconv.o move.o effect.o sintbl.o game.o stage.o globals.o 

a.exe : $(patsubst %.c,%.o,$(wildcard lib/fastsqrt/*.c)) \
	$(patsubst %.c,%.o,$(wildcard lib/coll-c/*.c)) $(objs) \
	$(patsubst %.c,%.o,$(wildcard lib/sxmlc/*.c)) $(objs)
	gcc -o a.exe $(objs) \
		./lib/coll-c/*.o ./lib/fastsqrt/*.o ./lib/sxmlc/*.o \
		./lib/ppcol/lib/LIBPPCOL.A -L./lib/allegro/lib -lalleg
	strip a.exe

clean:
	rm -f *.o 
	rm -f lib/coll-c/*.o
	rm -f lib/sxmlc/*.o
	rm -f lib/fastsqrt/*.o
	rm -f compile.log
	rm -f a.exe
