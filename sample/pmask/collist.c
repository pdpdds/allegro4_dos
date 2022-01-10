#include <stdlib.h>  //needed for malloc() / free()
#include <string.h>  //needed for memset()
#include <math.h>    //needed for fabs() and sqrt()
#include "pmask.h"   //optionally needed for check_pmask_collision()
#include "collist.h" //of course we need our own header

//#include <allegro.h> //remove me! for debugging only
#include "get_time.h" //remove me! for debugging only

#ifdef _MSC_VER
#	define DO_ERROR() _asm {int 3}
#else
#	define DO_ERROR() {((int*)0)[0] = 0;}
#endif



/*	Helper Function Prototypes  */
static void _sort_collidables_y ( COLLIST_ITEM *collidables, int num, int sort_algorithm );
//static void _sort_collidables_x ( COLLIST_ITEM *collidables, int num, int sort_algorithm );
static int _find_collisions_y ( COLLIST_ITEM *collidables, int num, int (*callback) (int n, void **list),	void *param );
//static int _find_collisions_x ( COLLIST_ITEM *collidables, int num, int (*callback) (int n, void **list),	void *param );
static void _sort_collidables_y_indirect ( COLLIST_ITEM **collidables, int num, int sort_algorithm );
static void _sort_collidables_x_indirect ( COLLIST_ITEM **collidables, int num, int sort_algorithm );
static int _find_collisions_y_indirect ( COLLIST_ITEM **collidables, int num, int (*callback) (int n, void **list), void *param );

//special ones used by latest algorithm
static int _find_collisions_x_indirect_skip ( COLLIST_ITEM **collidables, int num, COLLIST_ITEM *skip, int (*callback) (int n, void **list), void *param );
static int _find_collisions_2d_indirect ( COLLIST_ITEM **collidables, int num, int partitions, int (*callback) (int n, void **list), void *param, double *time );

//our attempts at wrapping so far...
static int _find_collisions_y_wrap ( COLLIST_ITEM *collidables, int num, int (*callback) (int n, void **list),	void *param, float max_x, float max_y );
//static int _find_collisions_2d_indirect_wrap ( COLLIST_ITEM **collidables, int num, int partitions, int (*callback) (int n, void **list), void *param, float max_x, float max_y, double *time );


//macros used for low-level 1-dimensional bounding-box collision detection
#define DIM_CHECK(value1,value2,range1,range2) (fabs((value1*2+range1) - (value2*2+range2)) >= ((range1) + (range2)))
//#define DIM_CHECK(value1,value2,range1,range2) (((value1) + (range1) <= (value2)) || ((value1) >= (value2) + (range2))) 
#define X_CHECK(c1,c2) DIM_CHECK((c1).x,(c2).x,(c1).w,(c2).w)
#define Y_CHECK(c1,c2) DIM_CHECK((c1).y,(c2).y,(c1).h,(c2).h)



//macros used for sorting / comparison
#define _COMPARE_ITEMS_2(i1,i2,primary,secondary) \
	(((i1).primary >= (i2).primary) && !(((i1).primary == (i2).primary) && ((i1).secondary < (i2).secondary)))
#define COMPARE_COLLIST_ITEMS_y_userdata(i1,i2) _COMPARE_ITEMS_2(i1,i2,y,userdata)
#define COMPARE_COLLIST_ITEMS_x_userdata(i1,i2) _COMPARE_ITEMS_2(i1,i2,x,userdata)


/*  Callback functions used by qsort  */
static int qsort_COLLIST_ITEM_y ( COLLIST_CONST  void *_a, COLLIST_CONST void *_b );
static int qsort_COLLIST_ITEM_x ( COLLIST_CONST void *_a, COLLIST_CONST void *_b );
static int qsort_COLLIST_ITEM_y_indirect ( COLLIST_CONST void *_a, COLLIST_CONST void *_b );
static int qsort_COLLIST_ITEM_x_indirect ( COLLIST_CONST void *_a, COLLIST_CONST void *_b );
static int qsort_pointers ( COLLIST_CONST void *_a, COLLIST_CONST void *_b );
static int qsort_random ( COLLIST_CONST void *_a, COLLIST_CONST void *_b );


/*  Private COLLIST methods  */
//static int update_collist ( COLLIST_LIST *collist );

enum { outbuf_size = 128 };

