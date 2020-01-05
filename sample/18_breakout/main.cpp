#include "core.h"

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

void makeScreenBlack() {
	//clear the screen to black
	//without this next line, the game "smears"
	//comment it out to try it!
	clear_to_color(screenBuffer, makecol(0, 0, 0));
}

void resetBricks() {
	for (int i = 0; i < BRICKS_VERTICALLY; i++) {
		for (int ii = 0; ii < BRICKS_HORIZONTALLY; ii++) {
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

void drawingThings() {
	makeScreenBlack();

	// draw the bricks
	for (int i = 0; i < BRICKS_VERTICALLY; i++) {
		for (int ii = 0; ii < BRICKS_HORIZONTALLY; ii++) {
			if (bricks[i][ii] != 0) {
				rectfill(screenBuffer,
					ii*BRICK_WIDTH, i*BRICK_HEIGHT,
					(ii + 1)*BRICK_WIDTH - BRICK_GAP, (i + 1)*BRICK_HEIGHT - BRICK_GAP,
					makecol(255, 0, 0));
			}
		}
	}

	// draw the BALL
	circlefill(screenBuffer,
		ball.x, ball.y,
		4, makecol(255, 255, 0));

	// draw the player
	rectfill(screenBuffer,
		playerPaddleEdgeX, PADDLE_POSITION_Y,
		playerPaddleEdgeX + PADDLE_WIDTH,
		PADDLE_POSITION_Y + PADDLE_HEIGHT, makecol(255, 255, 255));

	//textprintf_ex(screenBuffer, font, 10, 10, makecol(255, 255, 0),
		//-1, "Player Position (playerPaddleEdgeX is %i)", playerPaddleEdgeX);
	//textprintf_ex(screenBuffer, font, 10, 20, makecol(255, 255, 255),
		//-1, "ball (x,y) position is (%i, %i)", ball.x, ball.y);

}

void resetBall() {
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
}

void moveBall() {
	// update the ball's position for the next frame
	ball.x += ball.xv;
	ball.y += ball.yv;

	// if the ball is overlapping the rectangle
	if (ball.yv > 0) 
	{ // only if the ball is moving down
		if (doesOverlap(ball.x, ball.y,
			playerPaddleEdgeX + (PADDLE_WIDTH*0.0),
			PADDLE_POSITION_Y,
			playerPaddleEdgeX + (PADDLE_WIDTH*0.25),
			PADDLE_POSITION_Y + PADDLE_HEIGHT) == 1) {
			ball.xv = -5;
			ball.yv = -3;
		}
		if (doesOverlap(ball.x, ball.y,
			playerPaddleEdgeX + (PADDLE_WIDTH*0.25),
			PADDLE_POSITION_Y,
			playerPaddleEdgeX + (PADDLE_WIDTH*0.5),
			PADDLE_POSITION_Y + PADDLE_HEIGHT) == 1) {
			ball.xv = -3;
			ball.yv = -5;
		}
		if (doesOverlap(ball.x, ball.y,
			playerPaddleEdgeX + (PADDLE_WIDTH*0.5),
			PADDLE_POSITION_Y,
			playerPaddleEdgeX + (PADDLE_WIDTH*0.75),
			PADDLE_POSITION_Y + PADDLE_HEIGHT) == 1) {
			ball.xv = 3;
			ball.yv = -5;
		}
		if (doesOverlap(ball.x, ball.y,
			playerPaddleEdgeX + (PADDLE_WIDTH*0.75),
			PADDLE_POSITION_Y,
			playerPaddleEdgeX + (PADDLE_WIDTH*1.0),
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
					ii*BRICK_WIDTH, i*BRICK_HEIGHT,
					(ii + 1)*BRICK_WIDTH - BRICK_GAP,
					(i + 1)*BRICK_HEIGHT - BRICK_GAP) == 1) {
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
			resetBall();
		}
	}

}

void gameSetup() {
	resetBricks();

	resetBall();

	// start with the ball off the bottom of the screen
	ball.y = SCREEN_H + 50;
}

// program code aways begins at the top of main()
int main() {
	//The next line is a function drom core.cpp
	// it sets up Allegro for graphics and keyboard
	techInit();

	gameSetup();

	// this next while loop
	while (key[KEY_ESC] == false) {
		updatePaddlePosition();

		moveBall();

		if (numberOfBricksRemaining() == 0) {
			resetBricks();
		}

		drawingThings();

		// this line draws the screenBuffer to the screen
		// by drawing to off-screen memory first, then
		// copying the new total image to screen all at once,
		// we can avoid any flickering/flashing from the player
		// seeing characters drawn sequentially.
		// The technique us called double buffering.
		updateScreen();

		// is the game going too fast on your awesome modern hardware?
		// Experiment with numbers 0-15 here to slow down the application:
		rest(15);
	}
	return 0;
}

#ifdef _WIN32
END_OF_MAIN()
#endif