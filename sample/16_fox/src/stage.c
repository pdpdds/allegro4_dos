#include "stage.h"
#include "fox.h"

#ifdef _WIN32
#include <string.h>
#define M_PI 3.141592
#endif

extern OPTION option;
extern void load_data_callback(DATAFILE *dat);

#define MAX_MISSILE_SMOKE	3

void show_popup_message(char *msg, int show_emblem, long show_time);
void hide_popup_message(void);

#define ATTACK_4_WAY 	0
#define ATTACK_N_WAY 	1


DATAFILE *music_data = NULL;
DATAFILE *sound_data = NULL;
DATAFILE *stage_data = NULL;

COLOR_MAP global_trans_table;

int stage_number = -1;
int stage_clear_flag = FALSE;
long stage_clear_time = FALSE;

int fox_damage_flash_index = 0;
long fox_damage_flash_time = 0;

int enemy_unique_index = 0;
int enemy_target_index = 0;
ENEMY *attacked_enemy = NULL;

BITMAP *stage_emblem_bitmap = NULL;

List *scroll_bitmap_list = NULL;
List *scroll_speed_list = NULL;
List *scroll_overlap_list = NULL;
int scroll_offset[10];

BITMAP *missile_smoke[MAX_MISSILE_SMOKE];
BITMAP *guided_missile_smoke[MAX_MISSILE_SMOKE];

long boss_explosion_times[BITMAP_MAX_BOSS_EXPLOSION];
int boss_explosions[BITMAP_MAX_BOSS_EXPLOSION];
int boss_explosion_x[BITMAP_MAX_BOSS_EXPLOSION];
int boss_explosion_y[BITMAP_MAX_BOSS_EXPLOSION];
long boss_explod_begin_time;
long boss_explod_vibration_time;
int boss_explod_vibration_x;
int boss_explod_vibration_y;

long boss_warning_time;
int boss_warning_flag;

char* stage_start_message[6] = {	
	"STAGE 1\nOUTER SPACE",
	"STAGE 2\nUNIVERSE SPACE",
	"STAGE 3\nOUTER SPACE",
	"STAGE 4\nOUTER SPACE",
	"STAGE 5\nOUTER SPACE",	
	"STAGE 6\nOUTER SPACE",
};

/* counters for speed control and frame rate measurement */
// https://github.com/msikma/allegro-4.2.2-xc/blob/master/examples/exupdate.c
volatile int update_count = 0;
volatile int frame_count = 0;
volatile int fps = 0;

int game_paused = FALSE;
int game_end = FALSE;

int fox_weapon_max_power_toggle = -1;
long fox_weapon_max_power_time = -1;

void stage_pause(void);
// -----------------------------------------------------
/* timer callback for controlling the program speed */
void timer_proc(void)
{
	update_count++;
}
END_OF_FUNCTION(timer_proc)

/* timer callback for measuring the frame rate */
void fps_proc(void)
{
	fps = frame_count;
	frame_count = 0;
}
END_OF_FUNCTION(fps_proc)

/* timer callback for measuring stage elapsed time */
void stage_elapsed_time_proc(void)
{
	stage_elapsed_time += 2;
}
END_OF_FUNCTION(stage_elapsed_time_proc)


