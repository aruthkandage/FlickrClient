#ifndef __BASE64_H__
#define __BASE64_H__

#include <iostream>
#include <string>

class base64 {
    public:
    base64();
    base64(std::istream&, std::ostream&);

    std::string encode(const std::string&);

    private: // methods
    void encode_block(std::ostream&, const unsigned char*);
    void encode_block(std::ostream&, const std::string&, unsigned int);
    void encode_remaining(std::ostream&, const unsigned char*, unsigned int);
    void encode_remaining(std::ostream&, const std::string&, unsigned int);
    std::string encode_all(const std::string&);

    private: // data members
    std::istream* in;
    std::ostream* out;
};

#endif
