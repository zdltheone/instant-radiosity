#include "image.h"

Image::Image(int width, int height) 
: _width(width), _height(height) {
    _buffer = new vec4[_width * _height];
}

Image::~Image() {
    delete [] _buffer;
}

void Image::setPixel(unsigned int pixelRow, unsigned int pixelColumn, vec4 color) {
    assert(pixelRow < _height);
    assert(pixelColumn < _width);

    _buffer[pixelColumn * _width + pixelRow] = color;
}

vec4 Image::getPixel(unsigned int pixelRow, unsigned int pixelColumn) const {
    assert(pixelRow < _height);
    assert(pixelColumn < _width);

    return _buffer[pixelColumn * _width + pixelRow];
}

unsigned int Image::getWidth() const {
    return _width;
}

unsigned int Image::getHeight() const {
    return _height; 
}