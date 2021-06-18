#include "ImageMath.hpp"

void fastFourierTransform(uint32_t n, std::complex<double> x[], std::complex<double>* X)
{
    if(x != X) memcpy(X, x, n*sizeof(std::complex<double>));
    
    // Gentleman-Sande Butterfly algorithm
    uint32_t sub_probs = 1;
    uint32_t sub_prob_size = n;
    uint32_t half, i, j, j_begin, j_end;
    std::complex<double> w_step, w;
    std::complex<double> tmp1, tmp2;

    while(sub_prob_size>1)
    {
        half = sub_prob_size>>1;
        w_step = std::complex<double>(cos(-2*M_PI/sub_prob_size), sin(-2*M_PI/sub_prob_size));
        for(i = 0; i < sub_probs; ++i)
        {
            j_begin = i*sub_prob_size;
            j_end = j_begin+half;
            w = std::complex<double>(1,0);
            for(j = j_begin; j < j_end; ++j)
            {
                tmp1 = X[j];
                tmp2 = X[j+half];
                X[j] = tmp1+tmp2;
                X[j+half] = (tmp1-tmp2)*w;
                w *= w_step;
            }
        }
        sub_probs <<= 1;
        sub_prob_size = half;
    }

}

void inverseFastFourierTransform(uint32_t n, std::complex<double> X[], std::complex<double>* x)
{
    if(X != x) memcpy(x, X, n*sizeof(std::complex<double>));

    // Cooley-Tukey butterfly algorithm
    uint32_t sub_probs = n>>1;
    uint32_t sub_prob_size;
    uint32_t half = 1, i, j, j_begin, j_end;
    std::complex<double> w_step, w;
    std::complex<double> tmp1, tmp2;
    
    while(half < n)
    {
        sub_prob_size = half << 1;
        w_step = std::complex<double>(cos(2*M_PI/sub_prob_size), sin(2*M_PI/sub_prob_size));
        for(i = 0; i < sub_probs; ++i)
        {
            j_begin = i*sub_prob_size;
            j_end = j_begin+half;
            w = std::complex<double>(1,0);
            for(j = j_begin; j < j_end; ++j)
            {
                tmp1 = x[j];
                tmp2 = w*x[j+half];
                x[j] = tmp1+tmp2;
                x[j+half] = tmp1-tmp2;
                w *= w_step;
            }
        }
        sub_probs >>= 1;
        half = sub_prob_size;
    }

    for(uint32_t i=0; i < n; ++i)
    {
        x[i] /= n;
    }
    
}
