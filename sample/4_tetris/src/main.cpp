#include <iostream>
#define wys_tab 21
#define szer_tab 12
#include "gra.h"
#include "start.h"
#include "silnik.h"
#include "rozgrywka.h"
#include <allegro.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

float virtual_screen_x;
float virtual_screen_y;

float screen_ratio_x;
float screen_ratio_y;

int main()
{
#ifdef _WIN32
	virtual_screen_x = SCREEN_WIDTH;
	virtual_screen_y = SCREEN_HEIGHT;

	screen_ratio_x = 1.0;
	screen_ratio_y = 1.0;
#else
	virtual_screen_x = 320;
	virtual_screen_y = 200;

	screen_ratio_x = virtual_screen_x / (float)SCREEN_WIDTH;
	screen_ratio_y = virtual_screen_y / (float)SCREEN_HEIGHT;
#endif // _WIN32	

	start k_start;
	rozgrywka k_rozgrywka;

	int p = k_start.startuj();
	if (p == 0) { k_start.allegro_off(); return 0; }
	if (p == 1)
	{
		k_start.allegro_off();
		k_rozgrywka.play();
	}

	return 0;
}
END_OF_MAIN()




