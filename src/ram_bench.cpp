#include "ram_bench.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

#define CHUNK_SIZE 4096

bool search_in_file(const std::string& fileName, const int target) {
  const int fd = open(fileName.c_str(), O_RDONLY);
  if (fd < 0) {
    perror("Error opening file");
    return false;
  }

  constexpr size_t bufferSize = 4096;
  std::vector<char> buffer(bufferSize);
  bool found = false;

  while (true) {
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
    if (found) break;
  }

  close(fd);
  return found;
}

void load_memory(const std::string& fileName, const int target, const size_t iterations) {
  for (size_t i = 0; i < iterations; ++i) {
    if (!search_in_file(fileName, target)) {
      std::cerr << "Target not found in iteration " << i + 1 << "\n";
    }
  }
}

void generate_file(const char *filename, const size_t file_size_mb, const int seed) {
  const size_t total_bytes = file_size_mb * 1024 * 1024;
  char buffer[CHUNK_SIZE];

  srand(seed);

  for (char & i : buffer) {
    i = static_cast<char>(rand() % 256); // NOLINT
  }

  const int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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

  std::cout << "File "<< filename <<" of size " <<  file_size_mb <<" MB successfully generated.\n";

  close(fd);
}
