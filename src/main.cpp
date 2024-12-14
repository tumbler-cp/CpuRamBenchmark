#include <cstdio>
#include <ram_bench.h>
#include <string>



int main(const int argc, char **argv) {
  if (argc != 3) {
    perror("Wrong number of arguments. Shouldbe: ./CustomBench <filesize> <target number>");
  }

  auto *filename = "cpu.txt";

  generate_file(filename, static_cast<int>(std::stol(argv[1], nullptr, 10)), 42);

  const int target = static_cast<int>(std::stol(argv[2], nullptr, 10));
  constexpr  int repetitions = 100000;

  load_memory(filename, target, repetitions);

  return 0;
}
