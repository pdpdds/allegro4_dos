#include "game.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define GAME_WAIT 0
#define GAME_PLAY 1
int g_gameState = GAME_WAIT;

void randomize(void) {
	srand((unsigned)time(NULL));
	// 첫부분의 숫자는 거의 랜덤하지 않기에, 앞부분에서 랜덤한 개수로 뽑아서 버림
	for (int i = 0; i < (rand() % RAND_MAX); i++) (rand() % RAND_MAX);
}

void clear_board();

#define WD 15
#define X 230
#define Y 40
#define DX 45
#define STX 49

int g_screen_width = 0;
int g_screen_height = 0;

unsigned long int g_score[5];

unsigned char g_board[21][12];

/* 블럭 모양 설정 */
static char block[7][4][4][4] = {
		{ {{ 1, 1, 1, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }},

	   {{ 1, 1, 0, 0 },
		{ 1, 0, 0, 0 },
		{ 1, 0, 0, 0 },
		{ 0, 0, 0, 0 }},

	   {{ 1, 0, 0, 0 },
		{ 1, 1, 1, 0 },
		{ 0, 0, 0, 0 },
		  { 0, 0, 0, 0 }},

	   {{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 1, 1, 0, 0 },
		{ 0, 0, 0, 0 }} },//#1

		{ {{ 0, 0, 0, 0 },
		{ 1, 1, 1, 1 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }},

	   {{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 }},

	   {{ 0, 0, 0, 0 },
		{ 1, 1, 1, 1 },
		{ 0, 0, 0, 0 },
		  { 0, 0, 0, 0 }},

	   {{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 }} },//#2

		{ {{ 0, 1, 1, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }},

	   {{ 0, 1, 1, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }},

	   {{ 0, 1, 1, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 0, 0 },
		  { 0, 0, 0, 0 }},

	   {{ 0, 1, 1, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }} },//3

		{ {{ 0, 1, 0, 0 },
		{ 1, 1, 1, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }},

	   {{ 0, 1, 0, 0 },
		{ 1, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 0, 0 }},

	   {{ 0, 0, 0, 0 },
		{ 1, 1, 1, 0 },
		{ 0, 1, 0, 0 },
		  { 0, 0, 0, 0 }},

	   {{ 0, 1, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 0, 0 }} },//#4

		{ {{ 0, 0, 1, 0 },
		{ 1, 1, 1, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }},

	   {{ 1, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 0, 0 }},

	   {{ 1, 1, 1, 0 },
		{ 1, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		  { 0, 0, 0, 0 }},

	   {{ 1, 0, 0, 0 },
		{ 1, 0, 0, 0 },
		{ 1, 1, 0, 0 },
		{ 0, 0, 0, 0 }} },//#5

		{ {{ 1, 1, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }},

	   {{ 0, 1, 0, 0 },
		{ 1, 1, 0, 0 },
		{ 1, 0, 0, 0 },
		{ 0, 0, 0, 0 }},

	   {{ 1, 1, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 0, 0 },
		  { 0, 0, 0, 0 }},

	   {{ 0, 1, 0, 0 },
		{ 1, 1, 0, 0 },
		{ 1, 0, 0, 0 },
		{ 0, 0, 0, 0 }} },//#6

		{ {{ 0, 1, 1, 0 },
		{ 1, 1, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }},

	   {{ 1, 0, 0, 0 },
		{ 1, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 0, 0 }},

	   {{ 0, 1, 1, 0 },
		{ 1, 1, 0, 0 },
		{ 0, 0, 0, 0 },
		  { 0, 0, 0, 0 }},

	   {{ 1, 0, 0, 0 },
		{ 1, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 0, 0 }} } };//#7

static int color[8] = { 0,1,2,4,6,10,13,14 };
static int patten[8] = { 0,4,8,2,11,4,5,5 };
static unsigned long newscr = 0;  /* 점수를 저장하는 변수 선언 */

int InRect(int posx, int posy, int left, int top, int right, int bottom)
{
	if (posx > left && posx < right && posy > top && posy < bottom)
		return 1;	

	return 0;
}

/* 그래픽 모드에서 printf()기능을 하는 함수 */
int gprintf(int xpos, int ypos, char* format, ...)
{
	va_list argptr;
	char string[140];
	int count;

	va_start(argptr, format);

	count = vsprintf(string, format, argptr);
	RenderText(xpos, ypos, string);
	va_end(argptr);
	return(count);
}

/* 점수판의 점수와 newscr을 비교하여 정렬하는 함수 */
void highscr(void)
{
	static unsigned long int begin[5] = { 0, 0, 0, 0, 0 };
	
	static unsigned long int t[5];
	int i, j;

	FILE* fp;

	if ((fp = fopen("score.dat", "r")) == NULL)
	{
		fp = fopen("score.dat", "w+");
		for (i = 0; i < 5; i++)
			fprintf(fp, "%d\n", begin[i]);
	}

	for (i = 0; i < 5; i++)
		fscanf(fp, "%lu", &g_score[i]);
	fclose(fp);

	fp = fopen("score.dat", "w+");
	for (i = 0; i < 5; i++)
	{
		if (newscr >= g_score[i])
		{
			t[0] = g_score[i];
			g_score[i] = newscr;
			for (j = 0; j < 5; j++)
			{
				t[j + 1] = g_score[i + j + 1];
				g_score[i + j + 1] = t[j];
			}
			break;

		}//if
	}//for
	for (i = 0; i < 5; i++) 
		fprintf(fp, "%lu\n", g_score[i]);

	fclose(fp);
}

/* 점수판에 점수를 표시하는 함수 */
void GetScoreBoard(void)
{
	int i;

	FILE* fp;

	if ((fp = fopen("score.dat", "r")) == NULL)
	{
		highscr();
	}
	else
	{
		for (i = 0; i < 5; i++)
			fscanf(fp, "%lu", &g_score[i]);
	}

	fclose(fp);
}

int g_nx, g_ny, g_m, a, g_num, g_num1;
int g_test = 0;

void InitGame(int screen_width, int screen_height)
{
	g_screen_width = screen_width;
	g_screen_height = screen_height;

	GetScoreBoard();
	randomize();
	clear_board();
	
	a = 0;
	g_m = 0;
	g_nx = 5;
	g_ny = 0;
	g_test = 0;

	g_num = rand() % 7;
	g_num1 = rand() % 7;
}



/* 벽돌 지우는 함수 */
void erase(int num, int m, int nx, int ny)
{
	int row, col;

	
	for (row = 0; row < 4; row++)
		for (col = 0; col < 4; col++)
			if (block[num][m][row][col] == 1)
				RenderRect(X + WD * (nx + col), Y + WD * (ny + row),
					X + WD * (1 + nx + col), Y + WD * (1 + ny + row), 0, 0, 0, 1);
}//erase

void drawboard()
{
	int i, j;

	for (i = 1; (i < 20); i++)
		for (j = 1; j < 11; j++)
		{
			if (g_board[i][j] == 1)
			{
				RenderRect(X + WD * (j)+1, Y + WD * (i)+1,
					X + WD * (1 + j) - 1, Y + WD * (1 + i) - 1, 100, 100, 100, 1);
			}
			
		}
}

/* 벽돌 그리는 함수 */
void draw(int num, int m, int nx, int ny)
{
	int col, row;

	for (row = 0; row < 4; row++)
		for (col = 0; col < 4; col++)
			if (block[num][m][row][col] == 1)
			{
				RenderRect(X + WD * (nx + col), Y + WD * (ny + row),
					X + WD * (1 + nx + col), Y + WD * (1 + ny + row), 255,0,255,0);
				RenderRect(X + WD * (nx + col) + 1, Y + WD * (ny + row) + 1,
					X + WD * (1 + nx + col) - 1, Y + WD * (1 + ny + row) - 1, 0, 255, 255, 1);

			}//if
}//draw

/* board 배열을 초기화하는 함수 */
void clear_board()
{
	int i, j;

	for (i = 0; i < 20; i++)
		for (j = 0; j < 11; j++)
		{
			g_board[i][j] = 0;
		}
	

	for (i = 0; i < 20; i++)
		g_board[i][0] = 1;

	for (i = 0; i < 20; i++)
		g_board[i][11] = 1;

	for (i = 0; i < 12; i++)
		g_board[20][i] = 1;
}

/* board와 떨어지는 벽돌을 비교하는 함수 */
void comp_board(int num, int m, int nx, int ny, int* test)
{
	int i, j;
	for (i = ny; i < ny + 4 && i <= 20; i++)
	{
		for (j = nx; j < nx + 4; j++)
			if ((g_board[i][j] == 1) && (g_board[i][j] == block[num][m][i - ny][j - nx]))
				*test = 1;
	}
}

/* 떨어지는 벽돌의 모양을 board에 저장하는 함수 */
void set_board(int num, int m, int nx, int ny)
{
	int i, j;

	for (i = ny; (i < ny + 4 && i < 20); i++)
		for (j = nx; j < nx + 4; j++)
			if (block[num][m][i - ny][j - nx] == 1)
				g_board[i][j] = block[num][m][i - ny][j - nx];
}

void DrawWorld()
{
	int i, j, k;

	for (i = 0; i < 20; i++)
	{
		RenderRect(X, Y + WD * i, X + WD, Y + WD * (i + 1), 0, 255, 255, 1);
		RenderRect(X + 1, Y + WD * i + 1, X + WD - 1, Y + WD * (i + 1) - 1, 255, 255, 255, 0);
	}//for


	for (j = 0; j < 20; j++)
	{
		RenderRect(X + WD * 11, Y + WD * j, X + WD * 12, Y + WD * (j + 1), 0, 255, 255, 1);
		RenderRect(X + WD * 11 + 1, Y + WD * j + 1, X + WD * 12 - 1, Y + WD * (j + 1) - 1, 255, 255, 255, 0);
	}//for

	for (k = 0; k < 12; k++)
	{
		RenderRect(X + WD * k, Y + WD * 20, X + WD * (k + 1), Y + WD * 21, 0, 255, 255, 1);
		RenderRect(X + WD * k + 1, Y + WD * 20 + 1, X + WD * (k + 1) - 1, Y + WD * 21 - 1, 255, 255, 255, 0);
	}//for

	
	RenderRect(40, 186, 193, 217, 0, 255, 255, 1);
	
	//가로바
	RenderRect(40, 237, 190, 239, 0, 255, 255, 1);
	RenderRect(40, 257, 190, 259, 0, 255, 255, 1);
	RenderRect(40, 277, 190, 279, 0, 255, 255, 1);
	RenderRect(40, 297, 190, 299, 0, 255, 255, 1);
	RenderRect(40, 317, 190, 321, 0, 255, 255, 1);
	RenderRect(39, 346, 190, 350, 0, 255, 255, 1);

	//세로바 가장자리
	RenderRect(39, 187, 43, 347, 0, 255, 255, 1);
	RenderRect(190, 186, 194, 350, 0, 255, 255, 1);


	RenderRect(41, 188, 192, 348, 255, 255, 255, 0);
	RenderLine(41, 214, 191, 214, 255, 255, 255);
	RenderLine(41, 319, 191, 319, 255, 255, 255);
	RenderText(46, 194, "HIGH SCORE");

	//스코어를 출력한다.
	int x, y;
	for (i = 0; i < 5; i++)
	{
		RenderRect(DX, i * 20 + 219, DX + 143, i * 20 + 237, 255, 255, 255, 0);
		x = STX;
		y = i * 20 + 232;

		gprintf(x, y, "%11lu", g_score[i]);
	}

	y = 6 * 20 + 212;
	RenderRect(45, 327, 188, 345, 0, 0, 0, 1);
	gprintf(x, y, "%9lu", newscr);

	drawboard();
	draw(g_num, g_m, g_nx, g_ny);
	draw(g_num1,0, -5, 3);

	for (i = 0; i < 5; i++)
	{
		RenderRect(DX, i * 20 + 219, DX + 143, i * 20 + 237, 0, 0, 0, 1);
		x = STX;
		y = i * 20 + 222;
		
		gprintf(x, y, "%11lu", g_score[i]);
	}
}

/* board의 한 줄이 다 채워졌는지 검사하는 함수 */
void test_board(int ny)
{
	int i, j, k, test1, n = 1;
	int x, y;

	if (ny >= 16)
	{
		i = 19;
		while (n <= (20 - ny))
		{
			x = 40, y = 328;

			n++;
			test1 = 1;
			for (j = 1; j < 11; j++)
			{
				if (g_board[i][j] == 0)
					test1 = 0;
			}//for
			if (test1 == 1)
			{

				//sound4();
				newscr += 50;
				
				RenderRect(X + WD, Y + WD * i - 0.5, X + WD * 11, Y + WD * (i + 1) + 0.5, 255,255,255,1);

				
				

				

				for (j = i; j > 0; j--)
					for (k = 1; k < 11; k++)
						g_board[j][k] = g_board[j - 1][k];

				for (k = 1; k < 11; k++)
					g_board[0][k] = 0;
			}//if
			if (test1 != 1)
				i--;
		}//while
	}//if

	else
	{
		i = ny + 3;
		while (n <= 4)
		{
			n++;
			test1 = 1;
			x = 40, y = 328;

			for (j = 1; j < 11; j++)
			{
				if (g_board[i][j] == 0)
				{
					test1 = 0;
					break;
				}
			}//for
			if (test1 == 1)
			{
				
				RenderRect(X + WD, Y + WD * i - 0.5, X + WD * 11, Y + WD * (i + 1) + 0.5, 255,255,255,1);
				//sound4();
				newscr += 50;
				RenderRect(45, 327, 188, 345, 255, 255, 255, 1);
				//settextstyle(0, 0, 2);
				gprintf(x, y, "%9lu", newscr);

				
				for (j = i; j > 0; j--)
					for (k = 1; k < 11; k++)
						g_board[j][k] = g_board[j - 1][k];

				for (k = 1; k < 11; k++)
					g_board[0][k] = 0;
			}
			if (test1 != 1)
				i--;
		}//while
	}//if
}//end

/* 벽돌 떨구는 함수 */
void base_block(void)
{
	
	/*while (board[1][6] != 1)
	{
		

		

		do
		{
			draw(num, m, nx, ny);


			if (*test != 1)
			{
				comp_board(num, m, nx, ny + 1, test);
				if (*test == 1)
				{
					set_board(num, m, nx, ny);
					//sound1();
					test_board(ny);

				}
				else
				{
					erase(num, m, nx, ny);
					ny++;
				}//else
			}//if

		} while (*test == 0 && ny < 20);
		*test = 0;
		erase(num1, 0, -5, 3);
		num = num1;
	}*/
	//while
	//sound2();
}//base_block

void CheckGameEnd()
{
	if (g_board[1][6] == 1)
	{
		g_gameState = GAME_WAIT;
	}
}

void ProcessLogic(int key_left, int key_right, int key_up, int key_down, int key_space)
{

	
	
	if (g_gameState == GAME_WAIT)
	{
		if (key_space)
		{
			highscr();
			InitGame(g_screen_width, g_screen_height);
			g_gameState = GAME_PLAY;
			newscr = 0;
		}
		return;
	}

	//sound3();
	
	if (g_test == 0 && g_ny < 20)
	{
		if (key_left)
		{
			comp_board(g_num, g_m, g_nx - 1, g_ny, &g_test);
			if (g_test == 1)
				g_test = 0;

			else
			{
				//erase(g_num, g_m, g_nx, g_ny);
				--g_nx;
				//draw(g_num, g_m, g_nx, g_ny);
			}
		}

		if (key_right)
		{

			comp_board(g_num, g_m, g_nx + 1, g_ny, &g_test);
			if (g_test == 1)
				g_test = 0;
			else
			{
				//erase(g_num, g_m, g_nx, g_ny);
				++g_nx;
				//draw(g_num, g_m, g_nx, g_ny);
			}
		}


		if (key_up)
		{
			comp_board(g_num, (g_m + 1) % 4, g_nx, g_ny, &g_test);
			if (g_test == 1)
				g_test = 0;
			else
			{
				//erase(g_num, g_m, g_nx, g_ny);
				g_m = (++g_m) % 4;
				//draw(g_num, g_m, g_nx, g_ny);
			}
		}
		if (key_space)
		{

			while (g_test != 1)
				comp_board(g_num, g_m, g_nx, ++g_ny, &g_test);

			g_ny = g_ny - 1;

			set_board(g_num, g_m, g_nx, g_ny);
			//sound1();
			test_board(g_ny);
			a = 1;
		}

		if (g_test != 1)
		{
			comp_board(g_num, g_m, g_nx, g_ny + 1, &g_test);
			if (g_test == 1)
			{
				set_board(g_num, g_m, g_nx, g_ny);
				//sound1();
				test_board(g_ny);

			}
			else
			{
				g_ny++;
			}
		}
	}
	else
	{
		g_test = 0;
		g_num = g_num1;

		a = 0;
		g_m = 0;
		g_nx = 5;
		g_ny = 0;

		g_num1 = rand () % 7;
	}

	CheckGameEnd();
}