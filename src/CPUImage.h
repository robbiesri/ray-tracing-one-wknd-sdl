#pragma once

#include "AssertUtils.h"
#include "ColorUtils.h"

#include <vector>

// TODO: Use template for underlying color type?
// Type and count

class CPUImage {
public:
  CPUImage(uint32_t width, uint32_t height)
      : m_width(width), m_height(height) {
    m_imageData.resize(m_width * m_height);
  }

  Color3& operator()(uint32_t x, uint32_t y) {
    // TODO assert dimension
    WKND_ASSERT(x < m_width);
    WKND_ASSERT(y < m_height);
    return m_imageData[(m_width * y) + x];
  }

  // private:
  std::vector<Color3> m_imageData;
  uint32_t m_width;
  uint32_t m_height;
};

// Old image
//template <class ComponentFormatT> class CPUImage {
//public:
//  CPUImage(uint32_t width, uint32_t height, ColorBits colors)
//      : m_width(width), m_height(height), m_colors(colors) {
//    m_componentCount = CountColorBits(colors);
//    m_rowStride = m_componentCount * m_width;
//
//    m_imageData.resize(m_width * m_height * m_componentCount);
//  }
//
//  ComponentFormatT *getPixelData(uint32_t w, uint32_t h) {
//    return &m_imageData[(m_rowStride * h) + w];
//  }
//
//  // TODO Add 2d operator()
//
//  // private:
//  std::vector<ComponentFormatT> m_imageData;
//  uint32_t m_width;
//  uint32_t m_height;
//  ColorBits m_colors;
//  uint32_t m_componentCount;
//  uint32_t m_rowStride; // needed?
//};