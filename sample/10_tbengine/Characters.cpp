#include "Characters.h"
#include "Textures.h"

Hero::Hero(int x, int y, BITMAP *outfit_maska, BITMAP* outfit_wlosy, BITMAP* outfit_kostium)
{
	this->x = x;
	this->y = y;
	this->ground = true;
	this->jump = false;
	this->p_y = SCREEN_H;
	this->lewo = false;
	this->prawo = false;
	this->anim = false;
	this->acceleration = 1;
	BMP = load_bitmap("BMP/hero.bmp", NULL);
	masked_blit(outfit_maska, this->BMP, 0, 0, 0, 0, BMP->w, BMP->h);
	masked_blit(outfit_wlosy, this->BMP, 0, 0, 0, 0, BMP->w, BMP->h);
	masked_blit(outfit_kostium, this->BMP, 0, 0, 0, 0, BMP->w, BMP->h);
}

void Hero::draw(BITMAP* buffor, int timer)
{
	if (!ground && !anim)
	{
		if (p_y < y&&prawo)
			masked_blit(this->BMP, buffor, 50, 0, x, y, BMP->w / 3, BMP->h / 5);
		else if (p_y > y&&prawo)
			masked_blit(this->BMP, buffor, 100, 0, x, y, BMP->w / 3, BMP->h / 5);
		else if (p_y < y&&lewo)
			masked_blit(this->BMP, buffor, 50, 240, x, y, BMP->w / 3, BMP->h / 5);
		else
			masked_blit(this->BMP, buffor, 100, 240, x, y, BMP->w / 3, BMP->h / 5);
	}
	else if (!anim)
	{
		if (prawo)
			masked_blit(this->BMP, buffor, klatka, 80, x, y, BMP->w / 3, BMP->h / 5);
		else if (lewo)
			masked_blit(this->BMP, buffor, klatka, 160, x, y, BMP->w / 3, BMP->h / 5);
		else
			masked_blit(this->BMP, buffor, 0, 0, x, y, BMP->w / 3, BMP->h / 5);
	}
	else
	{
		masked_blit(this->BMP, buffor, klatka, 320, x, y, BMP->w / 3, BMP->h / 5);
	}
	if (key[KEY_E])
	{
		//textprintf_centre_ex(screen, font, 500, 500, makecol(255, 255, 255), -1, "%d", mx);
		//textprintf_centre_ex(screen, font, 500, 500, makecol(255, 255, 255), -1, "%d", hero->x);
		// putpixel(buffor,((x+40)+i*(x-mx)),(y+40)+i*(y-my),makecol(255,0,0));
		if (x >= CAMERA_OFFSET && x <= 2100 - SCREEN_W + CAMERA_OFFSET)
			line(buffor, x + 40, y + 40, mouse_x + x - CAMERA_OFFSET, mouse_y, makecol(255, 35, 0));
		else if (x <= CAMERA_OFFSET)                                              //LEWA STRONA PLANSZY
			line(buffor, x + 40, y + 40, mouse_x, mouse_y, makecol(255, 35, 0));
		else
		{
			for (int i = 0; i < 20; i++)
				line(buffor, x + 40, y + 40, mouse_x + x + (i * 5) - CAMERA_OFFSET, mouse_y, makecol(255, 35, 0));
		}
		poll_mouse();
	}
}

void Hero::move(int jump_height, int timer, bool &end)
{
	//    if(!key[KEY_A]&&!key[KEY_D]&&!jump)
	//    {
	//        lewo = false;
	//        prawo = false;
	//    }
	if (key[KEY_A])
	{
		x -= 3;
		lewo = true;
		prawo = false;
		klatka = timer * 50;
	}
	if (key[KEY_D])
	{
		x += 3;
		prawo = true;
		lewo = false;
		klatka = timer * 50;
	}
	if (key[KEY_W] && !jump && ground)
	{
		acceleration = 3;
		jump = true;
		ground = false;
		p_y = y - jump_height;
		if (p_y < 0)
			p_y = 0;
	}
	if (key[KEY_S])
	{
		ground = false;
	}

	if (key[KEY_Q])
	{
		anim = true;
		klatka = timer * 50;
	}
	if (anim && !key[KEY_Q])
	{
		if (timer > 1)
			anim = false;
	}
	if (jump)
	{
		if (acceleration < 5)
			acceleration += 0.2;
		if (y >= p_y)
			y -= acceleration;
		else
		{
			jump = false;
			p_y = SCREEN_H;
		}
	}
	if (!ground && !jump)
	{
		y += acceleration;
		acceleration += 0.2;
	}
	if (key[KEY_R])
	{
		x = 4050;
		y = -50;
		acceleration = 3;
	}
	if (y > SCREEN_H)
		end = true;
	Ground::gravity_blocks = 0;
}
void Hero::attack(std::vector <Bullet> &bullets, volatile int &rest)
{
	if (key[KEY_SPACE] && rest > 5)
	{
		Bullet *tmp = new Bullet(this->x, this->y, this->lewo);
		bullets.push_back(*tmp);
		rest = 0;
	}
}
//************************
Bullet::Bullet(int x, int y, bool direction)
{
	this->direction = direction;
	if (direction)
		this->x = x;
	else
		this->x = x + 40;
	this->y = y + 30;
	this->range = x + 350;
}

