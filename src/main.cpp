#include <Tape.h>
#include <TapeSorter.h>
#include <TmpTapeGenerator.h>
#include <algorithm>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cout << "Usage: " << argv[0]
              << " (config_file_path) (input_file_path) (output_file_path)"
              << std::endl;
    return 1;
  }
  std::string name_file = argv[1];
  std::ifstream f(name_file);
  json data = json::parse(f);
  size_t max_memory = data["max_memory"];
  std::chrono::milliseconds delay_read =
      std::chrono::milliseconds(data["delay_read"]);
  std::chrono::milliseconds delay_write =
      std::chrono::milliseconds(data["delay_write"]);
  std::chrono::milliseconds delay_rewind =
      std::chrono::milliseconds(data["delay_rewind"]);
  std::chrono::milliseconds delay_next =
      std::chrono::milliseconds(data["delay_next"]);
  std::chrono::milliseconds delay_prev =
      std::chrono::milliseconds(data["delay_prev"]);

  Tape input_tape(argv[2], std::filesystem::file_size(argv[2]), delay_write,
                  delay_read, delay_rewind, delay_prev, delay_next);

  Tape output_tape(argv[3], std::filesystem::file_size(argv[2]), delay_write,
                   delay_read, delay_rewind, delay_prev, delay_next);

  TmpTapeGenerator tape_generator(delay_write, delay_read, delay_rewind,
                                  delay_prev, delay_next);

  TapeSorter sorter{tape_generator, std::move(input_tape),
                    std::move(output_tape), max_memory};

  sorter.sort();

  return 0;
}
