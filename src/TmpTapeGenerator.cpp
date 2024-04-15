#include <TmpTapeGenerator.h>

#include <filesystem>
#include <fstream>
#include <string>

std::filesystem::path TmpTapeGenerator::generate(std::size_t file_size)
{
    std::filesystem::path file_path = std::filesystem::temp_directory_path() / ("tempTape" + std::to_string(counter_++) + ".tmp");

    std::ofstream fstream{file_path};
    fstream.close();

    try
    {
        std::filesystem::resize_file(file_path, file_size);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return file_path;
}

Tape TmpTapeGenerator::fromPath(std::filesystem::path path)
{
    return Tape{path, std::filesystem::file_size(path), delay_};
}

TmpTapeGenerator::~TmpTapeGenerator()
{
    for (int i = 0; i < counter_; ++i)
    {
        std::error_code ec;
        std::filesystem::remove(std::filesystem::temp_directory_path() / ("tempTape" + std::to_string(counter_)), ec);
    }
}
