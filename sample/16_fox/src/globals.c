#include <fox.h>

SAMPLE *sound_effect[20];

MIDI *bgm_midi[10];

BITMAP *fox_bitmap;
BITMAP *fox_bitmaps[4];
BITMAP *fox_light_bitmaps[4];
BITMAP *fox_life_heart_bitmap;
BITMAP *fox_life_small_bitmap;
BITMAP *fox_enemy_life_heart_bitmap;
BITMAP *fox_explosion_bitmaps[10 * 10];
BITMAP *fire_bitmaps[16 * 1];
BITMAP *fox_item_bitmaps[20];
BITMAP *fox_damage_bitmap;

static HashTable *fox_weapon_table;

int fox_life_count;
int fox_energy_count;

int fox_bitmap_index;
int fox_explosion;
int fox_explosion_time;

int boss_explosion;
int boss_explosion_time;

char fox_weapon_name[30];
char fox_weapon_sub_name[30];

int fox_weapon_level = 0;	// ������ �������� ��� ���� ��� �Ŀ��� �����Ѵ�.
int fox_weapon_max_level = FALSE;
int fox_weapon_sub_level = 0;

char fox_twinkling;		// ������ ��� ����
long fox_twinkling_time;	// ������ ������ �ð�

long fox_ghost_time;		// ���� ���� �ð�
long fox_attacked_time;		// �� ����� �浹�� �ð�

long fox_collided_time;		// �� ����� �浹�� �ð�
int fox_collided_index;		// �� ����� �浹�� �ð�

long fox_weapon_time = 0;						// ���� �߻� �ð�
long fox_item_weapon_time = 0;					// ������ ���� �߻� �ð�
long fox_urgency_weapon_time = 0;		// ���� ���� �߻� �ð�

long stage_elapsed_time = 0;				// �������� ����� �ð�

long fox_popup_message_time = 0;
long fox_popup_message_show_time = 0;
int fox_popup_show_emblem = FALSE;

BITMAP *explosion_bitmaps[10 * 10];
BITMAP *boss_explosion_bitmaps[8 * 8];

BITMAP *stage_bg_bitmap;
BITMAP *stage_buf_bitmap;
BITMAP *stage_popup_bitmap;
BITMAP *stage_bitmap;

int fox_explosion_sprite_count;
int enemy_explosion_sprite_count;
int boss_explosion_sprite_count;
int fire_sprite_count;

int fox_x = 0;			// ��ü ���� x��ġ
int fox_y = 0;			// ��ü ���� y��ġ
int fox_restore = 0;	// ��ü ���� ����
float fox_speed = 0.0;
