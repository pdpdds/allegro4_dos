#include "Primitives.h"

Pixel::Pixel(int x, int y, int color)
{
	this->x = x;
	this->y = y;
	this->color = color;
}

void Pixel::draw(BITMAP * bmp)
{
	putpixel(bmp, x, y, color);
}

Line::Line(int x1, int y1, int x2, int y2, int color)
{
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
	this->color = color;
}

void Line::draw(BITMAP * bmp)
{
	line(bmp, x1, y1, x2, y2, color);
}

void Line::field()
{
	if (key[KEY_P])
	{
		textprintf_centre_ex(screen, font, (x1 + x2) / 2, (y1 + y2) / 2, makecol(255, 255, 255), -1, "P: 0");
	}
}

void Line::length()
{
	if (key[KEY_L])
	{
		int result = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
		textprintf_centre_ex(screen, font, (x1 + x2) / 2, (y1 + y2) / 2 - 10, makecol(255, 255, 255), -1, "L: %d", result);
	}
}

Circle::Circle(int x, int y, int r, int color)
{
	this->x = x;
	this->y = y;
	this->r = r;
	this->color = color;
	this->scale = 0;
	this->fill = false;
	this->gradient = false;
	this->move = false;
}

void Circle::draw(BITMAP * bmp, bool fill, bool gradient, bool move)
{
	if (move == true)
	{
		if (mouse_b & 1 && (((this->x - mouse_x)*(this->x - mouse_x)) + ((this->y - mouse_y)*(this->y - mouse_y)) <= this->r*this->r))
		{
			this->x = mouse_x;
			this->y = mouse_y;
			if (mouse_needs_poll()) {
				poll_mouse();
			}

		}
		if (mouse_z && (((this->x - mouse_x)*(this->x - mouse_x)) + ((this->y - mouse_y)*(this->y - mouse_y)) <= this->r*this->r))
		{
			if (mouse_z - scale > 0) {
				this->r += 7;
			}
			if (mouse_z - scale < 0) {
				this->r -= 7;
			}
			scale = mouse_z;
		}
	}
	if (fill == false)
	{
		circle(bmp, x, y, r, color);
	}
	if (gradient == true)
	{
		for (int tmpx = x - r; tmpx != x + r + r; tmpx++)
		{
			for (int tmpy = y - r; tmpy != y + r + r; tmpy++)
			{
				if (((x - tmpx)*(x - tmpx)) + ((y - tmpy)*(y - tmpy)) <= r*r)
				{
					int tmp = sqrt(((tmpx - x)*(tmpx - x)) + ((tmpy - y)*(tmpy - y)));
					tmp = this->r - tmp + 18;
					double r1 = (getr(this->color));
					double g1 = (getg(this->color));
					double b1 = (getb(this->color));

					double r = (r1 / 255)*(tmp * 230 / this->r);
					double g = (g1 / 255)*(tmp * 230 / this->r);
					double b = (b1 / 255)*(tmp * 230 / this->r);
					if (r > 255)
						r = 255;
					if (g > 255)
						g = 255;
					if (b > 255)
						b = 255;
					putpixel(bmp, tmpx, tmpy, makecol(r, g, b));
				}
			}
		}
	}
	if (gradient == false)
	{
		circlefill(bmp, x, y, r, color);
	}
}

void Circle::moves(int xx) {
	this->x -= xx;
}
void Circle::field()
{
	if (key[KEY_P])
	{
		int result = 3.14*r*r;
		textprintf_centre_ex(screen, font, x, y - 10, makecol(255, 255, 255), -1, "P: %d", result);
	}
}

void Circle::length()
{
	if (key[KEY_L])
	{
		int result = 3.14 * 2 * r;
		textprintf_centre_ex(screen, font, x, y, makecol(255, 255, 255), -1, "L: %d", result);
	}
}

Rectangle::Rectangle(int x1, int y1, int x2, int y2, int color)
{
	this->x1 = x1;
	this->x2 = x2;
	this->y1 = y1;
	this->y2 = y2;
	this->color = color;
}

void Rectangle::draw(BITMAP * bmp, bool type)
{
	if (type == false)
	{
		rect(bmp, x1, y1, x2, y2, color);
		if (mouse_b & 1 && x1<mouse_x && y1<mouse_y && x2>mouse_x && y2>mouse_y)
		{
			this->x1 = mouse_x;
			this->y1 = mouse_y;
			if (mouse_needs_poll()) {
				poll_mouse();
			}
		}
	}
	else
	{
		rectfill(bmp, x1, y1, x2, y2, color);
	}

}

void Rectangle::field()
{
	if (key[KEY_P])
	{
		int result = sqrt((x2 - x1)*(x2 - x1))*sqrt((y2 - y1)*(y2 - y1));
		textprintf_centre_ex(screen, font, (x1 + x2) / 2, (y1 + y2) / 2, makecol(255, 255, 255), -1, "P: %d", result);
	}
}

