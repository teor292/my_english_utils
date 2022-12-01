#include "textworker.h"
#include <fstream>
#include "boost/algorithm/string.hpp"
#include "text_utils.h"
#include "DictionaryHolder.h"
#include <filesystem>
#include "fmt/format.h"
#include "globlogger.h"
#include "wlogic_error.h"
#include "fmt/xchar.h"

namespace fs = std::filesystem;

size_t TextWorker::Work(std::wstring path_to_file)
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

    fs::path pth{DictionaryHolder::GetDirPath()};
    pth /= "not_known.txt";

    std::error_code ec;
    fs::create_directories(DictionaryHolder::GetDirPath(), ec);
    if (ec)
    {
        LOGE("Failed create directory {} with code {}",
             DictionaryHolder::GetDirPath(),
             ec.value());
    }

    write_not_known(not_known_words, pth.wstring());

    add_not_known_to_dict(not_known_words, dictionary);

    DictionaryHolder::Save();

    return not_known_words.size();
}

void TextWorker::add_not_known_to_dict(const std::vector<std::string> &not_known,
                                       DictionaryList &dictionary)
{
    for (auto& word : not_known)
    {
        dictionary.words.emplace(word);
    }
}

void TextWorker::write_not_known(const std::vector<std::string> &not_known,
                                 const std::wstring& filename)
{
    std::ofstream f(filename);
    if (!f.is_open())
        throw wlogic_error(
                fmt::format(L"Failed open file {}", filename));

    for (auto & word : not_known)
    {
        f << word << "\n";
    }
}

void TextWorker::sort_not_known(std::vector<std::string>& not_known_words,
                                const std::vector<std::string>& not_sorted_words )
{
    std::vector<std::pair<std::string, size_t>> index_array;
    index_array.reserve(not_known_words.size());


    for (auto &word : not_known_words)
    {
        auto it = std::find_if(not_sorted_words.begin(), not_sorted_words.end(), [&word](const std::string &w) -> bool
        {
            return word == w;
        });
        if (it == not_sorted_words.end())
            throw std::logic_error(
                    fmt::format("Failed find word while sorting: {}", word));
        auto pos = std::distance(not_sorted_words.begin(), it);
        index_array.push_back(std::pair<std::string, size_t>(word, pos));
    }

    std::sort(index_array.begin(), index_array.end(), [](const std::pair<std::string, size_t>& first,
              const std::pair<std::string, size_t>& second) -> bool
    {
        return first.second < second.second;
    });

    not_known_words.clear();

    for (auto &pr : index_array)
    {
        not_known_words.push_back(pr.first);
    }
}


std::set<std::string> TextWorker::read_words_(const std::wstring& filename,
                                  std::vector<std::string> &not_sorted_words)
{
    std::ifstream f(filename, std::ios::in);
    if (!f.is_open())
        throw wlogic_error(
                fmt::format(L"Failed open file {}", filename));

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
