CFLAGS = -I. -Iinclude -I..\\..\allegro\include
LDFLAGS = -Llib -L..\..\allegro\lib

# -------- 실행 프로그램 이름 입력 --------
PROGRAM = dumbplay

# -------- 컴파일시킬 소스 파일들 --------
SRC = $(PROGRAM).c

# -------- 기본적인 컴파일 --------
all: $(PROGRAM).c
	@echo BUILD... $(PROGRAM).exe 
	gcc $(CFLAGS) $(LDFLAGS) $(SRC) -o $(PROGRAM).exe -laldmb -ldumb -lalleg  