#ifdef COLLIST_USE_PMASK
#	define CHECK_PMASK_COLLISION(mask1,mask2,rx,ry) (!((mask1) && (mask2)) || check_pmask_collision((mask1), (mask2), (rx), (ry), 0, 0))
#else
#	define CHECK_PMASK_COLLISION(mask1,mask2,rx,ry) 1
#endif





































COLLIST_LIST *create_collist(void (*get_item_callback) (COLLIST_ITEM *coldata)) {
	COLLIST_LIST *ret = NULL;
	ret = malloc(sizeof(COLLIST_LIST));
	ret->get_item_callback = get_item_callback;
	ret->array = NULL;
	ret->array_size = 0;
	ret->num_items = 0;
	return ret;
}
void destroy_collist(COLLIST_LIST *collist) {
	free(collist->array);
	free(collist);
	return;
}
static void _collist_add (COLLIST_LIST *collist, int n) {
	int nm = collist->num_items + n;
	if (nm > collist->array_size) {
		nm = (nm + 15) &~ 15;//round up to the nearest multiple of 16
		collist->array = realloc(collist->array, nm * sizeof(COLLIST_ITEM));
		collist->array_size = nm;
	}
	collist->num_items += n;
	return;
}
void add_to_collist(COLLIST_LIST *collist, void *userdata) {
	_collist_add(collist, 1);
	collist->array[collist->num_items - 1].userdata = userdata;
	collist->get_item_callback(&collist->array[collist->num_items - 1]);
	if (collist->array[collist->num_items - 1].userdata == NULL) collist->num_items -= 1;
}
void add_to_collist_coldata(COLLIST_LIST *collist, COLLIST_ITEM *coldata) {
	_collist_add(collist, 1);
	collist->array[collist->num_items - 1] = *coldata;
	if (collist->array[collist->num_items - 1].userdata == NULL) collist->num_items -= 1;
}
void remove_from_collist(COLLIST_LIST *collist, void *userdata) {
	remove_n_from_collist(collist, 1, &userdata);
}
void remove_n_from_collist(COLLIST_LIST *collist, int num, void **userdatas) {
	int i, removed = 0;
	qsort ( userdatas, num, sizeof(void*), qsort_pointers);
	for (i = 0; i < collist->num_items; i += 1) {
		void *tmp = collist->array[i].userdata;
		if (bsearch(&tmp, userdatas, num, sizeof(void*), qsort_pointers)) {
			removed += 1;
		}
		else {
			collist->array[i - removed] = collist->array[i];
		}
	}
	collist->num_items -= removed;
}
void remove_from_collist_during_callback(COLLIST_ITEM *coldata) {
	coldata->x = 1e18f;
	coldata->y = 1e18f;
	coldata->w = 0;
	coldata->h = 0;
	coldata->userdata = NULL;
//	coldata->mask = NULL;
}
int update_collist ( COLLIST_LIST *collist ) {
	return _update_collist(collist, SORT_QSORT);
}
int _update_collist ( COLLIST_LIST *collist, int sort_algorithm ) {
	int i, n = collist->num_items;
	void (*get_item_callback) (COLLIST_ITEM *coldata) = collist->get_item_callback;
	COLLIST_ITEM *array = collist->array;
	int removed = 0;
	//update until first removal
	for (i = 0; i < n; i += 1) {
		get_item_callback(&array[i]);
		if (!array[i].userdata) {
			removed += 1;
			break;
		}
	}
	//update after first removal
	for (; i+removed < n; i += 1) {
		array[i].userdata = array[i+removed].userdata;
		get_item_callback(&array[i]);
		if (!array[i].userdata) {
			removed += 1;
			i -= 1;
		}
	}
	collist->num_items -= removed;

	n = collist->num_items;
/*	for (i = 0; i < n; i += 1) {
		if (
			(array[i].x <  collist->min_x) ||
			(array[i].y <  collist->min_y) ||
			(array[i].x >= collist->max_x) ||
			(array[i].y >= collist->max_y)
		)	DO_ERROR(); //remove me!
;
	}*/

	//sort
	if (sort_algorithm != -1) _sort_collidables_y (
		collist->array, 
		collist->num_items, 
		sort_algorithm
	);
	return removed;
}
int _do_collist_collisions ( 
	COLLIST_LIST *collist, 
	int (*collide_callback) (int n, void **list), 
	void *param, 
	int sort_algorithm,
	int complexity
) 
{
	int r = 0;
	void (*get_item_callback) (COLLIST_ITEM *coldata) = collist->get_item_callback;
	int i, n;
	_update_collist(collist, sort_algorithm);
	n = collist->num_items;


	if (!complexity) {
		complexity = (int)sqrt(n / 16);
		if (complexity < 10) complexity = 1;
	}

/*	complexity = 10;
	return _find_collisions_2d_On(
		collist->array, n, 
		collide_callback, 1, 
		0, 5000, 
		param
	);//*/

	if (complexity <= 1) 
		r = _find_collisions_y ( collist->array, n, collide_callback, param );
	else {
		COLLIST_ITEM **ptrs = malloc(sizeof(COLLIST_ITEM*) * n);
		for (i = 0; i < n; i += 1) {
			ptrs[i] = &collist->array[i];
		}
		//return _find_collisions_y_indirect ( ptrs, n, collide_callback, param );
		r += _find_collisions_2d_indirect ( ptrs, n, complexity, collide_callback, param, NULL );
		free(ptrs);
	}


	return r;
}
int _do_collist_collisions_wrap ( 
	COLLIST_LIST *collist, 
	int (*collide_callback) (int n, void **list), 
	void *param, 
	float max_x, float max_y, 
	int sort_algorithm,
	int complexity
) 
{
	int r = 0;
	void (*get_item_callback) (COLLIST_ITEM *coldata) = collist->get_item_callback;
	double a, b, c;
	int n;
	a = get_time_s();
	_update_collist(collist, sort_algorithm);
	n = collist->num_items;
	b = get_time_s();
	a = b - a;
//	_sort_collidables_y (collist->array, collist->num_items, sort_algorithm);
//	n = collist->num_items;

	c = get_time_s();
	b = c - b;

	if (!complexity) {
		complexity = (int)sqrt(n / 16);
		if (complexity < 8) complexity = 1;
	}
//	if (complexity <= 1) 
//	if (!key[KEY_M])
//		r = _find_collisions_y_wrap ( collist->array, n, collide_callback, param, max_x, max_y );
//	else
		r = _find_collisions_y ( collist->array, n, collide_callback, param );
/*	else {
		COLLIST_ITEM **ptrs = malloc(sizeof(COLLIST_ITEM*) * n);
		for (i = 0; i < n; i += 1) {
			ptrs[i] = &collist->array[i];
		}
		r += _find_collisions_2d_indirect_wrap ( ptrs, n, complexity, collide_callback, param, max_x, max_y, &b );
		free(ptrs);
	}*/

	c = get_time_s() - c;

	return r;
}
int do_collist_collisions ( 
	COLLIST_LIST *collist, 
	int (*collide_callback) (int n, void **list), 
	void *param
) 
{
	int r = 0;
	r = _do_collist_collisions (collist, collide_callback, param, SORT_BUBBLE, 0 );
	return r;
}


















