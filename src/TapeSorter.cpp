#include "TapeSorter.h"

#include <algorithm>
#include <vector>
#include <deque>

namespace
{
    std::vector<int> tapeToVector(Tape &tape)
    {
        std::vector<int> result;

        tape.rewind();

        std::copy(tape.begin(), tape.end(), std::back_inserter(result));

        return result;
    }
}

TapeSorter::TapeSorter(TmpTapeGenerator tape_generator, Tape input, Tape output, std::size_t max_memory) : tape_generator_(std::move(tape_generator)), input_(std::move(input)), output_(std::move(output)), max_int_cnt_(max_memory / sizeof(int)) {}

void TapeSorter::sort()
{
    std::deque<std::filesystem::path> tapes;

    for (auto it = input_.begin(); it != input_.end();)
    {
        std::vector<int> tmp_vec;
        const auto file_size = std::min(input_.size(), it.offset() + max_int_cnt_ * sizeof(int)) - it.offset();
        auto end_it = Tape::Iterator{&input_, it.offset() + file_size};
        std::copy(it, end_it, std::back_inserter(tmp_vec));
        it = end_it;
        std::sort(tmp_vec.begin(), tmp_vec.end());

        auto tmp_tape_path = tape_generator_.generate(file_size);
        auto tmp_tape = tape_generator_.fromPath(tmp_tape_path);
        std::copy(tmp_vec.begin(), tmp_vec.end(), tmp_tape.begin());

        tapes.push_back(std::move(tmp_tape_path));
    }

    while (tapes.size() != 1)
    {
        auto fst_tape_path = std::move(tapes.front());
        tapes.pop_front();

        auto snd_tape_path = std::move(tapes.front());
        tapes.pop_front();

        auto fst_tape = tape_generator_.fromPath(fst_tape_path);
        auto snd_tape = tape_generator_.fromPath(snd_tape_path);

        auto output_tape_path = tape_generator_.generate(fst_tape.size() + snd_tape.size());
        auto output_tape = tape_generator_.fromPath(output_tape_path);

        merge(fst_tape, snd_tape, output_tape);

        output_tape.rewind();

        tapes.push_back(std::move(output_tape_path));
    }

    auto last_tape = tape_generator_.fromPath(tapes.front());
    std::copy(last_tape.begin(), last_tape.end(), output_.begin());
}

void TapeSorter::merge(Tape &fst, Tape &snd, Tape &out)
{
    auto fst_it = fst.begin();
    auto snd_it = snd.begin();
    auto out_it = out.begin();

    while (fst_it != fst.end() && snd_it != snd.end())
    {
        const int fst_val = *fst_it;
        const int snd_val = *snd_it;
        if (fst_val < snd_val)
        {
            *out_it = fst_val;
            ++fst_it;
            ++out_it;
        }
        else
        {
            *out_it = snd_val;
            ++snd_it;
            ++out_it;
        }
    }

    while (fst_it != fst.end())
    {
        *out_it = (int)*fst_it;
        ++fst_it;
        ++out_it;
    }

    while (snd_it != snd.end())
    {
        *out_it = (int)*snd_it;
        ++snd_it;
        ++out_it;
    }
}

Tape TapeSorter::getOutputTape()
{
    return std::move(output_);
}
