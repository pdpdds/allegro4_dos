#include "LIST.H"
#include "HASHTABLE.H"
#include "SXMLC.H"
#include "OPTION.H"
#include "HANCONV.H"
#include "HANOUT.H"
#include "GRADIENT.H"
#include "EFFECT.H"
#include "TOOL.H"
#include "move.h"
#include "DAT.H"
#include "SINTBL.H"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <allegro.h>

#ifdef _MSC_VER 
//not #if defined(_WIN32) || defined(_WIN64) because we have strncasecmp in mingw
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

//---------------------------
#define BITMAP_FOX_UP 				0
#define BITMAP_FOX_DOWN 			1
#define BITMAP_FOX_GO 				2

#define BITMAP_MAX_FOX_EXPLOSION 	28
#define BITMAP_MAX_BOSS_EXPLOSION 	50
#define BITMAP_MAX_BOSS_FIRE 		10

//---------------------------
#define	BALKAN						0
#define LASER						1

#define GUIDED_MISSILE				2
#define MISSILE						3

//---------------------------
#define BITMAP_BALKAN				0
#define BITMAP_BALKAN2				1
#define BITMAP_BALKAN3				2

#define BITMAP_LASER				3
#define BITMAP_LASER2				4
#define BITMAP_LASER3				5

#define BITMAP_GUIDED_MISSILE		6
#define BITMAP_GUIDED_MISSILE2		7
#define BITMAP_GUIDED_MISSILE3		8

#define BITMAP_MISSILE				9
#define BITMAP_MISSILE2				10
#define BITMAP_MISSILE3				11

//---------------------------
#define BITMAP_ITEM_BALKAN			0
#define BITMAP_ITEM_BALKAN2			1
#define BITMAP_ITEM_BALKAN3			2

#define BITMAP_ITEM_LASER			3
#define BITMAP_ITEM_LASER2			4
#define BITMAP_ITEM_LASER3			5

#define BITMAP_ITEM_GUIDED_MISSILE	6
#define BITMAP_ITEM_MISSILE			7
//---------------------------
	
#define MOVE_LEFT					0
#define MOVE_RIGHT					1
#define MOVE_UP						2
#define MOVE_DOWN					3
#define MOVE_ZOOMIN_LEFT			4
#define MOVE_ZOOMIN_RIGHT			5
#define MOVE_WAVE					6
#define MOVE_RIGHT_CENTER			7 	// ������
#define MOVE_FIXED					8 	// ������
#define MOVE_ORBIT					9
#define MOVE_GUIDED					10
#define MOVE_GUIDED_BACK			11

#define SOUND_ITEM			0
#define SOUND_ENEMY_EXPLOD	1
#define SOUND_FOX_EXPLOD		2
#define SOUND_POWERUP			3
#define SOUND_SIREN			4
#define SOUND_BOSS_LASER		5
#define SOUND_BOSS_SCREAM		6
#define SOUND_BOSS_EXPLOD		7
#define SOUND_MAX_POWER		8
#define SOUND_PAUSE			9

#define BGM_STAGE1			0
#define BGM_STAGE_CLEAR		1
#define BGM_BOSS			2
	
//---------------------------
extern SAMPLE *sound_effect[20];

extern MIDI *bgm_midi[10];

extern BITMAP *fox_bitmap;
extern BITMAP *fox_bitmaps[4];
extern BITMAP *fox_light_bitmaps[4];
extern BITMAP *fox_life_heart_bitmap;
extern BITMAP *fox_life_small_bitmap;
extern BITMAP *fox_enemy_life_heart_bitmap;
extern BITMAP *fox_explosion_bitmaps[10*10];
extern BITMAP *fire_bitmaps[16*1];
extern BITMAP *fox_item_bitmaps[20];
extern BITMAP *fox_damage_bitmap;

static HashTable *fox_weapon_table;

extern int fox_life_count;
extern int fox_energy_count;

extern int fox_bitmap_index;
extern int fox_explosion;
extern int fox_explosion_time;

extern int boss_explosion;
extern int boss_explosion_time;

extern char fox_weapon_name[30];
extern char fox_weapon_sub_name[30];

extern int fox_weapon_level;	// ������ �������� ��� ���� ��� �Ŀ��� �����Ѵ�.
extern int fox_weapon_max_level;
extern int fox_weapon_sub_level;

extern char fox_twinkling;		// ������ ��� ����
extern long fox_twinkling_time;	// ������ ������ �ð�

extern long fox_ghost_time;		// ���� ���� �ð�
extern long fox_attacked_time;		// �� ����� �浹�� �ð�

extern long fox_collided_time;		// �� ����� �浹�� �ð�
extern int fox_collided_index;		// �� ����� �浹�� �ð�

extern long fox_weapon_time;						// ���� �߻� �ð�
extern long fox_item_weapon_time;					// ������ ���� �߻� �ð�
extern long fox_urgency_weapon_time;		// ���� ���� �߻� �ð�

extern long stage_elapsed_time;				// �������� ����� �ð�

extern long fox_popup_message_time;
extern long fox_popup_message_show_time;
extern int fox_popup_show_emblem;

extern BITMAP *explosion_bitmaps[10*10];
extern BITMAP *boss_explosion_bitmaps[8*8];

extern BITMAP *stage_bg_bitmap;
extern BITMAP *stage_buf_bitmap;
extern BITMAP *stage_popup_bitmap;
extern BITMAP *stage_bitmap;

extern int fox_explosion_sprite_count;
extern int enemy_explosion_sprite_count;
extern int boss_explosion_sprite_count;
extern int fire_sprite_count;

