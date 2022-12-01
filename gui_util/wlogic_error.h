#ifndef WLOGIC_ERROR_H
#define WLOGIC_ERROR_H


#include <stdexcept>
#include <codecvt>

inline std::string convert(const std::wstring& str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(str);
}


class wlogic_error : public std::logic_error
{
public:

    explicit wlogic_error(std::wstring str)
        : std::logic_error{convert(str)}
    {
    }



private:

    std::string message_;

};

#endif // WLOGIC_ERROR_H
