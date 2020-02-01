#include "bitmap_ptr.h"

static void bitmap_deleter(BITMAP * p) { 
    destroy_bitmap(p);
} 

BITMAP_PTR bitmap_ptr(BITMAP * ptr) {
    return BITMAP_PTR(ptr, bitmap_deleter);
}
