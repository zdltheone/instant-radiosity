#ifndef __image_h__
#define __image_h__

#include "util.h"

class Image {
public:
    Image(int width, int height);
    ~Image();

    void setPixel(unsigned int width, unsigned int height, vec4 color);
    vec4 getPixel(unsigned int width, unsigned int height) const;

    unsigned int getWidth() const;
    unsigned int getHeight() const;
private:
    int _width;
    int _height;
    
    vec4* _buffer;
};

#endif