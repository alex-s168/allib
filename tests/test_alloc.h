#define test_alloc(ally) \
    void *a = yalloc(ally, 32);   \
    void *b = yalloc(ally, 64);   \
    void *c = yalloc(ally, 32);   \
    void *d = yalloc(ally, 1024); \
	void *e = yalloc(ally, 32);   \
    void *f = yalloc(ally, 2048); \
	assert(a != NULL); \
    assert(b != NULL); \
    assert(c != NULL); \
    assert(d != NULL); \
    assert(e != NULL); \
    assert(f != NULL); \
	/* check if we own all allocations -> no segfault */ \
    (void) *(volatile char *)a; \
    (void) *(volatile char *)b; \
    (void) *(volatile char *)c; \
    (void) *(volatile char *)d; \
	(void) *(volatile char *)e; \
    (void) *(volatile char *)f; \
	yfree(ally, a, 32);   \
    yfree(ally, b, 64);   \
    yfree(ally, c, 32);   \
    yfree(ally, d, 1024); \
    yfree(ally, e, 32);   \
    yfree(ally, f, 2048);
