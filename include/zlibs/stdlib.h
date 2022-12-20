#ifndef STDLIB_ID
#define STDLIB_ID 1007

#include <type.h>

#define get_func_addr ((int (*)(int, int)) *(int *) 0x199994)

/*
int main();
void *calloc(uint32_t nmemb, uint32_t lsize);
void free(void *mem);
void *malloc(uint32_t size);
void *realloc(void *ptr, uint32_t size);
*/

#define calloc(nmemb, lsize) ((void *(*)(uint32_t, uint32_t)) get_func_addr(STDLIB_ID, 3))(nmemb, lsize)
#define free(mem) ((void (*)(void *)) get_func_addr(STDLIB_ID, 4))(mem)
#define malloc(size) ((void *(*)(uint32_t)) get_func_addr(STDLIB_ID, 5))(size)
#define realloc(ptr, size) ((void *(*)(void *, uint32_t)) get_func_addr(STDLIB_ID, 6))(ptr, size)

#endif
