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

#include <flw/flf/loaders/textureloader/TextureFactory.h>

#include <flw/Log.h>

FLOGINIT_DEFAULT()

#if defined(FILLWAVE_BACKEND_OPENGL_ES_20)
#define GL_RED GL_RED_EXT
#define GL_RG8 GL_RG8_EXT
#define GL_RGBA8 GL_RGBA8_OES
#endif

namespace flw {
namespace flf {

flc::TextureConfig* TextureFactory::loadEmpty(GLint screenWidth, GLint screenHeight, GLenum format) {
  auto cfg = new flc::TextureConfig();

  cfg->mHeader.mFormat = format;
  cfg->mHeader.mInternalFormat = format;
  cfg->mHeader.mWidth = screenWidth;
  cfg->mHeader.mHeight = screenHeight;
  cfg->mHeader.mType = GL_UNSIGNED_BYTE;

  cfg->mContent.mMipmaps = GL_TRUE;
  cfg->mContent.mMipmapsLevel = 0;
  cfg->mContent.mCompression = GL_FALSE;
  cfg->mContent.mBorder = 0;

  cfg->mData = nullptr;

  cfg->mAllocation = flc::EMemoryAllocation::none;

  return cfg;
}

flc::TextureConfig* TextureFactory::loadVirtualFileCheckboard(
  GLuint width
  , GLuint height
  , GLubyte red
  , GLubyte green
  , GLubyte blue
  , GLenum format) {

  const GLint bytesPerPixel = [format](){
    switch (format) {
      case GL_RGBA:
        return 4;
      case GL_RGB:
        return 3;
      case GL_ALPHA:
      case GL_RED:
//      case GL_GREEN:
//      case GL_BLUE:
        return 1;
      default:
        fLogE("Not recognized texture format loading");
        return 0;
    }
  } ();
  //getBytesPerPixel(format);
  const GLint size = bytesPerPixel * width * height * sizeof(GLubyte);
  auto content = new GLubyte[size];

  GLint i, r = 0, g = 0, b = 0;
  for (i = 0; i < size; i += bytesPerPixel) {
    if (i % ((width * bytesPerPixel) * 8) == 0) {
      r ^= red;
      g ^= green;
      b ^= blue;
    }
    if (i % (bytesPerPixel * 8) == 0) {
      r ^= red;
      g ^= green;
      b ^= blue;
    }
    content[i] = r;
    content[i + 1] = g;
    content[i + 2] = b;
    content[i + 3] = 1;
  }

  auto cfg = new flc::TextureConfig();
  cfg->mHeader.mFormat = format;
  cfg->mHeader.mInternalFormat = format;
  cfg->mHeader.mWidth = width;
  cfg->mHeader.mHeight = height;
  cfg->mHeader.mType = GL_UNSIGNED_BYTE;

  cfg->mContent.mMipmaps = GL_TRUE;
  cfg->mContent.mMipmapsLevel = 0;
  cfg->mContent.mCompression = GL_FALSE;
  cfg->mContent.mBorder = 0;

  cfg->mData = make_pu_with_no_ownership<GLubyte>(content);

  cfg->mAllocation = flc::EMemoryAllocation::standardBrace;

  return cfg;
}

flc::TextureConfig* TextureFactory::loadVirtualFileColor(
  GLuint width
  , GLuint height
  , GLubyte red
  , GLubyte green
  , GLubyte blue
  , GLenum format) {

  const GLint bytesPerPixel = [format](){
    switch (format) {
      case GL_RGBA:
        return 4;
      case GL_RGB:
        return 3;
      case GL_ALPHA:
      case GL_RED:
//      case GL_GREEN:
//      case GL_BLUE:
        return 1;
      default:
        fLogE("Not recognized texture format loading");
        return 0;
    }
  } ();

  const int size = bytesPerPixel * width * height * sizeof(GLubyte);
  GLubyte* content = new GLubyte[size];

  for (int i = 0; i < size; i += bytesPerPixel) {
    content[i] = red;
    content[i + 1] = green;
    content[i + 2] = blue;
    content[i + 3] = 1;
  }

  auto cfg = new flc::TextureConfig();
  cfg->mHeader.mFormat = format;
  cfg->mHeader.mInternalFormat = format;
  cfg->mHeader.mWidth = width;
  cfg->mHeader.mHeight = height;
  cfg->mHeader.mType = GL_UNSIGNED_BYTE;

  cfg->mContent.mMipmaps = GL_FALSE;
  cfg->mContent.mMipmapsLevel = 0;
  cfg->mContent.mCompression = GL_FALSE;
  cfg->mContent.mBorder = 0;

  cfg->mData = make_pu_with_no_ownership<GLubyte>(content);

  cfg->mAllocation = flc::EMemoryAllocation::standardBrace;

  return cfg;
}

} /* flf */
} /* flw */
