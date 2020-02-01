OPTIMIZED_CFLAGS = -O3 -ffast-math
CFLAGS = $(OPTIMIZED_CFLAGS) -Wdeprecated-declarations  \
	-I. -I..\..\allegro\include
CPPFLAGS = $(OPTIMIZED_CFLAGS) -fpermissive -Wdeprecated-declarations  \
-I. -I..\..\allegro\include -I./Common
CC = gcc.exe
CPP = gpp.exe

.c.o: 
	$(CPP) $(CPPFLAGS) -c $< 2>&1 | tee -a compile.log

all: main.exe

main.exe : $(patsubst %.cc,%.o,$(wildcard src/*.cc))
	gcc -o main.exe \
	./src/*.o \
		-L../../allegro/lib -lalleg -lstdc++
	strip main.exe

clean:
	rm -f src/*.o 	
	rm -f compile.log
	rm -f main.exe
