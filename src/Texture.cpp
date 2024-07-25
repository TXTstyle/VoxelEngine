#include "Texture.hpp"
#include "stb/stb_image.h"
#include <cstdint>
#include <glad/glad.h>
#include <string>

using namespace Vision;

Texture::Texture(const std::string path, const bool alpha)
    : width(0), height(0), format(GL_RGB), imgFormat(GL_RGB), wrapS(GL_REPEAT),
      wrapT(GL_REPEAT), filterMin(GL_NEAREST), filterMax(GL_NEAREST) {

    if (alpha) {
        format = GL_RGBA8;
        imgFormat = GL_RGBA;
    }

    int width, height, BPP;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &BPP, 0);

    Generate(width, height, data);

    stbi_image_free(data);
}

void Texture::Bind() const { glBindTexture(GL_TEXTURE_2D, renderID); }

void Texture::Generate(uint32_t width, uint32_t height, unsigned char* data) {
    this->width = width;
    this->height = height;

    glGenTextures(1, &renderID);

    // Create Texture
    glBindTexture(GL_TEXTURE_2D, renderID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, imgFormat,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMax);

    glBindTexture(GL_TEXTURE_2D, 0);
}
