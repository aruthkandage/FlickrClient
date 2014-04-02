/**
 *  Qt Flickr Client
 *  Copyright (C) 2014 Aruth Kandage
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.*/ 

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
