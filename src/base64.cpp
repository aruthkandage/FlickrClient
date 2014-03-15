#include <base64.h>
#include <sstream>

using namespace std;

/* Encoding table */
const unsigned char base64::encoding_table[] = {
'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T',
'U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n',
'o','p','q','r','s','t','u','v','w','x','y','z','0','1','2','3','4','5','6','7',
'8','9','+','/'
};

const unsigned int base64::bytes_per_block = 3;

base64::base64() :
in(0),
out(0)
{
}

base64::base64(istream& _in, ostream& _out) :
in(&_in),
out(&_out)
{
}

std::string base64::encode(const string& input_string) throw(app::error) {
    return encode_all(input_string);
}

void base64::encode_block(ostream& encoded_stream, const unsigned char* block) {
    // Take a group of 24 bits, split into 4 6-bit numbers each mapped to a character
    encoded_stream << encoding_table[(block[0] & 0xfc) >> 2];
    encoded_stream << encoding_table[((block[0] & 0x3) << 4) | (block[1] >> 4)];
    encoded_stream << encoding_table[((block[1] & 0xf) << 2) | (block[2] >> 6)];
    encoded_stream << encoding_table[block[2] & 0x3f];

    // All output operations should have succeeded
    if(encoded_stream.fail()) {
        throw app::error("base64: ostream failure");
    }
}

void base64::encode_block(ostream& encoded_stream, const string& input_string, unsigned int index) {
    unsigned char block[] = { (unsigned char) input_string[index], 
                              (unsigned char) input_string[index+1],
                              (unsigned char) input_string[index+2] };

    encode_block(encoded_stream, block);
}

void base64::encode_remaining(ostream& encoded_stream, const unsigned char* remaining_bytes, unsigned int num_remaining_bytes) {
    switch(num_remaining_bytes) {
        case 1:
        // Take 12 bits
        encoded_stream << encoding_table[(remaining_bytes[0] & 0xfc) >> 2];
        encoded_stream << encoding_table[(remaining_bytes[0] & 0x3) << 4];
        // Pad with ==
        encoded_stream << "==";
        break;

        case 2:
        // Take 18 bits
        encoded_stream << encoding_table[(remaining_bytes[0] & 0xfc) >> 2];
        encoded_stream << encoding_table[((remaining_bytes[0] & 0x3) << 4) | (remaining_bytes[1] >> 4)];
        encoded_stream << encoding_table[(remaining_bytes[1] & 0xf) << 2];
        encoded_stream << "=";
        break;

        default:
        break;
    }

    // All output operations should have succeeded
    if(encoded_stream.fail()) {
        throw app::error("base64: ostream failure");
    }
}

void base64::encode_remaining(ostream& encoded_stream, const string& input_string, unsigned int index) {
    unsigned int num_remaining_bytes = input_string.length() - index;
    if(num_remaining_bytes > 0) {
        if(num_remaining_bytes == 1) {
            unsigned char remaining_bytes[] = { (unsigned char) input_string[index] };
            encode_remaining(encoded_stream, remaining_bytes, 1);
        } else if(num_remaining_bytes == 2) {
            unsigned char remaining_bytes[] = { (unsigned char) input_string[index] , (unsigned char) input_string[index+1] };
            encode_remaining(encoded_stream, remaining_bytes, 2);
        }
    }
}

/*
 * Encode chars from given istream to given ostream
 */
void base64::encode_all() {
    if(in == 0 || out == 0 || !in->good() || !out->good()) return;

    // Need enough space for null terminator
    char block[bytes_per_block+1];
    unsigned int bytes_to_encode;

    for(in->get(block, bytes_per_block+1) ;
        (bytes_to_encode = (unsigned int) in->gcount()) == bytes_per_block && in->good() ;
        in->get(block, bytes_per_block+1)) 
    {
        encode_block(*out, (const unsigned char*) block);
    }

    if(in->fail()) {
        // something went wrong in underlying stream
        throw app::error("base64: istream failure");
    }

    if(bytes_to_encode > 0) {
        encode_remaining(*out, block, bytes_to_encode);
    } 
}

std::string base64::encode_all(const string& input_string) {
    if(input_string.length() == 0) return string();

    stringstream encoded_stream;

    // Encode blocks of 24-bits
    unsigned int index;
    const unsigned int num_complete_blocks = input_string.length() / bytes_per_block;
    for(index=0; index < num_complete_blocks * bytes_per_block; index += bytes_per_block) {
        encode_block(encoded_stream, input_string, index);
    } 

    encode_remaining(encoded_stream, input_string, index);

    return encoded_stream.str();
}
