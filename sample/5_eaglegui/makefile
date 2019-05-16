OPTIMIZED_CFLAGS = -O3 -ffast-math
CFLAGS = $(OPTIMIZED_CFLAGS) -Wall  \
	-I. -I..\allegro\include -I..\include	
CFLAGS += -DUSE_DATAFILE
CPPFLAGS = $(OPTIMIZED_CFLAGS) -Wall  \
	-I. -I..\allegro\include -I./include
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

objs = AllegroSetup.o key_handler.o timer_handler.o AllegroUtilities.o Area.o \
	   	BitmapHandler.o Blending.o Blitting.o Color.o Drawing.o Eagle.o EagleCredits.o Error.o \
		FileWork.o GfxModeHandler.o InputHandler.o Math.o Resource.o ResourceRegistry.o Sound.o \
		StringWork.o TextDisplay.o TileMap.o Logging.o BitmapUtility.o MouseHandler.o

a.exe : $(patsubst %.cpp,%.o,$(wildcard GuiCode/*.cpp)) \
		$(patsubst %.cpp,%.o,$(wildcard TestPrograms/*.cpp)) $(objs)
	gcc -o a.exe $(objs) \
		./GuiCode/*.o \
		./TestPrograms/*.o \
		-L../allegro/lib -lalleg -lstdc++
	strip a.exe

clean:
	rm -f *.o 
	rm -f GuiCode/*.o	
	rm -f compile.log
	rm -f a.exe
