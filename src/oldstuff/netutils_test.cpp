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
#include <iostream>
#include <cassert>

using namespace std;

int main(int argc, char** argv) {
    string str_1 = "Ladies + Gentlemen";
    string str_2 = "An encoded string!";
    string str_3 = "Dogs, Cats & Mice"; 
    string str_4 = "☃";

    assert(netutils::percent_encode(str_1).compare("Ladies%20%2B%20Gentlemen") == 0);
    assert(netutils::percent_encode(str_2).compare("An%20encoded%20string%21") == 0);
    assert(netutils::percent_encode(str_3).compare("Dogs%2C%20Cats%20%26%20Mice") == 0);
    assert(netutils::percent_encode(str_4).compare("%E2%98%83") == 0); 

    cout << "percent_encode: tests passed!" << endl;

    return 0;
}
