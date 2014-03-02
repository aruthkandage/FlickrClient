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
