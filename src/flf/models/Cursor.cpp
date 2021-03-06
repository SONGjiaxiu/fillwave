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

#include <flw/flf/models/Cursor.h>
#include <flw/flf/loaders/ProgramLoader.h>

#include <flw/Fillwave.h>

namespace flw {
namespace flf {

Cursor::Cursor(Engine* engine, flc::Texture2D* texture)
  : Impostor(engine, FILLWAVE_ENDLESS, 0.06f, texture, ProgramLoader(engine).getProgram(EProgram::cursor))
  , mScreenFactor(static_cast<GLfloat>(engine->getScreenSize()[0]) / static_cast<GLfloat>(engine->getScreenSize()[1])) {
  initUniformsCache();
}

void Cursor::move(glm::vec2 position) {
  mProgram->use();
  flc::Uniform::push(mULCPosition, position);
  flc::Program::disusePrograms();
}

void Cursor::redraw() {
  mProgram->use();

  flc::Uniform::push(mULCScreenFactor, mScreenFactor);
  flc::Uniform::push(mULCSize, mSize);

  coreDraw();

  flc::Program::disusePrograms();
}

void Cursor::initUniformsCache() {
  mULCPosition = mProgram->getUniformLocation("uPosition");
  mULCScreenFactor = mProgram->getUniformLocation("uScreenFactor");
  mULCTextureUnit = mProgram->getUniformLocation("uTextureUnit");
  mULCSize = mProgram->getUniformLocation("uSize");

  mProgram->use();
  flc::Uniform::push(mULCTextureUnit, FILLWAVE_DIFFUSE_UNIT);
  flc::Program::disusePrograms();
}

} /* flf */
} /* flw */
