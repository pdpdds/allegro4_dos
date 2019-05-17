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

objs = animsel.o aster.o bullet.o demo.o dirty.o display.o \
	   	expl.o game.o star.o title.o

main.exe : $(objs)
	gcc -o main.exe $(objs) \
		-L../../allegro/lib -lalleg -lstdc++
	strip main.exe

clean:
	rm -f *.o 	
	rm -f compile.log
	rm -f main.exe
