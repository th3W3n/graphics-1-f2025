#define _CRT_SECURE_NO_WARNINGS
#include "Texture.h"
#include "raymath.h"
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image/stb_image_write.h>

void CreateSampleImage()
{
    int image_width = 512;
    int image_height = 512;
    std::vector<Pixel> pixels;
    pixels.resize(image_width * image_height);

    for (int y = 0; y < image_height; y++)
    {
        for (int x = 0; x < image_width; x++)
        {
            //flip the y since (0,0) is different
            //in stbi (top left) and texture (bottom left)
            Vector2 uv{x / (float)image_width, 1.0f - y / (float)image_height};
            Pixel &pixel = pixels[y * image_width + x];
            pixel.r = (uint8_t)(uv.x * 255.0f);
            pixel.g = (uint8_t)(uv.y * 255.0f);
        }
    }

    stbi_write_png("./assets/textures/sample.png", image_width, image_height, 4, pixels.data(), 0);
    stbi_write_jpg("./assets/textures/sample.jpg", image_width, image_height, 4, pixels.data(), 100);
}

GLuint LoadTexture(const char *path)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //normally uv in obj files set (0,0) at bottom-left
    //images like jpg/png etc. normally set (0,0) at top-left
    stbi_set_flip_vertically_on_load(true);

    int width, height, channels;
    unsigned char *data = stbi_load(path, &width, &height, &channels, 0);
    if (data)
    {
        GLenum format;
        switch (channels)
        {
            case 1 : format = GL_RED; break;
            case 4 : format = GL_RGBA; break;
            default: format = GL_RGB; break;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        return texture;
    }
    else //failed to load
    {
        stbi_image_free(data);
        return -1;
    }
}