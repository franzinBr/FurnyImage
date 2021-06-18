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