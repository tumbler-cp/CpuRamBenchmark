#ifndef CPU_BENCH_H
#define CPU_BENCH_H

#include <sys/types.h>

void generate_random_array(int *array, size_t size);
void sort_array(int *array, size_t size, int repetitions);

#endif //CPU_BENCH_H
