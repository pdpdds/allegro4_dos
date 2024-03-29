#include <allegro.h>
#include "game.h"

volatile long g_speed_counter = 0; //속도 카운터
volatile long g_tick_count = 0; //틱 카운트

void increment_speed_counter(void) //속도 카운터 값을 증가시키는 카운터 함수
{
	g_speed_counter++;
	g_tick_count++;
}
END_OF_FUNCTION(increment_speed_counter); //타이머 함수가 끝난다는 것을 명시적으로 설정

BITMAP* backBuffer;

int main()
{
	allegro_init();
	install_mouse();
	install_keyboard();
	install_timer(); //타이머 시스템을 초기화한다

#ifdef _WIN32
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 400, 0, 0) != 0) {
#else					
	if (set_gfx_mode(GFX_AUTODETECT, 640, 400, 0, 0) != 0) {
#endif
		if (set_gfx_mode(GFX_SAFE, 320, 200, 0, 0) != 0) {
			set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
			allegro_message("Unable to set any graphic mode\n%s\n", allegro_error);
			return 0;
		}
	}

	LOCK_VARIABLE(g_speed_counter);
	LOCK_FUNCTION(increment_speed_counter);

	install_int_ex(increment_speed_counter, BPS_TO_TIMER(10)); //초당 타이머 함수 호출 수(BEAT PER SECOND)

	backBuffer = create_bitmap(screen->w, screen->h);
	
	if (!backBuffer) 
	{
		allegro_message("Failed to create double buffer. Out of memory?");
		exit(1);
	}

	set_palette(desktop_palette);

	InitGame(SCREEN_W, SCREEN_H);
	
	while (key[KEY_ESC] == FALSE)
	{
		
		while (g_speed_counter > 0)
		{
			ProcessLogic(key[KEY_LEFT], key[KEY_RIGHT], key[KEY_UP], key[KEY_DOWN], key[KEY_SPACE]);
			g_speed_counter--;
		}

		clear_to_color(backBuffer, makecol(0, 0, 255)); //배경을 푸른색으로 초기화
		DrawWorld();

		blit(backBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

	}

	return 0;
}


extern void RenderRect(int x1, int y1, int x2, int y2, int r, int g, int b, int fill)
{
	if(fill)
		rectfill(backBuffer, x1, y1, x2, y2, makecol(r, g, b));
	else
		rect(backBuffer, x1, y1, x2, y2, makecol(r, g, b));
}

extern void RenderCircle(int x, int y, int radius, int r, int g, int b)
{
	circlefill(backBuffer, x, y, radius, makecol(r, g, b));
}

extern void RenderLine(int x, int y, int x2, int y2, int r, int g, int b)
{
	line(backBuffer, x, y, x2, y2, makecol(r, g, b));
}

extern void RenderText(int x, int y, char* szMessage)
{
	textout_ex(backBuffer, font, szMessage, x, y, makecol(255, 255, 255), -1);
}


#ifdef _WIN32
END_OF_MAIN()
#endif