void Bullet::draw(BITMAP *buffor)
{
	circlefill(buffor, x, y, 5, makecol(255, 255, 0));
}
void Bullet::move()
{
	if (direction)
		x -= 5;
	else
		x += 5;
}
int Bullet::del()
{
	if (x > range || x < range - 700)
		return 1;
	return 0;
}

Enemy::Enemy(int x, int y, int xmin, int xmax)
{
	this->x = x;
	this->y = y;
	this->ymax = y - 81;
	this->ymin = y + 81;
	this->xmin = xmin;
	this->xmax = xmax;
	this->hp = 2;
	this->dest = false;
	BMP = load_bitmap("BMP/enemy.bmp", NULL);
	init = false;
	all = false;
	this->direction = true;
}

void Enemy::draw(BITMAP* buffor)
{
	if (init)
		masked_blit(this->BMP, buffor, 0, 0, x, y, BMP->w, BMP->h);
}

void Enemy::ini(int x)
{
	if (x > this->x - 250)
		init = true;
}

void Enemy::move()
{
	if (hp == 2)
	{
		if (init && !all)
		{
			y -= 3;
			if (y <= ymax)
				all = true;
		}
		else if (all)
		{
			if (!direction)
			{
				x -= 3;
				if (xmin > x)
				{
					swap_bmp();
					direction = true;
				}
			}
			else if (direction)
			{
				x += 3;
				if (xmax < x)
				{
					swap_bmp();
					direction = false;
				}
			}
		}
	}
	if (hp == 1)
	{
		if (init && !all)
		{
			y -= 3;
			if (y <= ymax)
				all = true;
		}
		else if (all)
		{
			if (!direction)
			{
				x -= 5;
				if (xmin > x)
				{
					swap_bmp();
					direction = true;
				}
			}
			else if (direction)
			{
				x += 5;
				if (xmax < x)
				{
					swap_bmp();
					direction = false;
				}
			}
		}
	}
}

void Enemy::swap_bmp()
{
	BITMAP *tmp = create_bitmap(this->BMP->w, this->BMP->h);
	clear_to_color(tmp, makecol(255, 0, 255));
	for (int i = 0; i <= this->BMP->w - 1; i++)
	{
		for (int j = 0; j <= this->BMP->h - 1; j++)
		{
			putpixel(tmp, this->BMP->w - i, j, getpixel(BMP, i, j));
		}
	}
	this->BMP = tmp;
}

int Enemy::check_bullet(std::vector<Bullet> &bullets)
{
	for (std::vector<Bullet>::iterator it = bullets.begin(); it != bullets.end();)
	{
		if (it->x > x && it->x < x + BMP->w  && it->y > y && it->y < y + BMP->h)
		{
			it = bullets.erase(it);
			return 1;
		}
		else
			++it;
	}
	return 0;
}

int Enemy::movedead()
{
	y++;
	if (y > ymin)
		return 1;
	return 0;
}

int Enemy::check_hero(BITMAP* buffor, Hero &h)
{
	if (x < h.x + (h.BMP->w / 3) - 30 && x + BMP->w > h.x + 30 && y <h.y + (h.BMP->h / 5) - 30 && y + BMP->h >h.y)
	{
		for (int i = 0; i < BMP->w; i++)
		{
			for (int j = 0; j < BMP->h; j++)
			{
				int p = getpixel(h.BMP, x + i, y + j);
				int x = getpixel(BMP, i, j);
				if (p != x && p != makecol(255, 0, 255))
					return 1;
			}
		}
	}
	return 0;
}

