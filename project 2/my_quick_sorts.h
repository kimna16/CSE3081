typedef struct {
	unsigned int score;
	float data[3];
	char comments[16];
} ELEMENT;

typedef int _Cmpfun(const void*, const void*);

void swapFunc(void*, void*);

size_t partition(void*, size_t, _Cmpfun*);
size_t median_partition(void*, size_t, _Cmpfun*);

void qsort_orig(void*, size_t, size_t, _Cmpfun*);
void qsort_median_insert(void*, size_t, size_t, _Cmpfun*);
void qsort_median_insert_iter(void*, size_t, size_t, _Cmpfun*);
void isort(void*, size_t, size_t, _Cmpfun*);
