#include <my_units.h>

namespace my_units {

static double PI = 3.14159;
static double C2F = 9.0 / 5.0;

double degrees_to_radians(double degrees) {
    return degrees * PI / 180;
}

double radians_to_degrees(double degrees) {
    return degrees * 180 / PI;
}

double celcius_to_fahrenheit(double celcius) {
    return (celcius * C2F) + 32;
}

double fahrenheit_to_celcius(double fahrenheit) {
    return (fahrenheit - 32) / C2F;
}

}