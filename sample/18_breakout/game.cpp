#include "game.h"

#define GAME_WAIT 0
#define GAME_PLAY 1

#define BRICKS_X_COUNT 10
#define BRICKS_Y_COUNT 5

#define BRICK_WIDTH 32
#define BRICK_HEIGHT 20
#define BRICK_GAP 2

#define PADDLE_WIDTH 30
#define PADDLE_HEIGHT 6

#define BALL_VELOCITY_X 3
#define BALL_VELOCITY_Y 3

struct BallInfo 
{
	int vel_x, vel_y;
	int x, y;
};

int g_screen_width = 0;
int g_screen_height = 0;

int g_paddle_posx = 0;
int g_paddle_posy = 0;
struct BallInfo g_ball;
char g_bricks[BRICKS_X_COUNT][BRICKS_Y_COUNT];
int g_gameState = GAME_WAIT;


void ResetStage()
{
	for (int y = 0; y < BRICKS_Y_COUNT; y++)
	{
		for (int x = 0; x < BRICKS_X_COUNT; x++)
		{
			g_bricks[x][y] = 1;
		}
	}
}

static int GetBrickCount()
{
	int remainBlocksCount = 0;

	for (int y = 0; y < BRICKS_Y_COUNT; y++)
	{
		for (int x = 0; x < BRICKS_X_COUNT; x++)
		{
			if (g_bricks[x][y] != 0)
			{
				remainBlocksCount++;
			}
		}
	}

	return remainBlocksCount;
}


void ResetBall()
{
	g_ball.x = g_screen_width / 2;
	g_ball.y = g_screen_height / 2;
	g_ball.vel_x = BALL_VELOCITY_X;
	g_ball.vel_y = BALL_VELOCITY_Y;
}

int InRect(int posx, int posy, int left, int top, int right, int bottom)
{
	if (posx > left && posx < right && posy > top && posy < bottom)
		return 1;	

	return 0;
}

void UpdatePaddle(int paddle_posx) 
{	
	g_paddle_posx = paddle_posx;

	//패들이 오른쪽 가장자리를 넘어가지 못하게 처리한다.
	if (g_paddle_posx >= g_screen_width - PADDLE_WIDTH)
		g_paddle_posx = g_screen_width - PADDLE_WIDTH;
}

//공이 패들에 맞는 위치와 패들의 이동속도를 사용해서 공의 속도를 갱신하자
//지금은 구현하지 않음
void RandomBallSpeed()
{
	if (g_ball.x < g_paddle_posx + (PADDLE_WIDTH / 2))
	{
		if (g_ball.vel_x > 0)
			g_ball.vel_x *= -1;
	}
	else
	{
		if (g_ball.vel_x < 0)
			g_ball.vel_x *= -1;
	}
}

void UpdateBall() 
{	
	//패들과 공의 충돌체크
	//공이 아래로 내려가는 경우에만 패들과 충돌할 수 있다.
	if (g_ball.vel_y > 0)
	{ 
		if (InRect(g_ball.x, g_ball.y,
			g_paddle_posx,
			g_paddle_posy,
			g_paddle_posx + (PADDLE_WIDTH),
			g_paddle_posy + PADDLE_HEIGHT) == 1) 
		{			
			g_ball.vel_y *= -1; //패들과 공이 충돌했다면 반전시킨다.

			RandomBallSpeed();
		}				
	}

	// 속도 기반으로 볼의 위치를 갱신한다.
	g_ball.x += g_ball.vel_x;
	g_ball.y += g_ball.vel_y;

	//벽돌 상태를 갱신한다
	for (int y = 0; y < BRICKS_Y_COUNT; y++) 
	{
		for (int x = 0; x < BRICKS_X_COUNT; x++) 
		{
			if (g_bricks[x][y] != 0)  //벽돌이 존재한다면
			{ 
				//벽돌과 공이 충돌했다면 공의 y속도를 반전시키고 벽돌을 없앤다.
				if (InRect(g_ball.x, g_ball.y,
					x * BRICK_WIDTH, y * BRICK_HEIGHT,
					(x + 1) * BRICK_WIDTH - BRICK_GAP,
					(y + 1) * BRICK_HEIGHT - BRICK_GAP) == 1) 
				{					
					g_ball.vel_y *= -1;
					g_bricks[x][y] = 0;
				}
			}
		}
	}

	// 공이 화면의 가장자리와 부딪치면 속도를 반전시킨다.
	if (g_ball.x > g_screen_width) {
		g_ball.vel_x *= -1;
	}
	if (g_ball.x < 0) {
		g_ball.vel_x *= -1;
	}

	if (g_ball.y < 0) {
		g_ball.vel_y *= -1;
	}

	// 공이 바닥으로 내려가면 업데이트를 멈추고 대기상태로 만든다.
	if (g_ball.y > g_screen_height)
	{		
		g_gameState = GAME_WAIT;		
	}
}

void InitGame(int screen_width, int screen_height)
{
	g_screen_width = screen_width;
	g_screen_height = screen_height;

	g_paddle_posy = screen_height - PADDLE_HEIGHT;

	ResetStage();
	ResetBall();
}

void ProcessLogic(int mouseClicked, int mouse_posx)
{
	UpdatePaddle(mouse_posx);

	switch (g_gameState)
	{
	case GAME_PLAY:
	{
		UpdateBall();
	}
	break;
	case GAME_WAIT:
	{
		if (mouseClicked == 1)
		{
							
			ResetBall();			
			g_gameState = GAME_PLAY;
		}
	}
	break;
	}
}

void CheckGameEnd()
{
	if (GetBrickCount() == 0)
	{
		InitGame(g_screen_width, g_screen_height);
	}
}

extern void RenderRect(int x1, int y1, int x2, int y2, int r, int g, int b, int fill); //사각형을 그리는 함수
extern void RenderCircle(int x, int y, int radius, int r, int g, int b); // 원을 그리는 함수
extern void RenderLine(int x, int y, int x2, int y2, int r, int g, int b);
extern void RenderText(int x, int y, char* szMessage);

void DrawWorld()
{
	//벽돌을 화면에 그린다.
	for (int y = 0; y < BRICKS_Y_COUNT; y++)
	{
		for (int x = 0; x < BRICKS_X_COUNT; x++)
		{
			if (g_bricks[x][y] != 0)
			{
				RenderRect(x * BRICK_WIDTH, y * BRICK_HEIGHT, (x + 1) * BRICK_WIDTH - BRICK_GAP, (y + 1) * BRICK_HEIGHT - BRICK_GAP, 255, 0, 0, 1);
			}
		}
	}

	if (g_gameState == GAME_PLAY)
	{
		//공을 화면에 그린다
		RenderCircle(g_ball.x, g_ball.y, 4, 255, 255, 0);
	}

	// 패들을 화면에 그린다.
	RenderRect(g_paddle_posx, g_paddle_posy, g_paddle_posx + PADDLE_WIDTH, g_paddle_posy + PADDLE_HEIGHT, 255, 255, 255, 1);
}