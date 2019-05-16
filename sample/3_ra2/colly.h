#ifndef __TColly__
#define __TColly__

/* simple collection */
class TColly {
public:
    void **items;
    int size, grow, count;

    TColly(int asize, int agrow);
    ~TColly();
    void insert(void *p);
    int  remove(void *p);
    int  indexof(void *p);
};

#endif