Boss_green::Boss_green(int x, int y)
{
	this->x = x;
	this->y = y;
	this->hp = 25;
	this->xmin = x - 100;
	this->xmax = SCREEN_W - 100;
	this->init = true;
	this->direction = false;
	this->BMP = load_bitmap("BMP/boss1.bmp", NULL);
}

void Boss_green::draw(BITMAP* buffor)
{
	masked_blit(BMP, buffor, 0, 0, x, y, BMP->w, BMP->h);
	rect(buffor, 200, 100, 600, 150, makecol(255, 255, 255));
	for (int i = 0; i <= hp; i++)
		rectfill(buffor, 200, 100, 200 + (16 * i), 150, makecol(255, 127, 127));
}

void Boss_green::move()
{
	if (direction)
	{
		x -= 3;
		if (x <= xmin)
			this->direction = false;
	}
	else
	{
		x += 3;
		if (x >= xmax)
		{
			this->direction = true;
		}
	}
}

void Boss_green::attack(std::vector<Enemy> &enemies)
{
	if (hp == 23 && init)
	{
		for (int i = 0; i < 5; i++)
		{
			Enemy *en = new Enemy(0 - (i * 200), 470, 0, SCREEN_W);
			enemies.push_back(*en);
		}
		init = false;
	}
	if (hp == 20 && init)
	{
		for (int i = 0; i < 5; i++)
		{
			Enemy *en = new Enemy(SCREEN_W + (i * 200), 470, 0, SCREEN_W + (i * 200) + 1);
			en->init = true;
			enemies.push_back(*en);
		}
		init = false;
	}
	if (hp == 15 && init)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				Enemy *en = new Enemy(0 - (i * 50) - (400 * j), 470, 0, SCREEN_W);
				en->init = true;
				enemies.push_back(*en);
			}
		}
		init = false;
	}
	if (hp == 10 && init)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				Enemy *en = new Enemy(SCREEN_W + (i * 50) + (400 * j), 470, 0, (SCREEN_W + (i * 200) + (400 * j)));
				en->init = true;
				enemies.push_back(*en);
			}
		}
		init = false;
	}
	if (hp == 5 && init)
	{
		for (int i = 0; i < 5; i++)
		{
			Enemy *en = new Enemy(0 - (i * 200), 470, 0, SCREEN_W);
			en->init = true;
			enemies.push_back(*en);
		}
		for (int i = 0; i < 5; i++)
		{
			Enemy *en = new Enemy(SCREEN_W + (i * 200), 470, 0, SCREEN_W + (i * 200) + 1);
			en->init = true;
			enemies.push_back(*en);
		}
		init = false;
	}
	if (hp != 23 && hp != 20 && hp != 15 && hp != 10 && hp != 5)
	{
		init = true;
	}
}

Enemy::Enemy(int x, int y, int xmin, int xmax, bool ice)
{
	this->x = x;
	this->y = y;
	this->ymax = y - 81;
	this->ymin = y + 81;
	this->xmin = xmin;
	this->xmax = xmax;
	this->hp = 2;
	this->dest = false;
	BMP = load_bitmap("BMP/yeti.bmp", NULL);
	init = false;
	all = false;
	this->direction = true;
}


Ice_Ball::Ice_Ball(int x, int y, int r, int xmin, int xmax)
{
	this->x = x;
	this->y = y;
	this->r = r;
	this->xmin = xmin;
	this->xmax = xmax;
	this->ymax = y + r + r;
}

void Ice_Ball::draw(BITMAP *buffor)
{
	circlefill(buffor, x, y, 100, makecol(155, 155, 155));
}

void Ice_Ball::move()
{
	if (x > xmin)
		x -= 3;
	else
	{
		y += 3;
		if (y > ymax)
		{
			x = xmax;
			y = y - r - r;
		}
	}
}

int Ice_Ball::check_hero(Hero& h)
{
	if (x - 70 < h.x + (h.BMP->w / 3) && x + 70 > h.x && y - 70 < h.y + (h.BMP->h / 5) && y + 70 > h.y)
		return 1;
	return 0;
}

