#ifndef collist_H
#define collist_H

#ifdef __cplusplus
	extern "C" {
#endif

#define COLLIST_VERSION 0.90

#define COLLIST_CONST const

enum {	SORT_QSORT = 0,	SORT_BUBBLE = 1 };

typedef struct COLLIST_ITEM {
	float x, y, w, h;       //bounding box
	void *userdata;         //id used to report collision (user defined data format)
} COLLIST_ITEM;

typedef struct COLLIST_LIST {
	int num_items;
	int array_size;
	COLLIST_ITEM *array;
	void (*get_item_callback) (COLLIST_ITEM *coldata);
} COLLIST_LIST;

//create a new collidables list
COLLIST_LIST *create_collist(void (*get_item_callback) (COLLIST_ITEM *coldata));

//destroy a collidables list
void destroy_collist(COLLIST_LIST *collist);

//add element to a collidables list
void add_to_collist(COLLIST_LIST *collist, void *userdata);
//void add_to_collist_coldata(COLLIST_LIST *collist, COLLIST_ITEM *coldata);

//remove element from a collidables list (slow)
void remove_from_collist(COLLIST_LIST *collist, void *userdata);
//remove many elements from a collidables list (faster) (untested)
void remove_n_from_collist(COLLIST_LIST *collist, int num, void **userdata);
//remove element from collidables list during get_item_callback (fastest)
void remove_from_collist_during_callback(COLLIST_ITEM *coldata);

//you can call this to speed up unusual cases where large number of 
//objects are created / destroyed in a single tick
int update_collist ( COLLIST_LIST *collist );
int _update_collist ( COLLIST_LIST *collist, int sort_algorithm );
int _update_collist2 ( COLLIST_LIST *collist, int sort_algorithm );//expiremental

//check for collisions
int do_collist_collisions (
	COLLIST_LIST *collist, //your COLLIST_LIST
	int (*callback) (int n, void **array), //your collision handling callback
	void *param //a parameter for your collision handling callback
);
//check for collisions, with control over internal optimization parameters
int _do_collist_collisions (
	COLLIST_LIST *collist, //your COLLIST_LIST
	int (*callback) (int n, void **array), //your collision handling callback
	void *param, //a parameter for your collision handling callback
	int sort_algorithm, // SORT_BUBBLE or SORT_QSORT
	int complexity  //0 for automatic, 1 for simple, > 1 for manual
);

//expiremental
int _do_collist_collisions_wrap ( 
	COLLIST_LIST *collist, 
	int (*collide_callback) (int n, void **array), 
	void *param, 
	float max_x, float max_y, 
	int sort_algorithm,
	int complexity
);

//expiremental
int _find_collisions_2d2_indirect ( 
	COLLIST_LIST *collist,
	int partitions, 
	int (*callback) (int n, void **list),
	void *param
);





/*
int _find_collisions_2d_On ( 
	COLLIST_ITEM *collidables, 
	int num, 
	int (*callback) (int n, void **array), 
	int x_partitions, 
	double min_x, double max_x,
	void *param
);
*/


#ifdef __cplusplus
	}
#endif


#endif  /* ifndef collist_H */
