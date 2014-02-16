#include <iostream>
#include <area.h>

using namespace std;

int main(int argc, char** argv) {
	double b,h,r;

	cin >> b;
	cin >> h;
	cin >> r;

	cout << "Triangle: " << Area::triangle(b,h) << endl;
	cout << "Circle: " << Area::circle(r) << endl;
}
