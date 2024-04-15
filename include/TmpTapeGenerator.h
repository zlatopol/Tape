#pragma once

#include <chrono>

#include "Tape.h"

class TmpTapeGenerator {
public:
    TmpTapeGenerator(std::chrono::milliseconds delay) : delay_(delay) {}

    std::filesystem::path generate(std::size_t file_size);
    Tape fromPath(std::filesystem::path path);

    ~TmpTapeGenerator();

private:
    std::chrono::milliseconds delay_;
    int counter_{};
};