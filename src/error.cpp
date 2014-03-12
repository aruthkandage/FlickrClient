#include <error.h>
#include <sstream>

namespace app {

error::error() throw() :
std::exception(),
boost::exception()
{
}

error::error(const std::string& msg) throw() :
std::exception(),
boost::exception()
{
    (*this) << app::error::message(msg);
}

const char* error::what() const throw() {
    const std::string* error_message = boost::get_error_info<app::error::message>(*this);

    if(error_message != 0) {
        return error_message->c_str();
    } else {
        return "Error!";
    }
}

error_chain::error_chain() throw() :
error()
{
}

error_chain::error_chain(const error& err) throw() :
error()
{
chain.push_back(err);
}

error_chain::error_chain(const error_chain& other) throw() :
error(other),
chain(other.chain)
{
}

error_chain& error_chain::operator=(const error_chain& other) throw() {
    if(this == &other) return *this;

    error::operator=(other);
    chain = other.chain;

    return *this;
}

const char* error_chain::what() const throw() {
    std::stringstream what_stream;

    what_stream << error::what() << std::endl;
    for(unsigned int i=0; i < chain.size(); i++) {
        what_stream << "=> " << chain[i].what() << std::endl;
    }

    return what_stream.str().c_str();
}

}
