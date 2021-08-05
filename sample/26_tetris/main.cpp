#include <allegro.h>
#include "game.h"


volatile long g_speed_counter = 0; //�ӵ� ī����
volatile long g_tick_count = 0; //ƽ ī��Ʈ

void increment_speed_counter(void) //�ӵ� ī���� ���� ������Ű�� ī���� �Լ�
{
	g_speed_counter++;
	g_tick_count++;
}

int now()
{
	return g_tick_count;
}

END_OF_FUNCTION(increment_speed_counter); //Ÿ�̸� �Լ��� �����ٴ� ���� ��������� ����

BITMAP* g_backBuffer = 0;

int main()
{
	allegro_init();
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

	install_int_ex(increment_speed_counter, BPS_TO_TIMER(20)); //�ʴ� Ÿ�̸� �Լ� ȣ�� ��(BEAT PER SECOND)

	g_backBuffer = create_bitmap(screen->w, screen->h);
	
	if (!g_backBuffer)
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

		clear_to_color(g_backBuffer, makecol(0, 0, 0)); //����� Ǫ�������� �ʱ�ȭ
		DrawWorld();

		blit(g_backBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

	}

	return 0;
}


extern void RenderRect(int x1, int y1, int x2, int y2, int r, int g, int b)
{
	rectfill(g_backBuffer, x1, y1, x2, y2, makecol(r, g, b));
}

extern void RenderCircle(int x, int y, int radius, int r, int g, int b)
{
	circlefill(g_backBuffer, x, y, radius, makecol(r, g, b));
}


extern void RenderText(int x, int y, char* szMessage)
{
	textout_ex(g_backBuffer, font, szMessage, x, y, makecol(255, 255, 255), -1);
}

#ifdef _WIN32
END_OF_MAIN()
#endif