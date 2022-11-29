#include "text_utils.h"
#include <algorithm>
#include "boost/algorithm/string.hpp"

void check_symbol(std::string &word, char symbol)
{
    auto pos = word.find(symbol);
    if (pos == std::string::npos) return;
    word.erase(pos);
}

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
        check_symbol(word, ';');
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
