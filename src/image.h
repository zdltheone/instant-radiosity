#ifndef __image_h__
#define __image_h__

#include "util.h"

class Image {
public:
    Image(int width, int height);
    ~Image();

    void setPixel(unsigned int width, unsigned int height, XMFLOAT4 color);
    XMFLOAT4 getPixel(unsigned int width, unsigned int height) const;

    unsigned int getWidth() const;
    unsigned int getHeight() const;

    XMFLOAT4* getBuffer() const;
private:
    int _width;
    int _height;
    
    XMFLOAT4* _buffer;
};

#endif