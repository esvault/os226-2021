#include <stddef.h>

#include "pool.h"

void pool_init(struct pool *p, void *mem, unsigned long nmemb, unsigned long membsz) {
}

void *pool_alloc(struct pool *p) {
    struct pool_free_block *fb = p->free;
    if (fb) {
        p->free = fb->next;
        return fb;
    }
    if (p->freestart < p->freeend) {
        void *r = p->freestart;
        p->freestart = (char *) p->freestart + p->membsz;
        return r;
    }
	return NULL;
}

int belong_pool(struct pool *p, void *ptr) {
    return p->mem <= ptr && p->freeend >= ptr;
}

void pool_free(struct pool *p, void *ptr) {
    if (belong_pool(p, ptr)) {
        struct pool_free_block *fb = ptr;
        *fb = (struct pool_free_block) { .next = p->free };
        p->free = fb;
    }
}
