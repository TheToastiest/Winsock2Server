#pragma once
inline float interpolate(float a, float b, float factor) {
    return a + factor * (b - a);
}