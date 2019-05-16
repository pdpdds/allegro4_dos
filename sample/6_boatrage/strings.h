//#define SWEDISH
#define ENGLISH


#ifdef SWEDISH
//****************************************
//***             SWEDISH              ***
//****************************************
//  Ž ™ † „ ”
// Å Ä Ö å ä ö

//***             GENERAL              ***
#define S_GAMENAME "BoatRage"
#define S_AUTHOR   "Zenon"
#define S_VERSION  "1.0.5"
#define S_YES      "Ja"
#define S_NO       "Nej"
#define S_OK       "OK"
#define S_CANCEL   "Avbryt"
#define S_Y        'j'
#define S_N        'n'
#define S_O        'o'
#define S_C        'a'
#define S_PLAYER1  "Spelare 1"
#define S_PLAYER2  "Spelare 2"

#define S_ENGINE   "Motor:"
#define S_RUDDER   "Roder:"
//***             ENGINES              ***
#define S_ENG1NAME "Host-host"
#define S_ENG1D1   "En riktigt skruttig motor"
#define S_ENG1D2   " "

#define S_ENG2NAME "Hackolin"
#define S_ENG2D1   "En halvbra standardmotor"
#define S_ENG2D2   " "

#define S_ENG3NAME "Yimoshibu"
#define S_ENG3D1   "En bra japansk motor"
#define S_ENG3D2   " "

#define S_ENG4NAME "Revenge"
#define S_ENG4D1   "En hemlig amerikansk"
#define S_ENG4D2   "militär-motor"

#define S_ENG5NAME "Alienmotor"
#define S_ENG5D1   "En utomjordisk motor"
#define S_ENG5D2   "stulen från ryska maffian"

//***             RUDDERS              ***
#define S_RUD1NAME "O-styro"
#define S_RUD1D1   "Ett skrotigt budgetroder"
#define S_RUD1D2   " "

#define S_RUD2NAME "Plask-plask"
#define S_RUD2D1   "Ett någorlunda"
#define S_RUD2D2   "standardroder"

#define S_RUD3NAME "Yamasuya"
#define S_RUD3D1   "Ett coolt roder"
#define S_RUD3D2   "helt enkelt"

#define S_RUD4NAME "Steeromax"
#define S_RUD4D1   "Avancerat tyskt"
#define S_RUD4D2   "styrsystem"

#define S_RUD5NAME "Alienroder"
#define S_RUD5D1   "Ett utomjordiskt"
#define S_RUD5D2   "styrsystem"

//***          MENUS           ***
#define S_NEWGAME  "Nytt spel"
#define S_OPTIONS  "Inst„llningar"
#define S_EDITOR   "Editor"
#define S_CREDITS  "Medverkande"
#define S_QUIT     "Avsluta"

#define S_SINGLER  "Enkelt race"
#define S_SEASON   "S„song"
#define S_OWNLEVEL "Race p† egen bana"
#define S_OWNSEASON "S„song: Egna banor"
#define S_BACK     "Tillbaka"

#define S_EXITYES  "Ja! Avsluta!"
#define S_EXITNO   "Nej! Spela!"

//***          LEVEL SELECTION           ***
#define S_SELECTLEVEL "Välj bana:"
#define S_NEXTLEVEL "Nästa bana:"
#define S_LEVELNUMBERNAME "Nr %i: %s"
#define S_LEVELBY  "Av %s"
#define S_LEVELSIZE "Storlek: %ix%i"
#define S_LEVELLAPS "Antal varv: %i"

//***          UPGRADES           ***
#define S_UPGRADEBOAT "Fixa båten: %s"
#define S_YOURMONEY "Du har $%i"
#define S_YOUHAVE   "Du har redan"
#define S_COSTS     "Kostar $%i"
#define S_BUYUPGRADES1 "Skaffa uppgraderingar med SPACE!"
#define S_BUYUPGRADES2 "När du är klar tryck ENTER!"

//***          IN-GAME           ***
#define S_POSITION "DU KOM %i:a"
#define S_QUITSEASON1 "Vill du verkligen"
#define S_QUITSEASON2 "avbryta säsongen?"
#define S_CHECKPOINT "Checkpoint: %i/3"
#define S_GOAL "I mål!"
#define S_LAP "Varv %i/%i"

