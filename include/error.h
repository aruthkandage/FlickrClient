#ifndef __ERROR_H__
#define __ERROR_H__

#include <exception>
#include <string>
#include <vector>

class error_detail : public std::exception {
    public:
    error_detail(const char* msg = "") throw();
    virtual ~error_detail() throw() {}

    virtual const char* what() const throw();
    error_detail& set_message(const char*) throw();
    const std::vector<std::string>& get_args() const throw();
    error_detail& add_arg(const std::string& arg) throw();

    private:
    std::string message;
    std::vector<std::string> args; 
};

class error : public error_detail {
    error(const char* msg = "") throw();
    error(const char*, const error&) throw();
    virtual ~error() throw() {}

    virtual const char* what() const throw();

    protected:
    const std::vector<error_detail>& get_errors() const throw();

    private:
    std::vector<error_detail> errors;
};

#endif
