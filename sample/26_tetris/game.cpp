#include "game.h"
#include <iostream>

#define ROWS 14
#define COLS 10
#define TILE_WIDTH 14

const int shape_bar[4][4] = {
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{1, 1, 1, 1},
		{0, 0, 0, 0}
};

const int shape_square[4][4] = {
		{0, 0, 0, 0},
		{0, 1, 1, 0},
		{0, 1, 1, 0},
		{0, 0, 0, 0}
};

const int shape_j[4][4] = {
		{0, 0, 0, 0},
		{1, 0, 0, 0},
		{1, 1, 1, 0},
		{0, 0, 0, 0}
};

const int shape_l[4][4] = {
		{0, 0, 1, 0},
		{1, 1, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
};

const int shape_s[4][4] = {
		{0, 1, 1, 0},
		{1, 1, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
};

const int shape_z[4][4] = {
		{1, 1, 0, 0},
		{0, 1, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
};

const int shape_t[4][4] = {
		{0, 1, 0, 0},
		{1, 1, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
};

const int(*shapes[7])[4][4] = {
	&shape_bar,
	&shape_square,
	&shape_l,
	&shape_j,
	&shape_s,
	&shape_z,
	&shape_t,
};

typedef struct {
	int shape[4][4];
	int x;
	int y;
	int id;
} Piece;

typedef struct {
	int right;
	int left;
	int rot;
	int rotb;
	int drop;
	int fast;
} KeyTable;

extern int now();

int step_time = 5; // fall down a tile every step_time
int input_step_time = 1; // apply input at 15 fps
long next_step = 0;
long next_input_step = 0;
int score = 0;
int g_cleared_lines;

int g_map[ROWS * COLS] = { 0 };
Piece g_piece = { {0}, (COLS / 2 - 2), 0, 0 };


int rotate(Piece* piece) {
	Piece tmp;
	memcpy(&tmp, piece, sizeof(tmp));
	int n = 3;
	if (piece->id == 0) n = 4; // line
	for (int row = 0; row < n; row++) {
		for (int col = 0; col < n; col++) {
			piece->shape[row][col] = tmp.shape[n - col - 1][row];
		}
	}
	return 0;
}

int colliding(const int* map, const Piece* piece) {
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			int mapx = piece->x + col;
			int mapy = piece->y + row;
			if (mapy < 0 ||
				(piece->shape[row][col] &&
					(map[mapy * COLS + mapx] ||
						mapx < 0 || mapy >= ROWS || mapx >= COLS))) {
				printf("collision\n");
				return 1;
			}
		}
	}
	return 0;
}

int place_on_map(int* map, Piece* piece) {
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			int mapx = piece->x + col;
			int mapy = piece->y + row;
			int tile = piece->shape[row][col];
			if (tile) {
				map[mapy * COLS + mapx] = tile;
			}
		}
	}
	return 0;
}

void get_piece(Piece* piece) {
	//piece->id = random() % 7;
	piece->id = 0;
	memcpy(piece->shape, shapes[piece->id], sizeof(piece->shape));
}

void drop_line(int* map, int n) {
	for (int row = n; row > 0; row--) {
		if (row == ROWS) continue;
		memcpy(&map[row * COLS], &map[(row - 1) * COLS], sizeof(int) * COLS);
	}
}

int clear_lines(int* map) {
	int n_lines = 0;
	for (int row = 0; row < ROWS; row++) {
		int good = 1;
		for (int col = 0; col < COLS; col++) {
			if (!map[row * COLS + col]) {
				good = 0;
			}
		}
		if (good) {
			drop_line(map, row);
			n_lines++;
		}
	}
	return n_lines;
}

int step(int* map, Piece* piece, int* cleared_lines) {
	piece->y += 1;
	if (!colliding(map, piece)) {
		return 0;
	}
	piece->y -= 1;
	place_on_map(map, piece);
	*cleared_lines = clear_lines(map);

	// new piece
	piece->x = (COLS / 2 - 2);
	piece->y = 0;
	get_piece(piece);

	if (colliding(map, piece)) {
		return 2;
	}
	return 1;
}

