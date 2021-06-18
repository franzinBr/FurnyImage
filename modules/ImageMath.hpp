#ifndef IMAGEMATH_HPP
#define IMAGEMATH_HPP
#define _USE_MATH_DEFINES

#include <stdint.h>
#include <cmath>
#include <cstring>
#include <complex>

void fastFourierTransform(uint32_t n, std::complex<double> x[], std::complex<double>* X);
void inverseFastFourierTransform(uint32_t n, std::complex<double> X[], std::complex<double>* x);
void discreteFourierTransform2D(uint32_t m, uint32_t n, std::complex<double> x[], std::complex<double>* X);
void inverseDiscreteFourierTransform2D(uint32_t m, uint32_t n, std::complex<double> X[], std::complex<double>* x);


#endif // IMAGEMATH_HPP