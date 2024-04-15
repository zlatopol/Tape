#include <TapeSorter.h>
#include <TmpTapeGenerator.h>
#include <Tape.h>

#include <vector>
#include <algorithm>

Tape vectorToTape(const std::vector<int> &data, TmpTapeGenerator &tape_generator)
{
    auto tape = tape_generator.fromPath(tape_generator.generate(data.size() * sizeof(int)));

    std::copy(data.begin(), data.end(), tape.begin());

    tape.rewind();

    return tape;
}

std::vector<int> tapeToVector(Tape &tape)
{
    std::vector<int> result;

    tape.rewind();

    std::copy(tape.begin(), tape.end(), std::back_inserter(result));

    return result;
}

std::vector<int> generateVector() {
    const int val_cnt = 123456;

    std::vector<int> rand_vals(val_cnt);
    
    for (auto& el: rand_vals) {
        el = rand();
    }

    return rand_vals;
}

int main()
{
    TmpTapeGenerator tape_generator{{}};

    const auto input_vec = generateVector();

    auto input_tape = vectorToTape(input_vec, tape_generator);
    auto output_tape = tape_generator.fromPath(tape_generator.generate(input_tape.size()));

    TapeSorter sorter{tape_generator, std::move(input_tape), std::move(output_tape), 23 * sizeof(int)};

    sorter.sort();

    output_tape = sorter.getOutputTape();

    auto res_vec = tapeToVector(output_tape);
    auto check_vec = input_vec;
    std::sort(check_vec.begin(), check_vec.end());

    std::cout << "is equal: " << std::boolalpha << std::equal(res_vec.begin(), res_vec.end(), check_vec.begin(), check_vec.end()) << std::endl;
}
