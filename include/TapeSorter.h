#pragma once

#include <Tape.h>
#include <TmpTapeGenerator.h>

class TapeSorter {
public:
  TapeSorter(TmpTapeGenerator tape_generator, Tape input, Tape output_,
             std::size_t max_memory);

  void sort();

  Tape getOutputTape();

private:
  void merge(Tape &fst, Tape &snd, Tape &out);

  TmpTapeGenerator tape_generator_;
  Tape input_;
  Tape output_;
  std::size_t max_int_cnt_;
};
