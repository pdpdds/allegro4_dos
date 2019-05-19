#include "hardware.h"

extern int Retardo;

extern Configuracion *Config;

Hardware::Hardware()
{
	allegro_init();
	install_keyboard();
	install_timer();
	detect_midi_driver(4);
	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
	set_volume(128, 128);
	set_hardware_volume(128, 128);
	srand(time(0));
	Izquierda = KEY_LEFT;
	Derecha = KEY_RIGHT;
	Arriba = KEY_UP;
	Abajo = KEY_DOWN;
	Selecc = KEY_ENTER;
	Abort = KEY_ESC;
	set_color_depth(CDEPTH);
	Buffer = NULL;
	Fondo = NULL;
	set_window_title("Horacio Esquiador Remake - (c) Panik Software");
}

Hardware::~Hardware()
{
	DeInit();
	allegro_exit();
}

int Hardware::InitGraph() {

	if (Config->VerWindowed()) ModoGraf = GFX_AUTODETECT_WINDOWED;
	else
		ModoGraf = GFX_AUTODETECT_FULLSCREEN;

#ifndef _WIN32
	ModoGraf = GFX_AUTODETECT;
#endif

	if (set_gfx_mode(ModoGraf, ANCHOPANTALLA, ALTOPANTALLA, 0, 0) != 0) {
		allegro_message("No se puede iniciar modo gr?ico seleccionado\nSe va a intentar modo GFX_SAFE");
		if (set_gfx_mode(GFX_SAFE, ANCHOPANTALLA, ALTOPANTALLA, 0, 0) != 0) {
			set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
			allegro_message(
				"Incapaz de entrar a modo grafico\n%s\n",
				allegro_error);
			return 1;
		}
	}
	else
		if ((is_windowed_mode() && !Config->VerWindowed()) ||
			(!is_windowed_mode() && Config->VerWindowed()))
			Config->ToggleWindowed();
	return 0;
};

int Hardware::Init() {

	SpeccyColor[0] = makecol(0, 0, 0);
	SpeccyColor[1] = makecol(0, 0, 255);
	SpeccyColor[2] = makecol(255, 0, 0);
	SpeccyColor[3] = makecol(255, 0, 255);
	SpeccyColor[4] = makecol(0, 255, 0);
	SpeccyColor[5] = makecol(0, 255, 255);
	SpeccyColor[6] = makecol(255, 255, 0);
	SpeccyColor[7] = makecol(255, 255, 255);

	Original = font;

	Archivo = load_datafile("datos.dat");
	if (!Archivo) {
		allegro_message("                   ERROR FATAL\n No se encuentra el archivo DATOS.DAT");
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		return 1;
	}

	Buffer = create_bitmap_ex(CDEPTH, 400 + ANCHOPANTALLA, 400 + ALTOPANTALLA);
	Fondo = create_bitmap_ex(CDEPTH, ANCHOPANTALLA, ALTOPANTALLA);
	return 0;
}

void Hardware::DeInit() {
	destroy_bitmap(Buffer);
	destroy_bitmap(Fondo);
	unload_datafile(Archivo);
}

void Hardware::TextOut(const char *s, int x, int y, int color, int fondo) {
	if (fondo >= 0) fondo = SpeccyColor[fondo];
	if (color >= 0) color = SpeccyColor[color];
	textout_ex(Buffer, font, s, 200 + x, 200 + y, color, fondo);
}

void Hardware::TextOutCentre(const char *s, int y, int color, int fondo) {
	int x;
	x = int(ANCHOPANTALLA / 2);
	if (fondo >= 0) fondo = SpeccyColor[fondo];
	if (color >= 0) color = SpeccyColor[color];
	textout_centre_ex(Buffer, font, s, 200 + x, 200 + y, color, fondo);
}

void Hardware::TextOutCentre_ex(const char *s, int x, int y, int color, int fondo) {
	if (fondo >= 0) fondo = SpeccyColor[fondo];
	if (color >= 0) color = SpeccyColor[color];
	textout_centre_ex(Buffer, font, s, 200 + x, 200 + y, color, fondo);
}

void Hardware::SetFont(char *s1, char *s2) {
	FONT *Fuente;
	PALETTE pal;
	char *names[] = { s1, s2 };
	Fuente = load_dat_font("datos.dat", pal, names);
	set_palette(pal);
	/*   Si la paleta de la fuente no es correcta, necesitas repetir esta l?ea:
		 If the font palette is not correct, you must use this line again:
	*/
	Fuente = load_dat_font("datos.dat", pal, names); /**/
	font = Fuente;
}

void Hardware::SetFont(int Numero) {
	font = (FONT*)Archivo[Numero].dat;
}

void Hardware::DelFont() {
	font = Original;
}

void Hardware::cls(int color) {
	clear_to_color(Buffer, SpeccyColor[color]);
}

void Hardware::DrawFondoBuffer(int x, int y, int LongX, int LongY, int xo, int yo) {
	blit(Fondo, Buffer, xo, yo, 200 + x, 200 + y, LongX, LongY);
}

void Hardware::VuelcaBuffer() {
	blit(Buffer, screen, 200, 200, 0, 0, ANCHOPANTALLA, ALTOPANTALLA);
}

