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

#include <flw/flc/buffers/mVertexBufferText.h>
#include <flw/flc/pipeline/Program.h>

#include <flw/flf/hud/base/Sprite.h>
#include <flw/cmn/resources/FontLoader.h>
#include <flw/flf/models/base/IReloadable.h>
#include <map>

namespace flw {

class Engine;

enum class ETextEffect {
  none
  , bold
};

namespace flf {

/*! \class Text
 * \brief 2D Text on the screen.
 */

class Text : public IReloadable, public Sprite {
public:
  Text(const std::string &text,
      flc::Texture2D *texture,
      glm::vec2 position,
      Engine* engine,
      GLfloat scale,
      Font *font,
      glm::vec4 color = glm::vec4(1.0, 1.0, 1.0, 1.0),
      ETextEffect effect = ETextEffect::none);

  ~Text() override = default;

  void draw() override;

  void editAspectRatio(Engine *engine);

  void editString(std::string text);

  void editColor(glm::vec4 color);

  void editSize(GLfloat size);

  void editPosition(glm::vec2 position);

private:
  /* Text */
  std::string mText;
  glm::vec4 mColor;
  Font* mFont;
  flc::VertexBufferText* mVBO;

  /* IHUD */
  Engine* mEngine;
  GLint mUniformLocationCacheColor;
  GLint mViewportWidth;
  GLint mViewportHeight;

  flc::Program* createProgram(Engine* engine, ETextEffect effect);

  void createVBO();

  void clearVBO();

  void initVBO() override;

  void initVAO() override;

  void initBuffers() override;

  void initPipeline() override;

  void initUniformsCache() override;
};

} /* flf */
} /* flw */