static void _sort_collidables_y ( COLLIST_ITEM *collidables, int num, int algorithm ) {
	switch (algorithm) {
		default:
		case SORT_QSORT: {
			qsort ( collidables, num, sizeof(COLLIST_ITEM), qsort_COLLIST_ITEM_y);
		}
		break;
		case SORT_BUBBLE: {
			int i;
			i = 1;
			while ( i < num ) {
				if (COMPARE_COLLIST_ITEMS_y_userdata(collidables[i-1],collidables[i])) 
				{
					COLLIST_ITEM tmp = collidables[i];
					collidables[i] = collidables[i-1];
					collidables[i-1] = tmp;
					i -= 1;
					if (i == 0) i = 1;
				}
				else i += 1;
			}
			/*for (i = 1; i < num; i += 1) {
				if (collidables[i].y < collidables[i-1].y) {
					COLLIST_ITEM tmp = collidables[i];
					int j = i-1;
					while (1) {
						collidables[j+1] = collidables[j];
						if ((j == 0) || (tmp.y > collidables[j-1].y)) break;
						j -= 1;
					}
					collidables[j] = tmp;
				}
			}//*/
		}
		break;
	}
	return;
}

static void _sort_collidables_x ( COLLIST_ITEM *collidables, int num, int algorithm ) {
	switch (algorithm) {
		default:
		case SORT_QSORT: {
			qsort ( collidables, num, sizeof(COLLIST_ITEM), qsort_COLLIST_ITEM_x);
		}
		break;
		case SORT_BUBBLE: {
			int i;
			i = 1;
			while ( i < num ) {
				if (COMPARE_COLLIST_ITEMS_x_userdata(collidables[i-1],collidables[i])) 
				{
					COLLIST_ITEM tmp = collidables[i];
					collidables[i] = collidables[i-1];
					collidables[i-1] = tmp;
					i -= 1;
					if (i == 0) i = 1;
				}
				else i += 1;
			}
		}
		break;
	}
	return;
}


