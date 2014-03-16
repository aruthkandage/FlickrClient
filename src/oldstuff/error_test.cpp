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
