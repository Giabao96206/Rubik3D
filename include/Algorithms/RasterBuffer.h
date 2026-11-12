#pragma once
#include <array>
#include <cstdint>
#include <vector>
#include "Algorithms/Bresenham.h"

namespace rubik {
class RasterBuffer {
public:
    RasterBuffer(int width,int height);
    void clear(std::array<std::uint8_t,3> color={20,22,28});
    void setPixel(int x,int y,std::array<std::uint8_t,3> color);
    void drawLine(PixelPoint a,PixelPoint b,std::array<std::uint8_t,3> color);
    void fillPolygon(const std::vector<PixelPoint>& polygon,std::array<std::uint8_t,3> color);
    int width()const{return width_;} int height()const{return height_;}
    const std::vector<std::uint8_t>& data()const{return data_;}
private:int width_,height_;std::vector<std::uint8_t> data_;
};
}

