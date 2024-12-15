#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#include "cpu_bench.h"
#include "ram_bench.h"

using namespace std;

void *mem_load(void *arg) {
  (void) arg;
  const string filename = "benchmark_hyb_ram_file.txt";
  generate_file(filename.c_str(), 1024, 42);
  load_memory(filename, 42, 1000);
  return 0;
}

void *cpu_load(void *arg) {
  (void) arg;
  auto *array = static_cast<int *>(malloc(10000000 * sizeof(int)));
  generate_random_array(array, 10000000);
  sort_array(array, 10000000, 1000);
  free(array);
  return 0;
}

int main(const int argc, char **argv) {
  if (argc < 2) {
    cerr << "Not enough arguments: ./<program> <benchmark type (cpu|ram)> <arg1> ... <argn>" << endl;
    return EXIT_FAILURE;
  }

  string benchmark_type = argv[1];
  if (benchmark_type == "ram") {
    if (argc != 5) {
      cerr << "Wrong number of arguments. Usage: ./CustomBench ram <filesize> <target number> <iterations>" << endl;
      return EXIT_FAILURE;
    }

    try {
      const int filesize = stoi(argv[2]);
      const int target = stoi(argv[3]);
      const int iterations = stoi(argv[4]);

      const string filename = "benchmark_ram_file.txt";
      generate_file(filename.c_str(), filesize, 42);
      load_memory(filename, target, iterations);
    } catch (const exception &e) {
      cerr << "Error parsing arguments for RAM benchmark: " << e.what() << endl;
      return EXIT_FAILURE;
    }

  } else if (benchmark_type == "cpu") {
    if (argc != 4) {
      cerr << "Wrong number of arguments. Usage: ./CustomBench cpu <array size> <iterations>" << endl;
      return EXIT_FAILURE;
    }

    try {
      const int size = stoi(argv[2]);
      const int iterations = stoi(argv[3]);

      auto *array = static_cast<int *>(malloc(size * sizeof(int)));
      if (!array) {
        cerr << "Failed to allocate memory for array." << endl;
        return EXIT_FAILURE;
      }

      cout << "Running CPU benchmark..." << endl;
      generate_random_array(array, size);
      sort_array(array, size, iterations);

      free(array);
    } catch (const exception &e) {
      cerr << "Error parsing arguments for CPU benchmark: " << e.what() << endl;
      return EXIT_FAILURE;
    }

  } else if (benchmark_type == "hyb") {
    pthread_t threads[2];
    if (pthread_create(&threads[0], nullptr, mem_load, nullptr) != 0) {
      perror("Failed to create thread mem_load");
      exit(EXIT_FAILURE);
    }
    if (pthread_create(&threads[0], nullptr, cpu_load, nullptr) != 0) {
      perror("Failed to create thread cpu_load");
      exit(EXIT_FAILURE);
    }
    pthread_join(threads[0], nullptr);
    pthread_join(threads[1], nullptr);

  } else {
    cerr << "No such benchmark type: " << benchmark_type << ". Supported types: cpu, ram." << endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
