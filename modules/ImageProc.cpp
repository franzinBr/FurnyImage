#include "FurnyImage.hpp"
#include <cstring>

Image& Image::flip(uint8_t direction) 
{
    uint8_t tmp[4];
	uint8_t* px1;
	uint8_t* px2;

    switch (direction)
    {
        case 0:
		    for(int y = 0;y < h;++y) 
            {
                for(int x = 0;x < w/2;++x) {
                    px1 = &data[(x + y * w) * channels];
                    px2 = &data[((w - 1 - x) + y * w) * channels];

                    memcpy(tmp, px1, channels);
                    memcpy(px1, px2, channels);
                    memcpy(px2, tmp, channels);
                }
		    }
		    return *this;
            
        case 1:
		    for(int x = 0;x < w;++x) 
            {
			    for(int y = 0;y < h/2;++y) 
                {
                    px1 = &data[(x + y * w) * channels];
                    px2 = &data[(x + (h - 1 - y) * w) * channels];

                    memcpy(tmp, px1, channels);
                    memcpy(px1, px2, channels);
                    memcpy(px2, tmp, channels);
                }
		    }
		    return *this;

        default:
            break;
    }
}