int process_background_scroll(void)
{
	// https://www.allegro.cc/forums/thread/473433
	blit(stage_bg_bitmap, stage_buf_bitmap, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

	// -------------------------------
	ListIter iter3;
	void *v3;
	list_iter_init(&iter3, scroll_bitmap_list);

	int height = 0;
	while (1)
	{
		if (list_iter_next(&iter3, (void*)&v3) == CC_ITER_END) break;
		BITMAP *b = (BITMAP*)v3;
		height += b->h;
	}

	ListIter iter;
	int *v;
	list_iter_init(&iter, scroll_overlap_list);

	ListIter iter2;
	int *v2;
	list_iter_init(&iter2, scroll_speed_list);

	list_iter_init(&iter3, scroll_bitmap_list);

	int y = SCREEN_H - height;
	int i = 0;
	while (1)
	{
		if (list_iter_next(&iter, (void*)&v) == CC_ITER_END) break;
		int overlap = *v;

		list_iter_next(&iter2, (void*)&v2);
		int speed = *v2;

		list_iter_next(&iter3, (void*)&v3);
		BITMAP *b = (BITMAP*)v3;

		int offset = scroll_offset[i];
		offset += speed;

		if (offset >= b->w - SCREEN_W) offset = 0;
		scroll_offset[i] = offset;

		masked_blit(b, stage_buf_bitmap, offset, 0,
			0, y + overlap, stage_buf_bitmap->w, stage_buf_bitmap->h);
		y += b->h;
		i++;
	}
}

// ���� �̻��� �߻�
void launch_guided_missile(int count, BITMAP *bitmap, int power, int speed)
{
	int i;

	int fox_w = fox_bitmap->w;
	int fox_h = fox_bitmap->h;

	// ���� ����
	int weapon_h = bitmap->h;

	for (i = 0; i<count; i++) {
		WEAPON_GUIDED_MISSILE *weapon = malloc(sizeof(WEAPON_GUIDED_MISSILE));
		weapon->power = power;
		weapon->bitmap = bitmap;
		weapon->speed = speed;
		weapon->accel = 0.1;

		weapon->x = fox_x + (fox_w / 2);
		if (i % 2 == 0) {
			// ���� �̻����� ¦�� ��°�̸� ������ �߻�
			weapon->y = fox_y - weapon_h;
		}
		else {
			// ���� �̻����� Ȧ�� ��°�̸� �Ʒ����� �߻�
			weapon->y = fox_y + fox_h + weapon_h - 4;
		}

		if (list_size(enemy_list) > 0) {
			if ((int)list_size(enemy_list) - 1 < enemy_target_index) {
				enemy_target_index = 0;
			}

			void *v;
			list_get_at(enemy_list, enemy_target_index, &v);
			ENEMY *enemy = (ENEMY*)v;
			// ������ Ŀ���� ������ �ֱ⶧���� zoom �Ϸ�� ���鸸 ����
			if (enemy->zoom_done == TRUE) {
				weapon->target_enemy = enemy->number;
			}
			else {
				weapon->target_enemy = -1;
			}
			enemy_target_index++;
		}

		weapon->launch_time = stage_elapsed_time;

		weapon->smoke_begin = FALSE;
		weapon->smoke_index = 0;
		weapon->smoke_time = stage_elapsed_time;

		// ���� ����
		weapon->angle = 0.0;
		weapon->vx = weapon->speed;
		weapon->vy = 0;

		list_add(fox_guided_missile_list, weapon);
	}
}

// �̻��� �߻�
void launch_missile(int count, BITMAP *bitmap, int power, int speed)
{
	int i;

	int fox_w = fox_bitmap->w;
	int fox_h = fox_bitmap->h;

	// ���� ����
	int weapon_h = bitmap->h;

	for (i = 0; i<count; i++) {
		WEAPON_MISSILE *weapon = malloc(sizeof(WEAPON_MISSILE));
		weapon->power = power;
		weapon->bitmap = bitmap;
		weapon->speed = speed;
		weapon->accel = 0.1;
		list_new(&weapon->x_list);

		weapon->x = fox_x + (fox_w / 2);
		if (i % 2 == 0) {
			// ���� �̻����� ¦�� ��°�̸� ������ �߻�
			weapon->y = fox_y - weapon_h;
		}
		else {
			// ���� �̻����� Ȧ�� ��°�̸� �Ʒ����� �߻�
			weapon->y = fox_y + fox_h + weapon_h - 4;
		}

		weapon->smoke_begin = FALSE;
		weapon->smoke_index = 0;
		weapon->smoke_time = stage_elapsed_time;

		list_add(fox_missile_list, weapon);
	}
}

// ���⿡�� n-wayź �߻�
void launch_enemy_nway_weapon(float x, float y, float speed, int n, int odd)
{
	float deg_step = 360.0 / n;

	// Ȧ�� �������� deg_step/2 ��ŭ Ʋ����
	float deg = odd ? deg_step / 2 : 0;

	int i;
	for (i = 0; i<n; i++) {
		deg += deg_step;

		ENEMY_WEAPON *weapon = malloc(sizeof(ENEMY_WEAPON));

		void *out;
		hashtable_get(stage_res_table, "WEAPON", (void*)&out);
		ENEMY_RES *res = (ENEMY_RES*)out;

		void *v;
		list_get_at(res->bitmaps, 0, &v);
		BITMAP *b = (BITMAP*)v;

		weapon->bitmap = copy_sub_bitmap(b, 0, 0, b->w, b->h, 1.0);
		weapon->power = 1;
		weapon->x = x;
		weapon->y = y;
		weapon->speed = 1;

		float ang = deg2rad(deg);
		weapon->vx = LookUpCos(ang) * speed;
		weapon->vy = LookUpSin(ang) * speed;

		list_add(enemy_weapon_list, weapon);
	}
}

void init_fox_ranger()
{
	fox_x = 0;
	fox_y = SCREEN_H / 2 - (fox_bitmap->h / 2);

	fox_twinkling_time = stage_elapsed_time;	// ������ ���� �ð�
	fox_twinkling = 0;	// ������ toggle ����

	fox_ghost_time = stage_elapsed_time;	// ���� ���� �ð� (�����)

	fox_attacked_time = -1; // ���� ���� �ð�
	fox_collided_time = -1; // �浹�� �ð�

	fox_collided_index = 0;

	fox_damage_flash_time = -1;

	fox_bitmap_index = BITMAP_FOX_GO;

	fox_energy_count = 10;

	fox_explosion = -1;

	// ���� �ʱ�ȭ
	sprintf(fox_weapon_name, "LASER");
	sprintf(fox_weapon_sub_name, "");

	fox_weapon_level = 0;
	fox_weapon_sub_level = 0;

	// ���� �ִ�ġ ġƮŰ
	fox_weapon_max_power_toggle = -1;
	fox_weapon_max_power_time = stage_elapsed_time;

	game_paused = FALSE;
}

// ���� ������ �̵�
void process_fox_movement(void)
{
	int i;

	// ���������� Ŭ���� ������ ���� ������ ������ ������ �ڵ� �̵�..
	if (stage_clear_flag == TRUE) {
		int diff = stage_elapsed_time - stage_clear_time;
		if (diff >= 30) {
			fox_speed += 0.2;
			fox_x += fox_speed;
			stage_clear_time = stage_elapsed_time;
		}

		draw_sprite(stage_buf_bitmap, fox_bitmap, fox_x, fox_y);

	}
	else {
		// ���� ó��
		if (fox_energy_count <= 0) {
			int fox_w = fox_bitmaps[fox_bitmap_index]->w;
			int fox_h = fox_bitmaps[fox_bitmap_index]->h;

			int fox_cx = fox_x + fox_w / 2;
			int fox_cy = fox_y + fox_h / 2;

			BITMAP *b = fox_explosion_bitmaps[fox_explosion];
			int cx2 = b->w / 2;
			int cy2 = b->h / 2;
			draw_sprite(stage_buf_bitmap, b, fox_cx - cx2, fox_cy - cy2);

			int diff = stage_elapsed_time - fox_explosion_time;
			if (diff >= 50) {
				if (fox_explosion >= fox_explosion_sprite_count - 1) {
					// ȹ���� �������� ��
					if (list_size(fox_item_history) > 0) {
						ListIter iter;
						void *v;
						list_iter_init(&iter, fox_item_history);
						while (list_iter_next(&iter, (void*)&v) != CC_ITER_END)
						{
							ITEM *item = (ITEM*)v;
							ITEM *item2 = malloc(sizeof(ITEM));
							memcpy(item2, item, sizeof(ITEM));

							int index = item->bitmaps[item2->bitmap_index];
							int item_w = fox_item_bitmaps[index]->w;
							int item_h = fox_item_bitmaps[index]->h;

							int offset = rand_number(item_w * -1, item_w);
							item2->x = fox_cx + offset + item_w;
							if (item2->x >= SCREEN_W) {
								item2->x = SCREEN_W - item_w;
							}
							else if (item2->x <= 0) {
								item2->x = 0;
							}

							offset = rand_number(item_h * -1, item_h);
							item2->y = fox_cy + offset + item_h;
							if (item2->y >= SCREEN_H) {
								item2->y = SCREEN_H - item_h;
							}
							else if (item2->y <= 0) {
								item2->y = 0;
							}


							item2->speed = 1.0;
							item2->accel = 0;

							// ���� ������ ���� ���� ����
							int dir = 1 + rand() % 4;

							// left_top
							if (dir == 1) {
								item2->dx = -1;
								item2->dy = -1;
								// left_bottom
							}
							else if (dir == 2) {
								item2->dx = -1;
								item2->dy = 1;
								// right_top
							}
							else if (dir == 3) {
								item2->dx = 1;
								item2->dy = -1;
								// right_bottom
							}
							else if (dir == 4) {
								item2->dx = 1;
								item2->dy = 1;
							}

							item2->creation_time = stage_elapsed_time;
							list_add(enemy_item_list, item2);
						}

						list_remove_all_cb(fox_item_history, free);
					}

					// �Ʊ��� ��ü ���� ���Ƚ�..
					if (fox_life_count <= 0) {
						game_end = TRUE;
					}

					if(game_end != TRUE)
						init_fox_ranger();

				}
				else {
					fox_explosion++;
				}

				fox_explosion_time = stage_elapsed_time;

			}

		}
		else {
			if (keypressed()) {
				// �̵�
				if (key[KEY_DOWN]) {
					fox_y += 3;
					fox_bitmap_index = BITMAP_FOX_DOWN;
				}
				if (key[KEY_UP]) {
					fox_y -= 3;
					fox_bitmap_index = BITMAP_FOX_UP;
				}
				if (key[KEY_RIGHT]) {
					fox_x += 3;
				}
				if (key[KEY_LEFT]) {
					fox_x -= 3;
				}

			}

			if (keyreleased(KEY_DOWN) || keyreleased(KEY_UP)) {
				fox_restore = 1;
			}

			// ��ü ���� ����
			if (fox_restore) {
				fox_bitmap_index = BITMAP_FOX_GO;
				fox_restore = 0;
			}

			// �����̸� ���� ǥ�ø� ���� ������ ǥ��
			if (fox_ghost_time != -1) {
				// 4�� ������ ���� ����
				int passed_time = stage_elapsed_time - fox_ghost_time;
				if (passed_time >= 4000) {
					fox_ghost_time = -1;

				}
				else {
					int diff = stage_elapsed_time - fox_twinkling_time;
					// 0.05�ʸ��� �����Ÿ�
					if (diff > 100) {
						fox_twinkling = !fox_twinkling;
						if (fox_twinkling == 1) {
							fox_bitmap = fox_light_bitmaps[fox_bitmap_index];
						}
						else {
							fox_bitmap = fox_bitmaps[fox_bitmap_index];
						}

						fox_twinkling_time = stage_elapsed_time;
					}
				}

			}
			else {
				fox_bitmap = fox_bitmaps[fox_bitmap_index];
			}

			// �����κ��� ���� �޾�����
			if (fox_attacked_time != -1) {
				int diff = stage_elapsed_time - fox_attacked_time;

				// Ư�� �ð��� �����ٸ� ������ ��Ʈ�� ǥ��
				if (diff >= 100) {
					fox_bitmap = fox_bitmaps[fox_bitmap_index];
					fox_attacked_time = -1;

					// Ư�� �ð����� ��� ��Ʈ�� ǥ��
				}
				else {
					fox_bitmap = fox_light_bitmaps[fox_bitmap_index];
				}
			}

			// ����� �浹�� �Ǿ��ٸ�...
			if (fox_collided_time != -1) {
				int diff = stage_elapsed_time - fox_collided_time;

				// Ư�� �ð��� �����ٸ� ������ ��Ʈ�� ǥ��
				if (diff >= 100) {
					fox_collided_index++;
					fox_collided_time = stage_elapsed_time;

					if (fox_collided_index >= 10) {
						fox_collided_time = -1;
						fox_collided_index = 0;

						fox_bitmap = fox_bitmaps[fox_bitmap_index];
					}
				}

				if (fox_collided_time != -1) {
					if (fox_collided_index % 2 == 0)
						fox_bitmap = fox_bitmaps[fox_bitmap_index];
					else
						fox_bitmap = fox_light_bitmaps[fox_bitmap_index];
				}
			}

			// �����κ��� ���� �޾�����
			// ȭ���� ���������� ������ ó��
			if (fox_damage_flash_time != -1) {
				if (fox_damage_flash_index % 2 == 0)
					draw_sprite(stage_buf_bitmap, fox_damage_bitmap, 0, 0);

				// Ư�� �ð��� �����ٸ� ������ ��Ʈ�� ǥ��
				int diff = stage_elapsed_time - fox_damage_flash_time;
				if (diff >= 20) {
					fox_damage_flash_time = stage_elapsed_time;
					fox_damage_flash_index++;
					if (fox_damage_flash_index >= 5) {
						fox_damage_flash_index = 0;
						fox_damage_flash_time = -1;
					}
				}
			}

			// ���� ������ �̵� ���� ����
			if (fox_x <= 0) fox_x = 0;
			if (fox_x > SCREEN_W - fox_bitmap->w)
				fox_x = SCREEN_W - fox_bitmap->w;
			if (fox_y <= 0) fox_y = 0;
			if (fox_y > SCREEN_H - fox_bitmap->h)
				fox_y = SCREEN_H - fox_bitmap->h;

#if 0
			// ENTER�� ������ ��� ���� �̻��� �߻�
			if (keypressed() && key[KEY_ENTER]) {
				int diff = stage_elapsed_time - fox_urgency_weapon_time;
				if (diff >= 1000) {
					//launch_emergency_guided_missile(30, BITMAP_GUIDED_MISSILE, 100, 1);

					// ���� �߻�ð� ������Ʈ
					fox_urgency_weapon_time = stage_elapsed_time;
				}
			}
#endif

			// ���� ������/�̻��� �ִ� �Ŀ��� ��ü
			if (keypressed() && key[KEY_A]) {
				int diff = stage_elapsed_time - fox_weapon_max_power_time;
				if (diff >= 500) {
					play_sample(sound_effect[SOUND_MAX_POWER], 255, 127, 1000, 0);

					if (fox_weapon_max_power_toggle == -1) {
						fox_weapon_max_power_toggle = 0;
					}
					else if (fox_weapon_max_power_toggle == 0) {
						fox_weapon_max_power_toggle = 1;
					}
					else if (fox_weapon_max_power_toggle == 1) {
						fox_weapon_max_power_toggle = 0;
					}

					// -----------------------
					if (!strcasecmp(fox_weapon_name, "LASER")) {
						sprintf(fox_weapon_name, "%s", "BALKAN");
					}
					else {
						sprintf(fox_weapon_name, "%s", "LASER");
					}

					{
						void *out;
						hashtable_get(fox_weapon_table, fox_weapon_name, (void*)&out);
						List *list = (List*)out;
						int max_power = list_size(list);
						fox_weapon_level = max_power - 1;
					}

					// -----------------------
					if (fox_weapon_max_power_toggle == 0) {
						sprintf(fox_weapon_sub_name, "%s", "GUIDED_MISSILE");
					}
					else if (fox_weapon_max_power_toggle == 1) {
						sprintf(fox_weapon_sub_name, "%s", "MISSILE");
					}

					{
						void *out;
						hashtable_get(fox_weapon_table, fox_weapon_sub_name, (void*)&out);
						List *list = (List*)out;
						int max_power = list_size(list);
						fox_weapon_sub_level = max_power - 1;
					}

					fox_weapon_max_power_time = stage_elapsed_time;
				}
			}

			if (keypressed() && key[KEY_SPACE]) {
				int diff = stage_elapsed_time - fox_weapon_time;

				void *out;
				hashtable_get(fox_weapon_table, fox_weapon_name, (void*)&out);
				List *list = (List*)out;

				void *v;
				list_get_at(list, fox_weapon_level, &v);
				FOX_WEAPON *weapon = (FOX_WEAPON*)v;

				// �� ���� �߻�
				if (diff >= weapon->reload_time) {
					BITMAP *bitmap = weapon->bitmap;
					int power = weapon->power;
					int speed = weapon->speed;
					int bullet_count = weapon->launch_count;

					// ���� ����
					int weapon_h = bitmap->h;

					for (i = 0; i<bullet_count; i++) {
						// ���� ����
						WEAPON *weapon = malloc(sizeof(WEAPON));
						weapon->power = power;
						weapon->bitmap = bitmap;
						weapon->speed = speed;

						int fox_w = fox_bitmap->w;
						int fox_h = fox_bitmap->h;

						weapon->x = fox_x + fox_w - 5;

						int start_y = 0;
						float angle = 0;

						if (bullet_count == 1) {
							start_y = fox_y + (fox_h - weapon_h) / 2;
							angle = deg2rad(0);

						}
						else if (bullet_count == 2) {
							if (i == 0) {
								start_y = fox_y + (fox_h - weapon_h) / 2 - (weapon_h / 2) - 5;
							}
							else if (i == 1) {
								start_y = fox_y + (fox_h - weapon_h) / 2 + (weapon_h / 2) + 5;
							}
							angle = deg2rad(0);

						}
						else if (bullet_count == 3) {
							if (i == 0) {
								start_y = fox_y + (fox_h - weapon_h) / 2 - weapon_h - 5;
								angle = deg2rad(-7);
								weapon->x = weapon->x - 5;
							}
							else if (i == 1) {
								start_y = fox_y + (fox_h - weapon_h) / 2;
								angle = deg2rad(0);
							}
							else if (i == 2) {
								start_y = fox_y + (fox_h - weapon_h) / 2 + weapon_h + 5;
								angle = deg2rad(7);
								weapon->x = weapon->x - 5;
							}

						}
						else if (bullet_count == 4) {
							if (i == 0) {
								start_y = fox_y + (fox_h - weapon_h) / 2 - (weapon_h / 2) - 10;
								angle = deg2rad(-7);
								weapon->x = weapon->x - 15;
							}
							else if (i == 1) {
								start_y = fox_y + (fox_h - weapon_h) / 2 - (weapon_h / 2) - 5;
								angle = deg2rad(-0);
								weapon->x = weapon->x - 5;
							}
							else if (i == 2) {
								start_y = fox_y + (fox_h - weapon_h) / 2 + (weapon_h / 2) + 5;
								angle = deg2rad(0);
								weapon->x = weapon->x - 5;
							}
							else if (i == 3) {
								start_y = fox_y + (fox_h - weapon_h) / 2 + (weapon_h / 2) + 10;
								angle = deg2rad(7);
								weapon->x = weapon->x - 15;
							}

						}
						else if (bullet_count == 5 || bullet_count == 6) {
							if (i == 0) {
								start_y = fox_y + (fox_h - weapon_h) / 2 - (weapon_h / 2) - 10;
								angle = deg2rad(-12);
								weapon->x = weapon->x - 10;
							}
							else if (i == 1) {
								start_y = fox_y + (fox_h - weapon_h) / 2 - (weapon_h / 2) - 5;
								angle = deg2rad(-7);
								weapon->x = weapon->x - 5;
							}
							else if (i == 2) {
								start_y = fox_y + (fox_h - weapon_h) / 2;
								angle = deg2rad(0);
							}
							else if (i == 3) {
								start_y = fox_y + (fox_h - weapon_h) / 2 + (weapon_h / 2) + 5;
								angle = deg2rad(7);
								weapon->x = weapon->x - 5;
							}
							else if (i == 4) {
								start_y = fox_y + (fox_h - weapon_h) / 2 + (weapon_h / 2) + 10;
								angle = deg2rad(12);
								weapon->x = weapon->x - 10;
							}
						}
						weapon->y = start_y;

						weapon->vx = LookUpCos(angle) * 10.0;
						weapon->vy = LookUpSin(angle) * 10.0;

						// radians to allegro degrees
						weapon->angle = angle;

						list_add(fox_weapon_list, weapon);
					}

					// ���� �߻�ð� ������Ʈ
					fox_weapon_time = stage_elapsed_time;
				}

				// ���� �̻��� �߻� 
				if (!strcasecmp(fox_weapon_sub_name, "GUIDED_MISSILE") || !strcasecmp(fox_weapon_sub_name, "MISSILE")) {
					int diff = stage_elapsed_time - fox_item_weapon_time;

					void *out;
					hashtable_get(fox_weapon_table, fox_weapon_sub_name, (void*)&out);
					List *list = (List*)out;

					void *v;
					list_get_at(list, fox_weapon_sub_level, &v);
					FOX_WEAPON *weapon = (FOX_WEAPON*)v;

					if (diff >= weapon->reload_time) {
						BITMAP *bitmap = weapon->bitmap;
						int power = weapon->power;
						int speed = weapon->speed;
						int bullet_count = weapon->launch_count;

						// ���� �̻��� �߻�
						if (!strcasecmp(fox_weapon_sub_name, "GUIDED_MISSILE")) {
							launch_guided_missile(bullet_count, bitmap, power, speed);

							// �̻��� �߻�
						}
						else {
							launch_missile(bullet_count, bitmap, power, speed);
						}

						// ���� ���� �߻�ð� ������Ʈ
						fox_item_weapon_time = stage_elapsed_time;
					}
				}
			}

			// -----------------------------------------------------------------
			// ���� ������ ǥ��
			draw_sprite(stage_buf_bitmap, fox_bitmap, fox_x, fox_y);
		}
	}
}



// ������ �̵�
void process_enemy_item(void)
{
	ListIter iter;
	void *v;
	list_iter_init(&iter, enemy_item_list);
	while (list_iter_next(&iter, (void*)&v) != CC_ITER_END)
	{
		ITEM *item = (ITEM*)v;

		float x = item->x;
		float y = item->y;

		int index = item->bitmaps[item->bitmap_index];
		int w = fox_item_bitmaps[index]->w;
		int h = fox_item_bitmaps[index]->h;

		// �������� ȭ�� ������ �̵��ߴٸ� ����
		if (x < (fox_item_bitmaps[index]->w * -1)) {
			list_iter_remove(&iter, NULL);
			free(item);

		}
		else {
			// �����۰� ���� �������� �浹 üũ
			int fx = fox_x;
			int fy = fox_y;
			int fw = fox_bitmap->w;
			int fh = fox_bitmap->h;

			if (check_collision_bbox(fx, fy, fw, fh, x, y, w, h)) {
				// ȹ���� ������ ����
				// �Ʊ��Ⱑ �׾������ ���� �뵵
				ITEM *item2 = malloc(sizeof(ITEM));
				memcpy(item2, item, sizeof(ITEM));
				list_add(fox_item_history, item2);

				if (!strcasecmp(option.sound, "yes")) {
					play_sample(sound_effect[SOUND_ITEM], 255, 127, 1000, 0);
				}

				// �̻��� ���� �����̸�..
				if (!strcasecmp(item->name, "GUIDED_MISSILE") || !strcasecmp(item->name, "MISSILE")) {
					// ���� �̻��ϰ� ������ �������� ��� ����ġ ����
					if (!strcasecmp(fox_weapon_sub_name, item->name)) {
						fox_weapon_sub_level += 1;

						void *out;
						hashtable_get(fox_weapon_table, fox_weapon_sub_name, (void*)&out);
						List *list = (List*)out;

						// �ִ� �Ŀ�
						int max_power = list_size(list);

						// �������� ��� �Դ��� �ִ� �Ŀ��� �Ѿ�� �ִ� �Ŀ��� ��� ����
						if (fox_weapon_sub_level >= max_power - 1) {
							fox_weapon_sub_level = max_power - 1;
						}

						// �ƴϸ� ���ο� �̻��� ������ ��ü 
						// ���� �Ŀ��� �ʱ�ȭ
					}
					else {
						sprintf(fox_weapon_sub_name, "%s", item->name);
						fox_weapon_sub_level = 0;
					}

					if (!strcasecmp(option.sound, "yes")) {
						play_sample(sound_effect[SOUND_ITEM], 255, 127, 1000, 0);
					}

					// ���� ����� ������ �������� ��� ����ġ ����
				}
				else {
					if (!strcasecmp(fox_weapon_name, item->name)) {
						fox_weapon_level += 1;

						// �ִ� �Ŀ�
						void *out;
						hashtable_get(fox_weapon_table, fox_weapon_name, (void*)&out);
						List *list = (List*)out;

						// �ִ� �Ŀ�
						int max_level = list_size(list);

						// �������� ��� �Դ��� �ִ� �Ŀ��� �Ѿ�� �ִ� �Ŀ��� ��� ����
						if (fox_weapon_level >= max_level - 1) {
							fox_weapon_level = max_level - 1;

							show_popup_message("MMMMMMMMAX POWER!!", FALSE, 3000);

						}
						else {
							show_popup_message("POWER UP!!", FALSE, 3000);
						}

						if (!strcasecmp(option.sound, "yes")) {
							play_sample(sound_effect[SOUND_POWERUP], 255, 127, 1000, 0);
						}

						// �ƴϸ� ���� ��ü
					}
					else {
						hide_popup_message();

						// ��ü�� ���� ���Ⱑ �ִ� �Ŀ� ��ġ������ Ȯ��
						void *out;
						hashtable_get(fox_weapon_table, fox_weapon_name, (void*)&out);
						List *list = (List*)out;

						int max_level = list_size(list);

						int is_max_level = FALSE;
						if (fox_weapon_level >= max_level - 1) {
							is_max_level = TRUE;
						}

						// ------------------------------
						// ���� ��ü
						sprintf(fox_weapon_name, "%s", item->name);

						void *out2;
						hashtable_get(fox_weapon_table, fox_weapon_name, (void*)&out2);
						List *list2 = (List*)out2;

						max_level = list_size(list2);

						// ���� ���Ⱑ �ִ� �Ŀ� ��ġ������ �ִ�ġ�� �ο�
						if (is_max_level == TRUE) {
							fox_weapon_level = max_level - 1;

						}
						else {
							if (fox_weapon_level >= max_level - 1) {
								fox_weapon_level = max_level - 1;
							}
						}
					}
				}

				// ������ ����
				list_iter_remove(&iter, NULL);
				free(item);
				item = NULL;
			}
		}

		if (item != NULL) {
			draw_sprite(stage_buf_bitmap, fox_item_bitmaps[index], x, y);

			if (item->bitmap_count > 1) {
				int diff = stage_elapsed_time - item->bitmap_show_time;
				if (diff >= 200) {
					if (item->bitmap_index >= item->bitmap_count - 1) {
						item->bitmap_index = 0;
					}
					else {
						item->bitmap_index++;
					}
					item->bitmap_show_time = stage_elapsed_time;
				}
			}

			// 10�ʰ� ������ �������� ȭ������� ������.
			int diff = stage_elapsed_time - item->creation_time;
			int bouncing;
			if (diff >= 10000) {
				bouncing = FALSE;
			}
			else {
				bouncing = TRUE;
			}

			float new_x, new_y;
			float new_dx, new_dy;
			int bounced;

			moving_bouncing(item->dx, item->dy, item->x, item->y, w, h,
				item->speed, &new_x, &new_y, &new_dx, &new_dy, bouncing, &bounced);

			item->x = new_x;
			item->y = new_y;
			item->dx = new_dx;
			item->dy = new_dy;

			if (bounced == TRUE && item->accel > 0.0) {
				item->speed = 1.0;
				item->accel = 0.0;
				if (new_dx > 0) {
					item->dx = 1;
				}
				else {
					item->dx = -1;
				}
				if (new_dy > 0) {
					item->dy = 1;
				}
				else {
					item->dy = -1;
				}

			}
			else {
				item->speed += item->accel;
			}
		}
	}
}

// ���� ���� ǥ��
void process_show_life_meter(void)
{
	int i, x, y;

	x = 2;
	y = SCREEN_H - fox_life_small_bitmap->h - 2;
	draw_sprite(stage_buf_bitmap, fox_life_small_bitmap, x, y);
	char tmp[10];
	sprintf(tmp, "%d", fox_life_count);
	textout_ex(stage_buf_bitmap, font, tmp, fox_life_small_bitmap->w + 4, y, makecol(255, 0, 0), -1);

	int spacing = 1;
	x = SCREEN_W - (fox_life_heart_bitmap->w * fox_energy_count) - (spacing * fox_energy_count);
	y = SCREEN_H - fox_life_heart_bitmap->h - 2;
	for (i = 0; i<fox_energy_count; i++) {
		draw_sprite(stage_buf_bitmap, fox_life_heart_bitmap, x, y);
		x += fox_life_heart_bitmap->w;
		x += spacing;
	}
}

// �� ���� �� �̵�
void process_enemy_movement(void)
{
	ListIter iter;
	void *v;
	list_iter_init(&iter, enemy_advent_list);
	while (list_iter_next(&iter, (void*)&v) != CC_ITER_END)
	{
		ENEMY_ADVENT *e = (ENEMY_ADVENT*)v;

		int life = e->life;
		float speed = e->speed;
		int reload_time = e->reload_time;
		float first_location = e->first_location;
		int motion = e->motion;
		int revenge_nway = e->revenge_nway;
		float accel = e->accel;

		// �� ���� �ð��̸� �� ����
		if (stage_elapsed_time >= e->time) {
			ENEMY *enemy = malloc(sizeof(ENEMY));

			enemy->creation_time = stage_elapsed_time;

			sprintf(enemy->name, "%s", e->name);
			enemy->explosion = -1;
			enemy->max_life = life;
			enemy->life = life;
			enemy->base_speed = speed;
			enemy->speed = speed;
			enemy->accel = accel;
			enemy->revenge_nway = revenge_nway;
			sprintf(enemy->item, "%s", e->item);
			sprintf(enemy->orbit_name, "%s", e->orbit_name);

			if (!strcasecmp(enemy->name, "BOSS")) {
				// ---------------------
				enemy->angry_time = -1;
				enemy->angry_count = 0;
				enemy->angry_max_count = list_size(e->angry_point_list);

				list_copy_deep(e->angry_point_list, copy_to_int, &enemy->angry_point_list);

				// ---------------------
				enemy->fire_count = 0;
				enemy->fire_max_count = list_size(e->fire_point_list);

				list_copy_deep(e->fire_point_list, copy_to_int, &enemy->fire_point_list);

				// ---------------------
				list_new(&enemy->fire_list);

			}
			else {
				enemy->angry_time = -1;
				enemy->angry_count = 0;
				enemy->angry_max_count = 0;
				enemy->angry_point_list = NULL;

				enemy->fire_count = 0;
				enemy->fire_point_list = NULL;

				enemy->fire_list = NULL;
			}


			void *out;
			hashtable_get(stage_res_table, e->name, (void*)&out);
			ENEMY_RES *res = (ENEMY_RES*)out;

			void *v;
			list_get_at(res->bitmaps, 0, &v);
			BITMAP *b = (BITMAP*)v;

			enemy->w = b->w;
			enemy->h = b->h;

			// �ʱ� ��ġ ����
			if (!strcasecmp(e->side, "RIGHT")) {
				enemy->x = SCREEN_W;

				if (e->first_location == -1) {
					enemy->y = enemy->h + rand() % (SCREEN_H - enemy->h);
				}
				else {
					enemy->y = SCREEN_H * first_location / 100;
				}

			}
			else if (!strcasecmp(e->side, "LEFT")) {
				enemy->x = enemy->w * -1;

				if (e->first_location == -1) {
					enemy->y = enemy->h + rand() % (SCREEN_H - enemy->h);
				}
				else {
					enemy->y = SCREEN_H * first_location / 100;
				}

			}
			else if (!strcasecmp(e->side, "TOP")) {
				enemy->y = enemy->h * -1;

				if (e->first_location == -1) {
					enemy->x = enemy->w + rand() % (SCREEN_W - enemy->w);
				}
				else {
					enemy->x = SCREEN_W * first_location / 100;
				}

				enemy->x = enemy->x - (enemy->w / 2);

			}
			else if (!strcasecmp(e->side, "BOTTOM")) {
				enemy->y = SCREEN_H;

				if (e->first_location == -1) {
					enemy->x = enemy->w + rand() % (SCREEN_W - enemy->w);
				}
				else {
					enemy->x = SCREEN_W * first_location / 100;
				}

				enemy->x = enemy->x - (enemy->w / 2);
			}

			enemy->y = enemy->y - (enemy->h / 2);

			enemy->angle = 0.0;
			enemy->base_y = enemy->y;

			if (motion == MOVE_GUIDED || motion == MOVE_GUIDED_BACK) {
				enemy->trace = TRUE;
				enemy->trace_angle = calc_angle(fox_x, fox_y, enemy->x, enemy->y);
			}
			else {
				enemy->trace = FALSE;
				enemy->trace_angle = 0.0;
			}

			enemy->weak_x = res->weak_x;
			enemy->weak_y = res->weak_y;
			enemy->weak_w = res->weak_w;
			enemy->weak_h = res->weak_h;

			enemy->launch_time = stage_elapsed_time;
			enemy->launch_sub_time = stage_elapsed_time;
			enemy->sub_weapon_time = stage_elapsed_time;
			enemy->sub_weapon_index = 0;
			enemy->reload_time = reload_time;
			enemy->attacked_time = -1;
			enemy->destroyed = FALSE;

			enemy->number = enemy_unique_index++;

			if (!strcasecmp(enemy->name, "BOSS")) {
				enemy->motion = MOVE_LEFT;
				enemy->attack_begin = FALSE;
			}
			else {
				enemy->motion = motion;
				enemy->attack_begin = TRUE;
			}

			enemy->attack_pause = FALSE;
			enemy->attack_method = ATTACK_4_WAY;	// �������� ���
			enemy->attack_method_time = stage_elapsed_time;
			enemy->attack_odd_time = stage_elapsed_time;
			enemy->attack_odd = TRUE;

			enemy->bitmap_index = 0;

			list_new(&enemy->bitmaps);
			int j = 0;
			for (j = 0; j<(int)list_size(res->bitmaps); j++) {
				void *v;
				list_get_at(res->bitmaps, j, &v);
				BITMAP *b = (BITMAP*)v;

				BITMAP *b2 = copy_sub_bitmap(b, 0, 0, b->w, b->h, 1.0);

				// ����ź�� �߻��ϴ� ���� ��� �������� ���� ���ǻ��� ���������� ä����Ŵ
				if (enemy->revenge_nway > 0) {
					BITMAP *b3 = create_bitmap(b2->w, b2->h);
					clear(b3);
					draw_lit_sprite(b3, b2, 0, 0, makecol(255, 0, 0));
					list_add(enemy->bitmaps, b3);
					destroy_bitmap(b2);

				}
				else {
					list_add(enemy->bitmaps, b2);
				}
			}

			enemy->bitmap_show_time = stage_elapsed_time;

			if (motion == MOVE_ZOOMIN_LEFT) {
				enemy->bitmap_size = 0.0;
				enemy->zoom_done = FALSE;
			}
			else {
				enemy->bitmap_size = 1.0;
				enemy->zoom_done = TRUE;
			}

			enemy->bitmap_size_time = stage_elapsed_time;

			enemy->orbit_index = 0;
			enemy->orbit_time = stage_elapsed_time;

			//-------------------------------------------

			// �� ����
			list_add(enemy_list, enemy);
			//-------------------------------------------

			// �� ���� �� �������� ����Ÿ������ ����
			if (e->angry_point_list != NULL)
				list_remove_all_cb(e->angry_point_list, free);

			if (e->fire_point_list != NULL)
				list_remove_all_cb(e->fire_point_list, free);

			list_iter_remove(&iter, NULL);

			free(e);
		}
	}

	// ���� ���� 5�����̸� ����� �︲
	if (stage_elapsed_time >= boss_warning_time && boss_warning_flag == FALSE) {
		show_popup_message("Warning !!", FALSE, 3000);

		if (strcasecmp(option.music, "no")) {
			play_midi(bgm_midi[BGM_BOSS], 1);
		}

		if (!strcasecmp(option.sound, "yes")) {
			play_sample(sound_effect[SOUND_SIREN], 255, 127, 1000, 0);
		}

		boss_warning_flag = TRUE;
	}

	// �� �̵�
	{
		ListIter iter;
		void *v;
		list_iter_init(&iter, enemy_list);
		while (list_iter_next(&iter, (void*)&v) != CC_ITER_END)
		{
			ENEMY *enemy = (ENEMY*)v;

			// -----------------------------------------------------
			// ���� ���� ó��
			// -----------------------------------------------------
			if (enemy->destroyed == TRUE) {
				// ������ ���� �Ʒ��� ����Ʈ���鼭 ���� ó��
				if (!strcasecmp(enemy->name, "BOSS")) {
					float vy, vx;
					float angle;
					float new_x, new_y;

					float drop_tx = 0;
					float drop_ty = SCREEN_H + 50;

					int diff = stage_elapsed_time - boss_explod_begin_time;

					float x, y;

					// ���߽� 3�ʰ� ������ ����Ʈ��
					if (diff > 3000) {
						// �߶� �������� ���� �̵�
						moving_guided(2, enemy->x, enemy->y, drop_tx, drop_ty,
							&new_x, &new_y, &vx, &vy, &angle);
						enemy->x = new_x;
						enemy->y = new_y;

						x = new_x;
						y = new_y;

						// �߶� �������� ����������� ����
						float dist = calc_distance(drop_tx, drop_ty, new_x, new_y);
#if 0
						if (abs(dist) <= 70) {
							// ���� ȿ��
							void *v2;
							list_get_at(enemy->bitmaps, enemy->bitmap_index, &v2);
							BITMAP *b = (BITMAP*)v2;
							create_effect_firework(enemy->x, enemy->y, 300, b);
						}
#endif

						if (dist <= 10) {
							show_popup_message("STAGE CLEAR", TRUE, 0);

							stage_clear_flag = TRUE;
							stage_clear_time = stage_elapsed_time;

							// ���� �������� Ż���� ���� ���� ��� ��ġ��Ŵ
							fox_bitmap_index = BITMAP_FOX_GO;
							fox_bitmap = fox_bitmaps[fox_bitmap_index];

							fox_x = fox_bitmap->w * -1;
							fox_y = SCREEN_H / 2 - (fox_bitmap->h / 2);

							fox_speed = 0.0;

							// --------------------------------------
							list_remove_all_cb(enemy->bitmaps, destroy_bitmap);
							list_destroy(enemy->bitmaps);

							if (enemy->angry_point_list != NULL)
								list_remove_all_cb(enemy->angry_point_list, free);

							if (enemy->fire_point_list != NULL)
								list_remove_all_cb(enemy->fire_point_list, free);

							if (enemy->fire_list != NULL)
								list_remove_all_cb(enemy->fire_list, free);

							list_iter_remove(&iter, NULL);

							free(enemy);
							enemy = NULL;

							if (strcasecmp(option.music, "no")) {
								play_midi(bgm_midi[BGM_STAGE_CLEAR], 0);
							}
						}

						// 3�� �̳��� ������ 0.02�ʸ��� ���
					}
					else {
						int diff2 = stage_elapsed_time - boss_explod_vibration_time;
						if (diff2 > 20) {
							// -7 ~ 7 ������ ��ǥ�� x, y�� �������� ���
							boss_explod_vibration_x = -7 + rand() % 7;
							boss_explod_vibration_y = -7 + rand() % 7;

							boss_explod_vibration_time = stage_elapsed_time;
						}

						x = enemy->x + boss_explod_vibration_x;
						y = enemy->y + boss_explod_vibration_y;
					}

					// ���� ���
					if (enemy != NULL) {
#if 0
						// ��Ʋ���� ���
						void *out2;
						hashtable_get(stage_res_table, enemy->name, (void*)&out2);
						ENEMY_RES *res = (ENEMY_RES*)out2;

						void *v2;
						list_get_at(res->weapon_bitmaps, 0, &v2);
						BITMAP *b2 = (BITMAP*)v2;
						draw_sprite(stage_buf_bitmap, b2, x - 10, y + 67);
#endif

						// �ذ� ���
						void *v3;
						list_get_at(enemy->bitmaps, enemy->bitmap_index, &v3);
						BITMAP *b = (BITMAP*)v3;
						draw_sprite(stage_buf_bitmap, b, x, y);

						// ���߽� 2�ʰ� ������ 0.2�ʸ��� ���� ���� ��������Ʈ ���� ������Ŵ
						int explod_count;

						// 3�� �̻��̸�..
						if (diff > 3000) {
							int diff2 = stage_elapsed_time - (boss_explod_begin_time + 3000);

							// 2�� �̳��� 3���� ��������Ʈ�� �������� ��������Ƿ�, �ּ� 3�� �̻���� �����ؾ��Ѵ�.
							explod_count = (diff2 / 200) + 3;
							if (explod_count <= 0) {
								explod_count = 1;
							}
							else if (explod_count >= BITMAP_MAX_BOSS_EXPLOSION) {
								explod_count = BITMAP_MAX_BOSS_EXPLOSION;
							}

						}
						else {
							// 3�� �̳��� ���� ��������Ʈ ������ 3������ ���� ��������Ʈ�� ���
							explod_count = 3;
						}

						int k;
						for (k = 0; k<explod_count; k++) {
							if (boss_explosions[k] == -1) {
								boss_explosion_times[k] = stage_elapsed_time;
								boss_explosion_x[k] = 10 + rand() % (b->w - 20);
								boss_explosion_y[k] = 10 + rand() % (b->h - 40);
								boss_explosions[k] = rand() % boss_explosion_sprite_count;
							}
						}

						for (k = 0; k<explod_count; k++) {
							int diff2 = stage_elapsed_time - boss_explosion_times[k];
							// 0.05�� ���� �ѹ��� ��������Ʈ�� �ٲ㰡�� ���� ȿ���� �ش�.
							if (diff2 >= 50) {
								if (boss_explosions[k] >= boss_explosion_sprite_count - 1) {
									boss_explosions[k] = -1;
									if (!strcasecmp(option.sound, "yes")) {
										play_sample(sound_effect[SOUND_BOSS_EXPLOD], 255, 127, 1000, 0);
									}

								}
								else {
									int idx = boss_explosions[k];
									boss_explosions[k] = idx + 1;
								}

								boss_explosion_times[k] = stage_elapsed_time;
							}
						}

						// --------
						// ���� ��������Ʈ ���
						for (k = 0; k<explod_count; k++) {
							if (boss_explosions[k] == -1) continue;

							b = boss_explosion_bitmaps[boss_explosions[k]];
							int boss_cx = b->w / 2;
							int boss_cy = b->h / 2;

							int x = enemy->x - boss_cx + boss_explosion_x[k];
							int y = enemy->y - boss_cy + boss_explosion_y[k];

							draw_sprite(stage_buf_bitmap, b, x, y);
						}

					}

					// ������ �ƴ� �Ϲ� ���� ���
				}
				else {
					int explosion = enemy->explosion;

					int x = enemy->x;
					int y = enemy->y;
					int w = enemy->w;
					int h = enemy->h;

					int cx1 = x + w / 2;
					int cy1 = y + h / 2;

					BITMAP *b = explosion_bitmaps[explosion];
					int cx2 = b->w / 2;
					int cy2 = b->h / 2;

					// ���� �߾ӿ� ���� ��������Ʈ ǥ��
					draw_sprite(stage_buf_bitmap, b, cx1 - cx2, cy1 - cy2);

					int diff = stage_elapsed_time - enemy->explosion_time;
					if (diff >= 20) {
						explosion += 1;
						enemy->explosion_time = stage_elapsed_time;
					}

					if (explosion >= enemy_explosion_sprite_count - 1) {
						explosion = -1;

						// �������� �����ִ� �����̸� �������� ������ ����
						if (strlen(enemy->item) > 0) {
							ITEM *item = malloc(sizeof(ITEM));

							sprintf(item->name, "%s", enemy->item);
							item->x = x;
							item->y = y;
							item->accel = 0.0;
							item->speed = 1.0;

							// ���� ������ ���� ���� ����
							int dir = 1 + rand() % 4;

							// left_top
							if (dir == 1) {
								item->dx = -1;
								item->dy = -1;
								// left_bottom
							}
							else if (dir == 2) {
								item->dx = -1;
								item->dy = 1;
								// right_top
							}
							else if (dir == 3) {
								item->dx = 1;
								item->dy = -1;
								// right_bottom
							}
							else if (dir == 4) {
								item->dx = 1;
								item->dy = 1;
							}

							item->creation_time = stage_elapsed_time;

							if (!strcasecmp(enemy->item, "BALKAN") || !strcasecmp(enemy->item, "LASER")) {
								int index;
								if (!strcasecmp(enemy->item, "BALKAN"))
									index = BITMAP_ITEM_BALKAN;
								else
									index = BITMAP_ITEM_LASER;

								item->bitmaps[0] = index;
#if 0
								item->bitmaps[1] = index + 1;
								item->bitmaps[2] = index + 2;
								item->bitmap_count = 3;
#else
								item->bitmap_count = 1;
#endif

							}
							else {
								int index = 0;
								if (!strcasecmp(enemy->item, "GUIDED_MISSILE"))
									index = BITMAP_ITEM_GUIDED_MISSILE;
								else if (!strcasecmp(enemy->item, "MISSILE"))
									index = BITMAP_ITEM_MISSILE;

								item->bitmaps[0] = index;
								item->bitmap_count = 1;
							}

							item->bitmap_index = 0;
							item->bitmap_show_time = stage_elapsed_time;

							// --------------------------------------------------
							// �������� ���� ��ġ�� ȭ�鳻�� ����
							int index = item->bitmaps[item->bitmap_index];
							int w = fox_item_bitmaps[index]->w;
							int h = fox_item_bitmaps[index]->h;

							if (x > SCREEN_W - w) {
								item->x = SCREEN_W - w;
								item->dx = -1;
							}

							if (y > SCREEN_H - h) {
								item->y = SCREEN_H - h;
								item->dy = -1;
							}
							// --------------------------------------------------

							list_add(enemy_item_list, item);
						}

						// ----------------------------------
						list_remove_all_cb(enemy->bitmaps, destroy_bitmap);
						list_destroy(enemy->bitmaps);

						if (enemy->angry_point_list != NULL)
							list_remove_all_cb(enemy->angry_point_list, free);

						if (enemy->fire_point_list != NULL)
							list_remove_all_cb(enemy->fire_point_list, free);

						if (enemy->fire_list != NULL)
							list_remove_all_cb(enemy->fire_list, free);

						list_iter_remove(&iter, NULL);
						free(enemy);
						enemy = NULL;

					}
					else {
						enemy->explosion = explosion;
					}
				}

				// ���� �̵� ó��
			}
			else if (enemy->explosion == -1) {
				float x = enemy->x;
				float y = enemy->y;

#if 0
				// ��Ʋ���� ǥ��
				if (!strcasecmp(enemy->name, "BOSS")) {
					void *out;
					hashtable_get(stage_res_table, enemy->name, (void*)&out);
					ENEMY_RES *res = (ENEMY_RES*)out;

					void *v;
					list_get_at(res->weapon_bitmaps, enemy->sub_weapon_index, &v);
					BITMAP *b = (BITMAP*)v;

					draw_sprite(stage_buf_bitmap, b, x - 10, y + 67);

					int diff = stage_elapsed_time - enemy->sub_weapon_time;
					if (diff >= 20) {
						enemy->sub_weapon_index++;
						if (enemy->sub_weapon_index > list_size(res->weapon_bitmaps) - 1) {
							enemy->sub_weapon_index = 0;
						}
						enemy->sub_weapon_time = stage_elapsed_time;
					}
				}
#endif

				void *v;
				list_get_at(enemy->bitmaps, enemy->bitmap_index, &v);
				BITMAP *b = (BITMAP*)v;

				// ������ ��� ���� �޾����� ���� �޾������� ��������Ʈ�� ����Ѵ�.
				if (!strcasecmp(enemy->name, "BOSS")) {
					if (enemy->attacked_time != -1) {
						// ������ ��� �ι�° �̹����� ���� �޾������� �̹���
						//list_get_at(enemy->bitmaps, 1, &v);
						//b = (BITMAP*)v;
						void *out;
						hashtable_get(stage_res_table, "BOSS_DAMAGE", (void*)&out);
						ENEMY_RES *res = (ENEMY_RES*)out;

						void *v2;
						list_get_at(res->bitmaps, 0, &v2);
						b = (BITMAP*)v2;

						int diff = stage_elapsed_time - enemy->attacked_time;
						// Ư�� �ð��� �����ٸ� ���� ���� �ð� ����
						if (diff >= 50) {
							enemy->attacked_time = -1;
						}
					}
				}

				if (enemy->motion == MOVE_GUIDED || enemy->motion == MOVE_GUIDED_BACK) {
					enemy->w = b->w;
					enemy->h = b->h;

					// ���⸦ �Ʊ��⸦ ���Ͽ� ȸ����Ŵ
					fixed angle = ftofix(enemy->trace_angle * 128.0f / M_PI);

					pivot_sprite(stage_buf_bitmap, b,
						x + enemy->w / 2, y + enemy->h / 2,
						enemy->w / 2, enemy->h / 2, angle);

				}
				else if (enemy->motion == MOVE_ZOOMIN_LEFT) {
					BITMAP *b2 = resize_bitmap(b, enemy->bitmap_size);
					enemy->w = b2->w;
					enemy->h = b2->h;
					draw_sprite(stage_buf_bitmap, b2, x, y - (b2->h / 2));
					destroy_bitmap(b2);

				}
				else {
					enemy->w = b->w;
					enemy->h = b->h;
					draw_sprite(stage_buf_bitmap, b, x, y);
				}


				// ������ �ƴϰ� bitmap_index�� ������ �̸� ���� ����Ͽ� �ִϸ��̼� ȿ���� ��
				if (list_size(enemy->bitmaps) > 1 && strcasecmp(enemy->name, "BOSS")) {
					int diff = stage_elapsed_time - enemy->bitmap_show_time;
					if (diff >= 100) {
						if (enemy->bitmap_index >= (int)list_size(enemy->bitmaps) - 1) {
							enemy->bitmap_index = 0;
						}
						else {
							enemy->bitmap_index++;
						}
						enemy->bitmap_show_time = stage_elapsed_time;
					}
				}

				// ������ ó��
				float new_x = 0, new_y = 0;

				// ������ �������� ���� ���°� �ƴ϶�� ������ ó�� 
				if (!strcasecmp(enemy->name, "BOSS")) {
					// �������� ������ ���...
					if (enemy->motion == MOVE_FIXED) {
						new_x = enemy->x;
						new_y = enemy->y;

					}
					else {
						// ������ ȭ�� �����̸�..
						if (enemy->angry_time != -1) {
							// ��� ���� ����
							enemy->attack_pause = TRUE;

							float vy, vx;
							float angle;
							moving_guided(enemy->speed * 4, x, y, fox_x, fox_y,
								&new_x, &new_y, &vx, &vy, &angle);

							// ������������ ���� ��������� ȭ�� ������ �� �߾����� ����
							float dist = calc_distance(fox_x, fox_y, new_x, new_y);
							if (dist <= 50.0) {
								enemy->motion = MOVE_RIGHT_CENTER;
								enemy->angry_time = -1;
							}

						}
						else {
							// ���� ��������Ʈ�� ȭ��� ��ü �����Ͽ����� 
							// x�� �������� ����, y�� �������� ��/�Ʒ��� �̵�
							if (enemy->motion == MOVE_RIGHT_CENTER) {
								float vy, vx;
								float angle;
								float restore_x = SCREEN_W - enemy->w - 3;
								float restore_y = (SCREEN_H / 2) - (enemy->h / 2);
								moving_guided(enemy->speed * 4, x, y, restore_x, restore_y,
									&new_x, &new_y, &vx, &vy, &angle);

								// �Ÿ��� 5 �̳��� ��� ���������� ������
								float dist = calc_distance(restore_x, restore_y, new_x, new_y);
								if (dist <= 5.0) {
									new_x = restore_x;
									new_y = restore_y;

									// �ٽ� ���� ����
									enemy->attack_pause = FALSE;

									// ���� źȯ ��ȯ
									enemy->attack_method = !enemy->attack_method;
									enemy->attack_method_time = stage_elapsed_time;
									enemy->attack_odd_time = stage_elapsed_time;
									enemy->attack_odd = TRUE;

									if (enemy->attack_method == ATTACK_N_WAY) {
										// ������ ������ ����
										//enemy->motion = MOVE_FIXED;
										enemy->motion = MOVE_UP;
									}
									else {
										// ������ ������ ����
										enemy->motion = MOVE_UP;
									}
								}

							}
							else if (enemy->motion == MOVE_LEFT) {
								moving_left(enemy->speed, x, &new_x);
								new_y = y;

								if (new_x < SCREEN_W - enemy->w - 3) {
									new_x = SCREEN_W - enemy->w - 3;
									enemy->motion = MOVE_UP;
									enemy->attack_begin = TRUE;
								}

							}
							else if (enemy->motion == MOVE_UP) {
								moving_up(enemy->speed, y, &new_y);
								new_x = x;

								//if ( new_y <= (enemy->h/2)*-1 ) {
								if (new_y < -10) {
									enemy->motion = MOVE_DOWN;
								}

							}
							else if (enemy->motion == MOVE_DOWN) {
								moving_down(enemy->speed, y, &new_y);
								new_x = x;

								//if ( new_y > SCREEN_H-(enemy->h/2) ) {
								if (new_y > SCREEN_H - enemy->h + 10) {
									enemy->motion = MOVE_UP;
								}
							}
						}
					}

					// ������ �ƴ� ���� ó��
				}
				else {
					// �ܼ��� �������θ� �̵�
					if (enemy->motion == MOVE_LEFT) {
						moving_left(enemy->speed, x, &new_x);
						new_y = y;

#if 0
						// �ܼ��� ���������θ� �̵�
					}
					else if (enemy->motion == MOVE_RIGHT) {
						moving_right(enemy->speed, x, &new_x);
						new_y = y;
#endif

						// ���ο��� �Ʒ��� �̵�
					}
					else if (enemy->motion == MOVE_DOWN) {
						moving_down(enemy->speed, y, &new_y);
						new_x = x;

						// �Ʒ����� ���� �̵�
					}
					else if (enemy->motion == MOVE_UP) {
						moving_up(enemy->speed, y, &new_y);
						new_x = x;

						// ������ Ȯ�� �Ǹ鼭 �������θ� �̵�
					}
					else if (enemy->motion == MOVE_ZOOMIN_LEFT) {
						int diff = stage_elapsed_time - enemy->bitmap_size_time;
						if (diff >= 50) {
							enemy->bitmap_size += 0.05;
							if (enemy->bitmap_size >= 1.0) {
								enemy->bitmap_size = 1.0;
								enemy->zoom_done = TRUE;
							}
							enemy->bitmap_size_time = stage_elapsed_time;
						}

						moving_left(enemy->speed, x, &new_x);
						new_y = y;

					}
					else if (enemy->motion == MOVE_GUIDED) {
						if (enemy->trace == TRUE) {
							float vy, vx;
							float angle;

							enemy->trace_angle = calc_angle(fox_x, fox_y, x, y);

							moving_guided(enemy->speed, x, y, fox_x, fox_y,
								&new_x, &new_y, &vx, &vy, &angle);
							//enemy->trace_angle = angle;

							enemy->vx = vx;
							enemy->vy = vy;

							// �Ÿ��� 70 �̳��� ��� ��������̷� �̵�
							float dist = calc_distance(fox_x, fox_y, new_x, new_y);
							if (dist <= 70.0) {
								enemy->trace = FALSE;
							}

						}
						else {
							new_x = x + enemy->vx;
							new_y = y + enemy->vy;
						}

					}
					else if (enemy->motion == MOVE_GUIDED_BACK) {
						float vy, vx;
						float angle;

						enemy->trace_angle = calc_angle(fox_x, fox_y, x, y);

						moving_guided(enemy->speed, x, y, fox_x, fox_y,
							&new_x, &new_y, &vx, &vy, &angle);

						enemy->vx = vx;
						enemy->vy = vy;

						if (enemy->trace == TRUE) {
							// �Ÿ��� 100 �̳��� ��� ��ô ����
							float dist = calc_distance(fox_x, fox_y, new_x, new_y);
							if (dist <= 70.0) {
								enemy->trace = FALSE;

								// �⺻ �ӵ��� ����
								enemy->speed = 0.0;
								// ���ӵ� �ʱ�ȭ
								enemy->accel = 0.05;
							}

						}
						else {
							// ��ô ��尡 ����� ���¶�� ���� ����� �ݴ�� �̵�
							new_x = x + (enemy->vx * -1);
							new_y = y + (enemy->vy * -1);
						}

						// ������ �̵�
					}
					else if (enemy->motion == MOVE_WAVE) {
						float new_ang;
						moving_wave(enemy->speed, enemy->angle, x, enemy->base_y, &new_ang, &new_x, &new_y);
						enemy->angle = new_ang;

						// ���� ����Ÿ�� ����Ͽ� �̵�
					}
					else if (enemy->motion == MOVE_ORBIT) {
						void *out;
						hashtable_get(orbit_table, enemy->orbit_name, (void*)&out);
						List *orbit_list = (List*)out;

						int max_count = list_size(orbit_list);
#if 1
						if (enemy->orbit_index >= max_count - 1)
							enemy->orbit_index = max_count - 1;
#else
						if (enemy->orbit_index >= max_count - 1)
							enemy->orbit_index = 0;
#endif

						void *v2;
						list_get_at(orbit_list, enemy->orbit_index, &v2);
						ORBIT *orbit = (ORBIT*)v2;

						new_x = x + orbit->vx;
						new_y = y + orbit->vy;

						int diff = stage_elapsed_time - enemy->orbit_time;
						if (diff >= (int)orbit->time) {
							enemy->orbit_index++;
							enemy->orbit_time = stage_elapsed_time;
						}
					}

					enemy->speed += enemy->accel;
				}

				// ���Ⱑ ȭ�� ������ �̵��ߴٸ�..
				int out_of_range = FALSE;
				// �� ���� 1�� �̻��϶��� ��ũ�� �����˻縦 �Ѵ�.
				int diff = stage_elapsed_time - enemy->creation_time;
				if (diff > 1000) {
					if (new_x + enemy->w < 0.0) {
						out_of_range = TRUE;

					}
					else if (new_x >(float)SCREEN_W) {
						out_of_range = TRUE;

					}
					else if (new_y + enemy->h < 0.0) {
						out_of_range = TRUE;

					}
					else if (new_y >(float)SCREEN_H) {
						out_of_range = TRUE;

					}
					else {
						enemy->x = new_x;
						enemy->y = new_y;
					}
				}
				else {
					enemy->x = new_x;
					enemy->y = new_y;
				}

				// ������������ �������� �ƴϰ� ���� ���°� �ƴ϶�� �浹�˻�
				if (fox_explosion == -1 && fox_ghost_time == -1) {
					// ����κ��� �浹 ó������ �ƴҶ��� ������ ��ü �浹 �˻� �� ó��
					if (fox_collided_time == -1) {
						int fox_w = fox_bitmap->w;
						int fox_h = fox_bitmap->h;

						if (check_collision_bbox(fox_x, fox_y, fox_w, fox_h,
							enemy->x, enemy->y, enemy->w, enemy->h)) {

							fox_collided_time = stage_elapsed_time;
							fox_damage_flash_time = stage_elapsed_time;

							fox_energy_count -= enemy->life;

							// ������ ���� ����
							if (fox_energy_count <= 0) {
								// ���� ������ ��ü�� ����
								fox_life_count -= 1;

								fox_explosion = 0;
								fox_explosion_time = stage_elapsed_time;

								if (!strcasecmp(option.sound, "yes")) {
									play_sample(sound_effect[SOUND_FOX_EXPLOD], 255, 127, 1000, 0);
								}
							}
						}
					}
				}

				if (out_of_range == TRUE) {
					list_remove_all_cb(enemy->bitmaps, destroy_bitmap);
					list_destroy(enemy->bitmaps);

					if (enemy->angry_point_list != NULL)
						list_remove_all_cb(enemy->angry_point_list, free);

					if (enemy->fire_point_list != NULL)
						list_remove_all_cb(enemy->fire_point_list, free);

					if (enemy->fire_list != NULL)
						list_remove_all_cb(enemy->fire_list, free);

					list_iter_remove(&iter, NULL);
					free(enemy);
					enemy = NULL;
				}

			}

			// ������ ��� ���ݹ޾������� �� ��������Ʈ ǥ��
			if (enemy != NULL && !strcasecmp(enemy->name, "BOSS")) {
				// �� ��������Ʈ ���
				int j;
				ListIter iter2;
				void *v2;
				list_iter_init(&iter2, enemy->fire_list);
				while (list_iter_next(&iter2, (void*)&v) != CC_ITER_END)
				{
					ENEMY_FIRE *fire = (ENEMY_FIRE*)v;

					int diff = stage_elapsed_time - fire->time;
					if (diff >= 50) {
						int index = fire->index;
						index += 1;
						if (index >= fire_sprite_count - 1) {
							index = 0;
						}
						fire->index = index;
						fire->time = stage_elapsed_time;
					}

					BITMAP *b = fire_bitmaps[fire->index];

					int x = enemy->x + fire->x - (b->w / 2);
					int y = enemy->y + fire->y - b->h;

					x = x + boss_explod_vibration_x;
					y = y + boss_explod_vibration_y;

					draw_sprite(stage_buf_bitmap, b, x, y);
					//draw_trans_sprite(stage_buf_bitmap, b, x, y);
				}
			}
		}
	}
}

void process_fox_guided_missile(void)
{
	// �����������κ��� �߻�� ����ź �浹üũ ó��
	ListIter iter;
	void *v;
	list_iter_init(&iter, fox_guided_missile_list);
	while (list_iter_next(&iter, (void*)&v) != CC_ITER_END)
	{
		WEAPON_GUIDED_MISSILE *weapon = (WEAPON_GUIDED_MISSILE*)v;

		int w = weapon->bitmap->w;
		int h = weapon->bitmap->h;
		int power = weapon->power;

		float x = weapon->x;
		float y = weapon->y;

		int j;

		// ȭ�� ������ �̵� �ߴٸ� ����ź ����
		if (!check_collision_bbox(0, 0, SCREEN_W, SCREEN_H, x, y, w, h)) {
			list_iter_remove(&iter, NULL);
			free(weapon);
			weapon = NULL;

		}
		else {
			// ����ź�� ������ �浹 üũ
			for (j = 0; j < (int)list_size(enemy_list); j++) {
				void *v2;
				list_get_at(enemy_list, j, &v2);
				ENEMY *enemy = (ENEMY*)v2;

				// �� �������� �ƴϸ�..
				if (enemy->explosion == -1 && enemy->zoom_done == TRUE) {
					if (process_fox_weapon_attack(x, y, w, h, power, enemy) == TRUE) {
						list_iter_remove(&iter, NULL);
						free(weapon);
						weapon = NULL;
					}
				}
			}
		}

		// ���� �̻��� �̵� ó��
		if (weapon != NULL) {
			int w = weapon->bitmap->w;
			int h = weapon->bitmap->h;

			float x = weapon->x;
			float y = weapon->y;

			int j;

			int alive = FALSE;

			float tx=0, ty=0;

			// �߻��� 0.2�ʰ� ������ ���� ����
			int diff = stage_elapsed_time - weapon->launch_time;
			//if ( diff >= 200 ) {
			if (diff >= 0) {
				// �����ϴ� ���� ��� �ִ� ��� ���� �������� ����
				for (j = 0; j < (int)list_size(enemy_list); j++) {
					void *v;
					list_get_at(enemy_list, j, &v);
					ENEMY *enemy = (ENEMY*)v;

					if (enemy->explosion == -1) {
						if (weapon->target_enemy == enemy->number) {
							tx = enemy->x + enemy->weak_x;
							ty = enemy->y + enemy->weak_y + (enemy->weak_h / 2);
							alive = TRUE;
							break;
						}
					}
				}
			}

			// �����ϴ� ���� ������� ���� ���� ũ��(����)�� ����
			if (alive == FALSE) {
				tx = weapon->x + weapon->vx;
				ty = weapon->y + weapon->vy;
			}

			// ����ź�� ������ ���� ��ġ�� ������Ʈ
			float new_x, new_y;
			float new_vy, new_vx;
			float angle;

			moving_homing_missile(weapon->speed,
				weapon->x, weapon->y, tx, ty, weapon->angle, 6.0,
				&new_x, &new_y, &new_vx, &new_vy, &angle);


			weapon->x = new_x;
			weapon->y = new_y;

			weapon->vx = new_vx;
			weapon->vy = new_vy;

			weapon->angle = angle;

			weapon->speed += weapon->accel;

			//putpixel(stage_buf_bitmap, weapon->x, weapon->y, makecol(255,255,255));

			// �̻��� ���� �׸���
			if (weapon->smoke_begin == FALSE) {
				// �̻��� �߻��� 300ms ���Ŀ� ���� �׸��� ����
				int diff = stage_elapsed_time - weapon->smoke_time;
				if (diff >= 400) {
					weapon->smoke_begin = TRUE;
				}

				// ������ �����Ƿ� �̻��� ��ü�� ȸ����Ŵ
				// radians to allegro degrees
				fixed angle = ftofix(weapon->angle * 128.0f / M_PI);
				pivot_sprite(stage_buf_bitmap, weapon->bitmap, x + w / 2, y + h / 2, w / 2, h / 2, angle);

			}
			else {
				int smoke_index = weapon->smoke_index;
				BITMAP *smoke = guided_missile_smoke[smoke_index];

				int diff = stage_elapsed_time - weapon->smoke_time;
				if (diff >= 100) {
					if (smoke_index >= MAX_MISSILE_SMOKE - 1)
						smoke_index = MAX_MISSILE_SMOKE - 1;
					else
						smoke_index++;

					weapon->smoke_index = smoke_index;
					weapon->smoke_time = stage_elapsed_time;
				}

				int new_w = w + smoke->w;
				int new_h = MAX(h, smoke->h);

				// ������ ���� ��µǾ�� �ϹǷ�
				// �̻��� ��ü�� ������ ���� �׸��� ��ü �߽������� ȸ����Ŵ
				BITMAP *new_bitmap = create_bitmap(new_w, new_h);
				clear(new_bitmap);

				draw_sprite(new_bitmap, weapon->bitmap, new_w - w, (new_h - h) / 2);
				draw_sprite(new_bitmap, smoke, 0, (new_h - smoke->h) / 2);

				fixed angle = ftofix(weapon->angle * 128.0f / M_PI);
				pivot_sprite(stage_buf_bitmap, new_bitmap,
					x + w / 2, y + h / 2, smoke->w + (w / 2), new_h / 2, angle);

				destroy_bitmap(new_bitmap);
			}
		}
	}
}


// �Ʊ��⿡�� �߻�� ����� ������ �浹 üũ
int process_fox_weapon_attack(
	float weapon_x, float weapon_y, int weapon_w, int weapon_h,
	int weapon_power, ENEMY *enemy)
{
	int attacked = FALSE;
	if (!strcasecmp(enemy->name, "BOSS")) {
		// ���� ��ġ�� ���� y ���� ���̸� ���� �������� ħ��
		int weak_y1 = enemy->y + enemy->weak_y;
		int weak_y2 = enemy->y + enemy->weak_y + enemy->weak_h;

		if (weapon_y >= weak_y1 && weapon_y <= weak_y2) {
			if (check_collision_bbox(weapon_x, weapon_y, weapon_w, weapon_h,
				enemy->x + enemy->weak_x, enemy->y + enemy->weak_y, enemy->weak_w, enemy->weak_h)) {
				attacked = TRUE;
			}
		}
		else {
			if (check_collision_bbox(weapon_x, weapon_y, weapon_w, weapon_h,
				enemy->x, enemy->y, enemy->w, enemy->h)) {
				attacked = TRUE;
			}
		}
	}
	else {
		if (check_collision_bbox(weapon_x, weapon_y, weapon_w, weapon_h,
			enemy->x, enemy->y, enemy->w, enemy->h)) {
			attacked = TRUE;
		}
	}

	if (attacked == TRUE) {
		int attacked_weak = FALSE;

		// ������ ���� �޾������� ���� ����
		if (check_collision_bbox(weapon_x, weapon_y, weapon_w, weapon_h,
			enemy->x + enemy->weak_x, enemy->y + enemy->weak_y, enemy->weak_w, enemy->weak_h)) {
			attacked_weak = TRUE;
		}

		int life = enemy->life;
		if (attacked_weak) {
			life -= weapon_power;
			if (life <= 0) life = 0;

			enemy->life = life;
		}

		// �� ��ü ���ʹ� �Ҹ�
		if (life <= 0 && enemy->destroyed == FALSE) {
			enemy->destroyed = TRUE;

			// ������ ��� 3�� ���� ���� �������鼭 ���� ��Ŵ
			if (!strcasecmp(enemy->name, "BOSS")) {
				int k;
				for (k = 0; k<BITMAP_MAX_BOSS_EXPLOSION; k++) {
					boss_explosions[k] = -1;
				}

				boss_explod_begin_time = stage_elapsed_time;
				boss_explod_vibration_time = stage_elapsed_time;
				boss_explod_vibration_x = 0;
				boss_explod_vibration_y = 0;

				// ������ �ƴ� ��� �ٷ� ���� ��Ŵ
			}
			else {
				enemy->explosion = 0;
				enemy->explosion_time = stage_elapsed_time;

				void *v;
				list_get_at(enemy->bitmaps, enemy->bitmap_index, &v);
				BITMAP *b = (BITMAP*)v;

				create_effect_firework(enemy->x, enemy->y, 50, b);

				if (enemy->revenge_nway > 0) {
					launch_enemy_nway_weapon(enemy->x, enemy->y, 3, enemy->revenge_nway, TRUE);
				}

				if (strlen(enemy->item) > 0) {
					int index = 0;
					if (!strcasecmp(enemy->item, "BALKAN"))
						index = BITMAP_ITEM_BALKAN;
					else if (!strcasecmp(enemy->item, "LASER"))
						index = BITMAP_ITEM_LASER;
					else if (!strcasecmp(enemy->item, "GUIDED_MISSILE"))
						index = BITMAP_ITEM_GUIDED_MISSILE;
					else if (!strcasecmp(enemy->item, "MISSILE"))
						index = BITMAP_ITEM_MISSILE;

					int item_w = fox_item_bitmaps[index]->w;
					int item_h = fox_item_bitmaps[index]->h;

					enemy->x = enemy->x + (enemy->w - item_w) / 2;
					enemy->y = enemy->y + (enemy->h - item_h) / 2;
				}

				if (!strcasecmp(option.sound, "yes")) {
					play_sample(sound_effect[SOUND_ENEMY_EXPLOD], 255, 127, 1000, 0);
				}
			}

			// �� ���� ��� ����
		}
		else {
			// �����̸� ������ ó��
			float x, y;
			if (!strcasecmp(enemy->name, "BOSS")) {
				x = weapon_x;
				y = weapon_y;

			}
			else {
				x = enemy->x + (enemy->w / 2);
				y = enemy->y + (enemy->h / 2);
			}

			// ����ũ ȿ�� ����
			create_effect_spark_small(x, y, 10, 1, makecol(248, 208, 8));

			// ������ �Ʊ���κ��� ������ �޾����� ������ �����Ѵ�.
			enemy->attack_begin = TRUE;

			// ������ ���� �޾�����..
			if (attacked_weak) {
				enemy->attacked_time = stage_elapsed_time;
				attacked_enemy = enemy;
			}

			if (!strcasecmp(enemy->name, "BOSS")) {
				if (enemy->fire_max_count > 0 && enemy->fire_count < enemy->fire_max_count) {
					void *v;
					list_get_at(enemy->fire_point_list, enemy->fire_count, &v);
					int fire_point = *(int*)v;

					int life = enemy->max_life * fire_point / 100;
					if (enemy->life <= life) {
						// �� ����
						ENEMY_FIRE *fire = malloc(sizeof(ENEMY_FIRE));
						fire->time = stage_elapsed_time;
						fire->x = rand_number(40, enemy->w - 20);
						fire->y = rand_number(20, enemy->h - 20);
						fire->index = 0;
						list_add(enemy->fire_list, fire);

						enemy->fire_count += 1;
					}
				}

				// ȭ�� ���°� �ƴϰ�..
				if (enemy->angry_time == -1) {
					// ȭ�� �ִ� Ƚ���� 0���� ũ��, ���� ȭ�� Ƚ���� �ִ�ġ�� �Ѿ�� �ʾ�����..
					if (enemy->angry_max_count > 0 && enemy->angry_count < enemy->angry_max_count) {
						// �ִ� �������� ��% ���Ϸ� �������� ������ ���� �Ҹ��� ����.
						void *v;
						list_get_at(enemy->angry_point_list, enemy->angry_count, &v);
						int angry_point = *(int*)v;

						int life = enemy->max_life * angry_point / 100;
						if (enemy->life <= life) {
							if (!strcasecmp(option.sound, "yes")) {
								play_sample(sound_effect[SOUND_BOSS_SCREAM], 255, 127, 1000, 0);
							}

							enemy->angry_time = stage_elapsed_time;
							enemy->angry_count += 1;
						}
					}
				}
			}
		}

		return TRUE;

	}
	else {
		return FALSE;
	}
}

void process_fox_missile(void)
{
	// �̻��� �浹 ó��
	ListIter iter;
	void *v;
	list_iter_init(&iter, fox_missile_list);
	while (list_iter_next(&iter, (void*)&v) != CC_ITER_END)
	{
		WEAPON_MISSILE *weapon = (WEAPON_MISSILE*)v;

		int w = weapon->bitmap->w;
		int h = weapon->bitmap->h;
		int power = weapon->power;

		float x = weapon->x;
		float y = weapon->y;

		float *old_x = malloc(sizeof(float));
		*old_x = x;
		list_add(weapon->x_list, old_x);

		int j;

		// ȭ�� ������ �̵� �ߴٸ� �̻��� ����
		if (!check_collision_bbox(0, 0, SCREEN_W, SCREEN_H, x, y, w, h)) {
			list_iter_remove(&iter, NULL);
			list_remove_all_cb(weapon->x_list, free);
			list_destroy(weapon->x_list);
			free(weapon);
			weapon = NULL;

		}
		else {
			// ������� �浹 üũ
			for (j = 0; j < (int)list_size(enemy_list); j++) {
				void *v2;
				list_get_at(enemy_list, j, &v2);
				ENEMY *enemy = (ENEMY*)v2;

				// �� �������� �ƴϸ�..
				if (enemy->explosion == -1 && enemy->zoom_done == TRUE) {
					if (process_fox_weapon_attack(x, y, w, h, power, enemy) == TRUE) {
						list_iter_remove(&iter, NULL);
						list_remove_all_cb(weapon->x_list, free);
						list_destroy(weapon->x_list);
						free(weapon);
						weapon = NULL;
					}
				}
			}
		}

		if (weapon != NULL) {
			draw_sprite(stage_buf_bitmap, weapon->bitmap, x, y);

#if 1
			// �̻��� ���� �׸���
			if (weapon->smoke_begin == FALSE) {
				// �̻��� �߻��� 300ms ���Ŀ� ���� �׸��� ����
				int diff = stage_elapsed_time - weapon->smoke_time;
				if (diff >= 400) {
					weapon->smoke_begin = TRUE;
				}

			}
			else {
				int smoke_index = weapon->smoke_index;
				BITMAP *smoke = missile_smoke[smoke_index];

				int diff = stage_elapsed_time - weapon->smoke_time;
				if (diff >= 100) {
					if (smoke_index >= MAX_MISSILE_SMOKE - 1)
						smoke_index = MAX_MISSILE_SMOKE - 1;
					else
						smoke_index++;

					weapon->smoke_index = smoke_index;
					weapon->smoke_time = stage_elapsed_time;
				}

				draw_sprite(stage_buf_bitmap, smoke, x - smoke->w, y + (h / 2) - (smoke->h / 2));
			}

#else
			// �̻��� ���� �׸���
			int max_count = list_size(weapon->x_list);
			if (max_count > 0) {
				int j;
				j = max_count - 1;

				int num = 0;
				while (j >= 0) {
					// ������ �ֱ� x ��� ��ġ�� 10���� �׸�
					if (num > 10) break;

					void *v2;
					list_get_at(weapon->x_list, j, &v2);

					int tail_x = *((int*)v2);
					int k = 0;
					for (k = 0; k<10 - num; k++) {
						int x2 = -3 + rand() % 3;
						int y2 = -3 + rand() % 3;
						putpixel(stage_buf_bitmap, tail_x + x2, y + (h / 2) + y2, makecol(255, 255, num));
					}
					j--;
					num++;
				}
			}
#endif

			x += weapon->speed;

			weapon->x = x;
			weapon->y = y;

			weapon->speed += weapon->accel;
		}
	}
}

void process_enemy_energy_bar(void)
{
#if 0
	int x, y;
	x = SCREEN_W - fox_enemy_life_bar_bitmap->w;
	y = 0;
	draw_sprite(stage_buf_bitmap, fox_enemy_life_bar_bitmap, x, y);
#endif


	if (attacked_enemy == NULL) return;

	if (list_size(enemy_list) == 0) {
		attacked_enemy = NULL;
		return;
	}

	if (attacked_enemy->life <= 0) {
		attacked_enemy = NULL;

	}
	else {
		ENEMY *enemy = attacked_enemy;

		int max_bar_width = 50;
		int bar_height = 4;
		int spacing_x = 5;
		int spacing_y = 7;

		if (enemy->life < 0) enemy->life = 0;
		float energy_width = (float)enemy->life / (float)enemy->max_life *  (float)max_bar_width;

		int x = SCREEN_W - max_bar_width - spacing_x;
		int y = 0 + spacing_y;
		int x2 = x + max_bar_width;
		int y2 = y + bar_height;

		rectfill(stage_buf_bitmap, x, y, x2, y2, makecol(255, 255, 39));

		x2 = x + (int)energy_width;
		if (x2 <= x) x2 = x;
		rectfill(stage_buf_bitmap, x, y, x2, y2, makecol(250, 4, 4));

		draw_sprite(stage_buf_bitmap, fox_enemy_life_heart_bitmap, x - 6, y - 3);
	}
}

void process_fox_weapon(void)
{
	// ���� �浹 ó��
	ListIter iter;
	void *v;
	list_iter_init(&iter, fox_weapon_list);
	while (list_iter_next(&iter, (void*)&v) != CC_ITER_END)
	{
		WEAPON *weapon = (WEAPON*)v;

		float x = weapon->x;
		float y = weapon->y;
		int w = weapon->bitmap->w;
		int h = weapon->bitmap->h;
		int power = weapon->power;

		// �߻�� ����� ������ �浹 üũ
		ListIter iter2;
		void *v2;
		list_iter_init(&iter2, enemy_list);
		while (list_iter_next(&iter2, (void*)&v2) != CC_ITER_END)
		{
			ENEMY *enemy = (ENEMY*)v2;

			if (enemy->explosion == -1 && enemy->zoom_done == TRUE) {
				if (process_fox_weapon_attack(x, y, w, h, power, enemy) == TRUE) {
					list_iter_remove(&iter, NULL);
					free(weapon);
					weapon = NULL;
					break;
				}
			}
		}

		if (weapon != NULL) {
			//draw_sprite(stage_buf_bitmap, weapon->bitmap, x, y);
			// ��Ʈ�� ȸ�� ȿ���� �̺��ؼ� 2.5�� ������
			fixed angle = ftofix((weapon->angle * 2.5) * 128.0f / M_PI);
			pivot_sprite(stage_buf_bitmap, weapon->bitmap, x + w / 2, y + h / 2, w / 2, h / 2, angle);
			//rotate_sprite(stage_buf_bitmap, weapon->bitmap, x, y, angle);

			x += weapon->vx;
			y += weapon->vy;

			// -----------------------------------------------
			// ���Ⱑ ȭ�� ������ �̵��ߴٸ� ���� ����
			if (!check_collision_bbox(0, 0, SCREEN_W, SCREEN_H, x, y, w, h)) {
				list_iter_remove(&iter, NULL);
				free(weapon);
				weapon = NULL;

			}
			else {
				weapon->x = x;
				weapon->y = y;
			}
		}
	}
}

void process_enemy_weapon(void)
{
	// �� ���� �߻� ó��
	ListIter iter;
	void *v;
	list_iter_init(&iter, enemy_list);
	while (list_iter_next(&iter, (void*)&v) != CC_ITER_END)
	{
		ENEMY *enemy = (ENEMY*)v;

		// ������ ��� �� ���� ��ȯ
		if (!strcasecmp(enemy->name, "BOSS") && enemy->attack_begin == TRUE) {
			if (enemy->attack_method == ATTACK_N_WAY) {

				// 10�ʰ� ������ �� ���⸦ ������ 4�� ��ź���� ��ȯ
				int diff = stage_elapsed_time - enemy->attack_method_time;
				if (diff > 10000) {
					// �ϴ� ����ź ���⸦ �߻� ����
					enemy->attack_pause = TRUE;

					// ����ź�� ȭ�鿡 ��� ������� �� źȯ���� ��ȯ
					if (list_size(enemy_weapon_list) == 0) {
						enemy->attack_method = !enemy->attack_method;
						enemy->attack_method_time = stage_elapsed_time;
						enemy->attack_odd_time = stage_elapsed_time;
						enemy->attack_odd = TRUE;

						// �ٽ� ��/�Ʒ��� �̵�
						enemy->motion = MOVE_UP;

						// �� ���� �߻� ����
						enemy->attack_pause = FALSE;
					}
				}
			}
		}

		// ���� �߻簡 ��� ���� ���¸�..
		if (enemy->attack_pause == TRUE) continue;

		// ������ ���� �߻�� ���� ó�� (������ �ֹ���/�������� �ΰ����� �߻�)
		if (!strcasecmp(enemy->name, "BOSS") && enemy->attack_begin == TRUE) {
			// ������ �ı��� ���°� �ƴ϶�� ���� �߻� ó��
			if (enemy->destroyed == FALSE) {
				// �� ���� �߻�
				int firing_cycle;
				if (enemy->attack_method == ATTACK_N_WAY) {
					firing_cycle = 500;
				}
				else {
					firing_cycle = 200;
				}

				int diff = stage_elapsed_time - enemy->launch_time;
				if (diff >= firing_cycle) {
					// 4�� ��ź �߻�
					if (enemy->attack_method == ATTACK_4_WAY) {
						int j;
						for (j = 0; j<4; j++) {
							ENEMY_WEAPON *weapon = malloc(sizeof(ENEMY_WEAPON));

							void *out;
							hashtable_get(stage_res_table, "WEAPON", (void*)&out);
							ENEMY_RES *res = (ENEMY_RES*)out;

							void *v;
							list_get_at(res->bitmaps, 0, &v);
							BITMAP *b = (BITMAP*)v;

							weapon->bitmap = copy_sub_bitmap(b, 0, 0, b->w, b->h, 1.0);
							weapon->power = 1;
							weapon->x = enemy->x + enemy->weak_x + (enemy->weak_w / 2);
							weapon->y = enemy->y + enemy->weak_y + (enemy->weak_h / 2);
							weapon->speed = 4.0;

							float angle = 0;
							if (j == 0) {
								angle = deg2rad(-30);
							}
							else if (j == 1) {
								angle = deg2rad(-10);
							}
							else if (j == 2) {
								angle = deg2rad(10);
							}
							else if (j == 3) {
								angle = deg2rad(30);
							}

							weapon->vx = LookUpCos(angle) * weapon->speed * -1;
							weapon->vy = LookUpSin(angle) * weapon->speed;

							list_add(enemy_weapon_list, weapon);
						}

						// ����ź �߻�
					}
					else if (enemy->attack_method == ATTACK_N_WAY) {
						int x = enemy->x + enemy->weak_x + (enemy->weak_w / 2);
						int y = enemy->y + enemy->weak_y + (enemy->weak_h / 2);

						// 1�ʸ��� ���� źȯ Ʋ����
						int diff = stage_elapsed_time - enemy->attack_odd_time;
						if (diff > 1000) {
							enemy->attack_odd = !enemy->attack_odd;
							enemy->attack_odd_time = stage_elapsed_time;
						}

						// 20�� źȯ �߻�
						launch_enemy_nway_weapon(x, y, 3, 30, enemy->attack_odd);

					}

					enemy->launch_time = stage_elapsed_time;
				}

				// ����ź�϶��� ���� ���⸦ �߻����� �ʴ´�.
				// ����ź ���ϱ⵵ �ٻ۵�.. ����
				if (enemy->attack_method != ATTACK_N_WAY) {
					// ���� ���� �߻�
					diff = stage_elapsed_time - enemy->launch_sub_time;

					// ������ ȭ�� Ƚ���� �ü��� �߻� �ֱ�� ��������.
					// �ƹ��� ���� �߻��ص� 500ms ���Ϸδ� ���´�.
					int firing_cycle_time = 3000;

					if (enemy->angry_count > 0) {
						int step = (3000 - 500) / enemy->angry_max_count;
						firing_cycle_time = 3000 - step * enemy->angry_count;
					}

					if (diff >= firing_cycle_time) {
						ENEMY_WEAPON *weapon = malloc(sizeof(ENEMY_WEAPON));

						void *out;
						hashtable_get(stage_res_table, "BOSS_WEAPON", (void*)&out);
						ENEMY_RES *res = (ENEMY_RES*)out;

						void *v;
						list_get_at(res->bitmaps, 0, &v);
						BITMAP *b = (BITMAP*)v;

						weapon->bitmap = copy_sub_bitmap(b, 0, 0, b->w, b->h, 1.0);
						weapon->power = 5;
						weapon->x = enemy->x - enemy->w / 2;
						weapon->y = enemy->y + enemy->h - 20;
						weapon->speed = 4.0;

						float angle = deg2rad(0);
						weapon->vx = LookUpCos(angle) * weapon->speed * -1;
						weapon->vy = LookUpSin(angle) * weapon->speed;

						list_add(enemy_weapon_list, weapon);

						enemy->launch_sub_time = stage_elapsed_time;

						if (!strcasecmp(option.sound, "yes")) {
							play_sample(sound_effect[SOUND_BOSS_LASER], 255, 127, 1000, 0);
						}
					}
				}
			}

		}
		else {
			// ���� ȭ��� ��Ÿ������ ���� �߻�
			if (check_collision_bbox(0, 0, SCREEN_W, SCREEN_H,
				enemy->x, enemy->y, enemy->w, enemy->h)) {

				// �Ʊ��⸦ �����ϴ� ���� ��� ������ ���߸� ���̻� �߻���������
				int shoot = TRUE;
				if (enemy->motion == MOVE_GUIDED && enemy->trace == FALSE)
					shoot = FALSE;

				// �����κ��� ������ �߻�� �ð����κ��� x�� �̻� ���̳��� �� �߻�
				if (enemy->reload_time != -1) {
					int diff = stage_elapsed_time - enemy->launch_time;
					if (diff >= enemy->reload_time && shoot == TRUE) {
						ENEMY_WEAPON *weapon = malloc(sizeof(ENEMY_WEAPON));

						void *out;
						hashtable_get(stage_res_table, "WEAPON", (void*)&out);
						ENEMY_RES *res = (ENEMY_RES*)out;

						void *v;
						list_get_at(res->bitmaps, 0, &v);
						BITMAP *b = (BITMAP*)v;

						weapon->bitmap = copy_sub_bitmap(b, 0, 0, b->w, b->h, 1.0);
						weapon->power = 1;

						// ���� ���� �߻� ��ġ�� ���� �߽����� �������� ��� 
						// �Ʊ���� ���ϰ� ȸ�� ��Ų ��ġ�� ����
						if (enemy->motion == MOVE_GUIDED || enemy->motion == MOVE_GUIDED_BACK) {
							float cx = enemy->x + (enemy->w / 2);
							float cy = enemy->y + (enemy->h / 2);

							float x = enemy->x;
							float y = enemy->y + (enemy->h / 2);

							float new_x, new_y;
							calc_rotation(cx, cy, enemy->trace_angle, x, y, &new_x, &new_y);

							weapon->x = new_x;
							weapon->y = new_y;

						}
						else {
							weapon->x = enemy->x;
							weapon->y = enemy->y + (enemy->h / 2);
						}

						weapon->speed = 5;

						float new_x, new_y;
						float vy, vx;
						float angle;

						moving_guided(weapon->speed, weapon->x, weapon->y, fox_x, fox_y,
							&new_x, &new_y, &vx, &vy, &angle);

						weapon->vx = vx;
						weapon->vy = vy;

						list_add(enemy_weapon_list, weapon);

						enemy->launch_time = stage_elapsed_time;
					}
				}
			}
		}
	}

	// �����κ��� �߻�� ���� �̵� �� �浹 ó��
	{
		ListIter iter;
		void *v;
		list_iter_init(&iter, enemy_weapon_list);
		while (list_iter_next(&iter, (void*)&v) != CC_ITER_END)
		{
			ENEMY_WEAPON *weapon = (ENEMY_WEAPON*)v;

			float weapon_x = weapon->x;
			float weapon_y = weapon->y;
			int weapon_w = weapon->bitmap->w;
			int weapon_h = weapon->bitmap->h;
			int power = weapon->power;
			int direction = weapon->direction;

			// �߻�� ����� ������������ �浹 üũ
			int x = fox_x;
			int y = fox_y;
			int w = fox_bitmap->w;
			int h = fox_bitmap->h;

			draw_sprite(stage_buf_bitmap, weapon->bitmap, weapon_x, weapon_y);

			// ������������ �������� �ƴϰ� ���� ���°� �ƴ϶�� �浹�˻�
			if (fox_explosion == -1 && fox_ghost_time == -1) {
				int overlap = check_collision_bbox(weapon_x, weapon_y, weapon_w, weapon_h, x + 4, y + 4, w - 4, h - 4);

				// ������ ������ ���۵Ǿ��ٸ� ���� ���⸦ �¾Ƶ� ������ ���Ұ� ���� ����
				if (boss_explod_begin_time != -1) {
					overlap = FALSE;
				}

				// ���� ���⸦ �¾Ҵٸ�..
				if (overlap) {
					fox_attacked_time = stage_elapsed_time;
					fox_damage_flash_time = stage_elapsed_time;
					fox_energy_count -= power;

					// ������ ���� ����
					if (fox_energy_count <= 0) {
						// ���� ������ ��ü�� ����
						fox_life_count -= 1;

						fox_explosion = 0;
						fox_explosion_time = stage_elapsed_time;

						//create_effect_spark(x+w/2, y+h/2, 400);

						if (!strcasecmp(option.sound, "yes")) {
							play_sample(sound_effect[SOUND_FOX_EXPLOD], 255, 127, 1000, 0);
						}

					}

					// ���� ����
					list_iter_remove(&iter, NULL);
					destroy_bitmap(weapon->bitmap);
					free(weapon);
					weapon = NULL;
				}
			}

			if (weapon != NULL) {
				// ���� ���Ⱑ ȭ�� ������ �̵��ߴٸ� ���� ����
				if (!check_collision_bbox(0, 0, SCREEN_W, SCREEN_H, weapon_x, weapon_y, weapon_w, weapon_h)) {
					list_iter_remove(&iter, NULL);
					destroy_bitmap(weapon->bitmap);
					free(weapon);

				}
				else {
					weapon_x += weapon->vx;
					weapon_y += weapon->vy;
					weapon->x = weapon_x;
					weapon->y = weapon_y;
				}
			}
		}
	}
}


void free_enemy_advent()
{
	ListIter iter2;
	void *v2;
	list_iter_init(&iter2, enemy_advent_list);
	while (list_iter_next(&iter2, (void*)&v2) != CC_ITER_END)
	{
		ENEMY_ADVENT *e = (ENEMY_ADVENT*)v2;
		if (e->angry_point_list != NULL) {
			list_remove_all_cb(e->angry_point_list, free);
			list_destroy(e->angry_point_list);
		}

		if (e->fire_point_list != NULL) {
			list_remove_all_cb(e->fire_point_list, free);
			list_destroy(e->fire_point_list);
		}
	}

	list_remove_all_cb(enemy_advent_list, free);
	list_destroy(enemy_advent_list);
}

void load_enemy_advent(int stage)
{
	list_new(&enemy_advent_list);

	XMLDoc doc;
	XMLDoc_init(&doc);

#ifdef USE_DATAFILE
	char *xml = stage_data[DAT_STAGE_MAP].dat;
#else
	char tmp[1024];
	sprintf(tmp, "RES/STAGE%d/STAGE.MAP", stage);
	//sprintf(tmp, "RES/STAGE%d/BOSS.MAP", stage);
	char *xml;
	read_text_file(tmp, &xml);
#endif

	if (!XMLDoc_parse_buffer_DOM(xml, NULL, &doc)) {
		err("Couldn't load xml resources!");
	}

	XMLNode* root = XMLDoc_root(&doc);

	XMLNode* node;
	node = XMLNode_get_child(root, 0);
	while (1) {
		if (node == NULL) break;

		if (node->tag_type == TAG_COMMENT) {
			node = XMLNode_next_sibling(node);
			continue;
		}

		ENEMY_ADVENT *e = malloc(sizeof(ENEMY_ADVENT));
		e->time = atoi(node->tag);
		e->reload_time = -1;
		e->motion = -1;
		e->revenge_nway = 0;
		e->accel = 0.0;
		e->angry_point_list = NULL;
		e->fire_point_list = NULL;
		sprintf(e->orbit_name, "%s", "");
		sprintf(e->side, "RIGHT");

		XMLNode* node2;
		node2 = XMLNode_get_child(node, 0);
		while (1) {
			if (node2 == NULL) break;

			if (node2->tag_type == TAG_COMMENT) {
				node2 = XMLNode_next_sibling(node2);
				continue;
			}

			if (!strcasecmp(node2->tag, "NAME")) {
				sprintf(e->name, "%s", node2->text);

			}
			else if (!strcasecmp(node2->tag, "SIDE")) {
				sprintf(e->side, "%s", node2->text);

			}
			else if (!strcasecmp(node2->tag, "LOCATION")) {
				e->first_location = atof(node2->text);

			}
			else if (!strcasecmp(node2->tag, "ENERGY")) {
				e->life = atoi(node2->text);

			}
			else if (!strcasecmp(node2->tag, "SPEED")) {
				e->speed = atof(node2->text);

			}
			else if (!strcasecmp(node2->tag, "ACCEL")) {
				e->accel = atof(node2->text);

			}
			else if (!strcasecmp(node2->tag, "FIRING_CYCLE")) {
				if (node2->text != NULL) {
					e->reload_time = atoi(node2->text);
				}

			}
			else if (!strcasecmp(node2->tag, "ANGRY")) {
				if (node2->text != NULL) {
					list_new(&e->angry_point_list);
					string_to_list(node2->text, " ", e->angry_point_list);
				}

			}
			else if (!strcasecmp(node2->tag, "FIRE")) {
				if (node2->text != NULL) {
					list_new(&e->fire_point_list);
					string_to_list(node2->text, " ", e->fire_point_list);
				}

			}
			else if (!strcasecmp(node2->tag, "ITEM")) {
				if (node2->text != NULL) {
					sprintf(e->item, "%s", node2->text);
				}
				else {
					sprintf(e->item, "%s", "");
				}

			}
			else if (!strcasecmp(node2->tag, "MOTION")) {
				if (node2->text != NULL) {
					if (!strcasecmp("LEFT", node2->text))
						e->motion = MOVE_LEFT;
#if 0
					if (!strcasecmp("RIGHT", node2->text))
						e->motion = MOVE_RIGHT;
#endif
					else if (!strcasecmp("BOTTOM", node2->text))
						e->motion = MOVE_DOWN;
					else if (!strcasecmp("TOP", node2->text))
						e->motion = MOVE_UP;
					else if (!strcasecmp("ZOOMIN_LEFT", node2->text))
						e->motion = MOVE_ZOOMIN_LEFT;
#if 0
					else if (!strcasecmp("ZOOMIN_RIGHT", node2->text))
						e->motion = MOVE_ZOOMIN_RIGHT;
#endif
					else if (!strcasecmp("GUIDED", node2->text))
						e->motion = MOVE_GUIDED;
					else if (!strcasecmp("GUIDED_BACK", node2->text))
						e->motion = MOVE_GUIDED_BACK;
					else if (!strcasecmp("WAVE", node2->text))
						e->motion = MOVE_WAVE;
					else if (!strncasecmp("ORBIT", node2->text, 5)) {
						e->motion = MOVE_ORBIT;

						char *name;
						List *tokens;
						list_new(&tokens);
						int c = string_to_list(node2->text, ".", tokens);
						list_get_at(tokens, 1, &name);
						sprintf(e->orbit_name, "%s", name);
						list_destroy(tokens);
					}
				}

			}
			else if (!strcasecmp(node2->tag, "REVENGE_NWAY")) {
				if (node2->text != NULL) {
					e->revenge_nway = atoi(node2->text);
				}
			}


			node2 = XMLNode_next_sibling(node2);
		}

		list_add(enemy_advent_list, e);

		node = XMLNode_next_sibling(node);

		// ---------------------------------------------
		if (!strcasecmp(e->name, "BOSS")) {
			// ���� ���� �ð� 5������ warning �޼��� ���
			boss_warning_time = e->time - 5000;
			boss_warning_flag = FALSE;

#ifdef _DEBUG
			if (e->life > 5)
				e->life = 0;
#endif
		}
	}

	XMLDoc_free(&doc);

#ifndef USE_DATAFILE
	free(xml);
#endif
}

void free_fox_weapon(void)
{
	HashTableIter iter;
	hashtable_iter_init(&iter, fox_weapon_table);
	TableEntry *entry;

	while (hashtable_iter_next(&iter, &entry) != CC_ITER_END) {
		char const *key = entry->key;

		void *out;
		hashtable_get(fox_weapon_table, (void*)key, (void*)&out);
		List *list = (List*)out;

		ListIter iter2;
		void *v2;
		list_iter_init(&iter2, list);
		while (list_iter_next(&iter2, (void*)&v2) != CC_ITER_END)
		{
			FOX_WEAPON *weapon = (FOX_WEAPON*)v2;
			destroy_bitmap(weapon->bitmap);
		}

		list_remove_all_cb(list, free);
	}

	hashtable_remove_all(fox_weapon_table);
	hashtable_destroy(fox_weapon_table);
	fox_weapon_table = NULL;
}

void load_fox_weapon(int stage)
{
	char tmp[1024];

	XMLDoc doc;
	XMLDoc_init(&doc);

#ifdef USE_DATAFILE
	char *xml = stage_data[DAT_WEAPON_XML].dat;
#else
	sprintf(tmp, "RES/STAGE%d/WEAPON.XML", stage);
	char *xml;
	read_text_file(tmp, &xml);
#endif

	if (!XMLDoc_parse_buffer_DOM(xml, NULL, &doc)) {
		err("Couldn't load xml resources!");
	}

#ifdef USE_DATAFILE
	BITMAP *b = stage_data[DAT_FOX_BMP].dat;
#else
	sprintf(tmp, "RES/STAGE%d/FOX.BMP", stage);
	BITMAP *b = load_bitmap(tmp, NULL);
#endif

	hashtable_new(&fox_weapon_table);

	// --------------------------------

	XMLNode* root = XMLDoc_root(&doc);

	XMLNode* node;
	node = XMLNode_get_child(root, 0);
	while (1) {
		if (node == NULL) break;

		XMLNode* node2;
		node2 = XMLNode_get_child(node, 0);
		while (1) {
			if (node2 == NULL) break;

			List *weapon_list;
			list_new(&weapon_list);

			//printf("%s\n", node2->tag);

			XMLNode* node3;
			node3 = XMLNode_get_child(node2, 0);
			while (1) {
				if (node3 == NULL) break;

				//printf("\t%s\n", node3->tag);
				FOX_WEAPON *weapon = malloc(sizeof(FOX_WEAPON));

				XMLNode* node4;
				node4 = XMLNode_get_child(node3, 0);
				while (1) {
					if (node4 == NULL) break;

					if (!strcasecmp(node4->tag, "BITMAP")) {
						int x, y, w, h;

						sscanf(node4->text, "%d %d %d %d", &x, &y, &w, &h);
						weapon->bitmap = copy_sub_bitmap(b, x, y, w, h, 1.0);

					}
					else if (!strcasecmp(node4->tag, "POWER")) {
						weapon->power = atoi(node4->text);

					}
					else if (!strcasecmp(node4->tag, "FIRING_CYCLE")) {
						weapon->reload_time = atoi(node4->text);

					}
					else if (!strcasecmp(node4->tag, "SPEED")) {
						weapon->speed = atoi(node4->text);

					}
					else if (!strcasecmp(node4->tag, "S_LAUNCH")) {
						weapon->launch_count = atoi(node4->text);
					}

					//printf("\t\t%s\n", node4->tag);

					node4 = XMLNode_next_sibling(node4);
				}

				list_add(weapon_list, weapon);

				node3 = XMLNode_next_sibling(node3);
			}

			// BALKAN, LASER ..
			char *type = strdup(node2->tag);
			hashtable_add(fox_weapon_table, type, weapon_list);

			node2 = XMLNode_next_sibling(node2);
		}

		node = XMLNode_next_sibling(node);
	}

	XMLDoc_free(&doc);

#ifndef USE_DATAFILE
	if (b != NULL)
		destroy_bitmap(b);

	free(xml);
#endif
}

void load_orbit_xml(char *file_name)
{
	XMLDoc doc;
	XMLDoc_init(&doc);

	if (!XMLDoc_parse_file_DOM(file_name, &doc)) {
		printf("Error while loading %s\n", file_name);
		allegro_exit();
		exit(1);
	}

	hashtable_new(&orbit_table);

	// --------------------------------
	XMLNode* root = XMLDoc_root(&doc);

	XMLNode* node;
	node = XMLNode_get_child(root, 0);
	while (1) {
		if (node == NULL) break;

		List *orbit_list;
		list_new(&orbit_list);

		char **items = NULL;
		int c = split(node->text, '\n', &items);
		int i;
		for (i = 0; i < c; i++) {
			char *line = trim(*(items + i));
			if (strlen(line) > 0) {
				float vx, vy;
				unsigned int time;
				sscanf(line, "%f %f %d", &vx, &vy, &time);

				ORBIT *orbit = malloc(sizeof(ORBIT));
				orbit->vx = vx;
				orbit->vy = vy;
				orbit->time = time;
				list_add(orbit_list, orbit);
			}

			free(*(items + i));
		}

		free(items);

		char *key = strdup(node->tag);
		hashtable_add(orbit_table, key, orbit_list);

		node = XMLNode_next_sibling(node);
	}

	XMLDoc_free(&doc);
}

void show_popup_message(char *msg, int show_emblem, long show_time)
{
	char kssm_str[1024];
	han_conv(KS2TG, msg, kssm_str);

	fox_popup_message_time = show_time;
	fox_popup_message_show_time = stage_elapsed_time;

	fox_popup_show_emblem = show_emblem;

	// --------------------------------------
	List *lines;
	list_new(&lines);

	int c = string_to_list(kssm_str, "\n", lines);

	// -----------------------------------
	int width = 0;
	int height = 0;

	ListIter iter;
	void *v;
	list_iter_init(&iter, lines);
	int i = 0;
	while (list_iter_next(&iter, (void*)&v) != CC_ITER_END)
	{
		char *line = (char*)v;
		int x = 0;
		int y = (i * 16);

		width = MAX((int)strlen(line) * 8, width);
		height += 16;

		i++;
	}

	if (stage_popup_bitmap != NULL) {
		destroy_bitmap(stage_popup_bitmap);
		stage_popup_bitmap = NULL;
	}

	stage_popup_bitmap = create_bitmap(width, height);
	clear(stage_popup_bitmap);

	// -----------------------------------
	ListIter iter2;
	void *v2;
	list_iter_init(&iter2, lines);
	i = 0;
	while (list_iter_next(&iter2, (void*)&v2) != CC_ITER_END)
	{
		char *line = (char*)v2;
		int x = stage_popup_bitmap->w / 2 - (strlen(line) * 8) / 2;
		int y = (i * 16);

		al_han_textout(stage_popup_bitmap, line, x, y, makecol(255, 0, 0));
		al_han_textout(stage_popup_bitmap, line, x + 1, y + 1, makecol(255, 255, 255));

		i++;
	}

	// ------------------------------------
	list_remove_all_cb(lines, free);
	list_destroy(lines);
}

void hide_popup_message(void)
{
	if (stage_popup_bitmap != NULL) {
		destroy_bitmap(stage_popup_bitmap);
		stage_popup_bitmap = NULL;
	}

	fox_popup_message_time = -1;
	fox_popup_message_show_time = -1;
	fox_popup_show_emblem = FALSE;
}

void process_popup_message(void)
{
	if (stage_popup_bitmap == NULL) return;

	int x = SCREEN_W / 2 - stage_popup_bitmap->w / 2;
	int y = SCREEN_H / 2 - stage_popup_bitmap->h / 2;

	// �׻� ���
	if (fox_popup_message_time == 0) {
		if (fox_popup_show_emblem == TRUE && stage_emblem_bitmap != NULL) {
			int x2 = (SCREEN_W / 2) - (stage_emblem_bitmap->w / 2);
			int y2 = (SCREEN_H / 2) - (stage_emblem_bitmap->h / 2);
			draw_sprite(stage_bitmap, stage_emblem_bitmap, x2, y2);
		}

		draw_sprite(stage_bitmap, stage_popup_bitmap, x, y);

		// ���ѽð����� ���
	}
	else if (fox_popup_message_show_time != -1) {
		long diff = stage_elapsed_time - fox_popup_message_show_time;
		if (diff < fox_popup_message_time) {
			if (fox_popup_show_emblem == TRUE && stage_emblem_bitmap != NULL) {
				int x2 = (SCREEN_W / 2) - (stage_emblem_bitmap->w / 2);
				int y2 = (SCREEN_H / 2) - (stage_emblem_bitmap->h / 2);
				draw_sprite(stage_bitmap, stage_emblem_bitmap, x2, y2);
			}

			draw_sprite(stage_bitmap, stage_popup_bitmap, x, y);

		}
		else {
			hide_popup_message();
		}
	}
}

void show_debug()
{
	show_available_memory(stage_buf_bitmap, 0, 0);

	char txt[256];
	sprintf(txt, "F:%02d/T:%dms", fps, stage_elapsed_time);
	int x = text_length(font, txt) / 2;
	textout_ex(stage_buf_bitmap, font, txt, 0, 8, makecol(255, 255, 255), -1);
}

void process_debug_message(void)
{
	int fps = 0;
	char life[256];
	sprintf(life, "L: %03d / F: %02d / E: %02d / W: %02d / H: %02d",
		fox_energy_count, fps, list_size(enemy_list),
		list_size(fox_weapon_list), list_size(fox_guided_missile_list));
	textout_centre_ex(stage_buf_bitmap, font, life, SCREEN_W / 2, SCREEN_H - 12, makecol(255, 0, 0), -1);
}

int load_bitmaps(int stage)
{
#ifdef USE_DATAFILE
	BITMAP *b;

	b = stage_data[DAT_FOX_BMP].dat;

	fox_bitmaps[BITMAP_FOX_GO] = copy_sub_bitmap(b, 0, 0, 36, 11, 1.0);
	fox_bitmaps[BITMAP_FOX_DOWN] = copy_sub_bitmap(b, 0, 12, 36, 17, 1.0);
	fox_bitmaps[BITMAP_FOX_UP] = copy_sub_bitmap(b, 0, 30, 36, 16, 1.0);

	fox_light_bitmaps[BITMAP_FOX_GO] = copy_sub_bitmap(b, 81, 0, 36, 11, 1.0);
	fox_light_bitmaps[BITMAP_FOX_DOWN] = copy_sub_bitmap(b, 81, 12, 36, 17, 1.0);
	fox_light_bitmaps[BITMAP_FOX_UP] = copy_sub_bitmap(b, 81, 30, 36, 16, 1.0);

	fox_life_heart_bitmap = copy_sub_bitmap(b, 3, 52, 9, 8, 1.0);
	fox_life_small_bitmap = copy_sub_bitmap(b, 16, 52, 24, 7, 1.0);
	fox_enemy_life_heart_bitmap = copy_sub_bitmap(b, 60, 65, 46, 14, 1.0);

	fox_item_bitmaps[BITMAP_ITEM_BALKAN] = copy_sub_bitmap(b, 2, 67, 19, 14, 1.0);
#if 0
	fox_item_bitmaps[BITMAP_ITEM_BALKAN2] = copy_sub_bitmap(b, 15, 71, 11, 10, 1.0);
	fox_item_bitmaps[BITMAP_ITEM_BALKAN3] = copy_sub_bitmap(b, 27, 71, 11, 10, 1.0);
#endif

	fox_item_bitmaps[BITMAP_ITEM_LASER] = copy_sub_bitmap(b, 22, 67, 19, 14, 1.0);
#if 0
	fox_item_bitmaps[BITMAP_ITEM_LASER2] = copy_sub_bitmap(b, 15, 83, 11, 10, 1.0);
	fox_item_bitmaps[BITMAP_ITEM_LASER3] = copy_sub_bitmap(b, 27, 83, 11, 10, 1.0);
#endif

	fox_item_bitmaps[BITMAP_ITEM_GUIDED_MISSILE] = copy_sub_bitmap(b, 2, 83, 19, 14, 1.0);
	fox_item_bitmaps[BITMAP_ITEM_MISSILE] = copy_sub_bitmap(b, 22, 83, 19, 14, 1.0);

	BITMAP *b2 = copy_sub_bitmap(b, 108, 118, 8, 5, 1.0);
	int i;
	for (i = 0; i<MAX_MISSILE_SMOKE; i++) {
		BITMAP *b3 = resize_horizontal_bitmap(b2, 1.0 + ((float)i*0.1));
		missile_smoke[i] = b3;
	}
	destroy_bitmap(b2);

	b2 = copy_sub_bitmap(b, 108, 103, 8, 5, 1.0);
	for (i = 0; i<MAX_MISSILE_SMOKE; i++) {
		BITMAP *b3 = resize_horizontal_bitmap(b2, 1.0 + ((float)i*0.1));
		guided_missile_smoke[i] = b3;
	}
	destroy_bitmap(b2);

	b = stage_data[DAT_EXPLO9_BMP].dat;
	enemy_explosion_sprite_count =
		load_tile_bitmaps(explosion_bitmaps, b, 4, 2, 1.4);

	b = stage_data[DAT_EXPLO3_BMP].dat;
	boss_explosion_sprite_count =
		load_tile_bitmaps(boss_explosion_bitmaps, b, 5, 3, 1.0);

	b = stage_data[DAT_EXPLO2_BMP].dat;
	fox_explosion_sprite_count =
		load_tile_bitmaps(fox_explosion_bitmaps, b, 5, 6, 0.5);

	b = stage_data[DAT_FIRE_BMP].dat;
	fire_sprite_count = load_tile_bitmaps(fire_bitmaps, b, 16, 1, 0.6);

#else
	BITMAP *b;

	char tmp[1024];
	sprintf(tmp, "RES/STAGE%d/FOX.BMP", stage);
	b = load_bitmap(tmp, NULL);

	fox_bitmaps[BITMAP_FOX_GO] = copy_sub_bitmap(b, 0, 0, 36, 11, 1.0);
	fox_bitmaps[BITMAP_FOX_DOWN] = copy_sub_bitmap(b, 0, 12, 36, 17, 1.0);
	fox_bitmaps[BITMAP_FOX_UP] = copy_sub_bitmap(b, 0, 30, 36, 16, 1.0);

	fox_light_bitmaps[BITMAP_FOX_GO] = copy_sub_bitmap(b, 81, 0, 36, 11, 1.0);
	fox_light_bitmaps[BITMAP_FOX_DOWN] = copy_sub_bitmap(b, 81, 12, 36, 17, 1.0);
	fox_light_bitmaps[BITMAP_FOX_UP] = copy_sub_bitmap(b, 81, 30, 36, 16, 1.0);

	fox_life_heart_bitmap = copy_sub_bitmap(b, 3, 52, 9, 8, 1.0);
	fox_life_small_bitmap = copy_sub_bitmap(b, 16, 52, 24, 7, 1.0);
	fox_enemy_life_heart_bitmap = copy_sub_bitmap(b, 60, 65, 46, 14, 1.0);

	fox_item_bitmaps[BITMAP_ITEM_BALKAN] = copy_sub_bitmap(b, 2, 67, 19, 14, 1.0);
#if 0
	fox_item_bitmaps[BITMAP_ITEM_BALKAN2] = copy_sub_bitmap(b, 15, 71, 11, 10, 1.0);
	fox_item_bitmaps[BITMAP_ITEM_BALKAN3] = copy_sub_bitmap(b, 27, 71, 11, 10, 1.0);
#endif

	fox_item_bitmaps[BITMAP_ITEM_LASER] = copy_sub_bitmap(b, 22, 67, 19, 14, 1.0);
#if 0
	fox_item_bitmaps[BITMAP_ITEM_LASER2] = copy_sub_bitmap(b, 15, 83, 11, 10, 1.0);
	fox_item_bitmaps[BITMAP_ITEM_LASER3] = copy_sub_bitmap(b, 27, 83, 11, 10, 1.0);
#endif

	fox_item_bitmaps[BITMAP_ITEM_GUIDED_MISSILE] = copy_sub_bitmap(b, 2, 83, 19, 14, 1.0);
	fox_item_bitmaps[BITMAP_ITEM_MISSILE] = copy_sub_bitmap(b, 22, 83, 19, 14, 1.0);

	BITMAP *b2 = copy_sub_bitmap(b, 108, 118, 8, 5, 1.0);
	int i;
	for (i = 0; i<MAX_MISSILE_SMOKE; i++) {
		BITMAP *b3 = resize_horizontal_bitmap(b2, 1.0 + ((float)i*0.1));
		missile_smoke[i] = b3;
	}
	destroy_bitmap(b2);

	b2 = copy_sub_bitmap(b, 108, 103, 8, 5, 1.0);
	for (i = 0; i<MAX_MISSILE_SMOKE; i++) {
		BITMAP *b3 = resize_horizontal_bitmap(b2, 1.0 + ((float)i*0.1));
		guided_missile_smoke[i] = b3;
	}
	destroy_bitmap(b2);

	destroy_bitmap(b);

	// -------------------------------
	sprintf(tmp, "RES/STAGE%d/EXPLO9.BMP", stage);
	b = load_bitmap(tmp, NULL);
	enemy_explosion_sprite_count =
		load_tile_bitmaps(explosion_bitmaps, b, 4, 2, 1.2);
	destroy_bitmap(b);

	sprintf(tmp, "RES/STAGE%d/EXPLO3.BMP", stage);
	b = load_bitmap(tmp, NULL);
	boss_explosion_sprite_count =
		load_tile_bitmaps(boss_explosion_bitmaps, b, 5, 3, 1.0);
	destroy_bitmap(b);

	sprintf(tmp, "RES/STAGE%d/EXPLO2.BMP", stage);
	b = load_bitmap(tmp, NULL);
	fox_explosion_sprite_count =
		load_tile_bitmaps(fox_explosion_bitmaps, b, 5, 6, 0.5);
	destroy_bitmap(b);

	sprintf(tmp, "RES/STAGE%d/FIRE.BMP", stage);
	b = load_bitmap(tmp, NULL);
	fire_sprite_count = load_tile_bitmaps(fire_bitmaps, b, 16, 1, 0.6);
	destroy_bitmap(b);
#endif

	return TRUE;
}

int free_bitmaps(void)
{
	destroy_bitmap(fox_bitmaps[BITMAP_FOX_GO]);
	destroy_bitmap(fox_bitmaps[BITMAP_FOX_DOWN]);
	destroy_bitmap(fox_bitmaps[BITMAP_FOX_UP]);

	destroy_bitmap(fox_light_bitmaps[BITMAP_FOX_GO]);
	destroy_bitmap(fox_light_bitmaps[BITMAP_FOX_DOWN]);
	destroy_bitmap(fox_light_bitmaps[BITMAP_FOX_UP]);

	destroy_bitmap(fox_life_heart_bitmap);
	destroy_bitmap(fox_life_small_bitmap);
	destroy_bitmap(fox_enemy_life_heart_bitmap);

	destroy_bitmap(fox_item_bitmaps[BITMAP_ITEM_BALKAN]);
#if 0
	destroy_bitmap(fox_item_bitmaps[BITMAP_ITEM_BALKAN2]);
	destroy_bitmap(fox_item_bitmaps[BITMAP_ITEM_BALKAN3]);
#endif

	destroy_bitmap(fox_item_bitmaps[BITMAP_ITEM_LASER]);
#if 0
	destroy_bitmap(fox_item_bitmaps[BITMAP_ITEM_LASER2]);
	destroy_bitmap(fox_item_bitmaps[BITMAP_ITEM_LASER3]);
#endif

	destroy_bitmap(fox_item_bitmaps[BITMAP_ITEM_GUIDED_MISSILE]);
	destroy_bitmap(fox_item_bitmaps[BITMAP_ITEM_MISSILE]);

	int i;
	for (i = 0; i<MAX_MISSILE_SMOKE; i++) {
		destroy_bitmap(missile_smoke[i]);
	}

	for (i = 0; i<MAX_MISSILE_SMOKE; i++) {
		destroy_bitmap(guided_missile_smoke[i]);
	}

	for (i = 0; i<enemy_explosion_sprite_count; i++)
		destroy_bitmap(explosion_bitmaps[i]);

	for (i = 0; i<boss_explosion_sprite_count; i++)
		destroy_bitmap(boss_explosion_bitmaps[i]);

	for (i = 0; i<fox_explosion_sprite_count; i++)
		destroy_bitmap(fox_explosion_bitmaps[i]);

	for (i = 0; i<fire_sprite_count; i++)
		destroy_bitmap(fire_bitmaps[i]);
}

void load_sound(void)
{
#ifdef USE_DATAFILE
	sound_data = load_datafile_callback("SOUND.DAT", load_data_callback);
	if (sound_data) {
		sound_effect[SOUND_ITEM] = sound_data[DAT_ITEM_WAV].dat;
		sound_effect[SOUND_ENEMY_EXPLOD] = sound_data[DAT_EXPLOD4_WAV].dat;
		sound_effect[SOUND_POWERUP] = sound_data[DAT_POWERUP_WAV].dat;
		sound_effect[SOUND_FOX_EXPLOD] = sound_data[DAT_EXPLOD3_WAV].dat;
		sound_effect[SOUND_SIREN] = sound_data[DAT_SIREN_WAV].dat;
		sound_effect[SOUND_BOSS_LASER] = sound_data[DAT_LASER_B_WAV].dat;
		sound_effect[SOUND_BOSS_SCREAM] = sound_data[DAT_SCREAM_WAV].dat;
		sound_effect[SOUND_BOSS_EXPLOD] = sound_data[DAT_EXPLOD5_WAV].dat;
		sound_effect[SOUND_MAX_POWER] = sound_data[DAT_MAXPOWER_WAV].dat;
		sound_effect[SOUND_PAUSE] = sound_data[DAT_PAUSE_WAV].dat;
	}
	else {
		err("Couldn't load sound resources!");
	}
#else
	sound_effect[SOUND_ITEM] = load_sample("RES/SOUND/ITEM.WAV");
	sound_effect[SOUND_ENEMY_EXPLOD] = load_sample("RES/SOUND/EXPLOD4.WAV");
	sound_effect[SOUND_POWERUP] = load_sample("RES/SOUND/POWERUP.WAV");
	sound_effect[SOUND_FOX_EXPLOD] = load_sample("RES/SOUND/EXPLOD3.WAV");
	sound_effect[SOUND_SIREN] = load_sample("RES/SOUND/SIREN.WAV");
	sound_effect[SOUND_BOSS_LASER] = load_sample("RES/SOUND/LASER_B.WAV");
	sound_effect[SOUND_BOSS_SCREAM] = load_sample("RES/SOUND/SCREAM.WAV");
	sound_effect[SOUND_BOSS_EXPLOD] = load_sample("RES/SOUND/EXPLOD5.WAV");
	sound_effect[SOUND_MAX_POWER] = load_sample("RES/SOUND/MAXPOWER.WAV");
	sound_effect[SOUND_PAUSE] = load_sample("RES/SOUND/PAUSE.WAV");
#endif
}

void free_sound(void)
{
#ifdef USE_DATAFILE
	unload_datafile(sound_data);
	sound_data = NULL;

#else
	destroy_sample(sound_effect[SOUND_ITEM]);
	destroy_sample(sound_effect[SOUND_ENEMY_EXPLOD]);
	destroy_sample(sound_effect[SOUND_POWERUP]);
	destroy_sample(sound_effect[SOUND_FOX_EXPLOD]);
	destroy_sample(sound_effect[SOUND_SIREN]);
	destroy_sample(sound_effect[SOUND_BOSS_LASER]);
	destroy_sample(sound_effect[SOUND_BOSS_SCREAM]);
	destroy_sample(sound_effect[SOUND_BOSS_EXPLOD]);
	destroy_sample(sound_effect[SOUND_MAX_POWER]);
	destroy_sample(sound_effect[SOUND_PAUSE]);
#endif
}

int load_music(int stageNum)
{
#ifdef USE_DATAFILE
	if (!strcasecmp(option.music, "sc55") || !strcasecmp(option.music, "adlib")) {
		music_data = load_datafile_callback("SC55.DAT", load_data_callback);
	}
	else {
		music_data = load_datafile_callback("MT32.DAT", load_data_callback);
	}
	if (music_data == NULL) {
		err("Couldn't load music resources!");
	}

	bgm_midi[BGM_BOSS] = music_data[DAT_02_MID].dat;
	bgm_midi[BGM_STAGE1] = music_data[DAT_06_MID].dat;
	bgm_midi[BGM_STAGE_CLEAR] = music_data[DAT_03_MID].dat;

#else
	if (!strcasecmp(option.music, "sc55") || !strcasecmp(option.music, "adlib")) {
		if (stageNum == 1)
		{
			bgm_midi[BGM_BOSS] = load_midi("RES/MUSIC/SC-55/02.MID");
			bgm_midi[BGM_STAGE1] = load_midi("RES/MUSIC/SC-55/06.MID");
			bgm_midi[BGM_STAGE_CLEAR] = load_midi("RES/MUSIC/SC-55/03.MID");
		}
		else if (stageNum == 2)
		{
			bgm_midi[BGM_BOSS] = load_midi("RES/MUSIC/SC-55/02.MID");
			bgm_midi[BGM_STAGE1] = load_midi("RES/MUSIC/SC-55/07.MID");
			bgm_midi[BGM_STAGE_CLEAR] = load_midi("RES/MUSIC/SC-55/03.MID");
		}


	}
	else if (!strcasecmp(option.music, "mt32")) {
		if (stageNum == 1)
		{
			bgm_midi[BGM_BOSS] = load_midi("RES/MUSIC/MT-32/02.MID");
			bgm_midi[BGM_STAGE1] = load_midi("RES/MUSIC/MT-32/06.MID");
			bgm_midi[BGM_STAGE_CLEAR] = load_midi("RES/MUSIC/MT-32/03.MID");
		}
		else if (stageNum == 2)
		{

			bgm_midi[BGM_BOSS] = load_midi("RES/MUSIC/MT-32/02.MID");
			bgm_midi[BGM_STAGE1] = load_midi("RES/MUSIC/MT-32/07.MID");
			bgm_midi[BGM_STAGE_CLEAR] = load_midi("RES/MUSIC/MT-32/03.MID");
		}
	}
	
#endif

	return TRUE;
}

void free_music(void)
{
#ifdef USE_DATAFILE
	unload_datafile(music_data);
	music_data = NULL;

#else
	destroy_midi(bgm_midi[BGM_BOSS]);
	destroy_midi(bgm_midi[BGM_STAGE1]);
	destroy_midi(bgm_midi[BGM_STAGE_CLEAR]);
#endif
}

int free_stage_resource(void)
{
	if (scroll_bitmap_list != NULL) {
		list_remove_all_cb(scroll_bitmap_list, destroy_bitmap);
		list_destroy(scroll_bitmap_list);
		scroll_bitmap_list = NULL;
	}

	if (scroll_speed_list != NULL) {
		list_remove_all_cb(scroll_speed_list, free);
		list_destroy(scroll_speed_list);
		scroll_speed_list = NULL;
	}

	if (scroll_overlap_list != NULL) {
		list_remove_all_cb(scroll_overlap_list, free);
		list_destroy(scroll_overlap_list);
		scroll_overlap_list = NULL;
	}

	if (stage_res_table != NULL) {
		HashTableIter iter;
		hashtable_iter_init(&iter, stage_res_table);
		TableEntry *entry;

		while (hashtable_iter_next(&iter, &entry) != CC_ITER_END) {
			char const *key = entry->key;

			void *out;
			hashtable_get(stage_res_table, (void*)key, (void*)&out);
			ENEMY_RES *res = (ENEMY_RES*)out;

			list_remove_all_cb(res->bitmaps, destroy_bitmap);
			list_destroy(res->bitmaps);
		}

		hashtable_remove_all(stage_res_table);
		hashtable_destroy(stage_res_table);
		stage_res_table = NULL;
	}

	if (stage_emblem_bitmap != NULL) {
		destroy_bitmap(stage_emblem_bitmap);
		stage_emblem_bitmap = NULL;
	}
}

int load_stage_resource(int stage)
{
	scroll_bitmap_list = NULL;
	scroll_speed_list = NULL;
	scroll_overlap_list = NULL;

	stage_res_table = NULL;
	hashtable_new(&stage_res_table);

	XMLDoc doc;
	XMLDoc_init(&doc);

#ifdef USE_DATAFILE
	char *xml = stage_data[DAT_STAGE_RES].dat;
#else
	char tmp[1024];
	sprintf(tmp, "RES/STAGE%d/STAGE.RES", stage);
	char *xml;
	read_text_file(tmp, &xml);
#endif

	if (!XMLDoc_parse_buffer_DOM(xml, NULL, &doc)) {
		err("Couldn't load xml resources!");
	}

	XMLNode* root = XMLDoc_root(&doc);

	XMLNode* node;
	node = XMLNode_get_child(root, 0);

	BITMAP *b = NULL;

#ifdef USE_DATAFILE
	b = stage_data[DAT_STAGE_BMP].dat;
#else
	sprintf(tmp, "RES/STAGE%d/STAGE.BMP", stage);
	b = load_bitmap(tmp, NULL);
#endif

	while (1) {
		if (node == NULL) break;

		if (!strcasecmp(node->tag, "EMBLEM")) {
			int x, y, w, h;
			float size;
			sscanf(node->text, "%d %d %d %d %f", &x, &y, &w, &h, &size);
			stage_emblem_bitmap = copy_sub_bitmap(b, x, y, w, h, size);
		}

		if (!strcasecmp(node->tag, "BACKGROUND")) {
			XMLNode* node2;
			node2 = XMLNode_get_child(node, 0);

			while (1) {
				if (node2 == NULL) break;

				if (!strcasecmp(node2->tag, "SCROLL")) {
					XMLNode* node3;
					node3 = XMLNode_get_child(node2, 0);

					int color = makecol(0, 0, 0);

					int j;
					for (j = 0; j<10; j++)
						scroll_offset[j] = 0;

					while (1) {
						if (node3 == NULL) break;

						if (!strcasecmp(node3->tag, "COLOR")) {
							int r, g, b;
							sscanf(node3->text, "%d %d %d", &r, &g, &b);
							color = makecol(r, g, b);

						}
						else if (!strcasecmp(node3->tag, "SPEED")) {
							list_new(&scroll_speed_list);
							string_to_int_list(node3->text, " ", scroll_speed_list);

						}
						else if (!strcasecmp(node3->tag, "OVERLAP")) {
							list_new(&scroll_overlap_list);
							string_to_int_list(node3->text, " ", scroll_overlap_list);

						}
						else if (!strcasecmp(node3->tag, "BITMAP")) {
							// attributes
							int i;

							// value
							list_new(&scroll_bitmap_list);

							XMLNode* node4;
							node4 = XMLNode_get_child(node3, 0);

							while (1) {
								if (node4 == NULL) break;

								char **items = NULL;

								char *text = malloc(strlen(node4->text));

								trimmed(node4->text, &text);

								int c = split(text, '\n', &items);
								free(text);

								// ��Ʈ�� ��ü width�� ���
								int width = 0;
								int height = 0;
								for (i = 0; i < c; i++) {
									char *line = trim(*(items + i));
									if (strlen(line) > 0) {
										int x, y, w, h;
										float size;
										sscanf(line, "%d %d %d %d %f", &x, &y, &w, &h, &size);
										width += w;
										height = h;
									}
								}

								BITMAP *b3 = create_bitmap(width, height);
								clear(b3);

								int px = 0;
								int py = 0;
								for (i = 0; i < c; i++) {
									char *line = trim(*(items + i));
									if (strlen(line) > 0) {
										int x, y, w, h;
										float size;
										sscanf(line, "%d %d %d %d %f", &x, &y, &w, &h, &size);

										BITMAP *b2 = copy_sub_bitmap(b, x, y, w, h, size);
										draw_sprite(b3, b2, px, py);
										destroy_bitmap(b2);
										px += (w - 1);
									}
								}

								list_add(scroll_bitmap_list, b3);

								for (i = 0; i < c; i++)
									free(*(items + i));

								free(items);

								node4 = XMLNode_next_sibling(node4);
							}
						}

						node3 = XMLNode_next_sibling(node3);
					}

				}
				else if (!strcasecmp(node2->tag, "BITMAP")) {
					int x, y, w, h;
					sscanf(node2->text, "%d %d %d %d", &x, &y, &w, &h);

					BITMAP *b2 = copy_sub_bitmap(b, x, y, w, h, 1.0);

					int i = 0;
					for (i = 0; i<SCREEN_W; i += b2->w) {
						draw_sprite(stage_bg_bitmap, b2, i, 0);
					}

					destroy_bitmap(b2);

				}
				else if (!strcasecmp(node2->tag, "COLOR")) {
					XMLNode* node3;
					node3 = XMLNode_get_child(node2, 0);

					int start_color = 0;
					int end_color = 0;
					int method = 0;

					while (1) {
						if (node3 == NULL) break;

						if (!strcasecmp(node3->tag, "START")) {
							int r, g, b;
							sscanf(node3->text, "%d %d %d", &r, &g, &b);
							start_color = makecol(r, g, b);

						}
						else if (!strcasecmp(node3->tag, "END")) {
							int r, g, b;
							sscanf(node3->text, "%d %d %d", &r, &g, &b);
							end_color = makecol(r, g, b);

						}
						else if (!strcasecmp(node3->tag, "DIRECTION")) {
							if (!strcasecmp(node3->text, "VERTICAL")) {
								method = GRADIENTTEXTURE | GRADIENTRAISED | GRADIENTVERTICAL;
							}
							else {
								method = GRADIENTTEXTURE | GRADIENTRAISED | GRADIENTHORIZONTAL;
							}
						}

						node3 = XMLNode_next_sibling(node3);
					}

					init_gradient();
					draw_colour_gradient(stage_bg_bitmap, 0, 0,
						SCREEN_W, SCREEN_H, start_color, end_color, method);
				}

				node2 = XMLNode_next_sibling(node2);
			}
		}

		if (!strcasecmp(node->tag, "ENEMY")) {
			XMLNode* node2;
			node2 = XMLNode_get_child(node, 0);

			while (1) {
				if (node2 == NULL) break;

				if (!strcasecmp(node2->tag, "ITEM")) {
					ENEMY_RES *res = malloc(sizeof(ENEMY_RES));

					char *name = NULL;
					int i;
					for (i = 0; i < node2->n_attributes; i++) {
						if (!strcasecmp(node2->attributes[i].name, "name")) {
							name = node2->attributes[i].value;
						}
					}

					XMLNode* node3;
					node3 = XMLNode_get_child(node2, 0);

					while (1) {
						if (node3 == NULL) break;

						if (!strcasecmp(node3->tag, "WEAK")) {
							sscanf(node3->text, "%d %d %d %d",
								&res->weak_x, &res->weak_y, &res->weak_w, &res->weak_h);

						}
						else if (!strcasecmp(node3->tag, "BITMAP")) {
							// attributes
							int i;

							// value
							list_new(&res->bitmaps);

							char **items = NULL;
							int c = split(node3->text, '\n', &items);
							for (i = 0; i < c; i++) {
								char *line = trim(*(items + i));
								if (strlen(line) > 0) {
									int x, y, w, h;
									float scale;
									sscanf(line, "%d %d %d %d %f", &x, &y, &w, &h, &scale);

									BITMAP *b2 = copy_sub_bitmap(b, x, y, w, h, scale);

#if 0
									// ���������� ä��
									BITMAP *b3 = create_bitmap(w, h);
									clear(b3);
									draw_lit_sprite(b3, b2, 0, 0, makecol(255, 0, 0));
									list_add(res->bitmaps, b3);
									destroy_bitmap(b2);
#else
									list_add(res->bitmaps, b2);
#endif
								}

								free(*(items + i));
							}

							free(items);
						}

						node3 = XMLNode_next_sibling(node3);
					}

					char *key = strdup(name);
					hashtable_add(stage_res_table, key, res);
				}

				node2 = XMLNode_next_sibling(node2);
			}
		}

		node = XMLNode_next_sibling(node);
	}

	XMLDoc_free(&doc);

#ifndef USE_DATAFILE
	if (b != NULL)
		destroy_bitmap(b);

	free(xml);
#endif
}

int stage_destroy()
{
	list_remove_all_cb(fox_weapon_list, free);
	list_destroy(fox_weapon_list);

	{
		ListIter iter;
		void *v;
		list_iter_init(&iter, fox_missile_list);
		while (list_iter_next(&iter, (void*)&v) != CC_ITER_END)
		{
			WEAPON_MISSILE *weapon = (WEAPON_MISSILE*)v;
			list_remove_all_cb(weapon->x_list, free);
			list_destroy(weapon->x_list);
		}

		list_remove_all_cb(fox_missile_list, free);
		list_destroy(fox_missile_list);
	}

	list_remove_all_cb(fox_guided_missile_list, free);
	list_destroy(fox_guided_missile_list);

	{
		ListIter iter;
		void *v;
		list_iter_init(&iter, enemy_list);
		while (list_iter_next(&iter, (void*)&v) != CC_ITER_END)
		{
			ENEMY *enemy = (ENEMY*)v;
			list_remove_all_cb(enemy->bitmaps, destroy_bitmap);
		}

		list_remove_all_cb(enemy_list, free);
		list_destroy(enemy_list);
	}

	if (list_size(fox_item_history) > 0)
		list_remove_all_cb(fox_item_history, free);
	list_destroy(fox_item_history);

	if (list_size(enemy_item_list) > 0)
		list_remove_all_cb(enemy_item_list, free);
	list_destroy(enemy_item_list);

	{
		ListIter iter;
		void *v;
		list_iter_init(&iter, enemy_weapon_list);
		while (list_iter_next(&iter, (void*)&v) != CC_ITER_END)
		{
			ENEMY_WEAPON *weapon = (ENEMY_WEAPON*)v;
			destroy_bitmap(weapon->bitmap);
		}

		list_remove_all_cb(enemy_weapon_list, free);
		list_destroy(enemy_weapon_list);
	}


	if (orbit_table != NULL) {
		HashTableIter iter;
		hashtable_iter_init(&iter, orbit_table);
		TableEntry *entry;

		while (hashtable_iter_next(&iter, &entry) != CC_ITER_END) {
			char const *key = entry->key;

			void *out;
			hashtable_get(orbit_table, (void*)key, (void*)&out);
			List *list = (List*)out;

			list_remove_all_cb(list, free);
		}

		hashtable_remove_all(orbit_table);
		hashtable_destroy(orbit_table);
		orbit_table = NULL;
	}
}

void game_pause(void)
{
	remove_int(stage_elapsed_time_proc);
	clear_keybuf();

	if (strcasecmp(option.music, "no")) {
		midi_pause();
	}

	game_paused = TRUE;
}

void game_resume(void)
{
	if (strcasecmp(option.music, "no")) {
		midi_resume();
	}

	game_paused = FALSE;

	install_int_ex(stage_elapsed_time_proc, BPS_TO_TIMER(1000 / 2));
}


int stage_start(int stage)
{
	init_effect_spark();
	init_effect_firework();

	// -------------------------------------------------------
	list_new(&fox_weapon_list);
	list_new(&fox_missile_list);
	list_new(&fox_guided_missile_list);
	list_new(&fox_item_history);

	list_new(&enemy_list);
	list_new(&enemy_item_list);
	list_new(&enemy_weapon_list);

	// -------------------------------------------------------
	stage_number = stage;
	stage_clear_flag = FALSE;

	// -------------------------------------------------------
	/* install timer handlers to control and measure the program speed */
	LOCK_VARIABLE(update_count);
	LOCK_VARIABLE(frame_count);
	LOCK_VARIABLE(fps);
	LOCK_FUNCTION(timer_proc);
	LOCK_FUNCTION(fps_proc);

	LOCK_VARIABLE(stage_elapsed_time);

	install_int_ex(timer_proc, BPS_TO_TIMER(option.fps));
	install_int_ex(fps_proc, BPS_TO_TIMER(1));

	// 1�ʿ� 500�� �߻� (1�ʿ� 1000���� overhead �� Ŭ�� ���Ƽ�)
	// ��, �ѹ� �Ҹ������� +2 �� ���Ͽ� ms ������ �������� ���� �ð� ����
	install_int_ex(stage_elapsed_time_proc, BPS_TO_TIMER(1000 / 2));

	// --------------------------------------
	stage_elapsed_time = 0;

	// --------------------------------------
	fox_weapon_time = stage_elapsed_time;
	fox_urgency_weapon_time = stage_elapsed_time;
	fox_item_weapon_time = stage_elapsed_time;

	fox_life_count = 3;
	game_end = FALSE;

	boss_explod_begin_time = -1;
	boss_explod_vibration_time = -1;
	boss_explod_vibration_x = 0;
	boss_explod_vibration_y = 0;

	// -------------------------------------------------------
	fox_bitmap = fox_bitmaps[BITMAP_FOX_GO];
	fox_bitmap_index = BITMAP_FOX_GO;

	// -------------------------------------------------------
	show_popup_message(stage_start_message[stage-1], TRUE, 3000);

	// -------------------------------------------------------
	init_fox_ranger();

	// -------------------------------------------------------
	if (strcasecmp(option.music, "no")) {
		play_midi(bgm_midi[BGM_STAGE1], 1);
	}

	snow_flake *snow_flakes = NULL;  // make sure it starts by being NULL!!!

	clear_keybuf();

	update_count = 0;
	frame_count = 0;

	srand(time(0));

	while (1) {
		if (keypressed()) {
			if (key[KEY_ESC]) {
				break;
			}

			if (game_end == TRUE) {
				if (key[KEY_N]) break;

				if (key[KEY_Y]) {
					fox_life_count = 3;
					game_end = FALSE;

					hide_popup_message();
				}
			}

			if (key[KEY_ENTER] && game_paused == FALSE) {
				game_pause();

				show_popup_message("PAUSED", FALSE, 0);

				play_sample(sound_effect[SOUND_PAUSE], 255, 127, 1000, 0);
			}

			if (!key[KEY_ENTER] && game_paused == TRUE) {
				hide_popup_message();

				game_resume();
			}
		}

		if (game_paused == FALSE) {
			if (game_end == TRUE) {
				//game_pause();

				show_popup_message("Continue?", TRUE, 3000);
			}

			if (update_count > 0) {
				acquire_bitmap(stage_buf_bitmap);

				process_background_scroll();

				process_fox_movement();

				process_enemy_weapon();
				process_enemy_movement();

				process_fox_weapon();
				process_fox_missile();
				process_fox_guided_missile();

				process_enemy_item();

				process_show_life_meter();
				process_enemy_energy_bar();

				//process_debug_message();

				// ---------------------------------------------
				process_spark(stage_buf_bitmap);
				process_firework(stage_buf_bitmap);

				// ---------------------------------------------
				if (option.debug) {
					show_debug();
				}

				// ---------------------------------------------
				if (option.snow_flakes) {
					snow_flakes = clean_snow_flake(snow_flakes, FALSE);
					snow_flakes = create_snow_flake(snow_flakes);
					update_snow_flakes(snow_flakes);
					draw_snow_flakes(stage_buf_bitmap, snow_flakes);
				}

				release_bitmap(stage_buf_bitmap);

				update_count--;
				frame_count++;
			}

		}
		else {
			update_count = 0;
			frame_count = 0;
		}

		// -------------------------------------------------------
		blit(stage_buf_bitmap, stage_bitmap, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

		process_popup_message();

		acquire_screen();
		blit(stage_bitmap, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
		release_screen();

		if(stage_clear_flag == TRUE && fox_x > SCREEN_W)
		{
			stage++;
			break;
		}
	}

	remove_int(timer_proc);
	remove_int(fps_proc);
	remove_int(stage_elapsed_time_proc);

	if (strcasecmp(option.music, "no")) {
		stop_midi();
	}

	// free any other particles that are left.
	snow_flakes = clean_snow_flake(snow_flakes, TRUE);

	destroy_effect_spark();
	destroy_effect_firework();

	fade_out(3);

	rest(300);

	return stage;
}