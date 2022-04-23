#include <iostream>
#include <set>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include "DictionaryList.h"


namespace fs = boost::filesystem;
std::set<std::string> read_words(std::string filename, std::vector<std::string> &not_sorted_words);
DictionaryList get_dictionary(std::string filename);
void write_not_known(const std::vector<std::string> &not_known, std::string filename);
void add_not_known_to_dict(const std::vector<std::string> &not_known, DictionaryList &dictionary);
void save_dictionary(DictionaryList &dictionary, std::string filename);
void sort_not_known(std::vector<std::string>& not_known_words, const std::vector<std::string>& not_sorted_words);
void remove_from_start_end(std::string &word, const std::string& what);

int main()
{
    const std::string DICTIONARY = "dictionary.json";
    std::vector<std::string> not_sorted_words;
    auto set = read_words("subs.txt", not_sorted_words);
    auto dictionary = get_dictionary(DICTIONARY);

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

    return 0;
}

void sort_not_known(std::vector<std::string>& not_known_words, const std::vector<std::string>& not_sorted_words )
{
    std::vector<std::pair<std::string, int>> index_array(not_known_words.size());
    int i = 0;
    for (auto &word : not_known_words)
    {
        auto it = std::find_if(not_sorted_words.begin(), not_sorted_words.end(), [&word](const std::string &w) -> bool
        {
            return word == w;
        });
        if (it == not_known_words.end()) throw std::logic_error("Can't find word while sorting");
        auto pos = std::distance(not_sorted_words.begin(), it);
        index_array[i++] = std::pair<std::string, int>(word, pos);
    }

    std::sort(index_array.begin(), index_array.end(), [](const std::pair<std::string, int>& first,
              const std::pair<std::string, int>& second) -> bool
    {
        return first.second < second.second;
    });

    not_known_words.clear();

    for (auto &pr : index_array)
    {
        not_known_words.push_back(pr.first);
    }
}

void save_dictionary(DictionaryList &dictionary, std::string filename)
{
    std::ofstream f(filename);
    try
    {

        cereal::JSONOutputArchive ar(f);
        ar(dictionary);
    }  catch (const std::exception &)
    {

    }
}

void add_not_known_to_dict(const std::vector<std::string> &not_known, DictionaryList &dictionary)
{
    for (auto& word : not_known)
    {
        dictionary.words.emplace(word);
    }
}

void write_not_known(const std::vector<std::string> &not_known, std::string filename)
{
    std::ofstream f(filename);
    if (!f.is_open()) throw std::logic_error("Can't open file");

    for (auto & word : not_known)
    {
        f << word << "\n";
    }
}

DictionaryList get_dictionary(std::string filename)
{
    std::ifstream f(filename);
    fs::path src(filename);
    std::string tmp = filename + "_bak";
    fs::path dst(tmp);




    DictionaryList dict;
    {
        try
        {
            fs::copy_file(src, dst, fs::copy_option::overwrite_if_exists);
            cereal::JSONInputArchive ar(f);
            ar(dict);
        }  catch (const std::exception &)
        {

        }

    }

    return dict;

}


void check_words(std::vector<std::string> &words);
void add_words_to_set(const std::vector<std::string>& words,
                      std::set<std::string> &set,
                      std::vector<std::string> &not_sorted_words);

std::set<std::string> read_words(std::string filename, std::vector<std::string> &not_sorted_words)
{
    std::ifstream f(filename, std::ios::in);
    if (!f.is_open()) throw std::logic_error("Can't open file");

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

void add_words_to_set(const std::vector<std::string>& words,
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

void check_symbol(std::string &word, char symbol);
void remove_symbol(std::string &word, char symbol);

bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
};

bool is_kyrilic(const std::string& s)
{
    if (s.empty()) return false;
    return ((unsigned char)s[0]) >= 192; //'А'-'я'
}

void check_words(std::vector<std::string> &words)
{
    words.erase(std::remove_if(words.begin(), words.end(), [](const std::string &word)
    {
        if (word == "-") return true;
        return false;
    }), words.end());

    for (auto &word : words)
    {
        check_symbol(word, '\'');
        check_symbol(word, ',');
        check_symbol(word, '.');
        check_symbol(word, ':');
        check_symbol(word, '!');
        check_symbol(word, '?');
        remove_symbol(word, '\"');
        remove_from_start_end(word, "-");
        remove_from_start_end(word, "<i>");
        remove_from_start_end(word, "</i>");
        remove_from_start_end(word, "-");
        remove_symbol(word, ')');
        remove_symbol(word, '(');
        remove_symbol(word, '[');
        remove_symbol(word, ']');
        boost::trim(word);
        boost::to_lower(word);
    }

    words.erase(std::remove_if(words.begin(), words.end(), [](const std::string &word)
    {
        return word.empty();
    }), words.end());

    words.erase(std::remove_if(words.begin(), words.end(), [](const std::string &word)
    {
        return is_number(word);
    }), words.end());

    words.erase(std::remove_if(words.begin(), words.end(), [](const std::string& word)
    {
        return is_kyrilic(word);
    }), words.end());

    words.erase(std::remove_if(words.begin(), words.end(), [](const std::string& word)
    {
        return word == "<i>("
                || word == ")</i>"
                || word == ")"
                || word == "("
                || word == "<i>"
                || word == "</i>";
    }), words.end());
}

void remove_symbol(std::string &word, char symbol)
{
    auto pos = word.find(symbol);
    while (pos != std::string::npos)
    {
        word.erase(pos, 1);
        pos = word.find(symbol);
    }

}

void remove_from_start_end(std::string &word, const std::string& what)
{
    auto pos = word.find(what);
    while (pos == 0)
    {
        word.erase(pos, what.length());
        pos = word.find(what);
    }
    pos = word.rfind(what);
    while (pos != std::string::npos
           && pos + what.length() == word.length())
    {
        word.erase(pos, what.length());
        pos = word.rfind(what);
    }

}

void check_symbol(std::string &word, char symbol)
{
    auto pos = word.find(symbol);
    if (pos == std::string::npos) return;
    word.erase(pos);
}