static void _sort_collidables_y_indirect ( COLLIST_ITEM **collidables, int num, int algorithm ) {
	switch (algorithm) {
		default:
		case SORT_QSORT: {
			qsort ( collidables, num, sizeof(COLLIST_ITEM*), qsort_COLLIST_ITEM_y_indirect);
			//_qsort_y_indirect ( collidables, num );
		}
		break;
		case SORT_BUBBLE: {
			int i;
			i = 1;
			while ( i < num ) {
				if (COMPARE_COLLIST_ITEMS_y_userdata(*(collidables[i-1]),*(collidables[i]))) 
				{
					COLLIST_ITEM *tmp = collidables[i];
					collidables[i] = collidables[i-1];
					collidables[i-1] = tmp;
					i -= 1;
					if (i == 0) i = 1;
				}
				else i += 1;
			}
		}
		break;
	}
	return;
}

static void _sort_collidables_x_indirect ( COLLIST_ITEM **collidables, int num, int algorithm ) {
	switch (algorithm) {
		default:
		case SORT_QSORT: {
			qsort ( collidables, num, sizeof(COLLIST_ITEM*), qsort_COLLIST_ITEM_x_indirect);
			//_qsort_y_indirect ( collidables, num );
		}
		break;
		case SORT_BUBBLE: {
			int i;
			i = 1;
			while ( i < num ) {
				if (COMPARE_COLLIST_ITEMS_x_userdata(*(collidables[i-1]),*(collidables[i]))) 
				{
					COLLIST_ITEM *tmp = collidables[i];
					collidables[i] = collidables[i-1];
					collidables[i-1] = tmp;
					i -= 1;
					if (i == 0) i = 1;
				}
				else i += 1;
			}
		}
		break;
	}
	return;
}














static int _find_collisions_y ( 
	COLLIST_ITEM *collidables, 
	int num, 
	int (*callback) (int n, void **list),
	void *param
)
{
	int r = 0;
	void *outbuf[outbuf_size * 2 + 1];
	int num_out = 0;
	int i, j;
	outbuf[0] = param;
	for (i = 0; i < num; i += 1) {
		float h = collidables[i].y + collidables[i].h;
		for (j = i+1; (j < num) && (collidables[j].y < h); j += 1) {
			if (X_CHECK(collidables[i], collidables[j])) continue;
			if (!CHECK_PMASK_COLLISION(
				collidables[i].mask, collidables[j].mask, 
				(int)(collidables[i].x - collidables[j].x), 
				(int)(collidables[i].y - collidables[j].y))) 
			{
				continue;
			}
			outbuf[num_out * 2 + 1] = collidables[i].userdata;
			outbuf[num_out * 2 + 2] = collidables[j].userdata;
			num_out += 1;
			if (num_out == outbuf_size) {
				r += num_out;
				if (callback) if (callback(num_out, outbuf)) return r;
				num_out = 0;
			}
		}
	}
	if ((num_out > 0) && callback) callback(num_out, outbuf);
	r += num_out;
	return r;
}

