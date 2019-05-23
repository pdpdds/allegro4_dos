
moviesinput(int movie_section)
{
	/*This function is used when the game is not
	in gameplay and a movie is being shown it updates the
	position of characters and position of the movie*/

	/*The mouse values are updated after each call  */
	if ((mouse_b & 1) && (mousepressed == 1))mousepressed = 2;
	if (!(mouse_b & 1))mousepressed = 0;
	if ((mouse_b & 1) && (mousepressed == 0))mousepressed = 1;
	if ((mouse_b & 2) && (mousepressedright == 1))mousepressedright = 2;                          /*the right mouse has been pressed*/
	if (!(mouse_b & 2))mousepressedright = 0;                                              /* and is still pressed*/
	if ((mouse_b & 2) && (mousepressedright == 0))mousepressedright = 1;                /*the mousepressed integer is only
																				  /*the right mouse is not pressed*/
																				  /*equal to one directly after it is pressed*/

	if (movie_section == 100)
	{
		/*shows characters talking to the screen*/
		if (((introcount >= 20 && introcount <= 23) || (introcount == 398) || (introcount == 399)) && (mousepressed == 0 || mousepressed == 2))
		{
			talkback = 1;
			memset(&talkback_desc[1], 0, sizeof(talkback_desc[1]));
			if (introcount == 20)
			{
				strcpy(talkback_desc[talkback].text, "HURRY UP THE JET WILL BE BY ANY MINUTE");
				talkcolor = 1;
			}
			if (introcount == 21)
			{
				strcpy(talkback_desc[talkback].text, "I'M GOING AS FAST AS I CAN.");
				talkcolor = 4;
			}
			if (introcount == 22)
			{
				strcpy(talkback_desc[talkback].text, "OKAY I'M DONE.");
				talkcolor = 4;
			}
			if (introcount == 23)
			{
				strcpy(talkback_desc[talkback].text, "ALRIGHT I'LL RADIO THE CAPTAIN.");
				talkcolor = 1;
			}
			if (introcount == 398)
			{
				strcpy(talkback_desc[talkback].text, "AW MAN THAT BEAM MADE ME CRASH MY CAR.");
				talkcolor = 1;
			}
			if (introcount == 399)
			{
				strcpy(talkback_desc[talkback].text, "I'M GONNA FIND OUT WHO DID THIS");
				talkcolor = 1;
			}
		}
		else
		{
			talkback = 0;
		}
		/*movie continues whenever the player is not talking
		to the screen and the movie is not over*/
		if (talkback == 0 && introcount != 400 && introcount != 0)
		{
			introcount++;
		}
		if (introcount >= 400 || mousepressedright == 1)
		{
			movies = 0;
		}
	}

	/*This is the scene by the truck
	where the badguys find a car on fire*/

	if (movie_section == 1)
	{

		/*each time the integer moviecount is incremented the
		badguys will move according to the value set in the binary file*/
		fseek(f2, sizeof(struct movie1)*moviecount, SEEK_SET);
		fread(&movie1_1, sizeof(struct movie1), 1, f2);
		showscreen = movie1_1.atscreen;
		/*This if statment sets when the character will talk to the
		screen and what will be said*/
		if ((moviecount == 1 || moviecount == 82 || moviecount == 156 || (moviecount == 159) || moviecount == 263 || moviecount == 375) && (mousepressed == 0 || mousepressed == 2))
		{
			talkback = 1;
			memset(&talkback_desc[1], 0, sizeof(talkback_desc[1]));
			if (moviecount == 1)strcpy(talkback_desc[talkback].text, "DID YOU HEAR THAT?");
			if (moviecount == 82)strcpy(talkback_desc[talkback].text, "DID YOU KNOW YOUR DOOR IS BROKEN?");
			if (moviecount == 156)strcpy(talkback_desc[talkback].text, "WOW! COME OVER HERE THERE IS A CAR ON FIRE");
			if (moviecount == 159)strcpy(talkback_desc[talkback].text, "BUT WHERE IS THE DRIVER");
			if (moviecount == 263)strcpy(talkback_desc[talkback].text, "I DON'T SEE ANYONE. LET'S GET BACK TO THE TRUCK");
			if (moviecount == 375)
			{
				talkcolor = 4;
				strcpy(talkback_desc[talkback].text, "I'LL CALL IT IN ON THE RADIO");
			}
		}
		else
		{
			talkback = 0;

		}
		/*movie continues whenever the player is not talking
		to the screen and the movie is not over*/
		if (talkback == 0 && moviecount != 484)
		{
			moviecount++;
		}

		/*starts next movie once movie is over*/
		if (moviecount >= 484 || mousepressedright == 1)
		{
			moviecount = 484;
			movies = 2;
			moviecount2 = 1;
		}
	}
	/*This movie shows the truck driving along and has
	the bad guys talk to the screen*/
	if (movie_section == 2)
	{
		int speed = 0, truckxran, truckyran;

		/*once the badguys decide to leave the truck will
		start moving*/
		if (movie1talk > 3)
		{
			/*I used sin and cos to make the truck look
			like it is bouncing along*/
			speed = (cos(RAD*(rand() % 15))) * 6;
			truckxran = truck1x + 90 + (sin(RAD*(rand() % 90))) * 2;
			truckyran = 244 + (sin(RAD*(rand() % 90))) * 4;

		}
		/*causes the movie to loop the background*/
		if (moviecount2 >= 2560)
		{
			moviecount2 = 641;
			if (movie1talk == 16)movie1talk = 17;
		}
		/*once the last words are said the truck will slow down*/
		if (movie1talk < 16)moviecount2 += speed;
		else moviecount2 += speed * 0.8;

		/*once the last words are said the screen will stop moving*/
		if (movie1talk == 17)
		{

			if (moviecount2 > 1240)moviecount2 = 1240;

			truck1x -= speed * 0.5;

		}
		/*makes sure the screen starts at screen two*/
		if (showscreen < 2)showscreen = 2;
		/*shows the background moving*/

		/*moves the dialog along as the mouse is pressed*/
		if (mousepressed == 1 && movie1talk < 16)
		{
			movie1talk++;
		}
		if (movie1talk >= 16)talkback = 0;
		else
			talkback = 1;
		memset(&talkback_desc[1], 0, sizeof(talkback_desc[1]));
		/*decides what text to show*/
		switch (movie1talk % 16) {

		case 0:
			strcpy(talkback_desc[talkback].text, "CRACKLE...CRACKLE THE RADIO IS BROKEN AGAIN");
			talkcolor = 1;
			break;
		case 1:
			strcpy(talkback_desc[talkback].text, "LOOK'S LIKE WE HAVE TO GO BACK AND REPORT TO THE CAPTAIN");
			talkcolor = 4;
			break;
		case 2:
			strcpy(talkback_desc[talkback].text, "BUT WE NEED TO GUARD THE GATE");
			talkcolor = 1;
			break;
		case 3:
			strcpy(talkback_desc[talkback].text, "IT WILL ONLY TAKE 5 MINUTES");
			talkcolor = 4;
			break;
		case 4:
			strcpy(talkback_desc[talkback].text, "OKAY. SO KNOW THAT THEY KNOW THE BEAM WORKS");
			talkcolor = 1;
			break;
		case 5:
			strcpy(talkback_desc[talkback].text, "WHAT WILL THEY DO WITH THE SCIENTIST");
			talkcolor = 1;
			break;
		case 6:
			strcpy(talkback_desc[talkback].text, "DOES IT MATTER");
			talkcolor = 4;
			break;
		case 7:
			strcpy(talkback_desc[talkback].text, "I...I JUST THOUGHT THE SCIENTIST WAS REALLY SMART");
			talkcolor = 1;
			break;
		case 8:
			strcpy(talkback_desc[talkback].text, "YEAH RIGHT SURE");
			talkcolor = 4;
			break;
		case 9:
			strcpy(talkback_desc[talkback].text, "WELL IF YOU NEED TO KNOW I OVERHEARD THAT...");
			talkcolor = 4;
			break;
		case 10:
			strcpy(talkback_desc[talkback].text, "THEY WERE GOING TO SHIP THE BEAM OUT TONIGHT...");
			talkcolor = 4;
			break;
		case 11:
			strcpy(talkback_desc[talkback].text, "AND THEN BLOW UP THE BUILDING AND ANY EVIDENCE");
			talkcolor = 4;
			break;
		case 12:
			strcpy(talkback_desc[talkback].text, "REALLY! DO THEY NEED TO BE THAT CRAZY?");
			talkcolor = 1;
			break;
		case 13:
			strcpy(talkback_desc[talkback].text, "YOU KNEW THEY WERE CRAZY WHEN YOU JOINED");
			talkcolor = 4;
			break;
		case 14:
			strcpy(talkback_desc[talkback].text, "ACTUALLY NO I DIDN'T. DID YOU?");
			talkcolor = 1;
			break;
		case 15:
			strcpy(talkback_desc[talkback].text, "OF COURSE I DID.");
			talkcolor = 4;
			break;



		}
		/*once the truck is off the screen the next movie will begin*/
		if (truck1x < -600 || mousepressedright == 1)
		{
			truck1x = -600;
			showscreen = 5;
			movies = 3;
			moviecount3 = 490;
			movie1talk = 0;
		}
	}
	/*shows the movie where the badguys pull into the garage
	and you leave through the backdoor in the truck*/

	if (movie_section == 3)
	{
		int speed = 0;
		int truckxran = 160;
		int truckyran = 247;
		/*moves characters along as movie progresses by reading
		the binary file*/
		fseek(f2, sizeof(struct movie1)*moviecount3, SEEK_SET);
		fread(&movie1_1, sizeof(struct movie1), 1, f2);

		/*has characters talk to screen a different parts during the movie*/
		if ((moviecount3 == 493 || moviecount3 == 1086 || moviecount3 == 1087 || moviecount3 == 795) && (mousepressed == 0 || mousepressed == 2))
		{
			memset(&talkback_desc[1], 0, sizeof(talkback_desc[1]));
			talkback = 1;
			if (moviecount3 == 493)
			{
				strcpy(talkback_desc[talkback].text, "HURRY UP THE CAPTAIN IS BY THE JAIL CELLS");
				talkcolor = 1;
			}
			if (moviecount3 == 795)
			{
				strcpy(talkback_desc[talkback].text, "I'LL HIDE BEHIND THESE BOXES.");
				talkcolor = 1;
			}
			if (moviecount3 == 1086)
			{
				strcpy(talkback_desc[talkback].text, "THE JET CRASHED ACROSS THE RIVER BY THE BARN");
				talkcolor = 4;
			}
			if (moviecount3 == 1087)
			{
				strcpy(talkback_desc[talkback].text, "OKAY LET'S GO");
				talkcolor = 1;
			}

		}
		else
		{
			talkback = 0;

		}


		/*moves the truck into the garage*/
		if (moviecount3 != 1088)
		{
			if (truck2x > 162)
			{
				speed = (cos(RAD*(rand() % 15))) * 5;
				truck2x -= speed;
				draw_sprite(buffer, datafile[21].dat, truck2x, 247 + (sin(RAD*(rand() % 50))) * 6);
				truckyran = 247 + (sin(RAD*(rand() % 50))) * 6;
				truckxran = truck2x;
			}

		}
		/*has the truck exit the garage*/
		if (moviecount3 == 1088)
		{
			speed = (cos(RAD*(rand() % 15))) * 5;
			truck2x += speed;

			truckyran = 247 + (sin(RAD*(rand() % 50))) * 6;
			truckxran = truck2x;
		}
		/*once the truck is gone the movie is done*/
		if ((truck2x > 640 && moviecount3 == 1088) || mousepressedright == 1)
		{
			movies = 0;
			f_playerx = playerx = 575;
			f_playery = playery = 150;
			stopplayer();
			showplayer = 3;
			showscreen = 5;
			pick_choice = 0;
			screen_objects = 0;
			clearfire();
		}

		/*moves the movie along when characters are done talking
		and the truck has stopped moving*/
		if (talkback == 0 && moviecount3 < 1088 && truck2x <= 162)moviecount3++;

	}
	/*shows the movie where the main character is talking to the
	scientist in the sewers*/
	if (movie_section == 4)
	{
		/*moves movie along as player presses the mouse*/
		if (mousepressed == 1 && movie1talk < 13)
		{

			movie1talk++;
		}
		/*when the talking is done the movie is over*/
		if (movie1talk >= 12 || mousepressedright == 1)
		{
			movies = 0;
			level = 2;
		}
		else
			talkback = 1;
		memset(&talkback_desc[1], 0, sizeof(talkback_desc[1]));
		/*decides what text to show*/
		switch (movie1talk % 13) {

		case 0:
			strcpy(talkback_desc[talkback].text, "OKAY NOW WHO ARE YOU AND WHY ARE YOU HELPING ME?");
			talkcolor = 4;
			break;
		case 1:
			strcpy(talkback_desc[talkback].text, "I WAS JUST DRIVING ALONG AND I SAW...");
			talkcolor = 1;
			break;
		case 2:
			strcpy(talkback_desc[talkback].text, "OH YOU JUST HAPPENED TO BE DRIVING BY");
			talkcolor = 4;
			break;
		case 3:
			strcpy(talkback_desc[talkback].text, "LISTEN I DON'T HAVE TIME TO SQUEEZE THE TRUTH FROM YOU.");
			talkcolor = 4;
			break;
		case 4:
			strcpy(talkback_desc[talkback].text, "WE NEED TO GET TO THE LAB AND TAKE MY DATA OUT OF THE BEAM");
			talkcolor = 4;
			break;
		case 5:
			strcpy(talkback_desc[talkback].text, "WHAT WE NEED TO DO IS GET OUT OF HERE.");
			talkcolor = 1;
			break;
		case 6:
			strcpy(talkback_desc[talkback].text, "THEY ARE ABOUT TO BLOW THIS PLACE UP.");
			talkcolor = 1;
			break;
		case 7:
			strcpy(talkback_desc[talkback].text, "FIRST WE GET THE DATA DISC THEN WE LEAVE.");
			talkcolor = 4;
			break;
		case 8:
			strcpy(talkback_desc[talkback].text, "THEN WE GET TO THE PILOT OF THAT JET BEFORE THEY DO.");
			talkcolor = 4;
			break;
		case 9:
			strcpy(talkback_desc[talkback].text, "WHAT'S GOING ON HERE. SOMEONE BETTER PAY FOR MY CAR.");
			talkcolor = 1;
			break;
		case 10:
			strcpy(talkback_desc[talkback].text, "I'LL EXPLAIN EVERYTHING LATER.");
			talkcolor = 4;
			break;
		case 11:
			strcpy(talkback_desc[talkback].text, "PASSWORD : LEVEL2");
			talkcolor = 6;
			break;
		case 12:
			strcpy(talkback_desc[talkback].text, "FOLLOW ME. I'LL GET US TO THE LAB.");
			talkcolor = 4;
			break;


		}


	}

	if (movie_section == 5)
	{

		/*each time the integer pace2 is incremented the
		badguys will move according to the value set in the binary file*/
		if (pace2 < 440)pace2 = 440;
		fseek(f5, sizeof(struct mover)*(pace2), SEEK_SET);
		fread(&guy1, sizeof(struct mover), 1, f5);


		/*This if statment sets when the character will talk to the
		screen and what will be said*/
		if ((pace2 == 465 || pace2 == 466 || pace2 == 473 || pace2 == 544 || pace2 == 558 || pace2 == 566 || pace2 == 619 || pace2 == 620 || pace2 == 621) && (mousepressed == 0 || mousepressed == 2))
		{
			talkback = 1;
			memset(&talkback_desc[1], 0, sizeof(talkback_desc[1]));
			if (pace2 == 465)
			{
				talkcolor = 1;
				strcpy(talkback_desc[talkback].text, "DID YOU TURN OFF THE LIGHTS?");
			}
			if (pace2 == 466)
			{
				strcpy(talkback_desc[talkback].text, "NO MAYBE THE BULB BURNED OUT");
				talkcolor = 4;
			}
			if (pace2 == 473)
			{
				strcpy(talkback_desc[talkback].text, "I'M TAKING A BREAK");
				talkcolor = 1;
			}
			if (pace2 == 544)
			{
				strcpy(talkback_desc[talkback].text, "THE CAPTAIN WILL BE BY SOON");
				talkcolor = 1;
			}
			if (pace2 == 558)
			{
				strcpy(talkback_desc[talkback].text, "WHAT DOES HE LOOK LIKE");
				talkcolor = 4;
			}
			if (pace2 == 566)
			{
				strcpy(talkback_desc[talkback].text, "THE ONE THAT HAS A CAPTAIN UNIFORM");
				talkcolor = 1;
			}
			if (pace2 == 619)
			{
				strcpy(talkback_desc[talkback].text, "WHAT A DOPE");
				talkcolor = 1;
			}
			if (pace2 == 620)
			{
				strcpy(talkback_desc[talkback].text, "DID YOU SAY SOMETHING");
				talkcolor = 4;
			}
			if (pace2 == 621)
			{
				strcpy(talkback_desc[talkback].text, "NOTHING");
				talkcolor = 1;
			}

		}
		else
		{
			talkback = 0;
		}

		/*movie continues whenever the badguys are not talking
		to the screen and the movie is not over*/
		if (talkback == 0)
		{
			pace2++;
		}

		/*ends movie once movie is over*/
		if (pace2 >= 625 || mousepressedright == 1)
		{
			pace2 = 625;
			movies = 0;
			clear_to_color(redlight, makecol(255, 0, 255));
			if (lablights == 1)rectfill(redlight, 320, 0, 640, 240, makecol(32, 32, 48));
			if (stocklights == 1)rectfill(redlight, 320, 240, 640, 480, makecol(32, 32, 48));

			return;
		}

	}





}




