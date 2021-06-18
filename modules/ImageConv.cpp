#include "FurnyImage.hpp"

void zeroPad(long &value, int side, bool &isContinue)
{ 
    if(value < 0 || value > side-1) isContinue = true;
}

void nearNeighbor(long &value, int side, bool &isContinue)
{
    if(value < 0 ) value = 0;
    if(value > side-1) value = side-1;
}

void cyclic(long &value, int side, bool &isContinue)
{
    
     if(value < 0 ) value = value%side + side;
     if(value > side-1) value = value%side;
}

Image& Image::conv(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc, ConvPadType convType)
{
    void (*convv)(long &, int, bool &) ;

	switch (convType)
	{
		case ZERO:
			convv = zeroPad;
        case NEAR_NEIGHBOR:
            convv = nearNeighbor;
			break;
        case CYCLIC:
            convv = cyclic;
            break;
	}

    uint8_t * new_data = new uint8_t[w*h];
    uint64_t center = cr*ker_w + cc;
    for(uint64_t k=channel; k<size; k+=channels)
    {
        double c = 0;
        long i;
        for(i = -((long)cr); i < (long)(ker_h-cr); ++i)
        {
            long row = ((long)(k/channels))/w-i;
            
            bool isContinue = false;
            convv(row, h, isContinue);
            if(isContinue) continue;


            for(long j = -((long)cc); j < (long)(ker_w-cc); ++j)
            {
                long col = ((long)(k/channels))%w-j;

                bool isContinue2 = false;
                convv(col, w, isContinue2);
                if(isContinue2) continue;

                c += ker[center+i*(long)(ker_w+j)]*data[(row*w+col)*channels+channel];
            }
        }
        new_data[k/channels] = (uint8_t)BYTE_BOUND(round(c));
    }
    for(uint64_t k=channel; k<size; k+=channels)
    {
        data[k] = new_data[k/channels];
    }
    return *this;
}


void padKernel(uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc, uint32_t pw, uint32_t ph, std::complex<double>* pad_ker)
{
    for(long i= -((long)(cr));  i < ((long)(ker_h-cr)); ++i)
    {
        uint32_t r = (i<0) ? i+ph : i;
        for(long j= -((long)(cc)); j < ((long)(ker_w-cc)); ++j )
        {
            uint32_t c = (j<0) ? j+pw : j;
            pad_ker[r*pw+c] = std::complex<double>(ker[(i+cr)*ker_w+(j+cc)], 0);
        }
    }
}

void padImage(Image &img, uint8_t channel, uint32_t pw, uint32_t ph,std::complex<double>* pad_img, uint32_t cr, uint32_t cc, ConvPadType convType)
{
    if(convType == ZERO)
    {
        for(uint32_t i = 0; i < img.h; ++i)
        {
            for(uint32_t j = 0; j < img.w; ++j)
            {
                pad_img[i*pw+j] = std::complex<double>(img.data[(i*img.w+j)*img.channels+channel],0);
            }
        }
        return;
    }

    if(convType == NEAR_NEIGHBOR)
    {
        for(uint32_t i = 0; i < ph; ++i)
        {
            uint32_t r = (i < img.h) ? i : ((i<img.h+cr ? img.h - 1 : 0));
            for(uint32_t j = 0; j < pw; ++j)
            {
                uint32_t c = (j < img.w) ? j : ((j<img.w+cc ? img.w - 1 : 0));
                pad_img[i*pw+j] = std::complex<double>(img.data[(r*img.w+c)*img.channels+channel],0);
            }
        }
        return;
    }

    for(uint32_t i = 0; i < ph; ++i)
    {
        uint32_t r = (i < img.h) ? i : ((i<img.h+cr ? i%img.h : img.h-(ph+i) ));
        for(uint32_t j = 0; j < pw; ++j)
        {
            uint32_t c = (j < img.w) ? j : ((j<img.w+cc ? j%img.w : img.w-pw+i ));
            pad_img[i*pw+j] = std::complex<double>(img.data[(r*img.w+c)*img.channels+channel],0);
        }
    }

    return;
}

Image& Image::convFourier(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc, ConvPadType convType)
{
    uint32_t pw = 1<<((uint8_t)(ceil(log2(w+ker_w-1))));
    uint32_t ph = 1<<((uint8_t)(ceil(log2(h+ker_h-1))));
    uint64_t psize = pw*ph;

    std::complex<double>* pad_img = new std::complex<double>[psize];
    padImage(*this, channel, pw, ph, pad_img, cr, cc, convType);

    std::complex<double>* pad_ker = new std::complex<double>[psize];
    padKernel(ker_w, ker_h, ker, cr, cc, pw, ph, pad_ker);

    discreteFourierTransform2D(ph, pw, pad_img, pad_img);
    discreteFourierTransform2D(ph, pw, pad_ker, pad_ker);
    pointWiseMultiplication(psize, pad_img, pad_ker, pad_img);
    inverseDiscreteFourierTransform2D(ph, pw, pad_img, pad_img);

    for(uint32_t i = 0; i < h; ++i)
    {
        for(uint32_t j = 0; j < w; ++j)
        {
           data[(i*w+j)*channels+channel] = BYTE_BOUND((uint8_t)(round(pad_img[i*pw+j].real())));
        }
    }

    return *this;
}