#ifndef UTILS_FILE_H
#define UTILS_FILE_H


#include <filesystem>
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>

namespace fs = std::filesystem;

inline bool copy_file(const std::string& src, const std::string& dst)
{
    try
    {
        fs::copy_file(src, dst, fs::copy_options::overwrite_existing);
        return true;
    }
    catch (const std::exception& e)
    {}
    return false;
}

inline void delete_file(const std::string& src)
{
    try
    {
        fs::remove(src);
    }
    catch (const std::exception& e)
    {

    }
}


template<typename T>
bool restore_data(const std::string& src, const std::string& src_bak, T& result)
{


    try
    {
       std::ifstream f{src};
       cereal::JSONInputArchive ar(f);
       ar(result);

       return true;
    }  catch (const std::exception &)
    {
    }

    if (!fs::exists(src_bak)) return false;



    try
    {
        std::ifstream f_bak{src_bak};
        cereal::JSONInputArchive ar(f_bak);
        ar(result);

    }
    catch (const std::exception& e)
    {
        return false;
    }

    if (copy_file(src_bak, src))
    {
        delete_file(src_bak);
    }

    return true;
}

template<typename T>
void save_data(const std::string& src, const std::string& src_bak, T& data)
{
    copy_file(src, src_bak);

    try
    {
        std::ofstream f{src};
        cereal::JSONOutputArchive ar{f};
        ar(data);

        delete_file(src_bak);
    }
    catch (const std::exception& e)
    {
    }


}


#endif // UTILS_FILE_H