moviedisplay1(int movie_section)
{
	/*This function is used for display when the game is not
	in gameplay and a movie is being shown*/


	if (movie_section == 100)
	{
		if (introcount == 0)introcount = 1;
		if (introcount < 398)
		{
			set_trans_blender(0, 0, 0, 96);
			if (introcount < 20 || introcount>23)
			{
				blit(intro[0], buffer, 0, 0, 0, 0, 640, 480);
				draw_sprite(buffer, intro[1], introcount*0.75, 440);
				if (introcount > 40)
					draw_sprite(buffer, intro[2], 550, 700 - introcount * 1.6);
				if (introcount > 250)
				{
					/*shows the laser shooting the jet*/
					poly[0] = 49;
					poly[1] = 332;
					poly[2] = poly[0];
					poly[3] = 325;
					poly[4] = 570;
					poly[5] = (700 - introcount * 1.6) - 20 - introcount / 4;
					poly[6] = 570;
					poly[7] = (700 - introcount * 1.6) + 0 + introcount / 4;
					polygon(redlight, 4, poly, makecol(20, 225, 20));
				}
			}
		}
		else
		{                                            /*shows guy after crashing his
													 car*/
			blit(level1[1], buffer, 0, 0, 0, 0, 640, 480);
			snowing(1, 6);
			draw_sprite(buffer, datafile[23].dat, 260, 190);
			fires(290, 230, 700, 40, 3000, 425, 100);
			draw_sprite(buffer, datafile[3].dat, 50, 200);
			snowing(2, 6);
		}
		if (introcount >= 20 && introcount <= 23)
		{
			blit(intro[3], buffer, 0, 0, 0, 0, 640, 480);
			draw_sprite(buffer, character[21], 220, 200);
			draw_sprite(buffer, character[11], 300, 250);
		}


	}

	/*This is the scene by the truck
	where the badguys find a car on fire*/

	if (movie_section == 1)
	{

		/*each time the integer moviecount is incremented the
		badguys will move according to the value set in the binary file*/
		fseek(f2, sizeof(struct movie1)*moviecount, SEEK_SET);
		fread(&movie1_1, sizeof(struct movie1), 1, f2);
		showscreen = movie1_1.atscreen;


		/*the showscreen value is taken from the binary file
		and the background is blitted to the buffer*/
		blit(level1[showscreen], buffer, 0, 0, 0, 0, 640, 480);
		/*depending what screen is being shown the car or truck will be
		shown and fire will or will not be shown*/

		if (showscreen == 1)  draw_sprite(buffer, datafile[23].dat, 260, 190);
		if (showscreen == 2)  draw_sprite(buffer, datafile[21].dat, 90, 244);
		/*shows the snow in the background*/
		snowing(1, 4);
		if (showscreen == 1)fires(290, 230, 700, 40, 3000, 425, 100);
		/*shows what position the badguy is at and what the badguy looks like*/
		if (movie1_1.show2 > 0)draw_sprite(buffer, character[movie1_1.show2 + 9], movie1_1.x2, movie1_1.y2);
		if (movie1_1.show1 > 0)draw_sprite(buffer, character[movie1_1.show1 - 1], movie1_1.x1, movie1_1.y1);
		/*shows larger snow in the foreground*/
		snowing(2, 4);

	}
	/*This movie shows the truck driving along and has
	the bad guys talk to the screen*/
	if (movie_section == 2)
	{
		int c, speed = 0, truckxran, truckyran;

		/*once the badguys decide to leave the truck will
		start moving*/
		if (movie1talk > 3)
		{
			/*I used sin and cos to make the truck look
			like it is bouncing along*/
			speed = (cos(RAD*(rand() % 15))) * 12;
			truckxran = truck1x + 90 + (sin(RAD*(rand() % 90))) * 2;
			truckyran = 244 + (sin(RAD*(rand() % 90))) * 6;
			/*This polygon draws the headlights in front of the truck*/
			poly[0] = truckxran - 175;
			poly[1] = truckyran + 200;
			poly[2] = poly[0];
			poly[3] = poly[1] - 125;
			poly[4] = truckxran;
			poly[5] = truckyran + 125;
			poly[6] = truckxran;
			poly[7] = truckyran + 150;
			polygon(redlight, 4, poly, makecol(255, 255, 155));

		}

		/*shows the background moving*/
		if (moviecount2 <= 640 && movie1talk > 3)blit(level1[showscreen], buffer, 0, 0, moviecount2, 0, 640, 480);
		else    blit(level1[showscreen], buffer, 0, 0, 0, 0, 640, 480);

		/*once the truck is moving the background will scroll
		until the last words are shown.
		then the background will stop and truck will move
		off the screen*/
		if (movie1talk != 17)
		{
			for (c = 1; c < 50; c++)
			{
				if (moviecount2 - (c * 640) <= 640 && moviecount2 - (c * 640) >= -640)
					draw_sprite(buffer, level1[3], moviecount2 - (c * 640), 0);
			}
		}
		else
		{
			draw_sprite(buffer, level1[3], moviecount2 - 640, 0);
			draw_sprite(buffer, level1[4], moviecount2 - 1280, 0); /*the last screen to show
															  is of the garage*/
		}
		snowing(1, (speed)+2);    /*background snow speeds
								 up as the truck moves faster*/

								 /*once the first couple words are said the
								 truck will start moving*/
		if (movie1talk > 3)draw_sprite(buffer, datafile[21].dat, truckxran, truckyran);
		else
			draw_sprite(buffer, datafile[21].dat, 90, 244);



		snowing(2, (speed) * 2 + 4);   /*foreground snow speeds
								 up as the truck moves faster*/


	}
	/*shows the movie where the badguys pull into the garage
	and you leave through the backdoor in the truck*/

	if (movie_section == 3)
	{
		int speed = 0;
		int truckxran = 160;
		int truckyran = 247;
		/*moves characters along as movie progresses by reading
		the binary file*/
		fseek(f2, sizeof(struct movie1)*moviecount3, SEEK_SET);
		fread(&movie1_1, sizeof(struct movie1), 1, f2);


		/*shows the garage*/
		blit(level1[movie1_1.atscreen], buffer, 0, 0, 0, 0, 640, 480);
		/*shows objects on the shelf*/
		draw_sprite(buffer, datafile[26].dat, 270, 155);
		draw_sprite(buffer, datafile[28].dat, 230, 155);

		/*moves the truck into the garage*/
		if (moviecount3 != 1088)
		{
			if (truck2x > 162)
			{
				speed = (cos(RAD*(rand() % 15))) * 5;
				truck2x -= speed;
				draw_sprite(buffer, datafile[21].dat, truck2x, 247 + (sin(RAD*(rand() % 50))) * 6);
				truckyran = 247 + (sin(RAD*(rand() % 50))) * 6;
				truckxran = truck2x;
			}
			else
				/*keeps the truck still for a while*/
				draw_sprite(buffer, datafile[21].dat, 160, 247);
		}
		/*has the truck exit the garage*/
		if (moviecount3 == 1088)
		{
			speed = (cos(RAD*(rand() % 15))) * 5;
			truck2x += speed;
			draw_sprite(buffer, datafile[21].dat, truck2x, 247 + (sin(RAD*(rand() % 50))) * 6);
			truckyran = 247 + (sin(RAD*(rand() % 50))) * 6;
			truckxran = truck2x;
		}

		/*shows the truck headlights*/
		poly[0] = truckxran - 175;
		poly[1] = truckyran + 200;
		poly[2] = poly[0];
		poly[3] = poly[1] - 125;
		poly[4] = truckxran;
		poly[5] = truckyran + 125;
		poly[6] = truckxran;
		poly[7] = truckyran + 150;
		polygon(redlight, 4, poly, makecol(255, 255, 155));
		/*draws badguys*/
		if (moviecount3 < 735 || moviecount3>811)
		{
			if (movie1_1.show2 > 0)draw_sprite(buffer, character[movie1_1.show2 + 9], movie1_1.x2, movie1_1.y2);
			if (movie1_1.show1 > 0 && moviecount3 > 490)draw_sprite(buffer, character[movie1_1.show1 - 1], movie1_1.x1, movie1_1.y1);
		}
		else
			/*draws main character sneak behind boxes*/
			if (movie1_1.show1 > 0)draw_sprite(buffer, datafile[movie1_1.show1].dat, movie1_1.x1, movie1_1.y1);

	}
	/*shows the movie where the main character is talking to the
	scientist in the sewers*/
	if (movie_section == 4)
	{

		blit(level1[showscreen], buffer, 0, 0, 0, 0, 640, 480);
		draw_sprite(buffer, datafile[showplayer].dat, playerx, playery);
		draw_sprite(buffer, character[guy1.show + 19], 400, 100);



	}

	if (movie_section == 5)
	{
		blit(level2[10 - 7], buffer, 0, 0, 0, 0, 640, 480);
		draw_sprite(buffer, character[guy1.show + 9], guy1.x, guy1.y);
		draw_sprite(buffer, character[12], 200, 120);
	}

	draw_trans_sprite(buffer, redlight, 0, 0);  /*draws lighting for each movie*/
	fore_ground();                              /*draws text for each movie*/
}
