#include "game.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "helper.h"
#include <stdlib.h>


extern void RenderRect(int x1, int y1, int x2, int y2, int r, int g, int b); //�簢���� �׸��� �Լ�
extern void RenderCircle(int x, int y, int radius, int r, int g, int b); // ���� �׸��� �Լ�

#define GAME_END 0
#define GAME_PLAY 1
int g_gameState = GAME_PLAY;

//140 * 196 ȭ�� ����
//Ÿ��ũ�� 14
#define ROWS 14 
#define COLS 10 
#define TILE_WIDTH 14

#define COLLIDE_NO 0
#define COLLIDE 1
#define COLLIDE_GAME_OVER 2

//����. �� 7��
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

//���� ���� ����ü
typedef struct {
	int shape[4][4];
	int x;
	int y;
	int id;
} Piece;

//Ű�Է� ����ü
typedef struct {
	int right;
	int left;
	int rot;
	int rotb;
	int drop;
	int fast;
} KeyTable;

int step_time = 5; //���� �̵� �ð� ������
int input_step_time = 1; //�Է� ���� ������

long g_next_step = 0; //���� �̵������� �ð�. ����ð��� �� ���� ������ ������ �̵���Ų��.
long g_next_input_step = 0; //�Է¹��� �� �ִ� �ð�. ����ð��� �� ���� ���� ���ϸ� �Է��� ���� �� ����.
Piece g_piece = { {0}, (COLS / 2 - 2), 0, 0 };

int g_score = 0; //ȹ������
int g_map[ROWS * COLS] = { 0 }; //Ÿ�ϰ��� ���ʿ��� ��� 0���� �����Ѵ�.
KeyTable g_key_table = { 0 }; // Ű �Է��� ����

int set_game_end()
{
	g_gameState = GAME_END;
	return 0;
}

int rotate(Piece* piece) 
{
	Piece tmp;
	memcpy(&tmp, piece, sizeof(tmp));
	
	//������ ȸ���� ��� 3x4 �Ǵ� 4 x 3�ȿ� Ÿ���� ��� ����.
	int n = 3;
	if (piece->id == 0) 
		n = 4; 
	for (int row = 0; row < n; row++) 
	{
		for (int col = 0; col < n; col++) 
		{
			piece->shape[row][col] = tmp.shape[n - col - 1][row];
		}
	}
	return 0;
}

//1 �浹, 0 �浹���� ����
int colliding(const int* map, const Piece* piece) 
{
	for (int row = 0; row < 4; row++) 
	{
		for (int col = 0; col < 4; col++) 
		{
			int mapx = piece->x + col;
			int mapy = piece->y + row;
			if (mapy < 0 ||
				(piece->shape[row][col] &&
					(map[mapy * COLS + mapx] ||
						mapx < 0 || mapy >= ROWS || mapx >= COLS))) 
			{
				return 1;
			}
		}
	}

	return 0;
}

