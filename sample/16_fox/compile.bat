PATH=%~dp0\..\..\djgpp\i586-pc-msdosdjgpp\bin;%~dp0\..\..\djgpp\bin;%~dp0\..\..\bin;%~dp0\..\..\tools;%PATH
set GCC_EXEC_PREFIX=%~dp0\..\..\djgpp\lib\gcc\
set DJGPP_GCC=gcc
set DJDIR=%~dp0\..\..\djgpp\i586-pc-msdosdjgpp
make clean
make
