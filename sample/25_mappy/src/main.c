#include <allegro.h>
#include "mappyal.h"
#include "player.h"

volatile long g_speed_counter = 0; //속도 카운터

void increment_speed_counter() //속도 카운터 값을 증가시키는 카운터 함수
{
	g_speed_counter++;
}

END_OF_FUNCTION(increment_speed_counter); //타이머 함수가 끝난다는 것을 명시적으로 설정

BITMAP* buffer;

int mapxoff = 0, mapyoff = 0; //오프스크린을 위한 x, y 좌표

int main(void)
{
	allegro_init(); //알레그로 엔진 초기화
	install_keyboard(); //키보드 설치
	set_color_depth(16);

	install_timer(); //타이머 시스템을 초기화한다

	LOCK_VARIABLE(g_speed_counter);
	LOCK_FUNCTION(increment_speed_counter);

	install_int_ex(increment_speed_counter, BPS_TO_TIMER(60)); //초당 타이머 함수 호출 수(BEAT PER SECOND)

#ifdef _WIN32
	if  (set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0)) { //640 * 480 해상도
#else
	/* set a graphics mode sized 320x200 */
	if (set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0) != 0) {
#endif
		if (set_gfx_mode(GFX_SAFE, 640, 400, 0, 0) != 0) {
			set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
			allegro_message("Unable to set any graphic mode\n%s\n", allegro_error);
			return 1;
		}
	}


	//매피 맵 데이터 로드
	if (MapLoad((char*)"simple.FMP") != 0)
	{
		allegro_message("mappy map file load fail!!");
		return 1;
	}

	//플레이어 로드
	InitPlayer("guy.bmp");

	buffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
	clear(buffer);

	while (TRUE)
	{

		while (g_speed_counter > 0)
		{
			
			if (key[KEY_ESC])
			{
				break;
			}

			mapxoff = player->x + player->width / 2 - SCREEN_WIDTH / 2 + 10;
			mapyoff = player->y + player->height / 2 - SCREEN_HEIGHT / 2 + 10;

			//avoid moving beyond the map edge
			if (mapxoff < 0) mapxoff = 0;
			if (mapxoff > (mapwidth * mapblockwidth - SCREEN_WIDTH))
				mapxoff = mapwidth * mapblockwidth - SCREEN_WIDTH;
			if (mapyoff < 0)
				mapyoff = 0;
			if (mapyoff > (mapheight * mapblockheight - SCREEN_HEIGHT))
				mapyoff = mapheight * mapblockheight - SCREEN_HEIGHT;

			UpdatePlayer();

			g_speed_counter--;
		}

		//백그라운드 타일을 먼저 그린다.
		MapDrawBG(buffer, mapxoff, mapyoff, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
		//포그라운드 타일을 그린다.
		MapDrawFG(buffer, mapxoff, mapyoff, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, 0);

		RenderPlayer(buffer, mapxoff, mapyoff);

		//화면버퍼에 블리팅한다.
		vsync();
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
		release_screen();
	}

	destroy_bitmap(buffer);

	MapFreeMem(); //매피 맵 데이터를 파괴한다.

	allegro_exit();

	return 0;
}
END_OF_MAIN()
