#ifndef RAM_BENCH_H
#define RAM_BENCH_H

#include <sys/types.h>
#include <string>

bool search_in_file(const std::string &fileName, int target);
void load_memory(const std::string& fileName, int target, size_t iterations);
void generate_file(const char *filename, size_t file_size_mb, int seed);

#endif // RAM_BENCH_H
