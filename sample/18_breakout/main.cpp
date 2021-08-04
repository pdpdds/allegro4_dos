#include <allegro.h>
#include "game.h"

volatile long g_speed_counter = 0; //�ӵ� ī����

void increment_speed_counter(void) //�ӵ� ī���� ���� ������Ű�� ī���� �Լ�
{
	g_speed_counter++;
}
END_OF_FUNCTION(increment_speed_counter); //Ÿ�̸� �Լ��� �����ٴ� ���� ��������� ����

BITMAP* backBuffer;

int main()
{
	allegro_init();
	install_mouse();
	install_keyboard();
	install_timer(); //Ÿ�̸� �ý����� �ʱ�ȭ�Ѵ�

#ifdef _WIN32
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 320, 200, 0, 0) != 0) {
#else					
	if (set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0) != 0) {
#endif
		if (set_gfx_mode(GFX_SAFE, 320, 200, 0, 0) != 0) {
			set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
			allegro_message("Unable to set any graphic mode\n%s\n", allegro_error);
			return 0;
		}
	}

	LOCK_VARIABLE(g_speed_counter);
	LOCK_FUNCTION(increment_speed_counter);

	install_int_ex(increment_speed_counter, BPS_TO_TIMER(30)); //�ʴ� Ÿ�̸� �Լ� ȣ�� ��(BEAT PER SECOND)

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
			ProcessLogic(mouse_b, mouse_x);
			g_speed_counter--;

		}

		clear_to_color(backBuffer, makecol(0, 0, 255)); //����� Ǫ�������� �ʱ�ȭ
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
	textout_ex(backBuffer, font, szMessage, x, y, makecol(0, 0, 0), -1);
}


#ifdef _WIN32
END_OF_MAIN()
#endif