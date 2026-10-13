#pragma once
#include <vector>
#include <string>
#include <mutex>
#include <cstdint>

namespace Menu {
    struct Cmd {
        enum Type { Rect, Text, Circle, Line } type;
        float x,y,w,h,r;
        uint32_t col;
        std::string text;
    };

    void Rect(float x, float y, float w, float h, uint32_t c);
    void Text(float x, float y, const char* t, uint32_t c);
    void Circle(float x, float y, float r, uint32_t c);
    void Line(float x1, float y1, float x2, float y2, uint32_t c);

    void OnTouch(float x, float y, bool down, bool up);
    void Render(float sw, float sh);

    // consumed by renderer (Metal / UIKit overlay)
    std::vector<Cmd>& Commands();
    std::mutex&       Mutex();
}
