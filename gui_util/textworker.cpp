#include "textworker.h"
#include <fstream>
#include "boost/algorithm/string.hpp"
#include "text_utils.h"
#include "DictionaryHolder.h"

size_t TextWorker::Work(std::string path_to_file)
{
    std::vector<std::string> not_sorted_words;
    auto set = read_words_(path_to_file, not_sorted_words);
    auto& dictionary = DictionaryHolder::GetData();

    std::vector<std::string> not_known_words;

    std::set_difference(set.begin(),
                       set.end(),
                       dictionary.words.begin(),
                       dictionary.words.end(),
                       std::inserter(not_known_words, not_known_words.begin()));

    sort_not_known(not_known_words, not_sorted_words);

    write_not_known(not_known_words, "not_known.txt");

    add_not_known_to_dict(not_known_words, dictionary);

    save_dictionary(dictionary, DICTIONARY);
}


std::set<std::string> TextWorker::read_words_(const std::string& filename,
                                  std::vector<std::string> &not_sorted_words)
{
    std::ifstream f(filename, std::ios::in);
    if (!f.is_open())
        throw std::logic_error("Can't open file");

    std::string line;

    std::set<std::string> result;

    while (std::getline(f, line))
    {
        if (line.empty()) continue;
        std::vector<std::string> words;
        boost::split(words, line, boost::is_any_of(" "));

        check_words(words);
        add_words_to_set(words, result, not_sorted_words);

    }
    return result;
}

void TextWorker::add_words_to_set(const std::vector<std::string>& words,
                      std::set<std::string> &set,
                      std::vector<std::string> &not_sorted_words)
{
    for (auto &word : words)
    {
        auto result = set.emplace(word);
        if (result.second)
        {
            not_sorted_words.push_back(word);
        }
    }
}
