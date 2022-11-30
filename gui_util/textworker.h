#ifndef TEXTWORKER_H
#define TEXTWORKER_H

#include <string>
#include <set>
#include <vector>

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


};

#endif // TEXTWORKER_H
