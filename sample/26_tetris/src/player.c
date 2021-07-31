// Tetris Avalanche
// Gregory McIntyre
// 1999, 2000


#include "tetris.h"


BOOL player_piece_move(int i, int dx, int dy);
void player_piece_limit(int i);
void player_piece_rotate(int i, int dir);
void player_piece_stack(int i);
void player_stack_deletelines(int i);
void player_next_update(int i);


///  PLAYER FUNCTION DEFINITIONS  ///////////////////////////////////////////

///  - GENERAL  /////////////////////////////////////////////////////////////

void player_update(int i) {

	switch (p[i].process) {
	case (DELETING_LINES) :
		// continue the deleting lines process

		if (gDeleteCounter[i] > cDeleteTime) {
			p[i].process = NOTHING;
			gDeleteCounter[i] = 0;
			player_stack_deletelines(i);
		}
		break;

	case (NOTHING) :
	default :
		// make player's piece fall


		if (gFallCounter[i] >= p[i].fall_delay) {
			if (player_piece_move(i, 0, 1)) // if there is a collision
				p[i].fall_delay = 10;
			else
				p[i].fall_delay = cSlowest - gDifficulty;

			gFallCounter[i] = 0;
		}
		break;
	};
}



void player_key_check(int i) {
	if (p[i].process == NOTHING) {
		if (key[p[i].key_left] || key[p[i].key_right]) {
			// the player is pressing a key to move left or right
			if (gMoveCounter[i] >= cMoveDelay) {
				if (key[p[i].key_left])
					player_piece_move(i, -1, 0);
				if (key[p[i].key_right])
					player_piece_move(i, 1, 0);

				gMoveCounter[i] = 0;
			}
		} else  // the player is not pressing any key
			gMoveCounter[i] = cMoveDelay;

		if (key[p[i].key_rotl] || key[p[i].key_rotr]) {
			// the player is pressing a key to rotate
			if (gRotCounter[i] >= cRotDelay) {
				if (key[p[i].key_rotl])
					player_piece_rotate(i, 3);
				if (key[p[i].key_rotr])
					player_piece_rotate(i, 1);

				gRotCounter[i] = 0;
			}
		} else  // the player is not pressing any key
			gRotCounter[i] = cRotDelay;

		if ((key[p[i].key_down]) && (gDropCounter[i] >= cDropDelay)) {
			p[i].fall_delay = 0;
			gDropCounter[i] = 0;
		}
	}
}



char * player_read_name(int i, char * newname) {
	int k;
	int pos = 0;

	newname[0] = '\0';

	clear_keybuf();
	do {
		// redraw the name entry screen
		clear(gBuff);

		textprintf(gBuff, (FONT *)gData[FONT_NORM].dat, 8,  8,  15, "You achieved a high score, player %d!", i + 1);
		textprintf(gBuff, (FONT *)gData[FONT_NORM].dat, 8,  24, 15, "Name:");
		textprintf(gBuff, (FONT *)gData[FONT_NORM].dat, 44, 24, 23, newname);
		textprintf(gBuff, (FONT *)gData[FONT_NORM].dat, 44 + text_length((FONT *)gData[FONT_NORM].dat, newname), 24, 23, "_");

		blit(gBuff, screen, 0, 0, 0, 0, gBuff->w, gBuff->h);


		k = readkey() & 0xFF;

		if ((k >= ' ') && (k <= '~') && (pos < NAMELEN)) {
			newname[pos] = k;
			newname[pos + 1] = '\0';
			++pos;
		} else if ((k == 8) && (pos > 0)) {
			--pos;
			newname[pos] = '\0';
		}
	} while (k != 13);

	return newname;
}