static int _find_collisions_y_indirect ( 
	COLLIST_ITEM **collidables, 
	int num, 
	int (*callback) (int n, void **list),
	void *param
)
{
	int r = 0;
	void *outbuf[outbuf_size * 2 + 1];
	int num_out = 0;
	int i, j;
	outbuf[0] = param;
	for (i = 0; i < num; i += 1) {
		float h = collidables[i]->y + collidables[i]->h;
//		float x1 = collidables[i]->x;
//		float x2 = x1 + collidables[i]->w;
		for (j = i+1; (j < num) && (collidables[j]->y < h); j += 1) {
			if (X_CHECK(*collidables[i], *collidables[j])) continue;
			if (!CHECK_PMASK_COLLISION(
				collidables[i]->mask, collidables[j]->mask, 
				(int)(collidables[i]->x - collidables[j]->x), 
				(int)(collidables[i]->y - collidables[j]->y))) 
			{
				continue;
			}
			outbuf[num_out * 2 + 1] = collidables[i]->userdata;
			outbuf[num_out * 2 + 2] = collidables[j]->userdata;
			num_out += 1;
			if (num_out == outbuf_size) {
				r += num_out;
				if (callback) if (callback(num_out, outbuf)) return r;
				num_out = 0;
			}
		}
	}
	if ((num_out > 0) && callback) callback(num_out, outbuf);
	r += num_out;
	return r;
}

static int _find_collisions_x ( 
	COLLIST_ITEM *collidables, 
	int num, 
	int (*callback) (int n, void **list),
	void *param
)
{
	int r = 0;
	void *outbuf[outbuf_size * 2 + 1];
	int num_out = 0;
	int i, j;
	outbuf[0] = param;
	for (i = 0; i < num; i += 1) {
		float w = collidables[i].x + collidables[i].w;
		for (j = i+1; (j < num) && (collidables[j].x < w); j += 1) {
			if (Y_CHECK(collidables[i], collidables[j])) continue;
			if (!CHECK_PMASK_COLLISION(
				collidables[i].mask, collidables[j].mask, 
				(int)(collidables[i].x - collidables[j].x), 
				(int)(collidables[i].y - collidables[j].y))) 
			{
				continue;
			}
			outbuf[num_out * 2 + 1] = collidables[i].userdata;
			outbuf[num_out * 2 + 2] = collidables[j].userdata;
			num_out += 1;
			if (num_out == outbuf_size) {
				r += num_out;
				if (callback) if (callback(num_out, outbuf)) return r;
				num_out = 0;
			}
		}
	}
	if ((num_out > 0) && callback) callback(num_out, outbuf);
	r += num_out;
	return r;
}

static int _find_collisions_x_indirect ( 
	COLLIST_ITEM **collidables, 
	int num, 
	int (*callback) (int n, void **list),
	void *param
)
{
	int r = 0;
	void *outbuf[outbuf_size * 2 + 1];
	int num_out = 0;
	int i, j;
	outbuf[0] = param;
	for (i = 0; i < num; i += 1) {
		float w = collidables[i]->x + collidables[i]->w;
		float y1 = collidables[i]->y;
		float y2 = y1 + collidables[i]->h;
		for (j = i+1; (j < num) && (collidables[j]->x < w); j += 1) {
			if (Y_CHECK(*collidables[i], *collidables[j])) continue;
			if (!CHECK_PMASK_COLLISION(
				collidables[i]->mask, collidables[j]->mask, 
				(int)(collidables[i]->x - collidables[j]->x), 
				(int)(collidables[i]->y - collidables[j]->y))) 
			{
				continue;
			}
			outbuf[num_out * 2 + 1] = collidables[i]->userdata;
			outbuf[num_out * 2 + 2] = collidables[j]->userdata;
			num_out += 1;
			if (num_out == outbuf_size) {
				r += num_out;
				if (callback) if (callback(num_out, outbuf)) return r;
				num_out = 0;
			}
		}
	}
	if ((num_out > 0) && callback) callback(num_out, outbuf);
	r += num_out;
	return r;
}

static int _find_collisions_x_indirect_skip ( 
	COLLIST_ITEM **collidables, 
	int num, 
//	float *skip_y, 
	COLLIST_ITEM *skip_ptr, 
	int (*callback) (int n, void **list),
	void *param
)
{
	int r = 0;
	void *outbuf[outbuf_size * 2 + 1];
	int num_out = 0;
	int i, j;
	outbuf[0] = param;
	for (i = 0; i < num; i += 1) {
		float w = collidables[i]->x + collidables[i]->w;
		float y1 = collidables[i]->y;
		float y2 = y1 + collidables[i]->h;
		for (j = i+1; (j < num) && (collidables[j]->x < w); j += 1) {
			if (Y_CHECK(*collidables[i], *collidables[j])) continue;
			if ((collidables[i] < skip_ptr) && (collidables[j] < skip_ptr))
				continue;
//			if ((y1 < skip_y) && (collidable[j]->y < skip_y))
//				continue;
			if (!CHECK_PMASK_COLLISION(
				collidables[i]->mask, collidables[j]->mask, 
				(int)(collidables[i]->x - collidables[j]->x), 
				(int)(collidables[i]->y - collidables[j]->y))) 
			{
				continue;
			}
			outbuf[num_out * 2 + 1] = collidables[i]->userdata;
			outbuf[num_out * 2 + 2] = collidables[j]->userdata;
			num_out += 1;
			if (num_out == outbuf_size) {
				r += num_out;
				if (callback) if (callback(num_out, outbuf)) return r;
				num_out = 0;
			}
		}
	}
	if ((num_out > 0) && callback) callback(num_out, outbuf);
	r += num_out;
	return r;
}


