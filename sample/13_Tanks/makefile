OPTIMIZED_CFLAGS = -O3 -ffast-math
CFLAGS = $(OPTIMIZED_CFLAGS) -Wdeprecated-declarations  \
	-I. -I..\..\allegro\include -I.\include
CPPFLAGS = $(OPTIMIZED_CFLAGS) -fpermissive -Wdeprecated-declarations  \
-I. -I..\..\allegro\include -I.\include
CC = gcc.exe
CPP = gpp.exe

.c.o: 
	$(CPP) $(CPPFLAGS) -c $< 2>&1 | tee -a compile.log

all: main.exe

objs = src/barrier.o src/bullet.o src/button.o src/coordinate.o src/game.o \
	   src/globals.o src/init.o src/keyListener.o src/main.o src/menu.o	\
	   src/mouseListener.o src/particle.o src/powerup.o src/state.o src/tank.o \
	   src/tileMap.o src/tools.o src/world.o

main.exe : $(objs)
	gcc -o main.exe $(objs) \
		-L../../allegro/lib -lalleg -lstdc++
	strip main.exe

clean:
	rm -f src/*.o 	
	rm -f compile.log
	rm -f main.exe
