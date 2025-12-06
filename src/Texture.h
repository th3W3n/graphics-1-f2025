#pragma once
#include <cstdint> //provides fixed-width (platform-independent) integer types
#include <glad/glad.h>

//each channel is 8 bits = 0 - 255 (and initialize to black)
struct Pixel
{
    uint8_t r = 0x00;
    uint8_t g = 0x00;
    uint8_t b = 0x00;
    uint8_t a = 0xFF;
};

void CreateSampleImage();
GLuint LoadTexture(const char *path);
void UnloadTexture(GLuint texture);