static int _find_collisions_2d_indirect ( 
	COLLIST_ITEM **collidables, 
	int num, 
	int y_partitions, 
	int (*callback) (int n, void **list),
	void *param,
	double *time
)
{
	int r = 0;
	int i, k;
	int n = num / y_partitions;
	int leftovers = 0;
	double b = 0, c = 0, t, t2;
	t = get_time_s();
	if (n) for (k = 0; k < y_partitions-1; k += 1) {
		int base = n * k - leftovers;
		COLLIST_ITEM *skip = collidables[base + leftovers];
		int maxn = n + leftovers;
		float maxy = collidables[base + maxn - 1]->y;
		int notleftover = 0;
		_sort_collidables_x_indirect(&collidables[base], maxn, SORT_QSORT);
		t2 = get_time_s();
		b += t2-t;
		r += _find_collisions_x_indirect_skip(&collidables[base], maxn, skip, callback, param);
		t = get_time_s();
		c += t - t2;
		for (i = 0; i < maxn; i += 1) {
			int i2 = base + maxn - 1 - i;
			if (collidables[i2]->y + collidables[i2]->h > maxy) {
				collidables[i2 + notleftover] = collidables[i2];
			}
			else notleftover += 1;
		}
		leftovers = maxn - notleftover;
	}
	if (1) {
		int base = n * (y_partitions - 1) - leftovers;
		int maxn = num - base;
		COLLIST_ITEM *skip = collidables[base + leftovers];
		float maxy = collidables[base + maxn - 1]->y;
//		int remove = 0;
		_sort_collidables_x_indirect(&collidables[base], maxn, SORT_QSORT);
		t2 = get_time_s();
		b += t2-t;
		r += _find_collisions_x_indirect_skip(&collidables[base], maxn, skip, callback, param);
//		r += _find_collisions_x_indirect(&collidables[base], maxn, callback, param);
		t = get_time_s();
		c += t - t2;
	}
//	atime2 = (atime2 * 7 + b) / 8;
//	atime3 = (atime3 * 7 + c) / 8;
	if (time) *time = c;
	return r;
}















static int qsort_COLLIST_ITEM_y ( COLLIST_CONST void *_a, COLLIST_CONST void *_b ) {
	COLLIST_CONST COLLIST_ITEM *a = _a;
	COLLIST_CONST COLLIST_ITEM *b = _b;
	return COMPARE_COLLIST_ITEMS_y_userdata(*a,*b) ? 1 : -1;
}
static int qsort_COLLIST_ITEM_x ( COLLIST_CONST void *_a, COLLIST_CONST void *_b ) {
	COLLIST_CONST COLLIST_ITEM *a = _a;
	COLLIST_CONST COLLIST_ITEM *b = _b;
	return COMPARE_COLLIST_ITEMS_x_userdata(*a,*b) ? 1 : -1;
}
static int qsort_COLLIST_ITEM_y_indirect ( COLLIST_CONST void *_a, COLLIST_CONST void *_b ) {
	COLLIST_CONST COLLIST_ITEM *COLLIST_CONST *a = _a;
	COLLIST_CONST COLLIST_ITEM *COLLIST_CONST *b = _b;
	return COMPARE_COLLIST_ITEMS_y_userdata(**a,**b) ? 1 : -1;
}
static int qsort_COLLIST_ITEM_x_indirect ( COLLIST_CONST void *_a, COLLIST_CONST void *_b ) {
	COLLIST_CONST COLLIST_ITEM *COLLIST_CONST *a = _a;
	COLLIST_CONST COLLIST_ITEM *COLLIST_CONST *b = _b;
	return COMPARE_COLLIST_ITEMS_x_userdata(**a,**b) ? 1 : -1;
}
static int qsort_pointers ( COLLIST_CONST void *_a, COLLIST_CONST void *_b ) {
	COLLIST_CONST void *COLLIST_CONST *a = _a;
	COLLIST_CONST void *COLLIST_CONST *b = _b;
	return (int)*a - (int)*b;
}
static int qsort_random ( COLLIST_CONST void *_a, COLLIST_CONST void *_b ) {
	if (rand() & 1) return 1;
	else return -1;
}


