OPTIMIZED_CFLAGS = -O3 -ffast-math
CFLAGS = $(OPTIMIZED_CFLAGS) -Wdeprecated-declarations  \
	-I. -I..\..\allegro\include
CPPFLAGS = $(OPTIMIZED_CFLAGS) -fpermissive -Wdeprecated-declarations  \
-I. -I..\..\allegro\include
CC = gcc.exe
CPP = gpp.exe

.c.o: 
	$(CPP) $(CPPFLAGS) -c $< 2>&1 | tee -a compile.log

all: main.exe

objs = Armour.o Bullet.o Character.o Cobra.o Explosion.o Health.o \
       HealthBar.o HeavyArmour.o LightArmour.o LinkedList.o main.o \
	   Plane.o Player.o Power.o Terrain.o Tile.o Viper.o Word.o

main.exe : $(objs)
	gcc -o main.exe $(objs) \
		-L../../allegro/lib -lalleg -lstdc++
	strip main.exe

clean:
	rm -f *.o 	
	rm -f compile.log
	rm -f main.exe
