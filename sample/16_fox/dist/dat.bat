@rem ---------------------------------------------------
@del OPENING.DAT
@tool\dat.exe -c2 OPENING.DAT -a res\alleg.BMP
@tool\dat.exe OPENING.DAT -a res\djgpp.BMP

@copy res\alleg.BMP alleg_pal.bmp
@tool\dat.exe -c2 OPENING.DAT -a alleg_pal.bmp -t PAL
@del alleg_pal.bmp

@copy res\djgpp.BMP djgpp_pal.bmp
@tool\dat.exe OPENING.DAT -a djgpp_pal.bmp -t PAL
@del djgpp_pal.bmp

@tool\dat.exe OPENING.DAT -a res\earth.BMP
@tool\dat.exe OPENING.DAT -a res\opening.BMP

@copy res\opening.BMP opening_pal.bmp
@tool\dat.exe OPENING.DAT -a opening_pal.BMP -t PAL
@del opening_pal.bmp

@rem ---------------------------------------------------
@del INTRO.DAT
@tool\dat.exe INTRO.DAT -a res\intro\city.BMP
@copy res\intro\city.BMP city_pal.bmp
@tool\dat.exe INTRO.DAT -a city_pal.BMP -t PAL
@del city_pal.bmp

@tool\dat.exe INTRO.DAT -a res\intro\go_fox.BMP
@copy res\intro\go_fox.BMP go_fox_pal.bmp
@tool\dat.exe INTRO.DAT -a go_fox_pal.BMP -t PAL
@del go_fox_pal.bmp

@tool\dat.exe INTRO.DAT -a res\intro\guide.BMP
@copy res\intro\guide.BMP guide_pal.bmp
@tool\dat.exe INTRO.DAT -a guide_pal.BMP -t PAL
@del guide_pal.bmp

@tool\dat.exe INTRO.DAT -a res\intro\launch.BMP
@copy res\intro\launch.BMP launch_pal.bmp
@tool\dat.exe INTRO.DAT -a launch_pal.BMP -t PAL
@del launch_pal.bmp

@tool\dat.exe INTRO.DAT -a res\intro\planet.BMP
@copy res\intro\planet.BMP planet_pal.bmp
@tool\dat.exe INTRO.DAT -a planet_pal.BMP -t PAL
@del planet_pal.bmp

@tool\dat.exe INTRO.DAT -a res\intro\ready.BMP
@copy res\intro\ready.BMP ready_pal.bmp
@tool\dat.exe INTRO.DAT -a ready_pal.BMP -t PAL
@del ready_pal.bmp

@tool\dat.exe INTRO.DAT -a res\intro\ship.BMP
@copy res\intro\ship.BMP ship_pal.bmp
@tool\dat.exe INTRO.DAT -a ship_pal.BMP -t PAL
@del ship_pal.bmp

@tool\dat.exe INTRO.DAT -a res\intro\sky.BMP
@copy res\intro\sky.BMP sky_pal.bmp
@tool\dat.exe INTRO.DAT -a sky_pal.BMP -t PAL
@del sky_pal.bmp

@tool\dat.exe INTRO.DAT -a res\intro\talk1.BMP
@copy res\intro\talk1.BMP talk1_pal.bmp
@tool\dat.exe INTRO.DAT -a talk1_pal.BMP -t PAL
@del talk1_pal.bmp

@tool\dat.exe INTRO.DAT -a res\intro\talk2.BMP
@copy res\intro\talk2.BMP talk2_pal.bmp
@tool\dat.exe INTRO.DAT -a talk2_pal.BMP -t PAL
@del talk2_pal.bmp

@tool\dat.exe INTRO.DAT -a res\intro\intro.xml -t TXT

@rem ---------------------------------------------------
@del STAGE1.DAT
@tool\dat.exe -c2 STAGE1.DAT -a res\stage1\explo2.BMP
@tool\dat.exe STAGE1.DAT -a res\stage1\explo3.BMP
@tool\dat.exe STAGE1.DAT -a res\stage1\explo9.BMP
@tool\dat.exe STAGE1.DAT -a res\stage1\fire.BMP
@tool\dat.exe STAGE1.DAT -a res\stage1\fox.BMP

@tool\dat.exe STAGE1.DAT -a res\stage1\stage.bmp
@copy res\stage1\stage.BMP stage_pal.bmp
@tool\dat.exe STAGE1.DAT -a stage_pal.bmp -t PAL
@del stage_pal.bmp

@tool\dat.exe STAGE1.DAT -a res\stage1\stage.res -t TXT
@tool\dat.exe STAGE1.DAT -a res\stage1\stage.map -t TXT
@tool\dat.exe STAGE1.DAT -a res\stage1\weapon.xml -t TXT

@rem ---------------------------------------------------
@del STAGE2.DAT
@tool\dat.exe -c2 STAGE2.DAT -a res\stage2\explo2.BMP
@tool\dat.exe STAGE2.DAT -a res\stage2\explo3.BMP
@tool\dat.exe STAGE2.DAT -a res\stage2\explo9.BMP
@tool\dat.exe STAGE2.DAT -a res\stage2\fire.BMP
@tool\dat.exe STAGE2.DAT -a res\stage2\fox.BMP

@tool\dat.exe STAGE2.DAT -a res\stage2\stage.bmp
@copy res\stage2\stage.BMP stage_pal.bmp
@tool\dat.exe STAGE2.DAT -a stage_pal.bmp -t PAL
@del stage_pal.bmp

@tool\dat.exe STAGE1.DAT -a res\stage1\stage.res -t TXT
@tool\dat.exe STAGE1.DAT -a res\stage1\stage.map -t TXT
@tool\dat.exe STAGE1.DAT -a res\stage1\weapon.xml -t TXT