Button::Button(int x, int y)
{
	this->x = x;
	this->y = y;
	this->state = false;
}
void Button::draw(BITMAP *buffor)
{
	rectfill(buffor, x, y, x + 20, y + 50, makecol(155, 155, 155));
	if (!state)
		circlefill(buffor, x + 10, y + 25, 5, makecol(240, 20, 20));
	else
		circlefill(buffor, x + 10, y + 25, 5, makecol(20, 240, 20));
}
void Button::active(Hero& h, std::vector<Enemy> &enemies)
{
	if (!state)
		if (x < h.x + (h.BMP->w / 3) - 30 && x + 20 > h.x + 30 && y <h.y + (h.BMP->h / 5) - 30 && y + 50 >h.y && key[KEY_Q])
		{
			Enemy *en1 = new Enemy(3250, 130, 3250, 3900, true);
			Enemy *en2 = new Enemy(2800, 400, 2850, 3250, true);
			Enemy *en3 = new Enemy(2170, 400, 2170, 2500, true);
			Enemy *en4 = new Enemy(1520, 400, 1520, 1900, true);
			Enemy *en5 = new Enemy(650, 400, 450, 940, true);
			Enemy *en6 = new Enemy(450, 400, 450, 940, true);

			en1->hp = 1;
			en1->BMP = load_bitmap("BMP/yeti2.bmp", NULL);

			en5->hp = 1;
			en5->BMP = load_bitmap("BMP/yeti2.bmp", NULL);

			en6->hp = 1;
			en6->BMP = load_bitmap("BMP/yeti2.bmp", NULL);


			enemies.push_back(*en1);
			enemies.push_back(*en2);
			enemies.push_back(*en3);
			enemies.push_back(*en4);
			enemies.push_back(*en5);
			enemies.push_back(*en6);
			state = true;
		}
}

Boss_ice::Boss_ice(int x, int y)
{
	this->x = x;
	this->y = y;
	this->hp = 40;
	this->px = 350;
	this->py = 300;
	this->state = 0;
	this->init = true;
	BMP = load_bitmap("BMP/boss2.bmp", NULL);
}
void Boss_ice::draw(BITMAP* buffor)
{
	masked_blit(BMP, buffor, 0, 0, x, y, BMP->w, BMP->h);
	rect(buffor, 200, 20, 600, 70, makecol(255, 255, 255));
	for (int i = 0; i <= hp; i++)
		rectfill(buffor, 200, 20, 200 + (10 * i), 70, makecol(255, 127, 127));
}
int Boss_ice::check_hero(Hero& h)
{
	if (x + 30 < h.x + (h.BMP->w / 3) && x + 70 > h.x && y + 10 < h.y + (h.BMP->h / 5) && y + 80 > h.y)
		return 1;
	return 0;
}

void Boss_ice::move(int speed)
{
	if (x < px)
		x += speed;
	if (x > px)
		x -= speed;
	if (y < py)
		y += speed;
	if (y > py)
		y -= speed;
}

