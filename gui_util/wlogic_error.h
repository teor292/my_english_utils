#ifndef WLOGIC_ERROR_H
#define WLOGIC_ERROR_H


#include <stdexcept>


class wlogic_error : public std::exception
{
public:

    explicit wlogic_error(std::wstring str)
        : message_{std::move(str)}
    {}



    const std::wstring& GetMessage() const
    {
        return message_;
    }

private:

    std::wstring message_;

};

#endif // WLOGIC_ERROR_H
