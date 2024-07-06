#pragma once

#include <cstdint>
#include <string>

namespace Vision {
class Texture {
  private:
    unsigned int renderID;
    uint32_t width, height, BPP;
    uint32_t format, imgFormat;
    uint32_t wrapS, wrapT;
    uint32_t filterMin, filterMax;

    void Generate(uint32_t width, uint32_t height, unsigned char* data);

  public:
    Texture(const std::string path, const bool alpha);

    inline uint32_t& GetID() { return renderID; }
    void Bind() const;
};
} // namespace Vision
