#include <allegro.h>

volatile long g_speed_counter = 0; //속도 카운터

void increment_speed_counter() //속도 카운터 값을 증가시키는 카운터 함수
{
	g_speed_counter++;
}
END_OF_FUNCTION(increment_speed_counter); //타이머 함수가 끝난다는 것을 명시적으로 설정

BITMAP* screenBuffer;


#define GAME_WAIT 0
#define GAME_PLAY 1

int gameState = GAME_WAIT;


#define BRICKS_HORIZONTALLY 10
#define BRICKS_VERTICALLY 5

#define BRICK_WIDTH 32
#define BRICK_HEIGHT 20
#define BRICK_GAP 2

#define PADDLE_WIDTH 30
#define PADDLE_HEIGHT 6
#define PADDLE_POSITION_Y (SCREEN_H-PADDLE_HEIGHT)

struct ball_typ {
	int xv, yv;
	int x, y;
};

int playerPaddleEdgeX;
ball_typ ball;

int bricks[BRICKS_VERTICALLY][BRICKS_HORIZONTALLY];

void ResetStage()
{
	for (int i = 0; i < BRICKS_VERTICALLY; i++)
	{
		for (int ii = 0; ii < BRICKS_HORIZONTALLY; ii++)
		{
			bricks[i][ii] = 1;
		}
	}
}

int numberOfBricksRemaining() {
	int numberOfBricksRemaining;

	numberOfBricksRemaining = 0;

	for (int i = 0; i < BRICKS_VERTICALLY; i++) {
		for (int ii = 0; ii < BRICKS_HORIZONTALLY; ii++) {
			if (bricks[i][ii] != 0) {
				numberOfBricksRemaining++;
			}
		}
	}
	return numberOfBricksRemaining;
}

void Render()
{
	clear_to_color(screenBuffer, makecol(0, 0, 255));

	// draw the bricks
	for (int i = 0; i < BRICKS_VERTICALLY; i++) {
		for (int ii = 0; ii < BRICKS_HORIZONTALLY; ii++) {
			if (bricks[i][ii] != 0) {
				rectfill(screenBuffer,
					ii * BRICK_WIDTH, i * BRICK_HEIGHT,
					(ii + 1) * BRICK_WIDTH - BRICK_GAP, (i + 1) * BRICK_HEIGHT - BRICK_GAP,
					makecol(255, 0, 0));
			}
		}
	}


	if (gameState == GAME_PLAY)
	{
		// draw the BALL
		circlefill(screenBuffer,
			ball.x, ball.y,
			4, makecol(255, 255, 0));
	}
	
	// draw the player
	rectfill(screenBuffer,
		playerPaddleEdgeX, PADDLE_POSITION_Y,
		playerPaddleEdgeX + PADDLE_WIDTH,
		PADDLE_POSITION_Y + PADDLE_HEIGHT, makecol(255, 255, 255));

}

void ResetBall()
{
	ball.x = SCREEN_W / 2;
	ball.y = SCREEN_H / 2;
	ball.xv = 4;
	ball.yv = 2;
}

int doesOverlap(int objectX, int objectY, int areaLeft, int areaTop, int areaRight, int areaBottom)
{
	if (ball.x > areaLeft &&
		ball.x < areaRight &&
		ball.y > areaTop &&
		ball.y < areaBottom) {
		return 1; // 1 here means yes
	}
	return 0; // 0 here means no
}

void updatePaddlePosition() {
	// for now, put the player's paddle where the mouse is

	playerPaddleEdgeX = mouse_x;

	if (playerPaddleEdgeX >= 320 - PADDLE_WIDTH)
		playerPaddleEdgeX = 320 - PADDLE_WIDTH;

}

