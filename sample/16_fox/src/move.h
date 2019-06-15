#include <allegro.h>

float deg2rad(float deg);
float rad2deg(float rad);

float calc_angle(float x, float y, float x2, float y2);
float calc_distance(float sx, float sy, float tx, float ty);
void calc_rotation(float cx, float cy, float angle, float x, float y, float *new_x, float *new_y);

void moving_left(float speed, float x, float *new_x);
void moving_up(float speed, float y, float *new_y);
void moving_down(float speed, float y, float *new_y);
void moving_right(float speed, float x, float *new_x);

void moving_guided(float speed, float sx, float sy, float tx, float ty, 
		float *new_x, float *new_y, float *new_vx, float *new_vy, float *angle);
void moving_wave(float speed, float angle, float x, float base_y, 
		float *new_angle, float *new_x, float *new_y);
void moving_bouncing(float vx, float vy, float x, float y, int w, int h, 
		float speed, float *new_x, float *new_y, float *new_vx, float *new_vy, int bouncing, int *bounced);
void moving_homing_missile(float speed, float x1, float y1, float x2, float y2, 
		float angle, float limit_turn_angle,
		float *new_x, float *new_y, float *new_vx, float *new_vy, float *new_angle);
