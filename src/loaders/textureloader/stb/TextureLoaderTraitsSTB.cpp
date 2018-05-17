/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Filip Wasil and Fillwave community members
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <fillwave/loaders/textureloader/TTextureLoader.h>
#include <fillwave/loaders/textureloader/stb/TextureLoaderTraitsSTB.h>

#include <fillwave/Texturing.h>

#include <fillwave/common/Strings.h>
#include <fillwave/common/Macros.h>

#include <iterator>

#include <fillwave/Log.h>

FLOGINIT_DEFAULT()

using namespace std;

namespace flw {
namespace flf {

template <>
flc::TextureConfig* TTextureLoader<TextureLoaderTraitsSTB>::load(const std::string &filePath,
    flc::EFlip flip,
    GLenum format,
    std::string rootPath,
  flc::ECompression compression,
    GLenum cubeTarget) {

  fLogD("Texture ", filePath.c_str(), " loading ...");
  const size_t posCheckboard = filePath.find(".checkboard");
  const size_t posColor = filePath.find(".color");
  const size_t posDDS = filePath.find(".dds");
  uint8_t r = 0, g = 0, b = 0;

  auto textureGenerator = TextureLoaderTraitsSTB().mGenerator;

  if (filePath == rootPath) {
    fLogD("Empty texture ", filePath, " generation and loading ...");
    auto cfg = textureGenerator.loadVirtualFileColor(512, 512, 0, 0, 0);
    cfg->mHeader.mCubeTarget = cubeTarget;
    return cfg;
  }

  if (posColor != std::string::npos) {
    fLogD("Color texture ", filePath, " generation and loading ...");
    std::string sub = filePath.substr(rootPath.size(), posColor);
    std::vector<std::string> tokens = split(sub, '_');
    if (tokens.size() < 3) {
      fLogE("Could not found color parameters in ", filePath);
      return nullptr;
    }

    r = atoi(tokens[0].c_str());
    g = atoi(tokens[1].c_str());
    b = atoi(tokens[2].c_str());

    auto cfg = textureGenerator.loadVirtualFileColor(512, 512, r, g, b);
    cfg->mHeader.mCubeTarget = cubeTarget;
    return cfg;
  }
  if (posCheckboard != std::string::npos) {
    fLogD("Checkboard texture ", filePath, " generation and loading ...");
    std::string sub = filePath.substr(rootPath.size(), posCheckboard);
    std::vector<std::string> tokens = split(sub, '_');
    if (tokens.size() < 3) {
      fLogE("Could not found color parameters in ", filePath);
      return nullptr;
    }

    r = atoi(tokens[0].c_str());
    g = atoi(tokens[1].c_str());
    b = atoi(tokens[2].c_str());

    auto cfg = textureGenerator.loadVirtualFileCheckboard(512, 512, r, g, b);
    cfg->mHeader.mCubeTarget = cubeTarget;
    return cfg;
  }
  if (posDDS != std::string::npos) {
    fLogE("Compressed Texture ", filePath, " not supported yet");
    return nullptr;
  }

  GLint w, h, n;
  auto content = stbi_load(filePath.c_str(), &w, &h, &n, textureGenerator.getBytesPerPixel(format));
  if (NULL == content) { //xxx NULL, not nullptr because the stb library uses NULL
    FILE *f;
    auto errorNo = fopen_s(&f, filePath.c_str(), "rb");
    if (errorNo) {
      fLogE("Texture ", filePath, " not found");
    } else {
      fLogE("Texture ", filePath, " found but not supported ");
      fclose(f);
    }
    return nullptr;
  }
  fLogD("Image ",filePath, " size ", w, "x", h, " pixel ", n, " bytes per pixel");
  auto cfg = new flc::TextureConfig();

  cfg->mHeader.mFormat = format;
  cfg->mHeader.mWidth = w;
  cfg->mHeader.mHeight = h;
  cfg->mHeader.mType = GL_UNSIGNED_BYTE;

  cfg->mContent.mMipmaps = GL_TRUE;
  cfg->mContent.mMipmapsLevel = 0;

  cfg->mHeader.mCubeTarget = cubeTarget;

  if (compression == flc::ECompression::none) {
    cfg->mContent.mCompression = GL_FALSE;
    cfg->mHeader.mInternalFormat = format;
  } else {
    cfg->mContent.mCompression = GL_TRUE;
    cfg->mHeader.mInternalFormat = textureGenerator.getCompression(compression);
    cfg->mContent.mCompressionSize = 0;
    fLogF("Texture compression feature not ready");
  }

  cfg->mContent.mBorder = 0;

  cfg->mData = content;

  cfg->mAllocation = flc::EMemoryAllocation::cstyle;

  fLogD("Flipping Texture", filePath, " ...");
  switch (flip) {
    case flc::EFlip::vertical:
#pragma omp parallel for schedule(guided) num_threads(2)
      for (int row = 0; row < h / 2; ++row) {
        for (int column = 0; column < w; ++column) {
          const int pixelOffset1 = row * w * n + column * n;
          const int pixelOffset2 = (h - row - 1) * w * n + column * n;
          for (int byteInPixel = 0; byteInPixel < n; ++byteInPixel) {
            const int exchangeIndex1 = pixelOffset1 + byteInPixel;
            const int exchangeIndex2 = pixelOffset2 + byteInPixel;
            cfg->mData[exchangeIndex1] ^= cfg->mData[exchangeIndex2];
            cfg->mData[exchangeIndex2] ^= cfg->mData[exchangeIndex1];
            cfg->mData[exchangeIndex1] ^= cfg->mData[exchangeIndex2];
          }
        }
      }
      break;

    case flc::EFlip::horizontalVertical:
#pragma omp parallel for schedule(guided) num_threads(2)
      for (int row = 0; row < h; ++row) {
        for (int column = 0; column < w / 2; ++column) {
          const int pixelOffset1 = row * w * n + column * n;
          const int pixelOffset2 = row * w * n + (w - column - 1) * n;
          for (int byteInPixel = 0; byteInPixel < n; ++byteInPixel) {
            const int exchangeIndex1 = pixelOffset1 + byteInPixel;
            const int exchangeIndex2 = pixelOffset2 + byteInPixel;
            cfg->mData[exchangeIndex1] ^= cfg->mData[exchangeIndex2];
            cfg->mData[exchangeIndex2] ^= cfg->mData[exchangeIndex1];
            cfg->mData[exchangeIndex1] ^= cfg->mData[exchangeIndex2];
          }
        }
      }
      break;

    case flc::EFlip::horizontal:
#pragma omp parallel for schedule(guided) num_threads(2)
      for (int row = 0; row < h; ++row) {
        for (int column = 0; column < w / 2; ++column) {
          const int pixelOffset1 = row * w * n + column * n;
          const int pixelOffset2 = row * w * n + (w - column - 1) * n;
          for (int byteInPixel = 0; byteInPixel < n; ++byteInPixel) {
            const int exchangeIndex1 = pixelOffset1 + byteInPixel;
            const int exchangeIndex2 = pixelOffset2 + byteInPixel;
            cfg->mData[exchangeIndex1] ^= cfg->mData[exchangeIndex2];
            cfg->mData[exchangeIndex2] ^= cfg->mData[exchangeIndex1];
            cfg->mData[exchangeIndex1] ^= cfg->mData[exchangeIndex2];
          }
        }
      }
#pragma omp parallel for schedule(guided) num_threads(2)
      for (int row = 0; row < h / 2; ++row) {
        for (int column = 0; column < w; ++column) {
          const int pixelOffset1 = row * w * n + column * n;
          const int pixelOffset2 = (h - row - 1) * w * n + column * n;
          for (int byteInPixel = 0; byteInPixel < n; ++byteInPixel) {
            const int exchangeIndex1 = pixelOffset1 + byteInPixel;
            const int exchangeIndex2 = pixelOffset2 + byteInPixel;
            cfg->mData[exchangeIndex1] ^= cfg->mData[exchangeIndex2];
            cfg->mData[exchangeIndex2] ^= cfg->mData[exchangeIndex1];
            cfg->mData[exchangeIndex1] ^= cfg->mData[exchangeIndex2];
          }
        }
      }
      break;

    case flc::EFlip::none:
      break;
  }
  return cfg;
}

template <>
flc::TextureConfig* TTextureLoader<TextureLoaderTraitsSTB>::load(GLint screenWidth, GLint screenHeight, GLenum /*format*/) {
  return TextureLoaderTraitsSTB().mGenerator.loadEmpty(screenWidth, screenHeight);
}

template <>
const std::vector<flc::ECompression> TTextureLoader<TextureLoaderTraitsSTB>::getSupportedCompressionFormats() {
  return { };
}

template
class TTextureLoader<TextureLoaderTraitsSTB>;

} /* flf */
} /* flw */