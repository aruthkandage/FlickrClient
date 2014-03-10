#ifndef __ERROR_H__
#define __ERROR_H__

#include <exception>

class error : public std::exception {
    public:
    error(const char* _message = "", const error* _nested_error = 0) throw() :
    message(_message),
    nested_error(_nested_error)
    {
    }

    virtual ~error() throw() {}

    virtual const char* what() throw() { return message; }

    bool has_nested() throw() { return (nested_error != 0); }
    const error* get_nested_error() throw() { return nested_error; }

    private:
    const char* message;
    const error* nested_error;
};

#endif
