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
