/*global integers for use with objects*/
int add_item = 1;
int pick_choice, item_choice;
int oldscreen_objects, screen_objects, talkback;

typedef struct OBJECT
{
	int state;
	char text[40];
}    OBJECT;

OBJECT object[10][10][25];
OBJECT item[10][10][25];
/*constants for menu*/
#define  WALK   1
#define  TALK   2
#define  OPEN   3
#define  USE   4
#define  GET   5
#define  CLOSE   6
#define  PUSH   7
#define  HIT   8
#define  LOOK  9
#define  PULL   10
#define  PLEAD   11
#define  GIVE   12
#define  TURNON   13
#define  TURNOFF   14
#define  PERSON   15
/*define constants for items and objects*/
/*SCREEN 1*/
#define  KEY       1
#define  CAR       1
#define  TRUNK     2
/*SCREEN 2*/
#define  CROWBAR   3
#define  TRUCK     1
#define  BACKDOOR  2
/*SCREEN 5*/
#define  DOOR1     1
#define  DOOR2     2
#define  OILSPILL  3
#define  SEWER     4
#define  SHELF     5
#define  BOXES     6
#define  RAG       7
#define  MATCHES   8
/*SCREEN 6*/
#define JAIL_COMP  1
#define JAIL_GATE  2
/*SCREEN 8*/
#define  DOOR1     1
#define  DOOR2     2
#define  PANEL     3
#define  COMPARTMENT 4
#define  MONITOR   5
#define  BEAM      6
/*SCREEN 9*/
#define  TANK      3
#define  JEEP      4
/*SCREEN 10*/
#define  UNIFORM     3
#define  CAPT_UNIFORM  4
#define  ARMORY_SHELF 5
#define  STOCK_GUARD  6

/*the choices for player along with the verbs or nouns
to be used with selection*/
char *choice_desc[][20] =
{
{
 "",
 "WALK",
 "TALK",
 "OPEN",
 "USE",
 "GET",
 "CLOSE",
 "PUSH",
 "HIT",
 "LOOK",
 "PULL",
 "PLEAD",
 "GIVE",
 "TURN ON",
 "TURN OFF",
 "OPTIONS",
},
{
 "",
 "TO",
 "TO",
 "",
 "",
 "",
 "",
 "",
 "",
 "AT",
 "",
 "TO",
 "",
 "",
 "",
 "",
},
{
 "",
 "",
 "",
 "",
 "WITH",
 "",
 "",
 "",
 "WITH",
 "",
 "",
 "",
 "TO",
 "",
 "",
 "",
}
};
/*The on screen objects each set of text
corresponds with the screen currently being shown
(That is why there is some empty spots such as
there is no screen 0 or that there is no objects on
screen 3 or 4)*/
char *onscreen_desc[][20] =
{
{
	"",
}
,
{
"",
 "CAR",
 "TRUNK",
 "CROWBAR",
 },

{
"",
"TRUCK",
 "BACK DOOR",
 },

{
	"",
},

{
	"",
},

{
"",
"DOOR 1",
"DOOR 2",
"OIL SPILL",
"MAN HOLE",
"SHELF",
"BOXES",
"CLEAN RAG",
"MATCHES",
},
{
"",
"JAIL GATE COMPUTER",
"JAIL GATE",
"",
"MAN HOLE",
"",
"",
"OILY RAG",
"",
},
{
	"",
},
{
"",
"DOOR 1",
"DOOR 2",
"BEAM SIDE PANEL",
"BEAM COMPARTMENT",
"COMPUTER MONITOR",
"BEAM",
},
{
"",
"DOOR 1",
"DOOR 2",
"TANK",
"JEEP",
},
{
"",
"DOOR 1",
"DOOR 2",
"UNIFORM",
"CAPTAIN UNIFORM",
"ARMORY SHELF",
"GUARD"
},

};
/*As a side note I tried using talkback_desc as
a pointer instead and once I started writing
words that were about 20 characters in length.
The program started to write to the other pointers
I had. I couldn't fix it so I decided best if I
don't use it as a pointer*/
typedef struct TALKBACK
{
	char text[80];
}    TALKBACK;
TALKBACK talkback_desc[8];