void Boss_ice::attack(std::vector <Enemy> &enemies, Hero &h)
{
	if (hp < 38 && hp>33)
	{
		if (state == 0)
		{
			px = 200;
			py = 450;
		}
		else if (state == 1)
		{
			px = 350;
			py = 300;
		}
		else if (state == 2)
		{
			px = 350;
			py = 500;
		}
		else if (state == 3)
		{
			px = 350;
			py = 300;
		}
		else if (state == 4)
		{
			px = 500;
			py = 450;
		}
		else if (state == 5)
		{
			px = 350;
			py = 300;
		}
		if ((px <= x || px <= x + 1 || px <= x + 2) && (py == y || py == y + 1 || py == y + 2 || py == y + 3 || py == y + 4))
			state++;
		if (state > 5)
			state = 0;
		move(3);
	}
	if (hp < 34 && hp>29)
	{
		if (state == 0)
		{
			px = 0;
			py = 100;
		}
		if (state == 1)
		{
			px = 700;
			py = 100;
		}
		if (x == 0 || x == 1 || x == 2)
			state = 1;
		else if (x == 700 || x == 699 || x == 698)
			state = 0;
		if (state > 1)
			state = 0;
		move(3);
	}
	if (hp < 30 && hp>28)
	{
		px = 350;
		py = 300;
		if (init)
			for (int i = 0; i < 7; i++)
			{
				Enemy *en = new Enemy(1200 + (i * 250), 550, 0, SCREEN_W, true);
				if (i % 3 == 0)
				{
					en = new Enemy(-1200 - (i * 250), 550, 0, SCREEN_W, true);
					en->hp = 1;
				}
				en->init = true;
				enemies.push_back(*en);
			}
		init = false;
		move(3);
	}
	if (hp < 29 && hp>25)
	{
		init = true;
		if (state == 0)
		{
			px = 0;
			py = 100;
		}
		if (state == 1)
		{
			px = 700;
			py = 100;
		}
		if (state == 2)
		{
			px = 700;
			py = 200;
		}
		if (state == 3)
		{
			px = 100;
			py = 200;
		}
		if (px == x&&py == y)
			state++;
		if (state > 3)
			state = 0;
		move(5);
	}
	if (hp < 26 && hp>19)
	{
		if (state == 0)
		{
			px = 0;
			py = 450;
		}
		if (state == 1)
		{
			px = 700;
			py = 450;
		}
		if (px == x&&py == y)
			state++;
		if (state > 2)
			state = 0;
		move(5);
	}
	if (hp == 19)
	{
		px = 350;
		py = 300;
		if (init)
			for (int i = 0; i < 3; i++)
			{
				Enemy *en = new Enemy(1200 + (i * 50), 550, 0, SCREEN_W, true);
				en->init = true;
				enemies.push_back(*en);
			}
		init = false;
		move(3);
	}
	if (hp < 19 && hp> 12)
	{
		srand(time(NULL));
		init = true;
		if (state == 0)
		{
			px = rand() % 3 * 200 + 200;
			py = 150;
		}
		if (state == 1)
		{
			px = rand() % 3 * 200 + 200;
			py = 450;
		}
		if (px == x&&py == y)
			state++;
		if (state > 2)
			state = 0;
		move(5);
	}
	if (hp < 13 && hp> 5)
	{
		if (state == 0)
		{
			px = SCREEN_W - h.x;
			py = h.y;
		}

		if (px == x&&py == y)
			state++;
		if (state > 0)
			state = 0;
		move(5);
	}
	if (hp < 6)
	{
		if (state == 0)
		{
			px = SCREEN_W - h.x;
			py = h.y;
		}

		if (px == x&&py == y)
			state++;
		if (state > 0)
			state = 0;
		move(5);
	}
}
void Boss_ice::add_red()
{

	BITMAP *tmp = BMP;
	for (int i = 0; i < 104; i++)
	{
		for (int j = 0; j < 104; j++)
		{
			int pix = getpixel(tmp, i, j);
			if (pix == getpixel(tmp, 50, 15))
			{
				putpixel(tmp, i, j, makecol(255, 255 - (abs(hp - 40) * 5), 255 - (abs(hp - 40) * 5)));
			}
		}
		blit(tmp, BMP, 0, 0, 0, 0, 104, 104);
	}
}

Cannon::Cannon(int x, int y, bool dir)
{
	this->x = x;
	this->y = y;
	this->bx = x;
	this->by = y + 10;
	this->dir = dir;
	this->BMP = load_bitmap("BMP/cannon.bmp", NULL);
}

void Cannon::draw(BITMAP *buffor)
{
	masked_blit(BMP, buffor, 0, 0, x, y, BMP->w, BMP->h);
	circlefill(buffor, bx, by, 5, makecol(255, 0, 0));
}

int Cannon::attack(Hero &h)
{
	if (!dir) {
		bx -= 3;
		if (bx < x - 500)
			bx = x;
	}
	else {
		by -= 3;
		if (by < y - 500)
			by = y;
	}
	if (h.x + h.BMP->w / 3 > bx   &&  h.x<bx + BMP->w / 3 - 20 && h.y + (h.BMP->h / 5)>by    && h.y < by)
		return 1;
	return 0;
}

Enemy::Enemy(int x, int y, int xmin, int xmax, bool ice, bool fire)
{
	this->x = x;
	this->y = y;
	this->ymax = y - 81;
	this->ymin = y + 81;
	this->xmin = xmin;
	this->xmax = xmax;
	this->hp = 2;
	this->dest = false;
	BMP = load_bitmap("BMP/orc.bmp", NULL);
	init = false;
	all = false;
	att = false;
	this->direction = true;
}

void Enemy::attack()
{
	this->bx = x - 20;
	this->by = y + 30;
	this->att = true;
}
int Enemy::moveattack(BITMAP * buffor, Hero h)
{
	if (att)
	{
		circlefill(buffor, bx, by, 5, makecol(255, 0, 0));
		bx -= 7;
		if (bx <= 0)
			att = false;
		else if (h.x + h.BMP->w / 3 > bx + 30 && h.x<bx + BMP->w / 3 && h.y + (h.BMP->h / 5)>by  && h.y < by)
			return 1;
	}
	return 0;
}

