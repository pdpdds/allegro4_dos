OPTIMIZED_CFLAGS = -O3 -ffast-math
CFLAGS = $(OPTIMIZED_CFLAGS) -Wdeprecated-declarations  \
	-I. -I..\allegro\include
CPPFLAGS = $(OPTIMIZED_CFLAGS) -fpermissive -Wdeprecated-declarations  \
-I. -I..\allegro\include
CC = gcc.exe
CPP = gpp.exe

.c.o: 
	$(CPP) $(CPPFLAGS) -c $< 2>&1 | tee -a compile.log

all: main.exe

objs = menumagm.o boatrage.o control.o mapedit.o menu.o objects.o \
	   	player.o sprite.o tilemap.o view.o wrappers.o

main.exe : $(objs)
	gcc -o main.exe $(objs) \
		-L../allegro/lib -lalleg -lstdc++
	strip main.exe

clean:
	rm -f *.o 	
	rm -f compile.log
	rm -f main.exe