void init_objects()
{
	/*function called when program is first started to set
	up object values and text*/

	/*character talking to screen*/
	strcpy(talkback_desc[0].text, "");
	strcpy(talkback_desc[1].text, "");   /*blank this is reserved
										for when the player
										or badguys talk to the screen*/
	strcpy(talkback_desc[2].text, "WHAT!?!?");
	strcpy(talkback_desc[3].text, "NO WAY");
	strcpy(talkback_desc[4].text, "I DON'T THINK SO");
	strcpy(talkback_desc[5].text, "YEAH RIGHT");
	strcpy(talkback_desc[6].text, "UH UN");
	strcpy(talkback_desc[7].text, "GET CLOSER");
	/*screen 1 items*/

	strcpy(item[1][1][1].text, "CAR KEY");

	object[1][CAR][OPEN].state = 0;
	strcpy(object[1][CAR][OPEN].text, "IT'S TOO HOT!");
	object[1][CROWBAR][GET].state = 1;
	/*screen 5 items*/
	object[5][RAG][GET].state = 1;
	object[5][MATCHES][GET].state = 1;
}
objects()
{
	/*if anything is already opened or already closed
	the player will say it's already open or it's already closed*/
	if (object[showscreen][screen_objects][OPEN].state == 1 && pick_choice == OPEN)
		strcpy(object[showscreen][screen_objects][OPEN].text, "IT'S ALREADY OPEN");
	if (object[showscreen][screen_objects][OPEN].state == 0 && pick_choice == CLOSE)
		strcpy(object[showscreen][screen_objects][CLOSE].text, "IT'S ALREADY CLOSED");

	/*actions for each screen and the responses from different choices
	and screen_objects selected or item used*/
	if (showscreen == 1)
	{
		strcpy(object[1][TRUNK][USE].text, "");
		if (pick_choice == OPEN && screen_objects == TRUNK && object[1][TRUNK][OPEN].state == 0)
		{
			strcpy(object[1][TRUNK][OPEN].text, "THE TRUNK IS LOCKED");
		}
		if (pick_choice == USE && screen_objects == TRUNK && strcmp(item[1][1][item_choice].text, "CAR KEY") == 0)
		{
			object[1][TRUNK][OPEN].state = 1;
			strcpy(object[1][TRUNK][USE].text, "THE KEY WORKED!");
		}
		if (pick_choice == CLOSE && screen_objects == TRUNK && object[1][TRUNK][OPEN].state == 1)
		{
			object[1][TRUNK][OPEN].state = 0;
			strcpy(object[1][TRUNK][CLOSE].text, "THE TRUNK IS CLOSED NOW");
		}
	}



	if (showscreen == 2)
	{
		if (pick_choice == OPEN && screen_objects == BACKDOOR && object[2][BACKDOOR][OPEN].state == 0)
		{
			strcpy(object[2][BACKDOOR][OPEN].text, "THE DOOR IS LOCKED. HMM... MAYBE I COULD PRY IT OPEN.");
		}
		if (pick_choice == USE && screen_objects == TRUNK && strcmp(item[1][1][item_choice].text, "CROWBAR") == 0)
		{
			object[2][BACKDOOR][OPEN].state = 1;
			strcpy(object[2][BACKDOOR][USE].text, "CRACKKK IT WORKED!");
			delete_items(item_choice);
		}
		if (pick_choice == CLOSE && screen_objects == BACKDOOR)
		{
			strcpy(object[showscreen][screen_objects][CLOSE].text, "");
		}
	}

	if (showscreen == 5)
	{
		if (pick_choice == OPEN && screen_objects == DOOR1 && object[5][DOOR1][OPEN].state == 0)
		{
			object[5][DOOR1][OPEN].state = 1;
			strcpy(object[5][DOOR1][OPEN].text, "IT OPENED.");
		}
		if (pick_choice == OPEN && screen_objects == DOOR2 && object[5][BACKDOOR][OPEN].state == 0)
		{
			strcpy(object[5][DOOR2][OPEN].text, "THE DOOR IS LOCKED. LOOKS LIKE IT NEEDS A KEY");
		}
		if (pick_choice == USE && screen_objects == OILSPILL && strcmp(item[1][1][item_choice].text, "CLEAN RAG") == 0)
		{
			strcpy(object[5][OILSPILL][USE].text, "GOOD IT'S OILY NOW. THAT MIGHT BE HANDY.");
			/*text changes from clean rag to oily rag*/
			strcpy(item[1][1][item_choice].text, "OILY RAG");
		}
	}
	if (showscreen == 6)
	{
		if (pick_choice == USE && screen_objects == JAIL_COMP && strcmp(item[1][1][item_choice].text, "OILY RAG") == 0)
		{
			object[6][JAIL_COMP][USE].state = 1;
			strcpy(object[6][JAIL_COMP][USE].text, "NOW IT'S TIME TO LIGHT IT!");
			delete_items(item_choice);  /*once the oily rag is used the rag is deleted*/
		}
		if (pick_choice == USE && screen_objects == RAG && strcmp(item[1][1][item_choice].text, "MATCHES") == 0)
		{
			object[6][RAG][USE].state = 1;
			strcpy(object[6][RAG][USE].text, "YES!! THE GATES OPENED");
			delete_items(item_choice); /*once the matches are used the matches are deleted*/
			item_choice = 0;
			clearfire();
		}
		if (pick_choice == PUSH && screen_objects == SEWER && object[6][RAG][USE].state == 1)
		{
			object[6][SEWER][PUSH].state = 1;
			strcpy(object[6][SEWER][PUSH].text, "LET'S GET OUT OF HERE!");
			stopplayer();
		}
	}
	if (showscreen == 8)
	{
		/*If the gas alarm is on the doors are locked*/
		if (pick_choice == OPEN && screen_objects == DOOR1 && object[8][DOOR1][OPEN].state == 0)
		{
			if (gas_off == FALSE)strcpy(object[8][DOOR1][OPEN].text, "THE SECURITY SYSTEM HAS LOCKED DOWN THE ROOM. COUGH COUGH");
			else   strcpy(object[8][DOOR1][OPEN].text, "THE DOORS CAN BE ACCESSED FROM THE COMPUTER");

		}
		if (pick_choice == OPEN && screen_objects == DOOR2 && object[8][DOOR2][OPEN].state == 0)
		{
			if (gas_off == FALSE)strcpy(object[8][DOOR2][OPEN].text, "THE SECURITY SYSTEM HAS LOCKED DOWN THE ROOM. COUGH COUGH");
			else   strcpy(object[8][DOOR2][OPEN].text, "THE DOORS CAN BE ACCESSED FROM THE COMPUTER");

		}
		/*monitor is show if player turns on the monitor*/
		if (pick_choice == TURNON && screen_objects == MONITOR)
		{
			object[8][MONITOR][TURNON].state = 1;
			strcpy(object[8][MONITOR][TURNON].text, "THAT'S A NICE COMPUTER");
			security();

		}
		if (pick_choice == USE && screen_objects == PANEL && strcmp(item[1][1][item_choice].text, "GRENADE") == 0)
		{
			object[8][PANEL][USE].state = 1;
			strcpy(object[8][PANEL][USE].text, "YEAH! COOL");/*
			strcpy(onscreen_desc[8][3], "PANEL WIRES");when the cover of the panel is blown off
													   the text description changes*/
			delete_items(item_choice);  /*once the GRENADE is used the GRENADE is deleted*/
		}
		if (pick_choice == OPEN && screen_objects == COMPARTMENT && object[8][COMPARTMENT][OPEN].state == 0)
		{
			strcpy(object[8][COMPARTMENT][OPEN].text, "IT'S ELECTRICALLY LOCKED");
		}
		/*The compartment is opened if the wires are pushed or pulled*/
		if ((pick_choice == PUSH || pick_choice == PULL) && screen_objects == PANEL && object[8][PANEL][USE].state == 1)
		{
			strcpy(object[8][PANEL][PUSH].text, "YES! THE COMPARTMENT OPENED");
			strcpy(object[8][PANEL][PULL].text, "YES! THE COMPARTMENT OPENED");
			clearfire();
			object[8][COMPARTMENT][OPEN].state = 1;
		}
	}
	if (showscreen == 9)
	{
		/*gives different messages if guard is on the inside or
		if the door is locked*/
		if (pick_choice == OPEN && screen_objects == DOOR2 && object[10][DOOR1][OPEN].state == 0 || pace2 < 625)
		{
			if (pace2 < 625)
				strcpy(object[9][DOOR2][OPEN].text, "THERE'S A GUARD WALKING AROUND IN THERE.");
			else
				strcpy(object[9][DOOR2][OPEN].text, "IT'S LOCKED FROM THE OTHER SIDE.");

		}
		/*The disc needs to be picked up before the tank can be used*/
		if (pick_choice == USE && screen_objects == TANK && strcmp(item[1][1][item_choice].text, "TANK KEY") == 0)
		{
			if (follow < 609)
				strcpy(object[9][TANK][USE].text, "WE NEED TO FIND THE DATA DISC FIRST.");
			else
			{
				delete_items(item_choice);
				level = 3;
			}
		}
	}
	if (showscreen == 10)
	{
		/*player can change to guard or captain uniform*/
		if (pick_choice == GET && screen_objects == UNIFORM)
		{
			if (uniform == 0)
			{
				strcpy(object[10][UNIFORM][GET].text, "SMELLS FUNNY BUT IT FITS GOOD.");
				uniform = 1;
			}
			else
				strcpy(object[10][UNIFORM][GET].text, "NO WAY I WANT TO BE A CAPTAIN.");
		}

		if (pick_choice == GET && screen_objects == CAPT_UNIFORM)
		{
			strcpy(object[10][CAPT_UNIFORM][GET].text, "COOL NOW THOSE GUARDS WILL TALK TO ME.");
			uniform = 2;
		}
		if (pick_choice == TALK && screen_objects == STOCK_GUARD && object[10][STOCK_GUARD][TALK].state != 1)
		{
			if (uniform == 0)
				strcpy(object[10][STOCK_GUARD][TALK].text, "WHO ARE YOU? GET LOST.");
			if (uniform == 1)
				strcpy(object[10][STOCK_GUARD][TALK].text, "I'M WAITING FOR THE CAPTAIN");
			if (uniform == 2)
			{
				strcpy(object[10][STOCK_GUARD][TALK].text, "SIR! HERE ARE YOUR SUPPLIES");
				add_item++;
				strcpy(item[1][1][add_item].text, "TANK KEY");
				add_item++;
				strcpy(item[1][1][add_item].text, "GRENADE");
				object[10][STOCK_GUARD][TALK].state = 1;
			}
		}
	}
	/*NEXT LEVEL*/

}

