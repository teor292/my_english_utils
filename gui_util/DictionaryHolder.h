#ifndef DICTIONARYHOLDER_H
#define DICTIONARYHOLDER_H

#include "DictionaryList.h"

class DictionaryHolder
{
public:

    static const char* GetDirPath();
    static size_t Load();
    static void Save();

    static DictionaryList& GetData()
    {
        return current_data_;
    }

private:

    inline static DictionaryList current_data_;

};

#endif // DICTIONARYHOLDER_H
