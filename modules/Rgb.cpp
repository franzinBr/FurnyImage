#include "Rgb.hpp"

Rgb::Rgb() : r(0), g(0), b(0) {}
Rgb::Rgb(uint8_t unique) : r(unique), g(unique), b(unique) {}
Rgb::Rgb(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
Rgb::Rgb(int unique): r((uint8_t)unique), g((uint8_t)unique), b((uint8_t)unique) {}
Rgb::Rgb(int r, int g, int b): r((uint8_t)r), g((uint8_t)g), b((uint8_t)b) {}
Rgb::~Rgb() {}
