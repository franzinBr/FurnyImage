#include "FurnyImage.hpp"
#include "Point.hpp"
#include <cstring>
#include <stdio.h>

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

Image& Image::overlap(const Image& img, Point &&point) 
{
    return this->overlap(img, point);
}

Image& Image::overlap(const Image& img, Point &point)
{
    uint8_t* srcPx;
	uint8_t* dstPx;

	for(int sy = 0; sy < img.h ;++sy) 
    {
		if(sy + point.y < 0) continue;
		else if(sy + point.y >= h) break;

		for(int sx = 0;sx < img.w;++sx) 
        {
			if(sx + point.x < 0) continue;
			else if(sx + point.x >= w) break;

			srcPx = &img.data[(sx + sy * img.w) * img.channels];
			dstPx = &data[(sx + point.x + (sy + point.y) * w) * channels];

			float srcAlpha = img.channels < 4 ? 1 : srcPx[3] / 255.f;
			float dstAlpha = channels < 4 ? 1 : dstPx[3] / 255.f;

			if(srcAlpha > .99 && dstAlpha > .99) {
				if(img.channels >= channels) {
					memcpy(dstPx, srcPx, channels);
				}
				else {

					memset(dstPx, srcPx[0], channels);
				}
			}
			else {
				float outAlpha = srcAlpha + dstAlpha * (1 - srcAlpha);
				if(outAlpha < .01) {
					memset(dstPx, 0, channels);
				}
				else {
					for(int chnl = 0;chnl < channels;++chnl) {
						dstPx[chnl] = (uint8_t)BYTE_BOUND((srcPx[chnl]/255.f * srcAlpha + dstPx[chnl]/255.f * dstAlpha * (1 - srcAlpha)) / outAlpha * 255.f);
					}
					if(channels > 3) {dstPx[3] = (uint8_t)BYTE_BOUND(outAlpha * 255.f);}
				}
			}
			
		}

	}

	return *this;
}

Image& Image::toGray()
{
	if(channels < 3) 
	{
		printf("this image is already in grayscale");
		return *this;
	}
	
	uint8_t* grayImage = new uint8_t[w*h*1];

	for(size_t i = 0, j = 0; i < size; i+=channels, j++)
	{
		int gray = 0.2126*data[i] + 0.7152*data[i+1] + 0.0722*data[i+2];
		memset(grayImage+j, gray, 1);
	}
	channels = 1;
	size = w*h*channels;

	delete[] data;
	data = grayImage;
	grayImage = nullptr;

	return *this;
}

Image& Image::crop(Point p1, Point p2 )
{

	int *len = Point::lengthXY(p1, p2);
	Image cropped(len[0], len[1], channels);
	memset(cropped.data, 0, cropped.size);
	Point first(p1.x < p2.x ? p1.x : p2.x, p1.y < p2.y ? p1.y : p2.y);

	for(uint16_t y = 0; y < len[1];++y) 
	{
		if(y + len[1] >= h) break;
		for(uint16_t x = 0;x < len[0];++x) 
		{
			if(x + len[0] >= w) break;
			memcpy(&cropped.data[(x + y * len[0]) * channels], &data[(x + first.x + (y + first.y) * w) * channels], channels);
		}
	}

	*this = cropped;
	return *this;

}

uint8_t binary(uint8_t value, uint8_t tresh) {return value > tresh ? 255 : 0;}
uint8_t binaryInv(uint8_t value, uint8_t tresh) {return value > tresh ? 0 : 255;}
uint8_t trunc(uint8_t value, uint8_t tresh) {return value > tresh ? tresh : value;}
uint8_t tozero(uint8_t value, uint8_t tresh) {return value > tresh ? value : 0;}
uint8_t tozeroInv(uint8_t value, uint8_t tresh) {return value > tresh ? 0 : value;}


Image& Image::thresholding(uint8_t treshValue, TreshType type)
{
	treshValue = BYTE_BOUND(treshValue);
	

	if(channels >= 3) this->toGray();

	uint8_t (*tresh)(uint8_t, uint8_t) ;

	switch (type)
	{
		case BINARY:
			tresh = binary;
			break;
		case BINARY_INV:
			tresh = binaryInv;
			break;
		case TRUNC:
			tresh = trunc;
			break;
		case TOZERO:
			tresh = tozero;
			break;
		case TOZERO_INV:
			tresh = tozeroInv;
			break;
		default:
			tresh = binary;
			break;
	}

	for(size_t i = 0; i < size; i++)
	{
		data[i] = tresh(data[i], treshValue);
	}

	return *this;
	
}