#ifndef IMAGEMATH_HPP
#define IMAGEMATH_HPP
#define _USE_MATH_DEFINES

#include <stdint.h>
#include <cmath>
#include <complex>

void fastFourierTransform(uint32_t n, std::complex<double> x[], std::complex<double>* X);



#endif // IMAGEMATH_HPP