void player_kill(int i) {
	// draw the last piece and pause for a little so the player knows what has
	// happened
	int n;

	piece_draw(p[i].win_x + p[i].piece_x*T_SIZE, p[i].win_y + p[i].piece_y*T_SIZE, p[i].piece_r, p[i].current);
	blit(gBuff, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
	rest(500);

	for (n = 0; n != WIN_H*T_SIZE; n += 2) {
		hline(screen, p[i].win_x, p[i].win_y + n, p[i].win_x + WIN_W*T_SIZE - 1, 0);
		hline(screen, p[i].win_x, p[i].win_y + WIN_H*T_SIZE - n - 1, p[i].win_x + WIN_W*T_SIZE - 1, 0);
		rest(10);
	}

	if (i == 0)
		gWinner = 1;
	else
		gWinner = 2;
}



///  - PLAYER PIECE  ////////////////////////////////////////////////////////

BOOL player_piece_move(int i, int dx, int dy) {
	// offsets the current piece by dx and dy, checking collisions
	BOOL bottomhit;
	BOOL stackhit;

	if (!piece_stackhit(&p[i].stack, p[i].current, p[i].piece_r, p[i].piece_x + dx, p[i].piece_y)) {

		stackhit  = piece_stackhit(&p[i].stack, p[i].current, p[i].piece_r, p[i].piece_x, p[i].piece_y + dy);
		bottomhit = piece_bottomhit(p[i].current, p[i].piece_r, p[i].piece_y + dy);

		if (stackhit) {
			player_piece_stack(i);
			return TRUE;
		}

		if (bottomhit) {
			player_piece_stack(i);
			return TRUE;
		}

		p[i].piece_x += dx;
		p[i].piece_y += dy;
	}

	player_piece_limit(i);

	return FALSE;
}



void player_piece_limit(int i) {
	int t, newx, dx;

	// pushes off sides if too close

	dx = 0;
	newx = 0;

	for (t = 0; t != 4; ++t) {
		if (p[i].piece_x + gPieceRef[p[i].piece_r][p[i].current]->tile[t].xoff < 0) {
			dx = -gPieceRef[p[i].piece_r][p[i].current]->tile[t].xoff;
			newx = dx;
			if (!piece_stackhit(&p[i].stack, p[i].current, p[i].piece_r, newx, p[i].piece_y))
				p[i].piece_x = newx;
		} else if (p[i].piece_x + gPieceRef[p[i].piece_r][p[i].current]->tile[t].xoff > (WIN_W - 1)) {
			dx = -gPieceRef[p[i].piece_r][p[i].current]->tile[t].xoff;
			newx = (WIN_W - 1) + dx;
			if (!piece_stackhit(&p[i].stack, p[i].current, p[i].piece_r, newx, p[i].piece_y))
				p[i].piece_x = newx;
		}

	}
}



void player_piece_rotate(int i, int dir) {

	if (!piece_stackhit(&p[i].stack, p[i].current,
	                    (p[i].piece_r + dir) & 0x3,
	                    p[i].piece_x, p[i].piece_y)) {
		p[i].piece_r += dir;
		p[i].piece_r &= 0x3;
	}

	player_piece_limit(i);
}



void player_piece_stack(int i) {
	int t, count, l, col;

	for (t = 0; t != 4; ++t) {
		if (p[i].piece_y + gPieceRef[p[i].piece_r][p[i].current]->tile[t].yoff >= 0)
			p[i].stack.tile[p[i].piece_y + gPieceRef[p[i].piece_r][p[i].current]->tile[t].yoff]
			[p[i].piece_x + gPieceRef[p[i].piece_r][p[i].current]->tile[t].xoff]
			= gPieceRef[p[i].piece_r][p[i].current]->tile[t].colour;
	}

	col = random() % WIN_W;

	// now is the time to hit with stored up penalties
	if (p[i].penalty != 0) {
		for (count = 0; count != p[i].penalty; ++count) {
			// move the stack up
			for (l = 0; l != WIN_H - 1; ++l) {
				for (t = 0; t != WIN_W; ++t)
					p[i].stack.tile[l][t] = p[i].stack.tile[l + 1][t];
				for (t = 0; t != WIN_W; ++t)
					p[i].stack.tile[l + 1][t] = 0;
			}

			// add an uncompleted line on the bottom row
			for (t = 0; t != WIN_W; ++t) {
				if (t != col)
					p[i].stack.tile[WIN_H - 1][t] = C0;
			}
		}
		p[i].penalty = 0;
	}
	player_next_update(i);
}


///  - PLAYER STACK  ////////////////////////////////////////////////////////

void player_stack_linecheck(int i) {
	int l, t;
	BOOL ok;

	if (p[i].process == NOTHING) {
		// set the current number of full_lines lines to zero
		for (l = 0; l != 4; ++l)
			p[i].full_lines[l] = -1;
		p[i].full_lines_count = 0;

		// count full_lines lines
		for (l = 0; l != WIN_H; ++l) {
			// assume the line is full_lines
			ok = TRUE;

			// check every tile in that line
			for (t = 0; t != WIN_W; ++t) {
				// if a tile is not present, the line is not full_lines
				if (p[i].stack.tile[l][t] == 0) {
					ok = FALSE;
					continue;
				}
			}

			// if that check didn't set ok to false, record the full_lines line
			if (ok != FALSE)
				p[i].full_lines[p[i].full_lines_count++] = l;
		}

		// if the player full_lines at least 1 line, add to the player's score
		if (p[i].full_lines_count != 0) {
			p[i].process = DELETING_LINES;
			gDeleteCounter[i] = 0;
		}
	}
}



void player_stack_deletelines(int i) {
	int t, count, l;

	// make rows above fall down
	for (count = 0; count != 4; ++count) {
		if (p[i].full_lines[count] != -1) {
			for (l = p[i].full_lines[count]; l != 0; --l) {
				for (t = 0; t != WIN_W; ++t)
					p[i].stack.tile[l][t] = p[i].stack.tile[l - 1][t];
			}
			// zero the top row
			for (t = 0; t != WIN_W; ++t)
				p[i].stack.tile[0][t] = 0;
		}
	}

	// if the player full_lines at least 1 line
	if (p[i].full_lines_count > 0) {
		// add to the player's score
		p[i].total_lines += p[i].full_lines_count;
		p[i].total_score += cScores[p[i].full_lines_count - 1];

		// check to see if we need to increase the level
		if (gPlayers == 1) {
			gDifficulty = (int)(p[0].total_lines / 10.0);

			if (gDifficulty >= cSlowest)
				gDifficulty = cSlowest - 1;
		}

		// if it is a 2 player game, penalise the other player
		if ((gPlayers == 2) && (p[i].full_lines_count > 1))
			p[((i == 0) ? 1 : 0)].penalty += p[i].full_lines_count;
	}
}



void player_stack_draw(int i) {
	int tempx, tempy, x, y;

	for (y = 0; y != WIN_H; ++y)
		for (x = 0; x != WIN_W; ++x)
			if (p[i].stack.tile[y][x]) {
				tempx = p[i].win_x + x*T_SIZE;
				tempy = p[i].win_y + y*T_SIZE;
				tile_draw(tempx, tempy, p[i].stack.tile[y][x]);
			}
}


///  - (PLAYER) NEXT BOX  /////////////////////////////////////////////////////

void player_next_update(int i) {

	// advance next piece
	p[i].current = gNext.piece;
	p[i].piece_r = 0;
	gNext.piece = random() % 7;

	// set the piece's x position
	p[i].piece_x = WIN_W/2 - 2;

	// move pieces down a little so they don't start off the top
	p[i].piece_y = -1;

	// pieces should have a short delay on entry
	gMoveCounter[i] = 0;
	gFallCounter[i] = 0;
	gDropCounter[i] = -(cSlowest - gDifficulty)/8;

	// check to see if a player is dead
	if (piece_stackhit(&p[i].stack, p[i].current, p[i].piece_r, p[i].piece_x, p[i].piece_y))
		player_kill(i);
}



void next_draw() {
	int nx, ny;

	nx = gNext.x;
	ny = gNext.y;

	if (gNext.visible == TRUE)
		piece_draw(nx + T_SIZE, ny, 0, gNext.piece);
}