//***          STARTUP           ***
#define S_STARTING1 "Startar BoatRage..."
#define S_STARTING2 "*******************"

#define S_ERRALLEG  "Kunde inte starta Allegro! Avbryter..."
#define S_OKALLEG   "Allegro startat"

#define S_ERRMOUSE  "Kunde inte hitta n†gon mus! Editorn m m avst„ngd."
#define S_OKMOUSE   "-knappars mus hittad"

#define S_ERRSND    "Kunde inte hitta ljudkortet! Ljud avst„ngt."
#define S_OKSND     "Ljudkort hittat"

#define S_ERRKEYB   "Kunde inte hitta tangentbordet! Avbryter..."
#define S_OKKEYB    "Tangentbord hittat"

#define S_ERRTIMER  "Kunde inte starta timern! Avbryter..."
#define S_OKTIMER   "Timern startad"

#define S_LOADING   "Laddar"
#define S_ERRLOADING1 "Kunde inte hitta filen \""
#define S_ERRLOADING2 "\"! Avbryter..."
#define S_LOADINGGFX "Laddar grafik..."

#define S_GFXSWITCH "V„xlar till "
#define S_GFXBIT    "-bitars "
#define S_GFXMODE   "-grafikl„ge..."

#define S_GFXFAIL1  "Misslyckades att sätta grafikläge."
#define S_GFXFAIL2  "Använder 320x200 istället."

//***          CREDITS           ***
#define S_AUTHORANDNET "Zenon och internet"
#define S_COAUTHOR  "Goran"
#define S_CODING    "Kodning:"
#define S_SOUNDFX   "Ljud FX:"
#define S_MUSIC     "Musik:"
#define S_GRAPHICS  "Grafik:"
#define S_LEVELS    "Banor:"

//***          ERRORS           ***
#define S_FILEDONTEXIST "Filen finns inte!"
#define S_NEEDMOUSEEDIT1 "Du måste ha en mus f”r"
#define S_NEEDMOUSEEDIT2 "att använda editorn!"


//***         SETTINGS           ***
#define S_CHECKREADME "(kolla README.TXT)"
#define S_VSYNC    "VSync();"

//***          EDITOR           ***
#define S_NEWLEVEL "Ny bana..."
#define S_LOADLEVEL "Ladda bana..."
#define S_SAVELEVEL "Spara bana..."
#define S_PROPERTIES "Egenskaper..."
#define S_FIXEDGES   "Fixa kanter!!!"
#define S_CLOSEEDITOR "Stäng editorn"

#define S_WHITE   "Vitt"
#define S_RED     "Rött"
#define S_GREEN   "Grönt"
#define S_BLUE    "Blått"
#define S_YELLOW  "Gult"
#define S_MAGENTA "Magenta"
#define S_CYAN    "Cyan"

#define S_BRUSH    "Penselstorlek: %i"

#define S_SMALL    "Litet ljus"
#define S_BIG      "Stort ljus"
#define S_ROTATING "Roterande fyr"

#define S_HOWBIGLEVEL "Hur stor ska banan vara?"
#define S_WIDTH       "Bredd (x):"
#define S_HEIGHT      "Höjd (y):"

#define S_LEVELPROPS "Egenskaper för banan"
#define S_NAME       "Namn:"
#define S_BY         "Av:"
#define S_NUMLAPS    "# varv:"
#define S_NUMSONG    "Låt #:"

#define S_ERRLAPS1 "Antalet varv måste vara"
#define S_ERRLAPS2 "ett heltal över 0!"

#define S_ERRSONG1 "Låtnumret måste vara"
#define S_ERRSONG2 "ett heltal mellan 1 och 16!"

#define S_ERRWH1 "Både bredd och höjd måste"
#define S_ERRWH2 "vara heltal över 0"

#define S_LAYER "%i: Lager #%i"
#define S_TILE  "Tile %i"

#define S_PLACELIGHT "2: Sätt ut ljus"
#define S_LCOLOR     "Färg: %s"
#define S_LTYPE      "Typ: %s"
#define S_PLACEBOAT  "%i: Sätt ut båt #%i"

