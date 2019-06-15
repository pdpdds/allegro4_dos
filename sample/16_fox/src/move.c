#include "MOVE.H"
#include <math.h>
#include <fastsqrt.h>
#include "sintbl.h"

#ifdef _WIN32
#define M_PI 3.141592
#endif

double fastsqrt(double r) // fast squareroot approx. function, handy for distance finding
{
	double x,y;
	double tempf;
	unsigned long *tfptr = ((unsigned long *)&tempf)+1;

	tempf = r;
	*tfptr=(0xbfcd4600-*tfptr)>>1;
	x=tempf;
	y=r*0.5;
	x*=1.5-x*x*y;  // I only need to estimate
	return x*r;
}

float deg2rad(float deg)
{
	float angle = deg * M_PI / 180.0;
	return angle;
}

float rad2deg(float rad)
{
	return rad * 180 / M_PI;
}

//  ~2.3 times faster than atan2 of stdlib
float atan2_approximation1(float y, float x)
{
    //http://pubs.opengroup.org/onlinepubs/009695399/functions/atan2.html
    //Volkan SALMA

    const float ONEQTR_PI = M_PI / 4.0;
	const float THRQTR_PI = 3.0 * M_PI / 4.0;
	float r, angle;
	float abs_y = fabs(y) + 1e-10f;      // kludge to prevent 0/0 condition
	if ( x < 0.0f )
	{
		r = (x + abs_y) / (abs_y - x);
		angle = THRQTR_PI;
	}
	else
	{
		r = (x - abs_y) / (x + abs_y);
		angle = ONEQTR_PI;
	}
	angle += (0.1963f * r * r - 0.9817f) * r;
	if ( y < 0.0f )
		return( -angle );     // negate if in quad III or IV
	else
		return( angle );
}

// -------------------------------------------------------------
// return: radian angle
float calc_angle(float x, float y, float x2, float y2)
{
	float dx = x2 - x;
	float dy = y2 - y;
	//return atan2(dy, dx);
	return atan2_approximation1(dy, dx);
}

float calc_distance(float sx, float sy, float tx, float ty)
{
	float vx=tx-sx, vy=ty-sy;
	float d=(float)fastsqrt((vx*vx+vy*vy));
    return d;
}

void calc_rotation(float cx, float cy, float angle, float x, float y, float *new_x, float *new_y)
{
  float s = LookUpSin(angle);
  float c = LookUpCos(angle);

  // translate point back to origin:
  x -= cx;
  y -= cy;

  // rotate point
  float rx = x * c - y * s;
  float ry = x * s + y * c;

  // translate point back:
  *new_x = rx + cx;
  *new_y = ry + cy;
}

// ----------------------------------------------
void moving_left(float speed, float x, float *new_x)
{
	*new_x = x - speed;
}

void moving_up(float speed, float y, float *new_y)
{
	*new_y = y - speed;
}

void moving_down(float speed, float y, float *new_y)
{
	*new_y = y + speed;
}

void moving_right(float speed, float x, float *new_x)
{
	*new_x = x + speed;
}

void moving_guided(float speed, float sx, float sy, float tx, float ty, 
		float *new_x, float *new_y, float *new_vx, float *new_vy, float *angle)
{
	// 목표물까지의 거리 d를 구하기
	float vx=tx-sx, vy=ty-sy;
	float d=(float)fastsqrt((double)(vx*vx+vy*vy));

	// 속도가 항상 일정하게 Speed가 되도록 속도 벡터(vx,vy)를 구하기:
	// 목표물까지의 거리 d가 왼쪽으로 움직임.
	if (d) {
		vx*=speed/d;
		vy*=speed/d;
	} else {
		vx=speed;
		vy=0.0;
	}

	// 이동
	*new_x = sx + vx;
	*new_y = sy + vy;

	*new_vx = vx;
	*new_vy = vy;
	
	*angle = calc_angle(sx, sy, *new_x, *new_y);
}

// https://codepen.io/osublake/pen/qNPBpJ
void moving_homing_missile(float speed, float x1, float y1, float x2, float y2, 
		float angle, // 이전 각도
		float limit_turn_angle,	// 선회 제한 각도 (각도가 작을수록 회전 반경이 넓어진다)
		float *new_x, float *new_y, float *new_vx, float *new_vy, float *new_angle)
{
	float angle2 = calc_angle(x1, y1, x2, y2);
	float theta = 0.0;

	if ( angle != angle2 ) {
		float turn = deg2rad(limit_turn_angle);
		float delta = angle2 - angle;

		float TAU = M_PI * 2.0;

		if ( delta > M_PI ) delta -= TAU;
		if ( delta < -M_PI ) delta += TAU;

		theta = delta > 0 ? turn: -turn;
		angle += theta;

		if (abs(delta) < turn) {
			angle = angle2;
		}
	}

	float vx = LookUpCos(angle) * speed;
	float vy = LookUpSin(angle) * speed;

	*new_x = x1 + vx;
	*new_y = y1 + vy;

	*new_vx = vx;
	*new_vy = vy;

	*new_angle = angle;
}


// https://gamedev.stackexchange.com/questions/9607/moving-an-object-in-a-circular-path
void moving_wave(float speed, float angle, float x, float base_y, float *new_angle, float *new_x, float *new_y)
{
	// 반경
	float radius = 30;

	// 각도 증가
	angle += 5.0;
	if ( angle > 360 ) angle = 0.0;

	float ang = deg2rad(angle);
	//int x2 = x + LookUpCos(ang)*radius;
	*new_y = base_y + LookUpSin(ang)*radius;
	*new_x = x-speed;
	*new_angle = angle;
}

void moving_bouncing(float vx, float vy, float x, float y, int w, int h, 
		float speed, float *new_x, float *new_y, float *new_vx, float *new_vy, int bouncing, int *bounced)
{
	*bounced = FALSE;

	if ( bouncing == TRUE ) {
		if(x > SCREEN_W-w) {
			x = SCREEN_W-w;
			vx *= -1;
			*bounced = TRUE;
		}

		if(y > SCREEN_H-h) {
			y = SCREEN_H-h;
			vy *= -1;
			*bounced = TRUE;
		}
		
		if(x < 0) {
			x = 0;
			vx *= -1;
			*bounced = TRUE;
		}

		if(y < 0) {
			y = 0;
			vy *= -1;
			*bounced = TRUE;
		}
	}

	vx *= speed;
	vy *= speed;

	*new_x = x + vx;
	*new_y = y + vy;
	*new_vx = vx;
	*new_vy = vy;
}

