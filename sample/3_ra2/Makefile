OPTIMIZED_CFLAGS = -O3 -ffast-math
CFLAGS = $(OPTIMIZED_CFLAGS) -Wall  \
	-I. -I..\..\allegro\include
CPPFLAGS = $(OPTIMIZED_CFLAGS) -Wall  \
-I. -I..\..\allegro\include
CC = gcc.exe
CPP = gpp.exe

.c.o: 
	$(CPP) $(CFLAGS) -c $< 2>&1 | tee -a compile.log

all: main.exe

objs = boots.o colly.o key.o keyhole.o level.o lrblock.o \
	   	object.o player.o ra2.o udblock.o

main.exe : $(objs)
	gcc -o main.exe $(objs) \
		-L../../allegro/lib -lalleg -lstdc++
	strip main.exe

clean:
	rm -f *.o 	
	rm -f compile.log
	rm -f main.exe
