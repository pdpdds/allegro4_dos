#include "game.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "helper.h"
#include <stdlib.h>


extern void RenderRect(int x1, int y1, int x2, int y2, int r, int g, int b); //사각형을 그리는 함수
extern void RenderCircle(int x, int y, int radius, int r, int g, int b); // 원을 그리는 함수

#define GAME_END 0
#define GAME_PLAY 1
int g_gameState = GAME_PLAY;

//140 * 196 화면 생성
//타일크기 14
#define ROWS 14 
#define COLS 10 
#define TILE_WIDTH 14

#define COLLIDE_NO 0
#define COLLIDE 1
#define COLLIDE_GAME_OVER 2

//조각. 총 7개
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

//조각 정보 구조체
typedef struct {
	int shape[4][4];
	int x;
	int y;
	int id;
} Piece;

//키입력 구조체
typedef struct {
	int right;
	int left;
	int rot;
	int rotb;
	int drop;
	int fast;
} KeyTable;

int step_time = 5; //조각 이동 시간 오프셋
int input_step_time = 1; //입력 수용 오프셋

long g_next_step = 0; //조각 이동가능한 시간. 현재시간이 이 값을 넘으면 조각을 이동시킨다.
long g_next_input_step = 0; //입력받을 수 있는 시간. 현재시간이 이 값을 넘지 못하면 입력을 받을 수 없다.
Piece g_piece = { {0}, (COLS / 2 - 2), 0, 0 };

int g_score = 0; //획득점수
int g_map[ROWS * COLS] = { 0 }; //타일공간 최초에는 모두 0으로 설정한다.
KeyTable g_key_table = { 0 }; // 키 입력을 저장

int set_game_end()
{
	g_gameState = GAME_END;
	return 0;
}

int rotate(Piece* piece) 
{
	Piece tmp;
	memcpy(&tmp, piece, sizeof(tmp));
	
	//조각은 회전할 경우 3x4 또는 4 x 3안에 타일이 모두 들어간다.
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

//1 충돌, 0 충돌하지 않음
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

//타일이 전부 채워진 줄을 검색해서
//보드에서 제거한다.
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

//조각을 이동시킨 다음 보드의 타일과 충돌이 있다면
//몇개의 라인을 제거할 수 있는지 체크한다.
//그리고 새로운 조각을 선택하고 좌표를 초기화한다.
//새로운 사각형이 나오자마자 보드의 타일과 충돌한다면 게임이 종료됐다는 걸 의미한다.

int step(int* map, Piece* piece, int* cleared_lines) 
{
	//조각을 y축으로 1칸 이동
	piece->y += 1;
	if (!colliding(map, piece)) 
	{
		//충돌없음
		return COLLIDE_NO;
	}

	piece->y -= 1;
	place_piece_on_map(map, piece);
	*cleared_lines = clear_lines(map);

	//새 조각을 선택한다.
	piece->x = (COLS / 2 - 2);
	piece->y = 0;
	get_piece(piece);

	//새로운 조각이 맵과 충돌한다면 보드가 타일로 가득찼다는 의미임
	if (colliding(map, piece)) 
	{
		return COLLIDE_GAME_OVER;
	}
	return COLLIDE;
}

//한번에 없앤 라인수에 따른 점수 갱신
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

	//키 입력을 받을 수 있는 시간인가
	if (now() > g_next_input_step) 
	{
		g_key_table.right = key_right;
		g_key_table.left = key_left;
		g_key_table.rot = key_up;
		g_key_table.fast = key_down; //아래 방향키를 눌렀다면 조각이 떨어지는 속도를 높인다.
		g_key_table.drop = key_space;

		//다음 키 입력을 받을 시간을 갱신
		g_next_input_step = now() + input_step_time;

		//오른쪽 방향키를 눌렀다면 조각을 오른쪽으로 이동시킨다.
		//만약 보드의 타일과 충돌한다면 조각의 좌표를 원래대로 되돌린다.
		if (g_key_table.right) 
		{
			g_piece.x += 1;
			if (colliding(g_map, &g_piece)) 
			{
				g_piece.x -= 1;
			}
		}

		//오른쪽 방향키를 눌렀다면 조각을 왼쪽으로 이동시킨다.
		//만약 보드의 타일과 충돌한다면 조각의 좌표를 원래대로 되돌린다.
		if (g_key_table.left) {
			g_piece.x -= 1;
			if (colliding(g_map, &g_piece)) {
				g_piece.x += 1;
			}
		}
		//윗쪽 방향키를 눌렀다면 조각이 회전가능한지 확인하고 조각의 배열값을 갱신한다.
		if (g_key_table.rot) 
		{
			Piece tempPiece;
			memcpy(&tempPiece, &g_piece, sizeof(Piece));
			rotate(&g_piece);

			//회전시킨 조작이 보드와 충돌한다면 조각 배열을 원상태로 돌린다.
			if (colliding(g_map, &g_piece)) 
			{
				memcpy(&g_piece, &tempPiece, sizeof(Piece));
			}
			
			g_key_table.rot = 0;
		}
	}

	int clear_lines = 0; //없앤 라인수
	
	//조각이 아래로 내려갈 수 있는지를 확인하고 보드의 타일과 충돌여부를 체크해서 게임을 계속 진행할 수 있는지 확인한다.
	int step_result = COLLIDE_NO;
	
	if (g_key_table.drop) //스페이스키를 눌러 조각을 떨어뜨리면
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

	if (step_result == COLLIDE_GAME_OVER) //조각 이동결과 게임을 더 이상 진행할 수 없으면 게임상태를 변경한다.
	{
		set_game_end();
		return;
	}

	if(clear_lines > 0) //없앤줄이 있으면 줄수에 따라 점수를 갱신한다.
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
	//보드 배경을 그린다
	struct Rect rect_frame = { 0, 0, COLS * TILE_WIDTH, ROWS * TILE_WIDTH };
	RenderRect(rect_frame.x, rect_frame.y, rect_frame.x + rect_frame.width, rect_frame.y + rect_frame.height, 0, 0, 255);

	int r = 100;
	int g = 100;
	int b = 100;

	//보드의 타일을 그린다
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

	//조각을 그린다
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