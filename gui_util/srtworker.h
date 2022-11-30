#ifndef SRTWORKER_H
#define SRTWORKER_H

#include <string>
#include <list>

class SrtWorker
{
public:

    static size_t Work(std::string path_to_file);

private:

    static std::list<std::string> read_all_text_(const std::string& filename);

    static void remove_tags_(std::list<std::string>& list);

    static void write_all_text_(const std::string& filename,
                               const std::list<std::string> &phrases);
};

#endif // SRTWORKER_H
