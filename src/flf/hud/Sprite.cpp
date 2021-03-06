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

#include <flw/flf/hud/base/Sprite.h>

#include <flw/Log.h>

FLOGINIT_DEFAULT()

namespace flw {
namespace flf {

/*! \class Sprite
 * \brief HUD base element.
 */

Sprite::Sprite(flc::Texture2D *texture, flc::Program *program, glm::vec2 position, glm::vec2 scale)
    : mTexture(texture)
    , mProgram(program)
    , mPosition(position)
    , mScale(scale) {
  mBlending = {
      GL_SRC_ALPHA,
      GL_ONE_MINUS_SRC_ALPHA,
      GL_FUNC_ADD
  };
}

Sprite::~Sprite() = default;

void Sprite::onAttached(ITreeNode* /*node*/) {
  // nothing
}

void Sprite::onDetached() {
  // nothing
}

void Sprite::draw() {
  if (nullptr == mTexture || NULL == mProgram) {
    fLogF("tried to draw a non drawable");
  }
  mProgram->use();
  mProgram->uniformPush("uPosition", mPosition);
  mProgram->uniformPush("uScale", mScale);
  coreDraw();
}

void Sprite::coreDraw() {
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(mBlending.mSrc, mBlending.mSrc);
  glBlendEquation(mBlending.mBlendEquation);
  mTexture->bind(FILLWAVE_DIFFUSE_UNIT);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  mTexture->unbind();
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
}

} /* flf */
} /* flw */
