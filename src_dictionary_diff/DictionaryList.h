#ifndef DICTIONARYLIST_H
#define DICTIONARYLIST_H

#include "cereal/archives/json.hpp"
#include "cereal/cereal.hpp"
#include "cereal/types/set.hpp"


struct DictionaryList
{
    std::set<std::string> words;

    template<typename ARCH>
    void serialize( ARCH & ar )
    {
        ar(CEREAL_NVP(words));
    }
};

#endif // DICTIONARYLIST_H