Boss_fire::Boss_fire(int x, int y)
{
	this->x = x;
	this->y = y;
	this->hp = 40;
	this->px = 350;
	this->py = 300;
	this->state = 0;
	this->init = true;
	BMP = load_bitmap("BMP/boss3.bmp", NULL);
}
void Boss_fire::draw(BITMAP* buffor)
{
	masked_blit(BMP, buffor, 0, 0, x, y, BMP->w, BMP->h);
	rect(buffor, 200, 20, 600, 70, makecol(255, 255, 255));
	for (int i = 0; i <= hp; i++)
		rectfill(buffor, 200, 20, 200 + (10 * i), 70, makecol(255, 127, 127));
}
int Boss_fire::check_hero(Hero& h)
{
	if (x + 30 < h.x + (h.BMP->w / 3) && x + 70 > h.x && y + 10 < h.y + (h.BMP->h / 5) && y + 80 > h.y)
		return 1;
	return 0;
}

void Boss_fire::move(int speed)
{
	if (x < px)
		x += speed;
	if (x > px)
		x -= speed;
	if (y < py)
		y += speed;
	if (y > py)
		y -= speed;
}

void Boss_fire::attack(std::vector <Enemy> &enemies, Hero &h)
{
	if (hp < 38 && hp>33)
	{
		if (state == 0)
		{
			px = 200;
			py = 450;
		}
		else if (state == 1)
		{
			px = 350;
			py = 300;
		}
		else if (state == 2)
		{
			px = 350;
			py = 500;
		}
		else if (state == 3)
		{
			px = 350;
			py = 300;
		}
		else if (state == 4)
		{
			px = 500;
			py = 450;
		}
		else if (state == 5)
		{
			px = 350;
			py = 300;
		}
		if (px == x&&py == y)
			state++;
		if (state > 5)
			state = 0;
		move(1);
	}
	if (hp < 34 && hp>29)
	{
		if (state == 0)
		{
			px = 0;
			py = 100;
		}
		if (state == 1)
		{
			px = 700;
			py = 100;
		}
		if (px == x&&py == y)
			state++;
		if (state > 2)
			state = 0;
		move(1);
	}
	if (hp < 30 && hp>28)
	{
		px = 350;
		py = 300;
		if (init)
			for (int i = 0; i < 7; i++)
			{
				Enemy *en = new Enemy(1200 + (i * 250), 550, 0, SCREEN_W, true);
				if (i % 3 == 0)
				{
					en = new Enemy(-1200 - (i * 250), 550, 0, SCREEN_W, true);
					en->hp = 1;
				}
				en->init = true;
				enemies.push_back(*en);
			}
		init = false;
		move(1);
	}
	if (hp < 29 && hp>25)
	{
		init = true;
		if (state == 0)
		{
			px = 0;
			py = 100;
		}
		if (state == 1)
		{
			px = 700;
			py = 100;
		}
		if (state == 2)
		{
			px = 700;
			py = 200;
		}
		if (state == 3)
		{
			px = 100;
			py = 200;
		}
		if (px == x&&py == y)
			state++;
		if (state > 3)
			state = 0;
		move(2);
	}
	if (hp < 26 && hp>19)
	{
		if (state == 0)
		{
			px = 0;
			py = 450;
		}
		if (state == 1)
		{
			px = 700;
			py = 450;
		}
		if (px == x&&py == y)
			state++;
		if (state > 2)
			state = 0;
		move(2);
	}
	if (hp == 19)
	{
		px = 350;
		py = 300;
		if (init)
			for (int i = 0; i < 3; i++)
			{
				Enemy *en = new Enemy(1200 + (i * 50), 550, 0, SCREEN_W, true);
				en->init = true;
				enemies.push_back(*en);
			}
		init = false;
		move(1);
	}
	if (hp < 19 && hp> 12)
	{
		srand(time(NULL));
		init = true;
		if (state == 0)
		{
			px = rand() % 3 * 200 + 200;
			py = 150;
		}
		if (state == 1)
		{
			px = rand() % 3 * 200 + 200;
			py = 450;
		}
		if (px == x&&py == y)
			state++;
		if (state > 2)
			state = 0;
		move(2);
	}
	if (hp < 13 && hp> 5)
	{
		if (state == 0)
		{
			px = SCREEN_W - h.x;
			py = h.y;
		}

		if (px == x&&py == y)
			state++;
		if (state > 0)
			state = 0;
		move(2);
	}
	if (hp < 6)          //  TODO
	{
		if (state == 0)
		{
			px = SCREEN_W - h.x;
			py = h.y;
		}

		if (px == x&&py == y)
			state++;
		if (state > 0)
			state = 0;
		move(2);
	}
}
