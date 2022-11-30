#ifndef TEXTWORKER_H
#define TEXTWORKER_H

#include <string>
#include <set>
#include <vector>
#include "DictionaryList.h"

class TextWorker
{
public:

    static size_t Work(std::string path_to_file);

private:

    static std::set<std::string> read_words_(const std::string& filename,
                                      std::vector<std::string> &not_sorted_words);

    static void add_words_to_set(const std::vector<std::string>& words,
                          std::set<std::string> &set,
                          std::vector<std::string> &not_sorted_words);

    static void sort_not_known(std::vector<std::string>& not_known_words,
                               const std::vector<std::string>& not_sorted_words );

    static void write_not_known(const std::vector<std::string> &not_known,
                                const std::string& filename);

    static void add_not_known_to_dict(const std::vector<std::string> &not_known,
                                           DictionaryList &dictionary);


};

#endif // TEXTWORKER_H
