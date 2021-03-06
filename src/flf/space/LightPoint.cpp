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

#include <flw/flf/space/LightPoint.h>
#include <flw/Log.h>

FLOGINIT("LightPoint", FERROR | FFATAL)

namespace flw {
namespace flf {

LightPoint::LightPoint(flc::Texture3DRenderable* tex, glm::vec3 pos, glm::vec4 intensity, Moveable* observed)
    : Light(pos, intensity, observed)
    , mShadowTexture(tex)
    , mSphere(1.0f, 10, 10) {

  const glm::vec3 axisX(1.0f, 0.0f, 0.0f);
  const glm::vec3 axisY(0.0f, 1.0f, 0.0f);
  const glm::vec3 axisZ(0.0f, 0.0f, 1.0f);
  mFaceCameras[GL_TEXTURE_CUBE_MAP_POSITIVE_X] = std::make_unique<CameraPerspective>(
    pos
    , glm::normalize(glm::angleAxis(glm::radians(90.0f), axisY) * glm::angleAxis(glm::radians(180.0f), axisX))
    , glm::radians(90.0f), 1.0f, 0.1f, 1000.0f);
  mFaceCameras[GL_TEXTURE_CUBE_MAP_NEGATIVE_X] = std::make_unique<CameraPerspective>(
    pos
    , glm::normalize(glm::angleAxis(glm::radians(-90.0f), axisY) * glm::angleAxis(glm::radians(180.0f), axisX))
    , glm::radians(90.0f), 1.0f, 0.1f, 1000.0f);
  mFaceCameras[GL_TEXTURE_CUBE_MAP_POSITIVE_Y] = std::make_unique<CameraPerspective>(
    pos
    , glm::normalize(glm::angleAxis(glm::radians(90.0f), axisX))
    , glm::radians(90.0f), 1.0f, 0.1f, 1000.0f);
  mFaceCameras[GL_TEXTURE_CUBE_MAP_NEGATIVE_Y] = std::make_unique<CameraPerspective>(
    pos
    , glm::normalize(glm::angleAxis(glm::radians(-90.0f), axisX))
    , glm::radians(90.0f), 1.0f, 0.1f, 1000.0f);
  mFaceCameras[GL_TEXTURE_CUBE_MAP_POSITIVE_Z] = std::make_unique<CameraPerspective>(
    pos
    , glm::normalize(glm::angleAxis(glm::radians(180.0f), axisZ))
    , glm::radians(90.0f), 1.0f, 0.1f, 1000.0f);
  mFaceCameras[GL_TEXTURE_CUBE_MAP_NEGATIVE_Z] = std::make_unique<CameraPerspective>(
    pos
    , glm::normalize(glm::angleAxis(glm::radians(180.0f), axisY) * glm::angleAxis(glm::radians(180.0f), axisZ))
    , glm::radians(90.0f), 1.0f, 0.1f, 1000.0f);
}

LightPoint::~LightPoint() = default;

flc::Texture3DRenderable* LightPoint::getShadowTexture() {
  return mShadowTexture;
}

CameraPerspective* LightPoint::getShadowCamera(GLenum id) {
  if (mFaceCameras.count(id) == 1) {
    return mFaceCameras[id].get();
  }
  fLogF("Can not get shadow camera. This should never happen. Good bye ...");
  return nullptr;
}

void LightPoint::updateShadowCamera() {
  if (mObserved) {
    mTranslation = mObserved->getTranslation();
  }
  for (auto &it : mFaceCameras) {
    if (it.second->getTranslation() != mTranslation) {
      it.second->moveTo(mTranslation);
      it.second->update();
    }
  }
}

} /* flf */
} /* flw */
