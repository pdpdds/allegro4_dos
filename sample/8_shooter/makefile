OPTIMIZED_CFLAGS = -O3 -ffast-math
CFLAGS = $(OPTIMIZED_CFLAGS) -Wdeprecated-declarations  \
	-I. -I..\..\allegro\include
CPPFLAGS = $(OPTIMIZED_CFLAGS) -fpermissive -Wdeprecated-declarations  \
-I. -I..\..\allegro\include
CC = gcc.exe
CPP = gpp.exe

.c.o: 
	$(CC) $(CFLAGS) -c $< 2>&1 | tee -a compile.log

all: main.exe

objs = src/animsel.o src/aster.o src/bullet.o src/demo.o src/dirty.o src/display.o \
	   	src/expl.o src/game.o src/star.o src/title.o

main.exe : $(objs)
	gcc -o main.exe $(objs) \
		-L../../allegro/lib -lalleg -lstdc++
	strip main.exe

clean:
	rm -f src/*.o 	
	rm -f compile.log
	rm -f main.exe
