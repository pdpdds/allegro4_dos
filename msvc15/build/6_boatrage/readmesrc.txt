(svenska finns en bit ner)

****************
ENGLISH/ENGELSKA
****************

Hi there!
Here's the source code for BoatRage 1.0.5. It's quite messy, but I hope you learn something from it at least.

NOTE: If you use code from BoatRage in your games/programs, you must put a note somewhere in the game or in your readme that you got the code from me.

Usually you can tell what's in one of these source files by looking at the name, for example sprite.h and sprite.cc contains the sprite base class, but the file menumagm (short for menu management) contains both the menus and the "void race()".

Display is an instance of my screenman class. It does stuff like double buffering, fading, and some other stuff. It's in wrappers.cc/h. It has an operator BITMAP *() so that I can use it as a BITMAP: clear(display); 
Sound and Music are also in wrappers.cc/h.

I use Rhide, and have enclosed the project files, so you can just type rhide in the directory you unzipped the project to and compile!

For the game to run when you have compiled it you of course need the datafile that comes with the game. Just download the game from http://come.to/boatrage/ if you haven't already and unzip it in the same folder.

For the code to compile you need:
DJGPP 2.0 or better
Allegro 3.9.15 WIP or better (should compile with minor changes with allegro 3.11, but why bother?)

DJGPP is a great freeware C/C++-kompilator. Get it at www.delorie.com
Allegro is a library of graphics and sound routines for DJGPP/Visual C++/Linux.

Questions? Just mail me! But no spam please!

//Henrik (henrik_83_@hotmail.com)



***************
SVENSKA/SWEDISH
***************
Hallå!
Här är källkoden till BoatRage. Den är ganska rörig och ostrukturerad, men jag hoppas du får ut nåt av den. 

OBS: om du använder kod från boatrage i dina spel, MÅSTE du skriva i din readme-fil eller nånstans i spelet att du använt min kod och skriv även min e-mailadress och adressen till min boatrage-sida ( http://come.to/boatrage/ ).

Filnamnen säger ungefär vad som finns i filerna (t ex sprite.cc), men i menumagm.cc finns det både menyer (som namnet anger) och själva Race-koden (void race()).
Display som finns lite överallt är en klass jag har gjort för att ta hand om skärmen. Den innehåller en typomvandlingskonstruktör (vilket ord!) så att man kan använda den istället för BITMAPs, t ex clear(display); . Koden till den finns i wrappers.cc och wrappers.h.

För att enklast kolla på källkoden, så: ta en dosprompt och gå till mappen med källkoden och skriv rhide (om du har Rhide, alltså).

För att spelet ska kunna köras när du kompilerat det behöver du spelets datafil också och gärna lite banor. Ladda helt enkelt hem spelet från min site: http://come.to/boatrage/ och packa upp i samma mapp.

För att koden ska kompilera krävs:
* DJGPP 2.0 (eller högre) (www.delorie.com)
* Allegro 3.11 (det kan gå med 3.1 och kanske t om med 3.0, men jag har inte testat, ladda hem senaste från http://www.talula.demon.co.uk/allegro/)

DJGPP är en suverän gnu-ware (gratis) C/C++-kompilator.
Allegro är ett gäng grafik- och ljudfunktioner till DJGPP. Finns också till Visual C++.

Har ni frågor så maila mig!

//Henrik (henrik_83_@hotmail.com)