static float MIN_DIFF (float a, float b, float c) {
	a -= b;
	a += c / 2;
	a = fmod(a, c);
	if (a < 0) a += c;
	a -= c / 2;
	return a;
}

static int _find_collisions_y_wrap ( 
	COLLIST_ITEM *collidables, 
	int num, 
	int (*callback) (int n, void **list),
	void *param, 
	float max_x, float max_y
)
{
	int r = 0;
	void *outbuf[outbuf_size * 2 + 1];
	int num_out = 0;
	int i, j;
	outbuf[0] = param;
	for (i = 0; i < num; i += 1) {
		float y = collidables[i].y;
		float h = collidables[i].y + collidables[i].h;
		j = i + 1;
		while (1) {
			float x = collidables[i].x;
			while (1) {
				for (; (j < num) && (collidables[j].y < h); j += 1) {
					if (DIM_CHECK(x, collidables[j].x, collidables[i].w, collidables[j].w)) continue;
					if (j == i) continue;
					if (!CHECK_PMASK_COLLISION(
						collidables[i].mask, collidables[j].mask, 
						(int)(x - collidables[j].x), 
						(int)(y - collidables[j].y)))
					{
						continue;
					}
					outbuf[num_out * 2 + 1] = collidables[i].userdata;
					outbuf[num_out * 2 + 2] = collidables[j].userdata;
					num_out += 1;
					if (num_out == outbuf_size) {
						r += num_out;
						if (callback) if (callback(num_out, outbuf)) return r;
						num_out = 0;
					}
				}
				x -= max_x;
				if (x + collidables[i].w < 0) break;
				j = 0;
			}
			h -= max_y;
			if (h < 0) break;
			y -= max_y;
			j = 0;
		}
	}
	if ((num_out > 0) && callback) callback(num_out, outbuf);
	r += num_out;
	return r;
}
/*
typedef struct BOB {
	COLLIST_ITEM *data;
	struct BOB *next;
} BOB;


static int _find_collisions_y_BOB ( 
	COLLIST_CONST BOB *top, 
	int (*callback) (int n, void **list),
	void *param
)
{
	int r = 0;
	void *outbuf[outbuf_size * 2 + 1];
	int num_out = 0;
	outbuf[0] = param;
	for ( ; top; top = top->next) {
		COLLIST_CONST BOB *frog;
		float x = top->data->x, y = top->data->y;
		float h = y + top->data->h;
		PMASK *mask = top->data->mask;
		frog = top->next;
		for (frog = top->next; frog && frog->data->y < h; frog = frog->next) {
			if (X_CHECK(*top->data, *frog->data)) continue;
			if (!CHECK_PMASK_COLLISION(
				mask, frog->data->mask, 
				(int)(x - frog->data->x), 
				(int)(top->data->y - frog->data->y))) 
			{
				continue;
			}
			outbuf[num_out * 2 + 1] = top->data->userdata;
			outbuf[num_out * 2 + 2] = frog->data->userdata;
			num_out += 1;
			if (num_out == outbuf_size) {
				r += num_out;
				if (callback) if (callback(num_out, outbuf)) return r;
				num_out = 0;
			}
		}
	}
	if ((num_out > 0) && callback) callback(num_out, outbuf);
	r += num_out;
	return r;
}

int _find_collisions_2d_On ( 
	COLLIST_ITEM *collidables, 
	int num, 
	int (*callback) (int n, void **list), 
	int x_partitions, 
	double min_x, double max_x,
	void *param
)
{
	int i;
	int r = 0;
	struct BOB **head = malloc(x_partitions * sizeof(BOB*));
	for (i = 0; i < x_partitions; i += 1) head[i] = NULL;
	for (i = num-1; i >= 0; i -= 1) {
		int section = 0;//floor((x_partitions-0.1) * (collidables[i].x - min_x) / (max_x-min_x));
		struct BOB *tmp = malloc(sizeof(BOB));
		if (section < 0) DO_ERROR(); //remove me!
		if (section >= x_partitions) DO_ERROR(); //remove me!
		tmp->next = head[section];
		tmp->data = &collidables[i];
		head[section] = tmp;
	}
	for (i = 0; i < x_partitions; i += 1) {
		_find_collisions_y_BOB(head[i], callback, param);
	}
	free(head);
	return 0;
}
*/

