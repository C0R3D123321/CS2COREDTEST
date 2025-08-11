#include "math.h"
#include <windows.h>

bool worldToScreen(const Vec3& pos, Vec2& screen, float* viewMatrix, int screenWidth, int screenHeight) {
    float w = viewMatrix[12] * pos.x + viewMatrix[13] * pos.y + viewMatrix[14] * pos.z + viewMatrix[15];
    if (w < 0.01f) return false;

    float invw = 1.0f / w;
    screen.x = (viewMatrix[0] * pos.x + viewMatrix[1] * pos.y + viewMatrix[2] * pos.z + viewMatrix[3]) * invw;
    screen.y = (viewMatrix[4] * pos.x + viewMatrix[5] * pos.y + viewMatrix[6] * pos.z + viewMatrix[7]) * invw;

    float x = screenWidth / 2.0f + 0.5f * screen.x * screenWidth + 0.5f;
    float y = screenHeight / 2.0f - 0.5f * screen.y * screenHeight + 0.5f;
    screen.x = x;
    screen.y = y;
    return true;
}