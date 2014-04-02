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
#include <cassert>
#include <error.h>

using namespace std;

struct basic_error_test {
    void operator()() {
        f();
        g();
        h();
    }

    void f() {
        try {
            throw app::error("f()");
        } catch(app::error& e) {
            cout << e.what() << endl; 
        }
    }

    void g() {
        try {
            throw app::error() << app::error::message("g()");
        } catch(app::error& e) {
            cout << e.what() << endl;
        }
    }

    class h_error : public app::error {
        public:
        error_param(int, data);
    };

    void h() {
        try {
            throw h_error() << h_error::message("h()") << h_error::data(7);
        } catch(h_error& e) {
            cout << e.what() << " " << *(boost::get_error_info<h_error::data>(e)) << endl;
        }
    }
};

class chained_test {
    public:
    void operator()() {
        h();
    }

    void f() {
        throw app::error("f()");
    }

    void g() {
        try {
            f();
        } catch(app::error& e) {
            throw app::error_chain(e) << app::error::message("g()");
        }
    }

    void h() {
        try {
            g();
        } catch(app::error_chain& e) {
            cout << e.what() << endl;
        } catch(app::error& e) {
            assert(false);
        }
    }
};

int main(int argc, char** argv) {
    basic_error_test()();
    chained_test()();
}