@rem ---------------------------------------------------
@del MT32.DAT
@tool\dat.exe -c2 MT32.DAT -a res\music\mt-32\01.mid
@tool\dat.exe MT32.DAT -a res\music\mt-32\02.mid
@tool\dat.exe MT32.DAT -a res\music\mt-32\03.mid
@tool\dat.exe MT32.DAT -a res\music\mt-32\04.mid
@tool\dat.exe MT32.DAT -a res\music\mt-32\05.mid
@tool\dat.exe MT32.DAT -a res\music\mt-32\06.mid
@tool\dat.exe MT32.DAT -a res\music\mt-32\07.mid
@tool\dat.exe MT32.DAT -a res\music\mt-32\08.mid
@tool\dat.exe MT32.DAT -a res\music\mt-32\09.mid
@tool\dat.exe MT32.DAT -a res\music\mt-32\10.mid
@tool\dat.exe MT32.DAT -a res\music\mt-32\11.mid
@tool\dat.exe MT32.DAT -a res\music\mt-32\12.mid
@tool\dat.exe MT32.DAT -a res\music\mt-32\13.mid
@tool\dat.exe MT32.DAT -a res\music\mt-32\14.mid
@tool\dat.exe MT32.DAT -a res\music\mt-32\15.mid
@tool\dat.exe MT32.DAT -a res\music\mt-32\16.mid
@tool\dat.exe MT32.DAT -a res\music\mt-32\17.mid
@tool\dat.exe MT32.DAT -a res\music\mt-32\18.mid
@tool\dat.exe MT32.DAT -a res\music\mt-32\19.mid
@tool\dat.exe MT32.DAT -a res\music\mt-32\20.mid

@rem ---------------------------------------------------
@del SC55.DAT
@tool\dat.exe -c2 SC55.DAT -a res\music\sc-55\01.mid
@tool\dat.exe SC55.DAT -a res\music\sc-55\02.mid
@tool\dat.exe SC55.DAT -a res\music\sc-55\03.mid
@tool\dat.exe SC55.DAT -a res\music\sc-55\04.mid
@tool\dat.exe SC55.DAT -a res\music\sc-55\05.mid
@tool\dat.exe SC55.DAT -a res\music\sc-55\06.mid
@tool\dat.exe SC55.DAT -a res\music\sc-55\07.mid
@tool\dat.exe SC55.DAT -a res\music\sc-55\08.mid
@tool\dat.exe SC55.DAT -a res\music\sc-55\09.mid
@tool\dat.exe SC55.DAT -a res\music\sc-55\10.mid
@tool\dat.exe SC55.DAT -a res\music\sc-55\11.mid
@tool\dat.exe SC55.DAT -a res\music\sc-55\12.mid
@tool\dat.exe SC55.DAT -a res\music\sc-55\13.mid
@tool\dat.exe SC55.DAT -a res\music\sc-55\14.mid
@tool\dat.exe SC55.DAT -a res\music\sc-55\15.mid
@tool\dat.exe SC55.DAT -a res\music\sc-55\16.mid
@tool\dat.exe SC55.DAT -a res\music\sc-55\17.mid
@tool\dat.exe SC55.DAT -a res\music\sc-55\18.mid
@tool\dat.exe SC55.DAT -a res\music\sc-55\19.mid
@tool\dat.exe SC55.DAT -a res\music\sc-55\20.mid

@rem ---------------------------------------------------
@del SOUND.DAT
@tool\dat.exe -c2 SOUND.DAT -a res\sound\item.wav
@tool\dat.exe SOUND.DAT -a res\sound\powerup.wav
@tool\dat.exe SOUND.DAT -a res\sound\explod3.wav
@tool\dat.exe SOUND.DAT -a res\sound\explod4.wav
@tool\dat.exe SOUND.DAT -a res\sound\explod5.wav
@tool\dat.exe SOUND.DAT -a res\sound\siren.wav
@tool\dat.exe SOUND.DAT -a res\sound\laser_b.wav
@tool\dat.exe SOUND.DAT -a res\sound\scream.wav
@tool\dat.exe SOUND.DAT -a res\sound\maxpower.wav
@tool\dat.exe SOUND.DAT -a res\sound\pause.wav
@tool\dat.exe SOUND.DAT -a res\sound\menu.wav
@tool\dat.exe SOUND.DAT -a res\sound\menu2.wav
@tool\dat.exe SOUND.DAT -a res\sound\go.wav

@rem ---------------------------------------------------
@del FONT.DAT
@tool\dat.exe -c2 FONT.DAT -a res\han.fnt -t DATA
@tool\dat.exe FONT.DAT -a res\eng.fnt -t DATA

@rem ---------------------------------------------------
@del dat.h dat1.h dat2.h dat3.h dat4.h dat5.h
@tool\dat.exe opening.dat -p DAT_ -h dat1.h
@tool\dat.exe intro.dat -p DAT_ -h dat2.h
@tool\dat.exe mt32.dat -p DAT_ -h dat3.h
@tool\dat.exe sound.dat -p DAT_ -h dat4.h
@tool\dat.exe stage1.dat -p DAT_ -h dat5.h
@tool\dat.exe font.dat -p DAT_ -h dat6.h
@..\..\bin\cat.exe dat?.h > dat_tmp.h
@..\..\bin\awk.exe '!/DAT_COUNT/' dat_tmp.h > dat.h
@del dat1.h dat2.h dat3.h dat4.h dat5.h dat6.h dat_tmp.h

@rem ---------------------------------------------------
pause
