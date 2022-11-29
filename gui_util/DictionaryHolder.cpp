#include "DictionaryHolder.h"
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

namespace
{
    const std::string DICTIONARY = "dictionary.json";
    const std::string DICTIONARY_BAK = "dictionary.json_bak";
}


size_t DictionaryHolder::Load()
{

    std::ifstream f{DICTIONARY};

    {
        try
        {
           fs::copy_file(DICTIONARY, DICTIONARY_BAK, fs::copy_options::overwrite_existing);
           cereal::JSONInputArchive ar(f);
           ar(current_data_);
        }  catch (const std::exception &)
        {

        }

    }

    return current_data_.words.size();
}

void DictionaryHolder::Save()
{
    std::ofstream f{DICTIONARY};
    try
    {

        cereal::JSONOutputArchive ar(f);
        ar(current_data_);
    }  catch (const std::exception &)
    {

    }
}
