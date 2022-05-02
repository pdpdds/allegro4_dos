CFLAGS = -I. -I../../allegro/include
LDFLAGS = -L../../allegro/lib


CC=$(DJGPP_GCC)


# -------- executable file name --------
PROGRAM = main

# -------- source file --------
SRC = $(PROGRAM).c

# -------- basic compile --------
all: $(PROGRAM).c
	@echo BUILD... $(PROGRAM).exe 
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) -o $(PROGRAM).exe -lalleg