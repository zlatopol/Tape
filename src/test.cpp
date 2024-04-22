#include <Tape.h>
#include <TapeSorter.h>
#include <TmpTapeGenerator.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>

TEST(Tape, Tape_rewind) {
  int arr[] = {1, 5, 6, 7, 9, 19};
  TmpTapeGenerator generator{{}, {}, {}, {}, {}};
  auto tape = generator.fromPath(generator.generate(sizeof(arr) + 4));
  for (auto &l : arr) {
    tape.write(l);
    tape.next();
  }
  tape.rewind();
  for (auto &l : arr) {
    ASSERT_EQ(tape.read(), l);
    tape.next();
  }
}
TEST(Tape, Tape_write) {
  TmpTapeGenerator generator{{}, {}, {}, {}, {}};
  auto tape = generator.fromPath(generator.generate(sizeof(int)));
  tape.write(10);
  ASSERT_EQ(tape.read(), 10);
}
TEST(Tape, Tape_read) {
  TmpTapeGenerator generator{{}, {}, {}, {}, {}};
  std::vector<int> vec = {1, 2, 3, 4};
  auto tape = generator.fromPath(generator.generate(vec.size() * sizeof(int)));
  std::copy(vec.begin(), vec.end(), tape.begin());
  tape.rewind();
  for (int i = 0; i < vec.size(); i++) {
    ASSERT_EQ(tape.read(), vec[i]);
    if (i != vec.size() - 1)
      tape.next();
  }
}
TEST(TmpTapeGenerator, generator) {
  TmpTapeGenerator generator{{}, {}, {}, {}, {}};
  auto tape = generator.fromPath(generator.generate(sizeof(int)));
  ASSERT_EQ(tape.size(), sizeof(int));
}
TEST(TapeSorter, sort) {
  TmpTapeGenerator generator{{}, {}, {}, {}, {}};
  std::vector<int> vec = {7, 6, 3, 4, 10, 314, 123};
  auto tape = generator.fromPath(generator.generate(vec.size() * sizeof(int)));
  std::copy(vec.begin(), vec.end(), tape.begin());
  tape.rewind();
  auto tape_out = generator.fromPath(generator.generate(tape.size()));
  TapeSorter sorter(generator, std::move(tape), std::move(tape_out), 10);
  sorter.sort();
  tape_out = sorter.getOutputTape();
  tape_out.rewind();
  std::sort(vec.begin(), vec.end());
  for (int i = 0; i < vec.size(); i++) {
    ASSERT_EQ(tape_out.read(), vec[i]);
    if (i != vec.size() - 1)
      tape_out.next();
  }
}
TEST(TapeSorter, big_size_sort) {
  TmpTapeGenerator generator{{}, {}, {}, {}, {}};
  const int val_cnt = 123456;

  std::vector<int> rand_vals(val_cnt);

  for (auto &el : rand_vals) {
    el = rand();
  }
  auto tape =
      generator.fromPath(generator.generate(rand_vals.size() * sizeof(int)));
  std::copy(rand_vals.begin(), rand_vals.end(), tape.begin());
  tape.rewind();
  auto tape_out = generator.fromPath(generator.generate(tape.size()));
  TapeSorter sorter(generator, std::move(tape), std::move(tape_out), 123);
  sorter.sort();
  tape_out = sorter.getOutputTape();
  tape_out.rewind();
  std::sort(rand_vals.begin(), rand_vals.end());
  for (int i = 0; i < rand_vals.size(); i++) {
    ASSERT_EQ(tape_out.read(), rand_vals[i]);
    if (i != rand_vals.size() - 1)
      tape_out.next();
  }
}
int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::InitGoogleMock(&argc, argv);

  return RUN_ALL_TESTS();
}