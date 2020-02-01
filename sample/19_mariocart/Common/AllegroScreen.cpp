#include "AllegroScreen.h"


#include <allegro.h>

AllegroScreen::AllegroScreen(const ObjectSize &size, bool fullScreen, ColorDepth colorDepth)
{	
	int bpp = colorDepth;

	if (colorDepth == DepthAuto)
		if ((bpp = desktop_color_depth()) == 0)
			bpp = 16;

	set_color_depth(bpp);
#ifdef _WIN32
	if (set_gfx_mode((fullScreen ? GFX_AUTODETECT_FULLSCREEN : GFX_AUTODETECT_WINDOWED), size.width(), size.height(), 0, 0))
#else
	if(set_gfx_mode(GFX_AUTODETECT, size.width(), size.height(), 0, 0))
#endif
		throw std::string("Could not set color depth");

	mBitmap = create_bitmap(SCREEN_W, SCREEN_H);
}
