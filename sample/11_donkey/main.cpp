#include <allegro.h>

#define Y 40
#define X 30
#define P 16

BITMAP *buffer;
BITMAP *pj;
BITMAP *barril;

BITMAP *roca;
BITMAP *escalera;
BITMAP *pjbmp;
BITMAP *pjclimb;
BITMAP *donkey;
BITMAP *princesa;
BITMAP *barrilbmp;

int bx[300];
int by[300];
int balt[300];
int bdir[300];
int bactual = 0;
int tiempo_creacion = 0;
int delta_tiempo = -40;

int px = 1 * P, py = 37 * P;
int alt = 0, dir = 0;
bool salto = false, caida = false;
bool escalando = false;
int dsalto, dcaida;
bool psalto = false;
int vidas = 3;
int invencible = 0;

int game = 0;

char mapa[Y][X + 1] = {
	"       pg                     ", //0
	"       gg                     ",
	"       gg                     ",
	"      xxxxxxxxnx              ",
	"              h               ",
	"d             h               ", //5
	"              h               ",
	"              h               ",
	"              h               ",
	"xxxxxxxxxxxxxxxxxxxxxxxxxnx   ",
	"                         h    ", //10
	"                         h    ",
	"                         h    ",
	"                         h    ",
	"                         h    ",
	"  xxxnxxxxxxxxxxxxxxxxxxxxxxxx", //15
	"     h                        ",
	"     h                        ",
	"     h                        ",
	"     h                        ",
	"     h                        ", //20
	"xxxxxxxxxxxxxxxxxxxxxxxxxxn   ",
	"                          h   ",
	"                          h   ",
	"                          h   ",
	"                          h   ", //25
	"                          h   ",
	"  xxnxxxxxxxxxxxxnxxxxxxxxxxxx",
	"    h            h            ",
	"    h            h            ",
	"    h            h            ", //30
	"    h            h            ",
	"    h            h            ",
	"xxxxxxxxxxxxxxxxxxxxxxxxxxn   ",
	"                          h   ",
	"                          h   ", //35
	"                          h   ",
	"                          h   ",
	"                          h   ",
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"  //39
};

bool verificar(int x, int y, int tamx, int tamy, char obj) {
	bool b = false;
	for (int i = x; i < x + tamx; i++) {
		for (int j = y; j < y + tamy; j++) {
			if (mapa[j / P][i / P] == obj) b = true;
		}
	}
	return b;
}

bool verificaru(int x, int y, int tamx, int tamy, char obj) {
	bool b = true;
	for (int i = x; i < x + tamx; i++) {
		for (int j = y; j < y + tamy; j++) {
			if (mapa[j / P][i / P] != obj) {
				b = false;
			}
		}
	}
	return b;
}

bool verificaru(int x, int y, int tamx, int tamy, char obj1, char obj2) {
	bool b = true;
	for (int i = x; i < x + tamx; i++) {
		for (int j = y; j < y + tamy; j++) {
			if (mapa[j / P][i / P] != obj1 && mapa[j / P][i / P] != obj2) {
				b = false;
			}
		}
	}
	return b;
}

bool verificar_o(int x, int y, int tamx, int tamy, int xo, int yo, int tamxo, int tamyo) {
	bool b = false;
	for (int i = x; i < x + tamx; i++) {
		for (int j = y; j < y + tamy; j++) {
			if ((i > xo && i < xo + tamxo) && (j > yo && j < yo + tamyo)) b = true;
		}
	}
	return b;
}

void dibujar() {

	for (int y = 0; y < Y; y++) {
		for (int x = 0; x < X; x++) {

			if (mapa[y][x] == 'x') draw_sprite(buffer, roca, x*P, y*P);
			if (mapa[y][x] == 'h') draw_sprite(buffer, escalera, x*P, y*P);
			if (mapa[y][x] == 'n') draw_sprite(buffer, roca, x*P, y*P);
			if (mapa[y][x] == 'p') draw_sprite(buffer, princesa, x*P, (y*P) + 4);
			if (mapa[y][x] == 'd') draw_sprite(buffer, donkey, x*P, y*P);
		}
	}

	if (escalando) blit(pjclimb, pj, alt * 30, 0, 0, 0, 30, 32);
	else blit(pjbmp, pj, alt * 30, dir * 32, 0, 0, 30, 32);
	draw_sprite(buffer, pj, px, py);

	for (int i = 0; i < bactual; i++) {
		blit(barrilbmp, barril, balt[i] * 24, 0, 0, 0, 24, 20);
		draw_sprite(buffer, barril, bx[i], by[i]);
	}

	textprintf(buffer, font, 200, 5, palette_color[11], "Vidas: %d", vidas);
	draw_sprite(screen, buffer, 0, 0);
}

void crear_barril() {

	if (tiempo_creacion == 40 + delta_tiempo) {

		srand(time(NULL));

		bx[bactual] = 5 * P;
		by[bactual] = 5 * P;
		bdir[bactual] = 3;
		balt[bactual] = 0;
		bactual++;

		tiempo_creacion = 0;
		delta_tiempo = rand() % 30;
	}

	tiempo_creacion++;
}

void destruir_barril() {

	for (int i = 0; i < bactual - 1; i++) {

		bx[i] = bx[i + 1];
		by[i] = by[i + 1];
		bdir[i] = bdir[i + 1];
		balt[i] = balt[i + 1];
	}

	bactual--;
}

