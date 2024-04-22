#pragma once

#include <chrono>

#include "Tape.h"

class TmpTapeGenerator {
public:
  TmpTapeGenerator(std::chrono::milliseconds delay_write,
                   std::chrono::milliseconds delay_read,
                   std::chrono::milliseconds delay_rewind,
                   std::chrono::milliseconds delay_prev,
                   std::chrono::milliseconds delay_next);
  TmpTapeGenerator() = default;

  std::filesystem::path generate(std::size_t file_size);
  Tape fromPath(std::filesystem::path path);

  ~TmpTapeGenerator();

private:
  int counter_{};
  std::chrono::milliseconds delay_write_ = std::chrono::milliseconds(1);
  std::chrono::milliseconds delay_read_ = std::chrono::milliseconds(1);
  std::chrono::milliseconds delay_rewind_ = std::chrono::milliseconds(1);
  std::chrono::milliseconds delay_next_ = std::chrono::milliseconds(1);
  std::chrono::milliseconds delay_prev_ = std::chrono::milliseconds(1);
};