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

#include <fillwave/models/animations/AnimatorAssimp.h>
#include <fillwave/Log.h>

FLOGINIT("Bone", FERROR | FFATAL)

namespace flw {
namespace flf {

Bone::Bone(const std::string& name, const glm::mat4& offsetMatrix, const glm::mat4& globalOffsetMatrix)
  : mName(name)
  , mOffsetMatrix(offsetMatrix)
  , mGlobalOffsetMatrix(globalOffsetMatrix) {
 // nothing
}

void Bone::setName(std::string name) {
  mName = name;
}

std::string Bone::getName() const {
  return mName;
}

glm::mat4 Bone::getOffsetMatrix() const {
  return mOffsetMatrix;
}

glm::mat4 Bone::getGlobalOffsetMatrix() const {
  return mGlobalOffsetMatrix;
}

void Bone::setOffsetMatrix(glm::mat4 m) {
  mOffsetMatrix = m;
}

void Bone::setGlobalOffsetMatrix(glm::mat4 m) {
  mGlobalOffsetMatrix = m;
}

void Bone::log() const {
  fLogI("Name: %s", (mName.c_str()));
}

} /* flf */
} /* flw */
