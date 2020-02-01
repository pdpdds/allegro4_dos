#include <cstdio>
#include <ctime>
#include "window.h"
#include "map.h"
#include "allegro.h"


void main_init();
void main_done();

void gui_demo();
void map_demo();

// MAIN FUNCTION ////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {
    main_init();
    if (argc > 1 && std::string(argv[1]) == "map")
        map_demo();
    else
        gui_demo();
    return 0;
}
END_OF_MAIN();



void main_init() {
    printf("Loading...\n");
    allegro_init();
    install_timer();
    install_keyboard();
    if (install_mouse() == -1) {
        fprintf(stderr, "\nError loading mouse: %s", allegro_error);
        exit(-1);
    }
}



void gui_demo() {
    set_color_depth(16);
#ifdef _WIN32
    if (set_gfx_mode((GFX_AUTODETECT_WINDOWED), 320, 200, 0, 0))
#else
    if (set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0))
#endif
        return;

    BITMAP * scrbuffer = create_bitmap(SCREEN_W, SCREEN_H);
    clear_to_color(scrbuffer, makecol16(50, 50, 150));
    blit(scrbuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

#include "../data/maindata.h"
    DATAFILE * maindata = load_datafile("data/maindata.dat");

    WINDOW win(10, 10, 250, 150, scrbuffer);
    win.set_name("GALGUI (Greg and Lucy's Graphical User Interface)");
    win.set_font((FONT *)maindata[ARIAL].dat);
    win.set_background((BITMAP *)maindata[WINDOW_BACK2].dat);

    set_color_conversion(COLORCONV_TOTAL);
    PALETTE pal;
    BITMAP * icon8 = load_bitmap("scenario/default/infant0.pcx", pal);
    BITMAP * icon = create_bitmap(24, 24);
    clear_to_color(icon, makecol16(255, 0, 255));

    if (icon8 != NULL)
        masked_blit(icon8, icon, 0, 0, 0, 0, 24, 24);

    BUTTON iconb(20, 100, 30, 30, win, NULL);
    iconb.set_icon(icon);

    BUTTON testb(100, 100, 100, 20, win, NULL);
    testb.set_name("Click me");

    BUTTON okayb(140, 155, 80, 20, win, NULL);
    okayb.make_okay();

    BUTTON cancelb(230, 155, 80, 20, win, NULL);
    cancelb.make_cancel();

    bool checkboxes[3];
    checkboxes[0] = checkboxes[1] = false;
    checkboxes[2] = true;
    CHECKBOX c1(20, 20, 12, 12, win, &(checkboxes[0]));
    CHECKBOX c2(20, 35, 12, 12, win, &(checkboxes[1]));
    CHECKBOX c3(20, 50, 12, 12, win, &(checkboxes[2]));

    LABEL l1(40, 22, 315, 12, win);
    l1.set("Checkbox One");
    LABEL l2(40, 37, 315, 12, win);
    l2.set("Checkbox Two");
    LABEL l3(40, 52, 315, 12, win);
    l3.set("Checkbox Three");

    win.run();

    destroy_bitmap(icon8);
    destroy_bitmap(icon);
}



void map_demo() {
    set_color_depth(16);
#ifdef _WIN32
    if (set_gfx_mode((GFX_AUTODETECT_WINDOWED), 320, 200, 0, 0))
#else
    if (set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0))
#endif
        return;

    MAP m(128, 128);

    BITMAP * scrbuffer = create_bitmap(SCREEN_W, SCREEN_H);
    clear_to_color(scrbuffer, makecol16(50, 50, 150));
    blit(scrbuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

    m.generate();

    clock_t last_time = clock();
    clock_t this_time, diff, wait;
    int mx = 0, my = 0;
    for (;;) {
        clear(scrbuffer);
        m.draw(scrbuffer, mx, my);
        blit(scrbuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        if (key[KEY_UP])    --my;
        if (key[KEY_DOWN])  ++my;
        if (key[KEY_LEFT])  --mx;
        if (key[KEY_RIGHT]) ++mx;
        if (key[KEY_ESC])
            break;

        // frame limiting
        this_time = clock();
        diff = this_time - last_time;
        last_time = this_time;
        wait = MAX(0, 17 - (diff * 1000/CLOCKS_PER_SEC)); // 17ms ~ 58 fps
        rest(wait);
    }
}

