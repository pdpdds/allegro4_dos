

///  TIMER FUNCTION DEFINITIONS  ////////////////////////////////////////////

// timer handlers

void t1_proc(void) {
	int i;
	for (i = 0; i != gPlayers; ++i) {
		++gFallCounter[i];
	}
}
END_OF_FUNCTION(t1_proc);


void t2_proc(void) {
	int i;
	for (i = 0; i != gPlayers; ++i) {
		++gMoveCounter[i];
		++gRotCounter[i];
		++gDropCounter[i];
	}
}
END_OF_FUNCTION(t2_proc);


void t3_proc(void) {
	int i;
	for (i = 0; i != gPlayers; ++i) {
		++gDeleteCounter[i];
	}
}
END_OF_FUNCTION(t3_proc);



