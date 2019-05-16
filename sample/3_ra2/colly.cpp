#include<stdlib.h>
#include"colly.h"

TColly::TColly(int asize, int agrow)
{
    size = asize; grow = agrow; count = 0;
    items = (void **) malloc(size*sizeof(void *));
}

TColly::~TColly()
{
    free(items);
//    fprintf(stderr, "colly destroyed and had %d items\n", count);
}

void TColly::insert(void *p)
{
    items[count++] = p;
    if (count > size) {
        size += grow;
        items = (void **) realloc(items, size*sizeof(void *));
    }
}

int TColly::remove(void *p)
{
    int i = 0, f = 0;
    while (i<count) {
        if (f = (items[i] == p)) break;
        i++;
    }
    if (f) items[i] = items[--count];
    return f;
}

int TColly::indexof(void *p)
{
    int i;
    for (i = count-1; i>=0; i--) if (items[i]==p) break;
    return i;
}

