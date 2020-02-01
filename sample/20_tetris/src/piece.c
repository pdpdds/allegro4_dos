// Tetris Avalanche
// Gregory McIntyre
// 1999, 2000


#include "tetris.h"


///  PIECE FUNCTION DEFINITIONS  ////////////////////////////////////////////

BOOL piece_bottomhit(int pieceid, int r, int y) {
	int t;

	for (t = 0; t != 4; ++t) {
		if (y + gPieceRef[r][pieceid]->tile[t].yoff >= WIN_H)
			return TRUE;
	}
	return FALSE;
}



BOOL piece_sidehit(int pieceid, int r, int x) {
	int t;
	for (t = 0; t != 4; ++t) {
		if ((x + gPieceRef[r][pieceid]->tile[t].xoff > WIN_W - 1)
		        || (x + gPieceRef[r][pieceid]->tile[t].xoff < 0))
			return TRUE;
	}
	return FALSE;
}



BOOL piece_stackhit(STACK * stack, int pieceid, int r, int x, int y) {
	int t;

	for (t = 0; t != 4; ++t) {
		if ((            y + gPieceRef[r][pieceid]->tile[t].yoff >= 0)
		        && (stack->tile[y + gPieceRef[r][pieceid]->tile[t].yoff]
		            [x + gPieceRef[r][pieceid]->tile[t].xoff]))
			return TRUE;
	}
	return FALSE;
}



void piece_draw(int x, int y, int r, int piece) {
	int tempx, tempy;
	int t;

	for (t = 0; t != 4; ++t) {
		tempx = x + gPieceRef[r][piece]->tile[t].xoff*T_SIZE;
		tempy = y + gPieceRef[r][piece]->tile[t].yoff*T_SIZE;

		tile_draw(tempx, tempy, gPieceRef[r][piece]->tile[t].colour);

		//rectfill(gBuff, tempx,     tempy,     tempx + T_SIZE - 2, tempy + T_SIZE - 2, gPieceRef[r][piece]->tile[t].colour + 3);
		//rectfill(gBuff, tempx + 1, tempy + 1, tempx + T_SIZE - 4, tempy + T_SIZE - 4, gPieceRef[r][piece]->tile[t].colour + 7);
	}

}



