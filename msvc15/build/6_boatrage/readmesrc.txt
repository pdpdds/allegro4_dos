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
Hall�!
H�r �r k�llkoden till BoatRage. Den �r ganska r�rig och ostrukturerad, men jag hoppas du f�r ut n�t av den. 

OBS: om du anv�nder kod fr�n boatrage i dina spel, M�STE du skriva i din readme-fil eller n�nstans i spelet att du anv�nt min kod och skriv �ven min e-mailadress och adressen till min boatrage-sida ( http://come.to/boatrage/ ).

Filnamnen s�ger ungef�r vad som finns i filerna (t ex sprite.cc), men i menumagm.cc finns det b�de menyer (som namnet anger) och sj�lva Race-koden (void race()).
Display som finns lite �verallt �r en klass jag har gjort f�r att ta hand om sk�rmen. Den inneh�ller en typomvandlingskonstrukt�r (vilket ord!) s� att man kan anv�nda den ist�llet f�r BITMAPs, t ex clear(display); . Koden till den finns i wrappers.cc och wrappers.h.

F�r att enklast kolla p� k�llkoden, s�: ta en dosprompt och g� till mappen med k�llkoden och skriv rhide (om du har Rhide, allts�).

F�r att spelet ska kunna k�ras n�r du kompilerat det beh�ver du spelets datafil ocks� och g�rna lite banor. Ladda helt enkelt hem spelet fr�n min site: http://come.to/boatrage/ och packa upp i samma mapp.

F�r att koden ska kompilera kr�vs:
* DJGPP 2.0 (eller h�gre) (www.delorie.com)
* Allegro 3.11 (det kan g� med 3.1 och kanske t om med 3.0, men jag har inte testat, ladda hem senaste fr�n http://www.talula.demon.co.uk/allegro/)

DJGPP �r en suver�n gnu-ware (gratis) C/C++-kompilator.
Allegro �r ett g�ng grafik- och ljudfunktioner till DJGPP. Finns ocks� till Visual C++.

Har ni fr�gor s� maila mig!

//Henrik (henrik_83_@hotmail.com)