void Hardware::RectFill(int x1, int y1, int x2, int y2, int color) {
	rectfill(Buffer, 200 + x1, 200 + y1, 200 + x2, 200 + y2, SpeccyColor[color]);
}

void Hardware::RectFill_ex(int x1, int y1, int x2, int y2, int color) {
	rectfill(Buffer, 200 + x1, 200 + y1, 200 + x2, 200 + y2, color);
}

void Hardware::SetFondo(int NumFondo) {
	BITMAP *Temp;
	Temp = (BITMAP*)Archivo[NumFondo].dat;
	blit(Temp, Fondo, 0, 0, 0, 0, Temp->w, Temp->h);
}

void Hardware::CopyFondo() {
	blit(Buffer, Fondo, 200, 200, 0, 0, ANCHOPANTALLA, ALTOPANTALLA);
}

void Hardware::ClrFondo(int Color) {
	clear_to_color(Fondo, SpeccyColor[Color]);
}

void Hardware::Borra(int x, int y, int LongX, int LongY) {
	blit(Fondo, Buffer, x, y, 200 + x, 200 + y, LongX, LongY);
}

void Hardware::Dibuja(int x, int y, void* Spr) {
	draw_compiled_sprite(Buffer, (COMPILED_SPRITE *)Spr, 200 + x, 200 + y);
}

int Hardware::Tecla() {
	if (!keypressed()) return -1;
	int TeclaPulsada;
	TeclaPulsada = readkey() >> 8;
	if (TeclaPulsada == Arriba) return ARRIBA;
	if (TeclaPulsada == Abajo) return ABAJO;
	if (TeclaPulsada == Izquierda) return IZQUIERDA;
	if (TeclaPulsada == Derecha) return DERECHA;
	if (TeclaPulsada == Selecc) return SELECC;
	if (TeclaPulsada == Abort) return ABORTK;
	return -1;
}

int Hardware::VerTecla() {
	if (!keypressed()) return -1;
	return readkey();
}

const int Hardware::VerTecla(int Sentido) {
	switch (Sentido) {
	case ARRIBA:
		return Arriba;
	case ABAJO:
		return Abajo;
	case IZQUIERDA:
		return Izquierda;
	case DERECHA:
		return Derecha;
	case SELECC:
		return Selecc;
	case ABORTK:
		return Abort;
	}
}

void Hardware::GrabarTecla(int Sentido, int Tecla) {
	switch (Sentido) {
	case ARRIBA:
		Arriba = Tecla >> 8;
		break;
	case ABAJO:
		Abajo = Tecla >> 8;
		break;
	case IZQUIERDA:
		Izquierda = Tecla >> 8;
		break;
	case DERECHA:
		Derecha = Tecla >> 8;
		break;
	case SELECC:
		Selecc = Tecla >> 8;
		break;
	case ABORTK:
		Abort = Tecla >> 8;
		break;
	}
}

void Hardware::EsperaTecla() {
	ClearKeyBuf();
	readkey();
}

void Hardware::Rest(int Ms) {
	rest(Ms);
}

int Hardware::TextHeight() {
	return text_height(font);
}

int Hardware::TextLength(char *String) {
	return text_length(font, String);
}

void Hardware::ClearKeyBuf() {
	clear_keybuf();
}

bool Hardware::Esc() {
	return key[KEY_ESC];
}

void Hardware::Interzona(int sentido) {

	int n, m;
	if (sentido > 0) {
		for (n = 0; n < 8; n++)
			for (m = ALTOPANTALLA / 2; m >= 0; m -= 16) {
				rectfill(screen, 0 + m, 0 + m, ANCHOPANTALLA - m, ALTOPANTALLA - m, SpeccyColor[n]);
				rest(7);
			}
	}
	else {
		for (n = 7; n >= 0; n--)
			for (m = ALTOPANTALLA / 2; m >= 0; m -= 16) {
				if (n == 8) n = 0;
				rectfill(screen, 0 + m, 0 + m, ANCHOPANTALLA - m, ALTOPANTALLA - m, SpeccyColor[n]);
				rest(7);
			}
	}
}

void Hardware::Fundido(int Sentido) {
	int n;

	drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);

	switch (Sentido) {
	case -1:
		for (n = 0; n < 256; n += 15) {
			set_trans_blender(0, 0, 0, n);
			blit(Fondo, Buffer, 0, 0, 200, 200, ANCHOPANTALLA, ALTOPANTALLA);
			rectfill(Buffer, 200, 200, 200 + ANCHOPANTALLA, 200 + ALTOPANTALLA, 0);
			VuelcaBuffer();
		}
		break;
	case 1:
		for (n = 256; n > 0; n -= 15) {
			set_trans_blender(0, 0, 0, n);
			blit(Fondo, Buffer, 0, 0, 200, 200, ANCHOPANTALLA, ALTOPANTALLA);
			rectfill(Buffer, 200, 200, 200 + ANCHOPANTALLA, 200 + ALTOPANTALLA, 0);
			VuelcaBuffer();
		}
		break;
	}
	drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
}

void Hardware::SolidMode() {
	solid_mode();
}

void Hardware::TransMode(int n) {
	drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
	set_trans_blender(0, 0, 0, n);
}