#define S_EXITEDITOR1 "Säker på att du"
#define S_EXITEDITOR2 "vill stänga editorn"
#define S_EXITEDITOR3 "utan att spara?"

#define S_F1HELP    "F1=hjälp"
#define S_EDITHELP1 "Byt verktyg med PageUp och Pagedown"
#define S_EDITHELP2 "Välj färg/tile/mm med piltangenterna"
#define S_EDITHELP3 "Kolla README.TXT för mer hjälp"

#endif










#ifdef ENGLISH
//****************************************
//***             ENGLISH              ***
//****************************************

//***             GENERAL              ***
#define S_GAMENAME "BoatRage"
#define S_AUTHOR "Zenon"
#define S_VERSION "1.0.4"
#define S_YES      "Yes"
#define S_NO       "No"
#define S_OK       "OK"
#define S_CANCEL   "Cancel"
#define S_Y        'y'
#define S_N        'n'
#define S_O        'o'
#define S_C        'c'
#define S_PLAYER1  "Player 1"
#define S_PLAYER2  "Player 2"

#define S_ENGINE   "Engine:"
#define S_RUDDER   "Rudder:"

//***             ENGINES              ***
#define S_ENG1NAME "Coughie"
#define S_ENG1D1 "A very crap engine"
#define S_ENG1D2 " "

#define S_ENG2NAME "Hacky"
#define S_ENG2D1 "Average standard"
#define S_ENG2D2 "engine"

#define S_ENG3NAME "Yimoshibu"
#define S_ENG3D1 "A good japanese"
#define S_ENG3D2 "engine"

#define S_ENG4NAME "Revenge"
#define S_ENG4D1 "A secret american"
#define S_ENG4D2 "military engine"

#define S_ENG5NAME "AlienEngine"
#define S_ENG5D1 "An extra-terrestial"
#define S_ENG5D2 "engine"

//***             RUDDERS              ***
#define S_RUD1NAME "Unsteer"
#define S_RUD1D1 "A crappy budget-"
#define S_RUD1D2 "rudder"

#define S_RUD2NAME "Splashy"
#define S_RUD2D1 "An average"
#define S_RUD2D2 "standard rudder"

#define S_RUD3NAME "Yamasuya"
#define S_RUD3D1 "Simply a cool"
#define S_RUD3D2 "rudder"

#define S_RUD4NAME "Steeromax"
#define S_RUD4D1 "Advanced German"
#define S_RUD4D2 "control system"

#define S_RUD5NAME "AlienRudder"
#define S_RUD5D1 "An alien"
#define S_RUD5D2 "control system"

//***             MENUS              ***
#define S_NEWGAME  "New game"
#define S_OPTIONS  "Settings"
#define S_EDITOR   "Editor"
#define S_CREDITS  "Credits"
#define S_QUIT     "Exit Game"

#define S_SINGLER   "Single Race"
#define S_SEASON    "Season"
#define S_OWNLEVEL  "Custom level"
#define S_OWNSEASON "Custom season"
#define S_BACK      "Back"

#define S_EXITYES  "Yes! Exit!"
#define S_EXITNO   "No! I wanna play!"

//***           LEVEL SELECTION           ***
#define S_SELECTLEVEL "Select level:"
#define S_NEXTLEVEL   "Next level:"
#define S_LEVELNUMBERNAME "#%i: %s"
#define S_LEVELBY     "By %s"
#define S_LEVELSIZE   "Size: %ix%i"
#define S_LEVELLAPS   "# of laps: %i"

//***          UPGRADES           ***
#define S_UPGRADEBOAT "Buy upgrades: %s"
#define S_YOURMONEY   "You have $%i"
#define S_YOUHAVE     "Already owned"
#define S_COSTS       "Costs $%i"

#define S_BUYUPGRADES1 "Buy upgrades with SPACE!"
#define S_BUYUPGRADES2 "When you're ready press ENTER!"

//***             IN-GAME              ***
#define S_CHECKPOINT  "Checkpoint: %i/3"
#define S_GOAL        "Goal!"
#define S_LAP         "Lap # %i/%i"
#define S_POSITION    "YOU'RE #%i"
#define S_QUITSEASON1 "Really wanna quit"
#define S_QUITSEASON2 "this season?"