void mover_barril() {
	for (int i = 0; i < bactual; i++) {
		if (bdir[i] == 3) {

			if (verificaru(bx[i], by[i] + 3, 24, 20, ' ')) by[i] += 3;
			else {
				int aux = 2;
				while (!verificaru(bx[i], by[i] + aux, 24, 20, ' ')) aux--;
				by[i] += aux;

				if (bx[i] < X*P / 2) bdir[i] = 0;
				else bdir[i] = 1;
			}
		}

		else if (bdir[i] == 0) {

			balt[i]--;
			if (balt[i] == -1) balt[i] = 3;

			bx[i] += 3;
		}

		else if (bdir[i] == 1) {

			balt[i]++;
			if (balt[i] == 4) balt[i] = 0;

			bx[i] -= 3;
		}

		if (verificaru(bx[i], by[i] + 1, 24, 20, ' ')) bdir[i] = 3;

		if (bx[i] < 0) destruir_barril();
	}
}

void aire(int tamx, int tamy) {
	if ((salto || caida) && psalto) {
		if (dir == 0 && px + 30 + 5 < X*P) px += 5;
		else if (px > 0) px -= 5;
	}

	if (salto) {
		if (verificaru(px, py - dsalto, tamx, tamy, ' ', 'h')) {
			py -= dsalto;
		}
		else {
			while (!verificaru(px, py - dsalto, tamx, tamy, ' ', 'h')) dsalto--;
			py -= dsalto;
		}
		dsalto--;
		if (dsalto <= 0) salto = false;
	}
	//caida
	if (verificaru(px, py + 1, tamx, tamy, ' ', 'h') && !salto && !caida && !escalando) {
		caida = true;
		dcaida = 1;
	}
	if (caida) {
		if (verificaru(px, py + dcaida, tamx, tamy, ' ', 'h')) py += dcaida;
		else {
			while (!verificaru(px, py + dcaida, tamx, tamy, ' ', 'h')) dcaida--;
			py += dcaida;
			caida = false;
			psalto = false;
		}
		dcaida++;
	}
}

void mover_pj() {

	if (key[KEY_RIGHT] && !escalando && !salto && !caida) {

		if (px + 30 + 5 < X*P) {
			dir = 0;
			alt++;
			if (alt == 3) alt = 0;

			px += 5;
		}
	}

	if (key[KEY_LEFT] && !escalando && !salto && !caida) {

		if (px - 5 > 0) {
			dir = 1;
			alt++;
			if (alt == 3) alt = 0;

			px -= 5;
		}
	}

	//empezar a escalar
	if (((key[KEY_UP] && verificar(px, py, 30, 32, 'h')) || (key[KEY_DOWN] &&
		verificar(px, py + 1, 30, 32, 'n'))) && !escalando && !salto && !caida) {
		alt = 0;
		dir = 0;
		escalando = true;
	}

	if (key[KEY_UP] && escalando) {
		alt++;
		if (alt == 2) alt = 0;

		py -= 5;
		if (verificaru(px, py, 30, 32, ' ')) escalando = false;
	}

	if (key[KEY_DOWN] && escalando) {
		bool b = false;
		if (verificar(px, py + 5, 30, 32, 'h') || verificar(px, py + 5, 30, 32, 'n')) {
			if (verificar(px, py + 5, 30, 32, 'x') && !verificar(px, py + 5, 30, 32, 'n')) b = true;
			else {
				alt++;
				if (alt == 2) alt = 0;

				py += 5;
			}
		}

		//dejar de escalar
		if (b) {
			int aux = 4;
			while (verificar(px, py + aux, 30, 32, 'x') && aux > 0) aux--;
			py += aux;
			escalando = false;
		}
	}

	//saltar
	if (key[KEY_UP] && !escalando && verificaru(px, py + 31 + 1, 30, 1, 'x', 'n')) {
		dsalto = 8;
		salto = true;
		psalto = true;
	}

	aire(30, 32);
}

void chocar() {
	if (invencible == 0) {
		for (int i = 0; i < bactual; i++) {
			if (verificar_o(px, py, 30, 32, bx[i], by[i], 24, 20)) {
				invencible = 20;
				vidas--;
			}
		}
	}

	invencible--;
	if (invencible < 0) invencible = 0;
}

void game_over() {
	if (vidas == 0) game = 1;
	if (verificar(px, py, 30, 32, 'g')) game = 2;
}

void init();
void deinit();

int main() {
	init();

	buffer = create_bitmap(X*P, Y*P);
	pj = create_bitmap(30, 32);
	barril = create_bitmap(24, 20);

	pjbmp = load_bitmap("pj.bmp", NULL);
	pjclimb = load_bitmap("pjclimb.bmp", NULL);
	escalera = load_bitmap("escalera.bmp", NULL);
	roca = load_bitmap("roca.bmp", NULL);
	princesa = load_bitmap("princesa.bmp", NULL);
	donkey = load_bitmap("donkey.bmp", NULL);
	barrilbmp = load_bitmap("barril.bmp", NULL);

	while (!key[KEY_ESC] && game == 0) {

		crear_barril();
		mover_barril();
		mover_pj();
		chocar();
		game_over();

		dibujar();
		rest(30);
		clear(buffer);
	}

	while (!key[KEY_ESC]) {
		if (game == 1) textprintf(screen, font, 200, 20, palette_color[13], "Has perdido");
		else textprintf(screen, font, 200, 20, palette_color[13], "Has ganado!");
	}

	deinit();
	return 0;
}
END_OF_MAIN()

void init() {
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);
#ifdef _WIN32
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, X*P, Y*P, 0, 0);
#else
	res = set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0);
#endif
	if (res != 0) {
		allegro_message(allegro_error);
		exit(-1);
	}

	install_keyboard();
	/* add other initializations here */
}

void deinit() {
	clear_keybuf();
	/* add other deinitializations here */
}
