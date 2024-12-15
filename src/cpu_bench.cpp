#include "cpu_bench.h"

#include <chrono>
#include <cstdlib>
#include <iostream>

void generate_random_array(int *array, const size_t size) {
  for (size_t i = 0; i < size; ++i) {
    array[i] = rand() % 100000; // NOLINT
  }
}

int compare(const void *a, const void *b) {
    return *(int *)(a) - *(int *)b; // NOLINT
}

void sort_array(int *array, const size_t size, const int repetitions) {
  for (int i = 0; i < repetitions; ++i) {
    auto start = std::chrono::high_resolution_clock::now();
    qsort(array, size, sizeof(int), compare);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << i << ". Elapsed: " << elapsed_seconds.count() << std::endl;
    std::cout << "Iteration " << i + 1 <<  " sorted" << std::endl;
    generate_random_array(array, size);
  }
}