add_items()
{
	/*called when a object is selected and the player tries
	to get it*/

	/*if the value of get.state is set to 1 the player can
	pickup the object. The player then says got it! and
	the item is added*/
	if (showscreen != 10)
	{
		if (object[showscreen][screen_objects][GET].state == 1)
		{
			object[showscreen][screen_objects][GET].state = 2;

			strcpy(object[showscreen][screen_objects][GET].text, "GOT IT!");
			add_item++;
			strcpy(item[1][1][add_item].text, onscreen_desc[showscreen][screen_objects]);
		}
	}
}
delete_items()
{
	/*called only if the item used cannot be used again
	and is deleted from the inventory*/
	int c;
	/*deletes the current item selected and moves the rest of the items up
	the list*/
	memset(item[1][1][item_choice].text, 0, sizeof(item[1][1][item_choice].text));
	for (c = item_choice; c < add_item; c++)
	{
		memset(item[1][1][c].text, 0, sizeof(item[1][1][c].text));
		strcpy(item[1][1][c].text, item[1][1][c + 1].text);
	}
	memset(item[1][1][add_item].text, 0, sizeof(item[1][1][add_item].text));
	add_item--;  /*once item is deleted the player has less items
				 so the add_item (carries value of how many
				 items the player has) integer is lowered*/
}
