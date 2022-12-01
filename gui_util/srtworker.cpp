#include "srtworker.h"
#include "fmt/xchar.h"
#include <fstream>
#include "boost/algorithm/string.hpp"
#include <filesystem>
#include "textworker.h"
#include "globlogger.h"
#include "wlogic_error.h"

namespace fs = std::filesystem;


namespace
{
    const auto BASE_DIR = "tmp";
    const auto SUBS = "subs.txt";
}

size_t SrtWorker::Work(std::wstring path_to_file)
{
    auto list = read_all_text_(path_to_file);
    remove_tags_(list);

    fs::path pth{BASE_DIR};
    pth /= SUBS;

    std::error_code ec;
    fs::create_directories(BASE_DIR, ec);
    if (ec)
    {
        LOGE("Failed create directory {} with code {}", BASE_DIR, ec.value());
    }

    write_all_text_(pth.wstring(), list);

    return TextWorker::Work(pth.string());
}

void SrtWorker::write_all_text_(const std::wstring& filename, const std::list<std::string> &phrases)
{
    std::ofstream f(filename);
    if (!f.is_open())
    {
        throw std::logic_error("fuck");
    }
//        throw std::logic_error(
//                fmt::format(L"Failed open file {}", filename));

    for (auto& str : phrases)
    {
        f << str << "\n";
    }
}


void SrtWorker::remove_tags_(std::list<std::string>& list)
{
    for (auto& str : list)
    {
        boost::replace_all(str, "<i>", "");
        boost::replace_all(str, "</i>", "");
    }
}


std::list<std::string> SrtWorker::read_all_text_(const std::wstring& filename)
{
    std::ifstream f(filename, std::ios::in);
    if (!f.is_open())
        throw std::logic_error("fuck");
//        throw std::logic_error(
//                fmt::format("Failed open file {}", filename));

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
