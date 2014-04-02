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

#ifndef __BASE64_H__
#define __BASE64_H__

#include <iostream>
#include <error.h>
#include <string>

class base64 {
    public:
    base64();
    base64(std::istream&, std::ostream&);

    std::string encode(const std::string&) throw(app::error);

    private: // methods
    void encode_block(std::ostream&, const unsigned char*);
    void encode_block(std::ostream&, const std::string&, unsigned int);
    void encode_remaining(std::ostream&, const unsigned char*, unsigned int);
    void encode_remaining(std::ostream&, const std::string&, unsigned int);
    void encode_all();
    std::string encode_all(const std::string&);

    private: // data members
    std::istream* in;
    std::ostream* out;

    static const unsigned char encoding_table[];
    static const unsigned int bytes_per_block;
};

#endif
