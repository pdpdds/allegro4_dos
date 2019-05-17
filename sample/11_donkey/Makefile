CFLAGS = -I. -I../../allegro/include
CPPFLAGS = -I. -I../../allegro/include
LDFLAGS = -L../../allegro/lib

# -------- 실행 프로그램 이름 입력 --------
PROGRAM = main

.c.o: 
	$(CPP) $(CFLAGS) -c $< 2>&1 | tee -a compile.log

# -------- 컴파일시킬 소스 파일들 --------
SRC = $(PROGRAM).cpp

# -------- 기본적인 컴파일 --------
all: $(PROGRAM).cpp
	@echo BUILD... $(PROGRAM).exe 
	gcc $(CPPFLAGS) $(LDFLAGS) $(SRC) -o $(PROGRAM).exe -lalleg -lstdc++	
	strip $(PROGRAM).exe

clean:
	rm -f *.o 	
	rm -f compile.log
	rm -f $(PROGRAM).exe