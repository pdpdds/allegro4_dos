CFLAGS = -I. -Iinclude -Iinclude\libxml2 -I..\\..\allegro\include
LDFLAGS = -Llib -L..\..\allegro\lib

# -------- 실행 프로그램 이름 입력 --------
PROGRAM = tree

# -------- 컴파일시킬 소스 파일들 --------
SRC = $(PROGRAM).c

# -------- 기본적인 컴파일 --------
all: $(PROGRAM).c
	@echo BUILD... $(PROGRAM).exe 
	gcc $(CFLAGS) $(LDFLAGS) $(SRC) -o $(PROGRAM).exe -lxml2 -lalleg -lz -llzma