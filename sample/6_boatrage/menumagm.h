//MENU MANAGEMENT'N'STUFF
void mainmenu();
void credits();
void singlerace();
void customrace();
void season();
void customseason();
void newgamemenu();
int quitmenu();
void mainmenu();
int selectlevel(int std=1,int still=0);
void showlevelinfo(char *file);
void letplayerupgrade(player &p);
void afterrace(player &p1, player &p2,int moneyamount);
void race(player &p1,player &p2);

//h†lla hastigheten p† spelet
void speeder();
//END_OF_FUNCTION(speeder); //beh”vs vid funktioner som k”rs av timern

//r„kna frames per second
void countfps();
//END_OF_FUNCTION(countfps); //beh”vs vid funktioner som k”rs av timern

//r„kna ned
void thecountdown();
//END_OF_FUNCTION(thecountdown); //beh”vs vid funktioner som k”rs av timern

extern volatile int fps,fpsc,fpsx,fpsxc;

