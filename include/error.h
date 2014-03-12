#ifndef __ERROR_H__
#define __ERROR_H__

#include <exception>
#include <string>
#include <vector>
#include <boost/exception/exception.hpp>
#include <boost/exception/info.hpp>
#include <boost/exception/get_error_info.hpp>

#define error_param(Type, Param) \
typedef boost::error_info<struct error_tag_ ## Param, Type> Param 

namespace app {

    class error : public std::exception, public boost::exception {
        public:
        error() throw();
        error(const std::string&) throw();
        virtual ~error() throw() {}

        virtual const char* what() const throw();

        error_param(std::string, message);
    };

    class error_chain : public error {
        public:
        error_chain() throw();
        error_chain(const error&) throw();
        error_chain(const error_chain&) throw();
        error_chain& operator=(const error_chain&) throw(); 
        virtual ~error_chain() throw() {}

        virtual const char* what() const throw();

        private:
        std::vector<error> chain;
    };
}

#endif
