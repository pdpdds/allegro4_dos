#include "player.h"
#include "helper.h"
#include "mappyal.h"
#include <allegro.h>

#define JUMPIT 1600

#define MAX_PLAYER_SPRITE 12
#define MAX_COLUMN_SPRITE 8

#define PLAYER_WIDTH  50
#define PLAYER_HEIGHT  64

BITMAP* player_image[MAX_PLAYER_SPRITE];
SPRITE* player = 0;
int facing = 1;
int jump = JUMPIT;

void InitPlayer(char* szName)
{
	BITMAP* pPlayer = load_bitmap(szName, NULL);
    int i = 0;
    for (i = 0; i < 8; i++)
        player_image[i] = grabframe(pPlayer, PLAYER_WIDTH, PLAYER_HEIGHT, 0, 0, MAX_COLUMN_SPRITE, i);
    destroy_bitmap(pPlayer);

	player = (SPRITE*)malloc(sizeof(SPRITE));
	player->x = 0;
	player->y = 32;
    player->curframe = 0;
    player->framecount = 0;
    player->framedelay = 6;
    player->maxframe = 7;
    player->width = player_image[0]->w;
    player->height = player_image[0]->h;
}

void UpdatePlayer()
{
    int oldpy = player->y;
    int oldpx = player->x;

    if (key[KEY_RIGHT])
    {
        facing = 1;
        player->x += 2;
        if (++player->framecount > player->framedelay)
        {
            player->framecount = 0;
            if (++player->curframe > player->maxframe)
                player->curframe = 1;
        }
    }
    else if (key[KEY_LEFT])
    {
        facing = 0;
        player->x -= 2;
        if (++player->framecount > player->framedelay)
        {
            player->framecount = 0;
            if (++player->curframe > player->maxframe)
                player->curframe = 1;
        }
    }
    else player->curframe = 0;

    //handle jumping
    if (jump == JUMPIT)
    {
        if (!collided(player->x + player->width / 2,
            player->y + player->height + 5))
            jump = 0;

        if (key[KEY_SPACE])
            jump = 30;
    }
    else
    {
        player->y -= jump / 3;
        jump--;
    }

    if (jump < 0)
    {
        if (collided(player->x + player->width / 2,
            player->y + player->height))
        {
            jump = JUMPIT;
            while (collided(player->x + player->width / 2,
                player->y + player->height))
                player->y -= 2;
        }
    }

	if (player->y >= SCREEN_HEIGHT)
		player->y = SCREEN_HEIGHT;
    if (player->x < 0)
        player->x = 0;
}


void RenderPlayer(BITMAP* buffer, int mapxoff, int mapyoff)
{
    

    //draw the player's sprite
    if (facing)
        draw_sprite(buffer, player_image[player->curframe],
            (player->x - mapxoff), (player->y - mapyoff + 1));
    else
        draw_sprite_h_flip(buffer, player_image[player->curframe],
            (player->x - mapxoff), (player->y - mapyoff));
}