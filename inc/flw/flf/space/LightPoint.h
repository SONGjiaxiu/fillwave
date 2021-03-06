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

#include <flw/flc/extended/rendering/Texture3DRenderable.h>
#include <flw/cmn/scene/CameraPerspective.h>
#include <flw/cmn/scene/Light.h>
#include <flw/flf/models/shapes/Sphere.h>
#include <map>

namespace flw {
namespace flf {

/*! \class LightPoint
 * \brief Not used.
 */

class LightPoint : public Light {
public:
  LightPoint(flc::Texture3DRenderable* tex, glm::vec3 pos, glm::vec4 intensity, Moveable* observed = nullptr);

  ~LightPoint() override;

  flc::Texture3DRenderable* getShadowTexture();

  CameraPerspective* getShadowCamera(GLenum id);

  void updateShadowCamera();

protected:
  flc::Texture3DRenderable* mShadowTexture;
  std::map<GLenum, pu<CameraPerspective>> mFaceCameras;

private:
  Sphere mSphere;
};

} /* flf */
} /* flw */
