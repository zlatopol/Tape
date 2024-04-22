#include "Tape.h"

Tape::Proxy::Proxy(Tape *tape) : tape_(tape) {}

const Tape::Proxy &Tape::Proxy::operator=(const Proxy &o) {
  return (*this = (int)o);
}

const Tape::Proxy &Tape::Proxy::operator=(int val) {
  tape_->write(val);
  return *this;
}

Tape::Proxy::operator int() const { return tape_->read(); }

Tape::Iterator::Iterator(Tape *tape, std::size_t offset)
    : tape_(tape), offset_(offset) {}

std::size_t Tape::Iterator::offset() const { return offset_; }

Tape::Proxy Tape::Iterator::operator*() { return Proxy{tape_}; }

Tape::Iterator &Tape::Iterator::operator++() {
  if (!tape_->end_of_file()) {
    tape_->next();
  }

  offset_ += sizeof(int);

  offset_ = std::min(tape_->size(), offset_);

  return *this;
}

Tape::Iterator Tape::Iterator::operator++(int) {
  Tape::Iterator it(*this);
  ++*this;
  return it;
}

bool Tape::Iterator::operator==(Iterator o) {
  return tape_ == o.tape_ && offset_ == o.offset_;
}

bool Tape::Iterator::operator!=(Iterator o) { return !(*this == o); }

Tape::Iterator Tape::begin() { return Iterator{this, offset()}; }

Tape::Iterator Tape::end() { return Iterator{this, size()}; }

std::size_t Tape::size() const { return file_size_; }

std::size_t Tape::max_offset() const { return file_size_ - sizeof(int); }

std::size_t Tape::offset() const { return offset_; }

Tape::Tape(std::filesystem::path file, std::size_t file_size,
           std::chrono::milliseconds delay_write,
           std::chrono::milliseconds delay_read,
           std::chrono::milliseconds delay_rewind,
           std::chrono::milliseconds delay_prev,
           std::chrono::milliseconds delay_next)
    : file_(file, std::ios::binary | std::ios::in | std::ios::out),
      file_size_(file_size), delay_next_(delay_next), delay_prev_(delay_prev),
      delay_read_(delay_read), delay_rewind_(delay_rewind),
      delay_write_(delay_write) {
  if (!file_.is_open()) {
    std::cerr << "Ошибка открытия файла" << std::endl;
    throw std::invalid_argument{"file is not opened"};
  }
}

Tape::~Tape() { file_.close(); }

void Tape::write(int data) {
  // Реализация записи данных на ленту
  file_.seekp(offset_);
  file_.write((char *)&data, sizeof(int));
  file_.flush();
  std::this_thread::sleep_for(delay_write_);
}

int Tape::read() {
  int num;
  file_.seekg(offset_);
  file_.read((char *)&num, sizeof(int));
  std::this_thread::sleep_for(delay_read_);
  return num;
}

// Перемотка ленты в начало
void Tape::rewind() {
  offset_ = 0;
  std::this_thread::sleep_for(delay_rewind_);
}

void Tape::prev() {
  // Сдвиг влево
  if (offset_ == 0) {
    std::cerr << "Can't go back\n";
    throw std::invalid_argument{"offset_ at the begining"};
  }

  offset_ -= sizeof(int);
  std::this_thread::sleep_for(delay_prev_);
}

void Tape::next() {
  // Сдвиг вправо
  if (!end_of_file()) {
    offset_ += sizeof(int);
  } else {
    std::cerr << "Can't go next\n";
    throw std::invalid_argument{"offset_ at the end"};
  }
  std::this_thread::sleep_for(delay_next_);
}

bool Tape::end_of_file() {
  // Проверка, находится ли текущая позиция ленты в конце
  return offset_ == max_offset();
}