void moveBall() {
	// update the ball's position for the next frame
	ball.x += ball.xv;
	ball.y += ball.yv;

	// if the ball is overlapping the rectangle
	if (ball.yv > 0)
	{ // only if the ball is moving down
		if (doesOverlap(ball.x, ball.y,
			playerPaddleEdgeX + (PADDLE_WIDTH * 0.0),
			PADDLE_POSITION_Y,
			playerPaddleEdgeX + (PADDLE_WIDTH * 0.25),
			PADDLE_POSITION_Y + PADDLE_HEIGHT) == 1) {
			ball.xv = -5;
			ball.yv = -3;
		}
		if (doesOverlap(ball.x, ball.y,
			playerPaddleEdgeX + (PADDLE_WIDTH * 0.25),
			PADDLE_POSITION_Y,
			playerPaddleEdgeX + (PADDLE_WIDTH * 0.5),
			PADDLE_POSITION_Y + PADDLE_HEIGHT) == 1) {
			ball.xv = -3;
			ball.yv = -5;
		}
		if (doesOverlap(ball.x, ball.y,
			playerPaddleEdgeX + (PADDLE_WIDTH * 0.5),
			PADDLE_POSITION_Y,
			playerPaddleEdgeX + (PADDLE_WIDTH * 0.75),
			PADDLE_POSITION_Y + PADDLE_HEIGHT) == 1) {
			ball.xv = 3;
			ball.yv = -5;
		}
		if (doesOverlap(ball.x, ball.y,
			playerPaddleEdgeX + (PADDLE_WIDTH * 0.75),
			PADDLE_POSITION_Y,
			playerPaddleEdgeX + (PADDLE_WIDTH * 1.0),
			PADDLE_POSITION_Y + PADDLE_HEIGHT) == 1) {
			ball.xv = 5;
			ball.yv = -3;
		}
	}

	// draw the bricks
	for (int i = 0; i < BRICKS_VERTICALLY; i++) {
		for (int ii = 0; ii < BRICKS_HORIZONTALLY; ii++) {
			if (bricks[i][ii] != 0) { // is the brick still here?
				if (doesOverlap(ball.x, ball.y,
					ii * BRICK_WIDTH, i * BRICK_HEIGHT,
					(ii + 1) * BRICK_WIDTH - BRICK_GAP,
					(i + 1) * BRICK_HEIGHT - BRICK_GAP) == 1) {
					// reverse ball's vertical direction
					ball.yv = -ball.yv;
					bricks[i][ii] = 0; // erase the brick
				}
			}
		}
	}

	// bounce off edges of screen
	if (ball.x > SCREEN_W) {
		ball.xv = -ball.xv;
	}
	if (ball.x < 0) {
		ball.xv = -ball.xv;
	}

	if (ball.y < 0) {
		ball.yv = -ball.yv;
	}

	// but reset ball if it goes off bottom of screen
	if (ball.y > SCREEN_H) {
		// lose!  

		if (mouse_b) { // makes the ball reappear
			ResetBall();
			gameState = GAME_WAIT;
		}
	}

}


void InitGame()
{
	ResetStage();
	ResetBall();
}

int main(int argc, char* argv[])
{
	allegro_init();
	install_mouse();
	install_keyboard();
	install_timer(); //타이머 시스템을 초기화한다

#ifdef _WIN32
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 320, 200, 0, 0) != 0) {
#else
					/* set a graphics mode sized 320x200 */
	if (set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0) != 0) {
#endif
		if (set_gfx_mode(GFX_SAFE, 320, 200, 0, 0) != 0) {
			set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
			allegro_message("Unable to set any graphic mode\n%s\n", allegro_error);
			return 0;
		}
	}

	LOCK_VARIABLE(g_speed_counter);
	LOCK_FUNCTION(increment_speed_counter);

	install_int_ex(increment_speed_counter, BPS_TO_TIMER(30)); //초당 타이머 함수 호출 수(BEAT PER SECOND)

	screenBuffer = create_bitmap(screen->w, screen->h);
	if (!screenBuffer) {
		allegro_message("Failed to create double buffer. Out of memory?");
		exit(1);
	}

	set_palette(desktop_palette);

	InitGame();

	// this next while loop
	while (key[KEY_ESC] == false)
	{
		while (g_speed_counter > 0)
		{
			updatePaddlePosition();

			switch (gameState)
			{
			case GAME_PLAY:
			{				
				moveBall();
			}
			break;
			case GAME_WAIT:
			{
				if (mouse_b == 1)
				{
					gameState = GAME_PLAY;
				}
			}
			break;
			}

			g_speed_counter--;

		}

		if (numberOfBricksRemaining() == 0)
		{
			InitGame();
		}

		Render();

		blit(screenBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

	}

	return 0;
}

#ifdef _WIN32
END_OF_MAIN()
#endif