int gameover() {
	puts("game over");
	return 0;
}

KeyTable key_table = { 0 };

void ProcessLogic(int key_left, int key_right, int key_up, int key_down, int key_space)
{
	key_table.right = key_right;
	key_table.left = key_left;
	key_table.rot = key_up;
	key_table.fast = key_down;
	key_table.drop = key_space;

	if (now() > next_input_step) {
		next_input_step = now() + input_step_time;
		if (key_table.right) {
			g_piece.x += 1;
			if (colliding(g_map, &g_piece)) {
				g_piece.x -= 1;
			}
		}
		if (key_table.left) {
			g_piece.x -= 1;
			if (colliding(g_map, &g_piece)) {
				g_piece.x += 1;
			}
		}
		if (key_table.rot) {
			Piece bak;
			memcpy(&bak, &g_piece, sizeof(bak));
			rotate(&g_piece);
			if (colliding(g_map, &g_piece)) {
				memcpy(&g_piece, &bak, sizeof(g_piece));
			}
			key_table.rot = 0; // can't hold rot
		}
	}
	if (key_table.drop || now() > next_step -
		(key_table.fast ? (7 * step_time / 8) : 0)) {
		int step_r = step(g_map, &g_piece, &g_cleared_lines);
		if (step_r) {
			if (step_time > 120) {
				step_time -= 20;
			}
			key_table.drop = 0;
		}
		if (step_r == 2) {
			gameover();
			return;
		}
		next_step = now() + step_time;
	}
	// score
	switch (g_cleared_lines) {
	case 1:
		score += 40;
		break;
	case 2:
		score += 100;
		break;
	case 3:
		score += 300;
		break;
	case 4:
		score += 1200;
		break;
	}

}


extern void RenderRect(int x1, int y1, int x2, int y2, int r, int g, int b); //사각형을 그리는 함수
extern void RenderCircle(int x, int y, int radius, int r, int g, int b); // 원을 그리는 함수

void InitGame(int screen_width, int screen_height)
{
	//srandom(time(NULL));

	next_step = now() + step_time;
	next_input_step = now() + input_step_time;

	get_piece(&g_piece);
}

struct Rect
{
	int x;
	int y;
	int width;
	int height;
};

void draw(int* map, Piece piece) {
	
	Rect rect_frame = { 0, 0, COLS * TILE_WIDTH, ROWS * TILE_WIDTH };
	RenderRect(rect_frame.x, rect_frame.y, rect_frame.x + rect_frame.width, rect_frame.y + rect_frame.height, 0, 0, 255);

	// map
	Rect rect_square = { 0, 0, 12, 12 };
	for (int row = 0; row < ROWS; row++) {
		for (int col = 0; col < COLS; col++) {
			int color = map[row * COLS + col];
			rect_square.y = row * TILE_WIDTH + 1;
			rect_square.x = col * TILE_WIDTH + 1;
			if (color != 0) {
				RenderRect(rect_square.x, rect_square.y, rect_square.x + rect_square.width, rect_square.y + rect_square.height, 0, 255, 0);
			}
			
		}
	}

	int r = 0;
	int g = 0;
	int b = 0;
	// piece TODO: abstract
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			int color = piece.shape[row][col];
			rect_square.y = (row + piece.y) * TILE_WIDTH + 1;
			rect_square.x = (col + piece.x) * TILE_WIDTH + 1;
			if (color != 0) {
				switch (piece.id) {
				case 0:
					r = 255;
					break;
				case 1:
					r = 255;
					g = 255;
					
					break;
				case 2:
					r = 255;
					g = 255;
					b = 255;
					break;
				case 3:
					g = 255;
					b = 255;
					break;
				case 4:
				
					b = 255;
					break;
				case 5:
					r = 255;					
					b = 255;
					break;
				case 6:
					
					g = 255;					
					break;
				case 7:
					b = 255;
					break;
				}
				RenderRect(rect_square.x, rect_square.y, rect_square.x + rect_square.width, rect_square.y + rect_square.height, r, g, b);
			}
		}
	}	
}

void DrawWorld()
{
	// drawing
	draw(g_map, g_piece);
}