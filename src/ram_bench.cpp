#include "ram_bench.h"

#include <chrono>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <vector>

#define CHUNK_SIZE 4096

bool search_in_file(const std::string &fileName, const int target) {
  const int fd = open(fileName.c_str(), O_RDONLY | O_DIRECT);
  if (fd < 0) {
    perror("Error opening file");
    return false;
  }

  std::vector<char> buffer(sizeof(int));
  bool found = false;

  while (true) {
    constexpr size_t bufferSize = 4096;
    const ssize_t bytesRead = read(fd, buffer.data(), bufferSize);
    if (bytesRead < 0) {
      perror("Error reading file");
      close(fd);
      return false;
    }
    if (bytesRead == 0) {
      break;
    }

    for (size_t i = 0; i < bytesRead; i += sizeof(int)) {
      int value;
      if ((bytesRead - i) >= sizeof(int)) {
        std::memcpy(&value, &buffer[i], sizeof(int));
        if (value == target) {
          found = true;
          break;
        }
      }
    }
    if (found)
      break;
  }

  close(fd);
  return found;
}


void load_memory(const std::string &fileName, const int target, const size_t iterations) {
  for (size_t i = 0; i < iterations; ++i) {
    auto start = std::chrono::high_resolution_clock::now();
    if (!search_in_file(fileName, target)) {
      std::cerr << "Target not found in iteration " << i + 1 << "\n";
    } else {
      std::cout << "Target found in iteration " << i + 1 << "\n";
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << diff.count() << " seconds\n";
  }
}

void generate_file(const char *filename, const size_t file_size_mb, const int seed) {
  const size_t total_bytes = file_size_mb * 1024 * 1024;
  char buffer[CHUNK_SIZE];

  srand(seed);

  for (char &i: buffer) {
    i = static_cast<char>(rand() % 1000); // NOLINT
  }

  const int fd = open(filename, O_WRONLY | O_CREAT | O_DIRECT, 0644);
  if (fd < 0) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  size_t bytes_written = 0;
  while (bytes_written < total_bytes) {
    const size_t to_write = (total_bytes - bytes_written) < CHUNK_SIZE ? (total_bytes - bytes_written) : CHUNK_SIZE;
    const ssize_t written = write(fd, buffer, to_write);
    if (written < 0) {
      perror("Error writing to file");
      close(fd);
      exit(EXIT_FAILURE);
    }
    bytes_written += written;
  }

  std::cout << "File " << filename << " of size " << file_size_mb << " MB successfully generated.\n";

  close(fd);
}
