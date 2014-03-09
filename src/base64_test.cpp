#include <iostream>
#include <base64.h>
#include <cassert>

using namespace std;

int main(int argc, char** argv) {
    base64 b64;

    assert(b64.encode("") == "");
    assert(b64.encode("f") == "Zg==");
    assert(b64.encode("fo") == "Zm8=");
    assert(b64.encode("foo") == "Zm9v");
    assert(b64.encode("foob") == "Zm9vYg==");
    assert(b64.encode("fooba") == "Zm9vYmE=");
    assert(b64.encode("foobar") == "Zm9vYmFy");
    
    assert(b64.encode("The qui") == "VGhlIHF1aQ==");
    assert(b64.encode("The quick brow") == "VGhlIHF1aWNrIGJyb3c=");
    assert(b64.encode("The quick brown fox jumps o") == "VGhlIHF1aWNrIGJyb3duIGZveCBqdW1wcyBv");
    assert(b64.encode("The quick brown fox jumps over the lazy dog's tail!") == "VGhlIHF1aWNrIGJyb3duIGZveCBqdW1wcyBvdmVyIHRoZSBsYXp5IGRvZydzIHRhaWwh");

    return 0;
}
