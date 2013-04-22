#ifndef __image_h__
#define __image_h__

#include "util.h"

class Image {
public:
    Image(int width, int height);
    ~Image();

    void setPixel(int width, int height, vec4 color);
    vec4 getPixel(int width, int height) const;

private:
    int _width;
    int _height;
    
    vec4* _buffer;
};

#endif