static int _find_collisions_y_interset_indirect ( COLLIST_ITEM **set1, int num1, COLLIST_ITEM **set2, int num2, int (*callback) (int n, void **list), void *param ) {
	int r = 0;
	void *outbuf[outbuf_size * 2 + 1];
	int num_out = 0;
	int i, j, k;
	float y1, h1;
	outbuf[0] = param;
	k = 0;
	for (i = 0; i < num1; i += 1) {
		y1 = set1[i]->y;
		h1 = y1 + set1[i]->h;
loopy1:	if ( (set2[k]->y < y1) ) goto loopy2;
		k += 1;
		if ( k == num2 ) break;
		goto loopy1;
loopy2:
//		while ( set2[k]->y < y1 ) k += 1;
		for ( j = k; j < num2 && set2[j]->y < h1; j += 1) {
			if (X_CHECK(*set1[i], *set2[j])) continue;
			if (!CHECK_PMASK_COLLISION(
				set1[i]->mask, set2[j]->mask, 
				(int)(set1[i]->x - set2[j]->x), 
				(int)(set1[i]->y - set2[j]->y))) 
			{
				continue;
			}
			outbuf[num_out * 2 + 1] = set1[i]->userdata;
			outbuf[num_out * 2 + 2] = set2[j]->userdata;
			num_out += 1;
			if (num_out == outbuf_size) {
				r += num_out;
				if (callback) if (callback(num_out, outbuf)) return r;
				num_out = 0;
			}
		}
	}
	if ((num_out > 0) && callback) callback(num_out, outbuf);
	r += num_out;
	return r;
}
/*
int _find_collisions_2d2_indirect ( 
	COLLIST_LIST *collist,
	int partitions, 
	int (*callback) (int n, void **list),
	void *param
)
{
	int i, j;
	int num = collist->num_items;
	int c = 0;
	int safety = partitions + 1;
	float min_x = collist->min_x;
	float inv_x = partitions / (collist->max_x - min_x);
	COLLIST_ITEM **ia = malloc ( sizeof ( COLLIST_ITEM * ) * num );
	COLLIST_ITEM *array = collist->array;
	int *ii = malloc ( sizeof ( int ) * safety );
	int *ii2 = malloc ( sizeof ( int ) * safety );
	memset(ii, 0, sizeof(int) * safety );
	for (i = 0; i < num; i += 1) {
		j = (int)((array[i].x - min_x) * inv_x);
		ii[j] += 1;
	}
	ii2[0] = 0;
	for (i = 1; i < safety; i += 1) {
		ii2[i] = ii2[i-1] + ii[i-1];
	}
	if ((ii2[partitions] != num) || (ii[partitions] != 0)) {
		DO_ERROR(); //remove me!
	}
	for (i = 0; i < num; i += 1) {
		j = (int)((array[i].x - min_x) * inv_x);
		ia[ii2[j]++] = &array[i];
	}
	c += _find_collisions_y_indirect ( &ia[0], ii[0], callback, param );
	for (i = 1; i < partitions; i += 1) {
		c += _find_collisions_y_interset_indirect ( &ia[ii2[i-1] - ii[i-1]], ii[i-1], &ia[ii2[i]-ii[i]], ii[i], callback, param );
		c += _find_collisions_y_interset_indirect ( &ia[ii2[i]-ii[i]], ii[i], &ia[ii2[i-1] - ii[i-1]], ii[i-1], callback, param );
		c += _find_collisions_y_indirect ( &ia[ii2[i]-ii[i]], ii[i], callback, param );
	}
	free ( ii );
	free ( ii2 );
	free ( ia );
	return c;
}
*/
