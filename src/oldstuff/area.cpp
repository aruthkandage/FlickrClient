#include <area.h>

double Area::triangle(double base, double height) {
	return base * height / 2;
}

double Area::circle(double radius) {
	return 3.14159 * (radius * radius);
}
