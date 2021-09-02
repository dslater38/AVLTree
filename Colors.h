#ifndef COLORS_H_INCLUDED
#define COLORS_H_INCLUDED
#pragma once
#include <cstdint>
#include <algorithm>

constexpr inline uint32_t rgb(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r << 16) | (g << 8) | b);
}

constexpr inline uint32_t hsl(uint8_t h, uint8_t s, uint8_t l)
{
    return ((h << 16) | (s << 8) | l);
}

constexpr inline uint8_t rValue(uint32_t c)
{
    return ((c & 0x00FF0000) >> 16);
}

constexpr inline uint8_t gValue(uint32_t c)
{
    return ((c & 0x0000FF00) >> 8);
}

constexpr inline uint8_t bValue(uint32_t c)
{
    return (c & 0x000000FF);
}


constexpr inline uint8_t hValue(uint32_t c)
{
    return ((c & 0x00FF0000) >> 16);
}

constexpr inline uint8_t sValue(uint32_t c)
{
    return ((c & 0x0000FF00) >> 8);
}

constexpr inline uint8_t lValue(uint32_t c)
{
    return (c & 0x000000FF);
}



constexpr inline uint8_t clrAvg(uint8_t a, uint8_t b)
{
    return static_cast<uint8_t>((static_cast<uint16_t>(a) + static_cast<uint16_t>(b)) >> 1);
}

constexpr inline uint32_t blend(uint32_t rgb1, uint32_t rgb2)
{
    return rgb(clrAvg(rValue(rgb1), rValue(rgb2)), clrAvg(gValue(rgb1), gValue(rgb2)), clrAvg(bValue(rgb1), bValue(rgb2)));
}

inline uint32_t rgb2hsl(uint32_t rgb)
{
    auto r = (rValue(rgb) / 255.0);
    auto g = (gValue(rgb) / 255.0);
    auto b = (bValue(rgb) / 255.0);
    

    auto min = std::min(std::min(r, g), b);
    auto max = std::max(std::max(r, g), b);
    auto delta = max - min;

    auto fL = (max + min) / 2.0;
    auto fH = 0.0;
    auto fS = 0.0;

    if (delta != 0)
    {
        fS = (fL <= 0.5) ? (delta / (max + min)) : (delta / (2 - max - min));

        auto hue = 0.0;
        
        if (r == max)
        {
            hue = ((g - b) / 6) / delta;
        }
        else if (g == max)
        {
            hue = (1.0 / 3.0) + ((b - r) / 6) / delta;
        }
        else
        {
            hue = (2.0 / 3.0) + ((r - g) / 6) / delta;
        }

        if (hue < 0.0)
        {
            hue += 1.0;
        }
        if (hue > 1.0)
        {
            hue -= 1.0;
        }
    }
    auto h = static_cast<uint8_t>(hue * 255.0);
    auto s = static_cast<uint8_t>(fS * 255.0);
    auto l = static_cast<uint8_t>(fL * 255.0);
    return hsl(h, s, l);
}


#endif // COLORS_H_INCLUDED