int place_piece_on_map(int* map, Piece* piece) 
{
	for (int row = 0; row < 4; row++) 
	{
		for (int col = 0; col < 4; col++) 
		{
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
	
	piece->id = rand() % 7;
	memcpy(piece->shape, shapes[piece->id], sizeof(piece->shape));
}

void drop_line(int* map, int n) 
{
	for (int row = n; row > 0; row--) 
	{
		if (row == ROWS) continue;
		memcpy(&map[row * COLS], &map[(row - 1) * COLS], sizeof(int) * COLS);
	}
}

//Ÿ���� ���� ä���� ���� �˻��ؼ�
//���忡�� �����Ѵ�.
int clear_lines(int* map) 
{
	int n_lines = 0;
	for (int row = 0; row < ROWS; row++)
	{
		int result = 1;
		for (int col = 0; col < COLS; col++) 
		{
			if (!map[row * COLS + col]) 
			{
				result = 0;
			}
		}
		if (result)
		{
			drop_line(map, row);
			n_lines++;
		}
	}
	return n_lines;
}

//������ �̵���Ų ���� ������ Ÿ�ϰ� �浹�� �ִٸ�
//��� ������ ������ �� �ִ��� üũ�Ѵ�.
//�׸��� ���ο� ������ �����ϰ� ��ǥ�� �ʱ�ȭ�Ѵ�.
//���ο� �簢���� �����ڸ��� ������ Ÿ�ϰ� �浹�Ѵٸ� ������ ����ƴٴ� �� �ǹ��Ѵ�.

int step(int* map, Piece* piece, int* cleared_lines) 
{
	//������ y������ 1ĭ �̵�
	piece->y += 1;
	if (!colliding(map, piece)) 
	{
		//�浹����
		return COLLIDE_NO;
	}

	piece->y -= 1;
	place_piece_on_map(map, piece);
	*cleared_lines = clear_lines(map);

	//�� ������ �����Ѵ�.
	piece->x = (COLS / 2 - 2);
	piece->y = 0;
	get_piece(piece);

	//���ο� ������ �ʰ� �浹�Ѵٸ� ���尡 Ÿ�Ϸ� ����á�ٴ� �ǹ���
	if (colliding(map, piece)) 
	{
		return COLLIDE_GAME_OVER;
	}
	return COLLIDE;
}

//�ѹ��� ���� ���μ��� ���� ���� ����
void update_score(int clearline)
{
	
	switch (clearline) {
	case 1:
		g_score += 40;
		break;
	case 2:
		g_score += 100;
		break;
	case 3:
		g_score += 300;
		break;
	case 4:
		g_score += 1200;
		break;
	}
}

void ProcessLogic(int key_left, int key_right, int key_up, int key_down, int key_space)
{
	if (g_gameState != GAME_PLAY)
		return;

	//Ű �Է��� ���� �� �ִ� �ð��ΰ�
	if (now() > g_next_input_step) 
	{
		g_key_table.right = key_right;
		g_key_table.left = key_left;
		g_key_table.rot = key_up;
		g_key_table.fast = key_down; //�Ʒ� ����Ű�� �����ٸ� ������ �������� �ӵ��� ���δ�.
		g_key_table.drop = key_space;

		//���� Ű �Է��� ���� �ð��� ����
		g_next_input_step = now() + input_step_time;

		//������ ����Ű�� �����ٸ� ������ ���������� �̵���Ų��.
		//���� ������ Ÿ�ϰ� �浹�Ѵٸ� ������ ��ǥ�� ������� �ǵ�����.
		if (g_key_table.right) 
		{
			g_piece.x += 1;
			if (colliding(g_map, &g_piece)) 
			{
				g_piece.x -= 1;
			}
		}

		//������ ����Ű�� �����ٸ� ������ �������� �̵���Ų��.
		//���� ������ Ÿ�ϰ� �浹�Ѵٸ� ������ ��ǥ�� ������� �ǵ�����.
		if (g_key_table.left) {
			g_piece.x -= 1;
			if (colliding(g_map, &g_piece)) {
				g_piece.x += 1;
			}
		}
		//���� ����Ű�� �����ٸ� ������ ȸ���������� Ȯ���ϰ� ������ �迭���� �����Ѵ�.
		if (g_key_table.rot) 
		{
			Piece tempPiece;
			memcpy(&tempPiece, &g_piece, sizeof(Piece));
			rotate(&g_piece);

			//ȸ����Ų ������ ����� �浹�Ѵٸ� ���� �迭�� �����·� ������.
			if (colliding(g_map, &g_piece)) 
			{
				memcpy(&g_piece, &tempPiece, sizeof(Piece));
			}
			
			g_key_table.rot = 0;
		}
	}

	int clear_lines = 0; //���� ���μ�
	
	//������ �Ʒ��� ������ �� �ִ����� Ȯ���ϰ� ������ Ÿ�ϰ� �浹���θ� üũ�ؼ� ������ ��� ������ �� �ִ��� Ȯ���Ѵ�.
	int step_result = COLLIDE_NO;
	
	if (g_key_table.drop) //�����̽�Ű�� ���� ������ ����߸���
	{
		g_key_table.drop = 0;

		while (step_result == COLLIDE_NO)
		{
			step_result = step(g_map, &g_piece, &clear_lines);
		}

		g_next_step = now() + step_time;
	}
	else if(now() > g_next_step - (g_key_table.fast ? (7 * step_time / 8) : 0))
	{
		step_result = step(g_map, &g_piece, &clear_lines);
		
		if (step_result)
		{
			if (step_time > 120) 
			{
				step_time -= 20;
			}
			
		}

		g_next_step = now() + step_time;
	}

	if (step_result == COLLIDE_GAME_OVER) //���� �̵���� ������ �� �̻� ������ �� ������ ���ӻ��¸� �����Ѵ�.
	{
		set_game_end();
		return;
	}

	if(clear_lines > 0) //�������� ������ �ټ��� ���� ������ �����Ѵ�.
		update_score(clear_lines);
}

int g_screen_width = 0;
int g_screen_height = 0;

void InitGame(int screen_width, int screen_height)
{
	g_screen_width = screen_width;
	g_screen_height = screen_height;
	srandom((unsigned int)time(NULL));

	g_next_step = now() + step_time;
	g_next_input_step = now() + input_step_time;

	get_piece(&g_piece);
}

struct Rect
{
	int x;
	int y;
	int width;
	int height;
};

void draw_tetris_world(int* map, Piece* piece) 
{
	//���� ����� �׸���
	struct Rect rect_frame = { 0, 0, COLS * TILE_WIDTH, ROWS * TILE_WIDTH };
	RenderRect(rect_frame.x, rect_frame.y, rect_frame.x + rect_frame.width, rect_frame.y + rect_frame.height, 0, 0, 255);

	int r = 100;
	int g = 100;
	int b = 100;

	//������ Ÿ���� �׸���
	struct Rect rect_square = { 0, 0, 12, 12 };
	for (int row = 0; row < ROWS; row++) 
	{
		for (int col = 0; col < COLS; col++) 
		{
			int color = map[row * COLS + col];
			rect_square.y = row * TILE_WIDTH + 1;
			rect_square.x = col * TILE_WIDTH + 1;
			if (color != 0) {
				RenderRect(rect_square.x, rect_square.y, rect_square.x + rect_square.width, rect_square.y + rect_square.height, r, g, b);
			}
		}
	}

	//������ �׸���
	for (int row = 0; row < 4; row++) 
	{
		for (int col = 0; col < 4; col++) {
			int color = piece->shape[row][col];
			rect_square.y = (row + piece->y) * TILE_WIDTH + 1;
			rect_square.x = (col + piece->x) * TILE_WIDTH + 1;
			if (color != 0) {
				switch (piece->id) 
				{
				case 0:
					r = 255;
					break;
				case 1:
					r = 255;
					g = 255;
					
					break;
				case 2:
					r = 200;
					g = 200;
					b = 200;
					break;
				case 3:
					g = 100;
					b = 100;
					break;
				case 4:
					r = 100;
					b = 100;
					break;
				case 5:
					r = 255;					
					b = 255;
					break;
				case 6:
					g = 255;					
					break;
				case 7:
					g = 50;
					b = 50;
					break;
				}
				RenderRect(rect_square.x, rect_square.y, rect_square.x + rect_square.width, rect_square.y + rect_square.height, r, g, b);
			}
		}
	}	

	if (g_gameState == GAME_END)
		RenderText(40, 40, "Game Over");
}

void DrawWorld()
{
	draw_tetris_world(g_map, &g_piece);
}