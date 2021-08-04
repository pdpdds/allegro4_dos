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

	//�е��� ������ �����ڸ��� �Ѿ�� ���ϰ� ó���Ѵ�.
	if (g_paddle_posx >= g_screen_width - PADDLE_WIDTH)
		g_paddle_posx = g_screen_width - PADDLE_WIDTH;
}

//���� �е鿡 �´� ��ġ�� �е��� �̵��ӵ��� ����ؼ� ���� �ӵ��� ��������
//������ �������� ����
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
	//�е�� ���� �浹üũ
	//���� �Ʒ��� �������� ��쿡�� �е�� �浹�� �� �ִ�.
	if (g_ball.vel_y > 0)
	{ 
		if (InRect(g_ball.x, g_ball.y,
			g_paddle_posx,
			g_paddle_posy,
			g_paddle_posx + (PADDLE_WIDTH),
			g_paddle_posy + PADDLE_HEIGHT) == 1) 
		{			
			g_ball.vel_y *= -1; //�е�� ���� �浹�ߴٸ� ������Ų��.

			RandomBallSpeed();
		}				
	}

	// �ӵ� ������� ���� ��ġ�� �����Ѵ�.
	g_ball.x += g_ball.vel_x;
	g_ball.y += g_ball.vel_y;

	//���� ���¸� �����Ѵ�
	for (int y = 0; y < BRICKS_Y_COUNT; y++) 
	{
		for (int x = 0; x < BRICKS_X_COUNT; x++) 
		{
			if (g_bricks[x][y] != 0)  //������ �����Ѵٸ�
			{ 
				//������ ���� �浹�ߴٸ� ���� y�ӵ��� ������Ű�� ������ ���ش�.
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

	// ���� ȭ���� �����ڸ��� �ε�ġ�� �ӵ��� ������Ų��.
	if (g_ball.x > g_screen_width) {
		g_ball.vel_x *= -1;
	}
	if (g_ball.x < 0) {
		g_ball.vel_x *= -1;
	}

	if (g_ball.y < 0) {
		g_ball.vel_y *= -1;
	}

	// ���� �ٴ����� �������� ������Ʈ�� ���߰� �����·� �����.
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

extern void RenderRect(int x1, int y1, int x2, int y2, int r, int g, int b, int fill); //�簢���� �׸��� �Լ�
extern void RenderCircle(int x, int y, int radius, int r, int g, int b); // ���� �׸��� �Լ�
extern void RenderLine(int x, int y, int x2, int y2, int r, int g, int b);
extern void RenderText(int x, int y, char* szMessage);

void DrawWorld()
{
	//������ ȭ�鿡 �׸���.
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
		//���� ȭ�鿡 �׸���
		RenderCircle(g_ball.x, g_ball.y, 4, 255, 255, 0);
	}

	// �е��� ȭ�鿡 �׸���.
	RenderRect(g_paddle_posx, g_paddle_posy, g_paddle_posx + PADDLE_WIDTH, g_paddle_posy + PADDLE_HEIGHT, 255, 255, 255, 1);
}