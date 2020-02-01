#include "map.h"
#include "bitmap_ptr.h"
#include <ctime>

int main(int argc, char argv[]) {
    uint mw = 150;
    uint mh = 150;

    allegro_init();
    {
        install_timer();
        install_keyboard();

#ifdef _WIN32
        if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1024, 800, 0, 0))
#else
        if (set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0))
#endif
            return 0;

        MAP map(mw, mh);        

        map.make_tiles();

        BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
        BITMAP* buffer2 = create_bitmap(SCREEN_W, SCREEN_H);
        clear(buffer);
        clear(buffer2);

        int x = mw*8, y = mh*4;
        int fps = 0;
        int fps_count = 0;
        const int fps_delay = 100;

        clock_t last_time = clock();
        do {
            clear(buffer);
            map.draw(buffer, -x, -y);

            if (fps_count > fps_delay) {
                if (retrace_count)
                    fps = (unsigned)(fps_delay*(70/(float)retrace_count));
                fps_count = 0;
                retrace_count = 0;
            } else {
                ++fps_count;
            }

            textprintf_ex(buffer, font, 0, 0, -1, -1, "FPS: %u", fps);
            //vsync();
            blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

            /* // perspective effect
               clear(buffer2.get());
               for (int i = 0; i != buffer.get()->h; ++i) {
               w = (int)(((buffer.get()->w*(1 - p))*i)/buffer.get()->h + p*buffer.get()->w);
               stretch_blit(buffer.get(), buffer2.get(), 0, i, 320, 1, (buffer.get()->w - w)/2, i, w, 1);
            //stretch_blit(buffer.get(), buffer2.get(), 0, i, 320, 1, (buffer.get()->w - w)/2, (int)(buffer.get()->h*(1 - ps) + (i*i/buffer.get()->h)*ps), w, (int)(buffer.get()->h*(1 - ps) + ((i + 1)*(i + 1)/buffer.get()->h)*ps) - (int)(buffer.get()->h*(1 - ps) + (i*i/buffer.get()->h)*ps));
            }
            textprintf_ex(buffer2.get(), font, 0, 0, -1, -1, "FPS: %u", fps);
            vsync();
            blit(buffer2.get(), screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
            */

            if (key[KEY_UP])    y -= 2;
            if (key[KEY_DOWN])  y += 2;
            if (key[KEY_LEFT])  x -= 4;
            if (key[KEY_RIGHT]) x += 4;

            // frame limiting
            //clock_t this_time = clock();
            //clock_t diff = this_time - last_time;
            //last_time = this_time;
            //clock_t wait = MAX(0, 17 - (diff * 1000/CLOCKS_PER_SEC)); // 17ms ~ 58 fps
            //rest(wait);
        } while (!key[KEY_ESC]);

    }
    allegro_exit();
    return 0;
}
END_OF_MAIN();
