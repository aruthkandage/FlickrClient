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

#include <netutils.h>
#include <cassert>
#include <openssl/hmac.h>
#include <sstream>

using namespace std;

namespace netutils {

string percent_encode(const string& str) {
    const char ascii_first_digit = 0x30;
    const char ascii_last_digit = 0x39;

    const char ascii_first_upper = 0x41;
    const char ascii_last_upper = 0x5A;

    const char ascii_first_lower = 0x61;
    const char ascii_last_lower = 0x7A;

    const char ascii_reserved_1 = 0x2D;
    const char ascii_reserved_2 = 0x2E;
    const char ascii_reserved_3 = 0x5F;
    const char ascii_reserved_4 = 0x7E;

    if(str.length() == 0) return string();

    stringstream percent_encoded;
    
    for(unsigned int i=0; i < str.length(); i++) {
        char current = str[i];

        // check if the current byte is one of the allowed characters
        if((current >= ascii_first_digit && current <= ascii_last_digit) ||
            (current >= ascii_first_upper && current <= ascii_last_upper) ||
            (current >= ascii_first_lower && current <= ascii_last_lower) ||
            current == ascii_reserved_1 || current == ascii_reserved_2 ||
            current == ascii_reserved_3 || current == ascii_reserved_4) {
            // then simply add it on
            percent_encoded << current;
        } else {
            // replace with %<hex value>
            percent_encoded << '%' << std::uppercase << std::hex << (((unsigned int) current) & 0xFF); 
        }
    }

    return percent_encoded.str();
}

std::string hmac_sha1(const std::string& key, const std::string& str) {
    // SHA-1 produces 20-byte long output
    const unsigned int sha1_output_len = 20;
    char sha1_output_buffer[sha1_output_len+1];
    unsigned int output_len = sha1_output_len;

    // OpenSSL HMAC-SHA1
    HMAC(EVP_sha1(), key.c_str(), (int) key.length(), (const unsigned char*) str.c_str(), str.length(), (unsigned char*) sha1_output_buffer, &output_len);  
    assert(output_len == sha1_output_len);
    sha1_output_buffer[output_len] = '\0';

    return string(sha1_output_buffer);
}

} // end namespace netutils
