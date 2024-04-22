#pragma once

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <thread>

class Tape {
  class Proxy {
  public:
    Proxy(Tape *tape);

    Proxy(const Proxy &) = delete;
    const Proxy &operator=(const Proxy &);

    const Proxy &operator=(int);
    operator int() const;

  private:
    Tape *tape_;
  };

public:
  class Iterator {
  public:
    using value_type = int;
    using iterator_category = std::forward_iterator_tag;
    using difference_type = ptrdiff_t;
    using pointer = Proxy *;
    using reference = Proxy;

    Iterator(Tape *tape, std::size_t offset);

    std::size_t offset() const;

    Proxy operator*();
    Iterator &operator++();
    Iterator operator++(int);

    bool operator==(Iterator o);

    bool operator!=(Iterator o);

  private:
    Tape *tape_;
    std::size_t offset_;
  };

  Iterator begin();

  Iterator end();

  Tape(std::filesystem::path file, std::size_t file_size,
       std::chrono::milliseconds delay_write,
       std::chrono::milliseconds delay_read,
       std::chrono::milliseconds delay_rewind,
       std::chrono::milliseconds delay_prev,
       std::chrono::milliseconds delay_next);

  Tape(Tape &&) = default;
  Tape &operator=(Tape &&) = default;

  ~Tape();

  void write(int data);

  std::size_t offset() const;

  std::size_t max_offset() const;

  std::size_t size() const;

  int read();

  // Перемотка ленты в начало
  void rewind();

  void prev();

  void next();

  bool end_of_file();

private:
  std::fstream file_;
  std::size_t offset_{0};
  std::size_t file_size_{};
  std::chrono::milliseconds delay_write_ = std::chrono::milliseconds(1);
  std::chrono::milliseconds delay_read_ = std::chrono::milliseconds(1);
  std::chrono::milliseconds delay_rewind_ = std::chrono::milliseconds(1);
  std::chrono::milliseconds delay_next_ = std::chrono::milliseconds(1);
  std::chrono::milliseconds delay_prev_ = std::chrono::milliseconds(1);
};