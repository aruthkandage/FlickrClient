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
    
    assert(b64.encode("This is just\r\na test\r\n!@#$%^&*()_+-=;':\",.<>/?") == "VGhpcyBpcyBqdXN0DQphIHRlc3QNCiFAIyQlXiYqKClfKy09Oyc6IiwuPD4vPw==");

    return 0;
}
