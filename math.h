#ifndef MATH_H
#define MATH_H

struct Vec3 {
    float x, y, z;
};

struct Vec2 {
    float x, y;
};

bool worldToScreen(const Vec3& pos, Vec2& screen, float* viewMatrix, int screenWidth, int screenHeight);

#endif