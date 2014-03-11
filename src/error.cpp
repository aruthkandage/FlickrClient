#include <error.h>

using namespace std;

error_detail::error_detail(const char* msg) throw() :
    message(msg)
{
}

const char* error_detail::what() const throw() {
    return message.c_str();
}

error_detail& error_detail::set_message(const char* msg) throw() {
    message = msg;
    return *this;
}

const vector<string>& error_detail::get_args() const throw() { 
    return args; 
}

error_detail& error_detail::add_arg(const string& arg) throw() {
    args.push_back(arg);
    return *this;
}

error::error(const char* msg) throw() :
    error_detail(msg)
{
}

error::error(const char* msg, const error& nested) throw() :
    error_detail(msg),
    errors(nested.get_errors())
{
    errors.push_back(nested);
}

const std::vector<error_detail>& error::get_errors() const throw() {
    return errors;
}
