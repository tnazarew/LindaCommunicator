#ifndef LINDA_EXCEPTION_H
#define LINDA_EXCEPTION_H

#include <exception>
#include <string>

namespace linda
{
    class LindaException : public std::exception
    {
    public:
        LindaException(std::string error) : error(error) { }
        ~LindaException() noexcept { }
        const char* what() const noexcept { return error.c_str(); }
    private:
        std::string error;
    };
}

#endif // LINDA_EXCEPTION_H