//***             STARTING              ***
#define S_STARTING1 "Starting BoatRage..."
#define S_STARTING2 "*******************"

#define S_ERRALLEG  "Couldn't start Allegro! Aborting..."
#define S_OKALLEG   "Allegro started"

#define S_ERRMOUSE  "Couldn't find a mouse! The editor is unaccessible."
#define S_OKMOUSE   "-button mouse found"

#define S_ERRSND    "Couldn't find a soundcard! Sound is off."
#define S_OKSND     "Soundcard found"

#define S_ERRKEYB   "Couldn't find a keyboard! Aborting..."
#define S_OKKEYB    "Keyboard found"

#define S_ERRTIMER  "Couldn't start the timer..."
#define S_OKTIMER   "Timer started"

#define S_LOADING   "Loading"
#define S_ERRLOADING1 "File not found: \""
#define S_ERRLOADING2 "\"! Aborting..."
#define S_LOADINGGFX "Loading graphics..."

#define S_GFXSWITCH "Switching to "
#define S_GFXBIT    "-bit "
#define S_GFXMODE   " GFX mode..."

#define S_GFXFAIL1  "Failed to set graphics mode."
#define S_GFXFAIL2  "Using 320x200 VGA instead."

//***             CREDITS              ***
#define S_AUTHORANDNET "Zenon and internet"
#define S_COAUTHOR  "Goran"
#define S_CODING    "Coding:"
#define S_SOUNDFX   "Sound FX:"
#define S_MUSIC     "Music:"
#define S_GRAPHICS  "Graphics:"
#define S_LEVELS    "Levels:"

//***             ERRORS              ***
#define S_FILEDONTEXIST "File not found!"

#define S_NEEDMOUSEEDIT1 "You need a mouse to"
#define S_NEEDMOUSEEDIT2 "use the editor!"

//***             SETTINGS              ***
#define S_CHECKREADME "(check README.TXT)"
#define S_VSYNC       "VSync();"

//***          EDITOR           ***
#define S_NEWLEVEL    "New level..."
#define S_LOADLEVEL   "Load level..."
#define S_SAVELEVEL   "Save level..."
#define S_PROPERTIES  "Properties..."
#define S_FIXEDGES    "Fix edges!!!"
#define S_CLOSEEDITOR "Exit editor"

#define S_WHITE   "White"
#define S_RED     "Red"
#define S_GREEN   "Green"
#define S_BLUE    "Blue"
#define S_YELLOW  "Yellow"
#define S_MAGENTA "Magenta"
#define S_CYAN    "Cyan"

#define S_BRUSH   "Brush %i"

#define S_SMALL     "Small light"
#define S_BIG       "Big light"
#define S_ROTATING "Rotating spotlight"

#define S_HOWBIGLEVEL "How big will the level be?"
#define S_WIDTH       "Width (x):"
#define S_HEIGHT      "Height (y):"

#define S_LEVELPROPS "Level Properties"
#define S_NAME       "Name:"
#define S_BY         "By:"
#define S_NUMLAPS    "# laps:"
#define S_NUMSONG    "Song #:"

#define S_ERRLAPS1 "# laps must be an integer > 1!"
#define S_ERRLAPS2 NULL

#define S_ERRSONG1 "# song must be an integer"
#define S_ERRSONG2 "between 1 and 16!"

#define S_ERRWH1   "Both the width and height must"
#define S_ERRWH2   "be integers over 0"

#define S_LAYER       "%i: Layer #%i"
#define S_TILE        "Tile %i"

#define S_PLACELIGHT "2: Place lights"
#define S_LCOLOR     "Color: %s"
#define S_LTYPE      "Type: %s"
#define S_PLACEBOAT  "%i: Place boat #%i"

#define S_EXITEDITOR1 "Are you sure you want to"
#define S_EXITEDITOR2 "exit the editor without"
#define S_EXITEDITOR3 "saving your work?"

#define S_F1HELP    "F1=Help!"
#define S_EDITHELP1 "Select tool with PageUp/Pagedown"
#define S_EDITHELP2 "Choose color/tile/etc with arrow keys"
#define S_EDITHELP3 "Check README.TXT for more info"

#endif

