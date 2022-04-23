#include <iostream>
#include <list>
#include <fstream>
#include <boost/algorithm/string.hpp>

using namespace std;

std::list<std::string> read_all_text(std::string filename);
void write_all_text(std::string filename, const std::list<std::string> &phrases);
void remove_tags(std::list<std::string>& list);

int main()
{
    auto list = read_all_text("subs.srt");
    remove_tags(list);
    write_all_text("subs.txt", list);
    return 0;
}

void remove_tags(std::list<std::string>& list)
{
    for (auto& str : list)
    {
        boost::replace_all(str, "<i>", "");
        boost::replace_all(str, "</i>", "");
    }
}

std::list<std::string> read_all_text(std::string filename)
{
    std::ifstream f(filename, std::ios::in);
    if (!f.is_open()) throw std::logic_error("Can't open file");

    std::list<std::string> result;
    std::string line;

    int count_of_headers = 0;

    std::string phrase;

    while (std::getline(f, line))
    {
        if (line.empty() || line == "\r")
        {
            count_of_headers = 0;
            if (!phrase.empty())
            {
                result.push_back(std::move(phrase));
            }
            phrase = "";
            continue;
        }
        ++count_of_headers;
        if (count_of_headers < 3) continue;

        boost::trim(line);

        phrase += line;
        phrase += " ";
    }

    return result;
}

void write_all_text(std::string filename, const std::list<std::string> &phrases)
{
    std::ofstream f(filename);
    if (!f.is_open()) throw std::logic_error("Can't open file");

    for (auto& str : phrases)
    {
        f << str << "\n";
    }
}


