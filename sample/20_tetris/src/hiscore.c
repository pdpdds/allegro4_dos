// Tetris Avalanche
// Gregory McIntyre
// 1999, 2000


#include "tetris.h"


///  HIGH SCORE TABLE FUNCTION DEFINITIONS  /////////////////////////////////

void table_save() {
	FILE * fp;
	int n;

	// read in the high score table
	fp = fopen("hiscore.tbl", "wb");

	for (n = 0; n != 10; ++n) {
		if ((gTable[n].score % 5) != 0)
			fprintf(stderr, "Score number %d was not divisible by 5 in table_save()!!", n + 1);

		// write the player's name
		fwrite(&gTable[n].name[0], sizeof(char), NAMELEN, fp);
		// write the player's score
		fwrite(&gTable[n].score, sizeof(unsigned long int), 1, fp);
	}
	fclose(fp);
}



void table_display() {
	int n;
	const int yspacing = 12;

	clear(screen);

	textprintf(screen, (FONT *)gData[FONT_NORM].dat, 8, 8,  -1, "HIGH SCORES");

	// print numbers
	for (n = 0; n != 10; ++n)
		textprintf(screen, (FONT *)gData[FONT_NORM].dat, 40,  32 + n*yspacing, 15, "%2d -", n + 1);

	// print names
	for (n = 0; n != 10; ++n)
		textprintf(screen, (FONT *)gData[FONT_NORM].dat, 76,  32 + n*yspacing, 23, "%-s", gTable[n].name);

	// print scores
	for (n = 0; n != 10; ++n)
		textprintf(screen, (FONT *)gData[FONT_NORM].dat, 200, 32 + n*yspacing, 31, "%9ld", gTable[n].score);

	textprintf(screen, (FONT *)gData[FONT_NORM].dat, 8, 180, -1, "Press any key to continue.");

	readkey();
}