void Rectangle::length()
{
	if (key[KEY_L])
	{
		int result = (2 * sqrt((x2 - x1)*(x2 - x1))) + (2 * sqrt((y2 - y1)*(y2 - y1)));
		textprintf_centre_ex(screen, font, (x1 + x2) / 2 - 10, (y1 + y2) / 2 - 10, makecol(255, 255, 255), -1, "L: %d", result);
	}
}

Triangle::Triangle(int x1, int y1, int x2, int y2, int x3, int y3, int color)
{
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
	this->x3 = x3;
	this->y3 = y3;
	this->color = color;
}

void Triangle::draw(BITMAP * bmp)
{
	triangle(bmp, x1, y1, x2, y2, x3, y3, color);
}


void Triangle::length()
{
	if (key[KEY_L])
	{
		int result = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1)) + sqrt((x3 - x2)*(x3 - x2) + (y3 - y2)*(y3 - y2)) + sqrt((x3 - x1)*(x3 - x1) + (y3 - y1)*(y3 - y1));
		textprintf_centre_ex(screen, font, (x1 + x2 + x3) / 3 - 10, (y1 + y2 + y3) / 3 - 10, makecol(255, 255, 255), -1, "L: %d", result);
	}
}

void Triangle::field()
{
	if (key[KEY_P])
	{
		int length = (sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1)) + sqrt((x3 - x2)*(x3 - x2) + (y3 - y2)*(y3 - y2)) + sqrt((x3 - x1)*(x3 - x1) + (y3 - y1)*(y3 - y1))) / 2;
		int ab = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
		int bc = sqrt((x3 - x2)*(x3 - x2) + (y3 - y2)*(y3 - y2));
		int ac = sqrt((x3 - x1)*(x3 - x1) + (y3 - y1)*(y3 - y1));
		int result = sqrt(length*((length - ab)*(length - bc)*(length - ac)));
		textprintf_centre_ex(screen, font, (x1 + x2 + x3) / 3, (y1 + y2 + y3) / 3, makecol(255, 255, 255), -1, "P: %d", result);
	}
}

void Triangle::middle_triangle()
{
	if (key[KEY_S]) {
		int ab = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
		int bc = sqrt((x3 - x2)*(x3 - x2) + (y3 - y2)*(y3 - y2));
		int ac = sqrt((x3 - x1)*(x3 - x1) + (y3 - y1)*(y3 - y1));
		int ab_d_x = (x1 + x2) / 2;
		int ab_d_y = (y1 + y2) / 2;
		int bc_e_x = (x2 + x3) / 2;
		int bc_e_y = (y2 + y3) / 2;
		int ac_f_x = (x1 + x3) / 2;
		int ac_f_y = (y1 + y3) / 2;
		int cd = sqrt((x3 - ab_d_x)*(x3 - ab_d_x) + (y3 - ab_d_y)*(y3 - ab_d_y));
		int ae = sqrt((x1 - bc_e_x)*(x1 - bc_e_x) + (y1 - bc_e_y)*(y1 - bc_e_y));
		int bf = sqrt((x2 - ac_f_x)*(x2 - ac_f_x) + (y2 - ac_f_y)*(y2 - ac_f_y));
		textprintf_centre_ex(screen, font, (x1 + x2 + x3) / 3 - 20, (y1 + y2 + y3) / 3 - 20, makecol(255, 255, 255), -1, "AE: %d BF: %d CD: %d", ae, bf, cd);
	}
}

Square::Square(int x1, int y1, int l, int color)
{
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = 0;
	this->y2 = 0;
	this->l = l;
	this->color = color;
}

void Square::draw(BITMAP * bmp, bool fill)
{
	x2 = this->x1 + this->l;
	y2 = this->y1 + this->l;
	if (fill == false)
	{
		rect(bmp, x1, y1, x2, y2, color);
	}
	else
	{
		rectfill(bmp, x1, y1, x2, y2, color);
	}
}

void Square::field()
{
	if (key[KEY_P])
	{
		int result = sqrt((x2 - x1)*(x2 - x1))*sqrt((x2 - x1)*(x2 - x1));
		textprintf_centre_ex(screen, font, (x1 + x2) / 2, (y1 + y2) / 2, makecol(255, 255, 255), -1, "P: %d", result);
	}
}

void Square::length()
{
	if (key[KEY_L])
	{
		int result = 4 * sqrt((x2 - x1)*(x2 - x1));
		textprintf_centre_ex(screen, font, (x1 + x2) / 2 - 10, (y1 + y2) / 2 - 10, makecol(255, 255, 255), -1, "L: %d", result);
	}
}



