#pragma once

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

#include <fillwave/OpenGL.h>

namespace flw {
namespace flc {

/*! \class ConditionalRender
 * \brief Operation of rendering only meshes passing the occlusion test.
 */

class ConditionalRender {
public:

  /*!
   * \brief Specifies the conditional rendering pass mode.
   *
   * \param Possible modes:
   *  GL_QUERY_WAIT,
   *  GL_QUERY_NO_WAIT,
   *  GL_QUERY_BY_REGION_WAIT,
   *  GL_QUERY_BY_REGION_NO_WAIT,
   */

  ConditionalRender(GLenum mode);

  virtual ~ConditionalRender();

  void begin(GLuint querryID) const;

  void end() const;

private:
  GLenum mMode;
};

} /* flc */
} /* flw */