extern int fox_x;			// ��ü ���� x��ġ
extern int fox_y;			// ��ü ���� y��ġ
extern int fox_restore;	// ��ü ���� ����
extern float fox_speed;

// ----------------------------------------------------------------
typedef struct {
	List *bitmaps;
#if 0
	List *weapon_bitmaps;
#endif
	int weak_x, weak_y;
	int weak_w, weak_h;
} ENEMY_RES;

static HashTable *stage_res_table;

// ----------------------------------------------------------------
// ����
typedef struct {
	float vx, vy;
	unsigned int time;
} ORBIT;

static HashTable *orbit_table;

typedef struct {
	BITMAP *bitmap;
	int power;
	float x, y;
	float vx, vy;
	int direction;
	float speed;
	float delay;
	float angle;
} WEAPON;

typedef struct {
	BITMAP *bitmap;
	float x, y;
	float speed;
	int power;
	float accel;
	List *x_list;
	int smoke_begin;
	int smoke_index;
	long smoke_time;
} WEAPON_MISSILE;

typedef struct {
	BITMAP *bitmap;
	int power;
	float x, y;
	float vx, vy;
	float angle;
	int target_enemy;
	float speed;
	float accel;
	int smoke_begin;
	int smoke_index;
	long smoke_time;
	long launch_time;
} WEAPON_GUIDED_MISSILE;

typedef struct {
	char name[30];				// ������ ����
	float x, y;
	float dx, dy;
	float speed;
	float accel;
	long creation_time;		// ���� �ð�

	int bitmaps[10];			// ��������Ʈ ��Ʈ�� �ε��� �迭
	int bitmap_index;			// ���� ��������Ʈ ��ȣ
	int bitmap_show_time;		// ��������Ʈ ��ü �ð�
	int bitmap_count;			// ��ü ��������Ʈ ����
} ITEM;

typedef struct {
	BITMAP *bitmap;
	int power;
	float x, y;
	float vx, vy;
	int direction;
	int speed;
	float delay;
} ENEMY_WEAPON;
						
typedef struct {
	int x, y;
	long time;
	int index;
} ENEMY_FIRE;

typedef struct {
	long creation_time;

	int number;
	int explosion;
	long explosion_time;
	int max_life;
	int life;
	char name[30];
	char item[30];					// ������ �̸�
	
	char side[30];					// ������ �̸�

	float x, y;
	int w, h;
	float vx, vy;

	int trace;
	float trace_angle;

	// ���� ����
	int weak_x, weak_y;
	int weak_w, weak_h;

	float base_y;				// WAVE �̵��� ���
	float base_speed;			// �̵� �ӵ�
	float speed;				// ���� �̵� �ӵ�
	float accel;
	long launch_time;			// ù��° ���� �߻� �ð�
	long launch_sub_time;			// �ι�° ���� �߻� �ð� (������ ����)
	long sub_weapon_time;		// �ι�° ���� ���ϸ��̼� �ð� (������ ����)
	int sub_weapon_index;		// �ι�° ���� ���ϸ��̼� �ð� (������ ����)
	int reload_time;			// ���� �߻� ���� �ð�
	char attack_begin;			// ���� �߻� ����
	char attack_pause;			// ���� �߻� ��� ����
	int attack_method;			// ���� �߻� �˰��� ����
	int attack_odd;				// ���� źȯ �߻�� źȯ Ʋ���ֱ�
	long attack_odd_time;		// ���� źȯ �߻�� źȯ�� Ʋ���� �ð�
	long attack_method_time;	// ���� �߻� �˰��� ���� �ð�
	int motion;					// �̵� �˰���
	long attacked_time;			// ���� ���� �ð�
	float angle;

	List *angry_point_list;
	long angry_time;			// ������ ���� �ð�
	int angry_max_count;		// �ִ� ���� Ƚ��
	int angry_count;			// ���� Ƚ��

	List *fire_point_list;
	int fire_count;
	int fire_max_count;			// �ִ� �� Ƚ��
	List *fire_list;			// ���� ��ġ���� ����ϴ� ����Ʈ

	List *bitmaps;
	int bitmap_index;			// ���� ��Ʈ�� ��ȣ
	int bitmap_show_time;		// ������ ��Ʈ�� ��� �ð�
	float bitmap_size;
	int bitmap_size_time;		// Ȯ��� �ð�
	
	char destroyed;			// �ı���
	int revenge_nway;				// ���Ƚ� n-way ź �߻�

	int zoom_done;
	
	char orbit_name[30];
	int orbit_index;
	long orbit_time;

} ENEMY;

static List *enemy_list;
static List *enemy_item_list;
static List *enemy_weapon_list;

static List *fox_item_history;

static List *fox_weapon_list;
static List *fox_guided_missile_list;
static List *fox_missile_list;

// ----------------------------------------------------------------
typedef struct {
	BITMAP *bitmap;
	int power;
	int reload_time;
	int speed;
	int launch_count;
} FOX_WEAPON;

typedef struct {
	int time;
	int kind;
	int life;
	float speed;
	int reload_time;	// ���� �߻� ���� �ð�
	char item[30];	
	char side[30];		// ���� ���� �� (right, left, top, bottom)
	float first_location;				// ���� ���� ��ǥ(%)
	int motion;			// �̵� �˰���
	char orbit_name[30];
	char name[30];
	int revenge_nway;
	float accel;
	List *angry_point_list;
	List *fire_point_list;
} ENEMY_ADVENT;

static List *enemy_advent_list;
// ----------------------------------------------------------------

int process_fox_weapon_attack(
		float weapon_x, float weapon_y, int weapon_w, int weapon_h, 
		int weapon_power, ENEMY *enemy);
void game_pause(void);
extern void timer_proc(void);
