#include "DictionaryHolder.h"
#include <fstream>
#include <filesystem>
#include "utils_file.h"

namespace fs = std::filesystem;

namespace
{
    const auto BASE_DIR = "data";
    const auto DICTIONARY = "dictionary.json";
    const auto DICTIONARY_BAK = "dictionary.json_bak";
}

const char* DictionaryHolder::GetDirPath()
{
    return BASE_DIR;
}


size_t DictionaryHolder::Load()
{
    fs::path pth{BASE_DIR};
    fs::path src = pth / DICTIONARY;
    fs::path src_bak = pth / DICTIONARY_BAK;

    restore_data(src.string(), src_bak.string(), current_data_);

    return current_data_.words.size();
}

void DictionaryHolder::Save()
{
    fs::path pth{BASE_DIR};

    std::error_code ed;
    fs::create_directories(pth, ed);

    fs::path src = pth / DICTIONARY;
    fs::path src_bak = pth / DICTIONARY_BAK;

    save_data(src.string(), src_bak.string(), current_data_);
}
