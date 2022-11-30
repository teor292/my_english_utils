#ifndef UTILS_FILE_H
#define UTILS_FILE_H


#include <filesystem>
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>
#include "globlogger.h"

namespace fs = std::filesystem;

inline bool copy_file(const std::string& src, const std::string& dst)
{
    try
    {
        fs::copy_file(src, dst, fs::copy_options::overwrite_existing);
        return true;
    }
    catch (const std::exception& e)
    {
        LOGE("Failed copy {} to {} with message {}", src, dst, e.what());
    }
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
        LOGE("Failed remove {} with message {}", src, e.what());
    }
}


template<typename T>
bool restore_data(const std::string& src, const std::string& src_bak, T& result)
{


    try
    {
       if (fs::exists(src))
       {
           std::ifstream f{src};
           cereal::JSONInputArchive ar(f);
           ar(result);

           LOGD("Read {} success", src);
           return true;
       }
       LOGD("File {} doesn't exist", src);

    }  catch (const std::exception &e)
    {
        LOGE("Failed restore from {} with message {}", src, e.what());
    }

    if (!fs::exists(src_bak))
    {
        LOGD("File {} doesn't exist", src_bak);
        return false;
    }



    try
    {
        std::ifstream f_bak{src_bak};
        cereal::JSONInputArchive ar(f_bak);
        ar(result);
        LOGD("Read {} success", src_bak);
    }
    catch (const std::exception& e)
    {
        LOGE("Failed restore from {} with message {}", src, e.what());
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

        LOGD("Save data to {} successfully", src);
    }
    catch (const std::exception& e)
    {
        LOGE("Failed save(alg) with files {} {}, message: {}", src, src_bak, e.what());
    }


}


#endif // UTILS_FILE_H
