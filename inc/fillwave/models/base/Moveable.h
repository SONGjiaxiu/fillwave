#pragma once

/*
* Copyright (c) 2017, Fillwave developers
* All rights reserved.
*
* Fillwave C++14 graphics engine.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright notice, this
*   list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright notice,
*   this list of conditions and the following disclaimer in the documentation
*   and/or other materials provided with the distribution.
*
*   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
*   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
*   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
*   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
*   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
*   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
*   OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <fillwave/OpenGL.h>
#include <fillwave/Math.h>
#include <fillwave/common/Observable.h>
#include <fillwave/models/base/Moveable.h>
#include <fillwave/common/Easing.h>
#include <fillwave/common/Allocator.h>
#include <memory>
#include <vector>
#include <functional>
#include <algorithm>

namespace flw {
namespace flf {

/*! \class Moveable
 * \brief Base for every object which has a 3D position.
 */

class Moveable : public Observable {
public:
  Moveable(
    glm::vec3 translation = glm::vec3(0.0), glm::quat r = glm::quat(1.0, 0.0, 0.0, 0.0), unsigned int callbacks = 1);

  ~Moveable() override = default;

  Moveable(const Moveable& arg) = default;

  void moveTo(glm::vec3 coordinates);

  void moveToX(GLfloat distance);

  void moveToY(GLfloat distance);

  void moveToZ(GLfloat distance);

  void moveBy(glm::vec3 coordinates);

  void moveByX(GLfloat distance);

  void moveByY(GLfloat distance);

  void moveByZ(GLfloat distance);

  void moveInDirection(glm::vec3 direction);

  glm::vec3 getTranslation();

  void scaleTo(GLfloat scale);

  void scaleTo(glm::vec3 scale);

  void scaleToX(GLfloat scale);

  void scaleToY(GLfloat scale);

  void scaleToZ(GLfloat scale);

  glm::vec3 getScale();

  void rotateTo(glm::quat rotation);

  void rotateTo(const glm::vec3 &axis, GLfloat angle);

  void rotateBy(const glm::vec3 &axis, GLfloat angle);

  void rotateByX(float angle);

  void rotateByY(float angle);

  void rotateByZ(float angle);

  glm::quat getRotation();

  void updateMatrixCache();

  bool isRefresh() const;

  void setRefresh(bool state);

  /* Parent */
  glm::mat4 getParentMMC() const;

  glm::quat getParentRotation() const;

  void waitInTime(float deltaTime);

  void moveBy(float deltaTime, const glm::vec3& deltaMove, Callback<float(float)> ease = LinearInterpolation);

  void moveTo(float durationInSeconds, const glm::vec3& endTranslation, Callback<float(float)> ease);

  void scaleBy(float deltaTime, const glm::vec3& aScale, Callback<float(float)> ease = LinearInterpolation);

  void scaleTo(float deltaTime, const glm::vec3& aScale, Callback<float(float)> ease = LinearInterpolation);

  void rotateBy(float deltaTime, const float aAngle, const glm::vec3& aAxis, Callback<float(float)> ease = LinearInterpolation);

  void rotateTo(float deltaTime, const float aAngle, const glm::vec3& aAxis, Callback<float(float)> ease = LinearInterpolation);

  void loop(int loops);

  void stop();

  bool isMoving();

  float stepInTime(float delta);

  void attachTimeCallback(float deltaTime, Callback<float(float)> aAction);

  template <typename ...ARGS>
  void attachTimeCallback(float deltaTime, Callback<float(float, ARGS...)> aAction, ARGS&&... args) {
    mTimeCallbacks.push_back([this, deltaTime, aAction, args...](float aDeltaTime) {
      if (mCallbackTimePassed == 0.0f) {
        aAction(0.0f, std::forward<ARGS...>(args...));
      }
      mCallbackTimePassed += aDeltaTime;
      aAction(mCallbackTimePassed/deltaTime >= 1.0f ? 1.0f : mCallbackTimePassed/deltaTime, args...);
      if (mCallbackTimePassed < deltaTime) {
        return 0.0f;
      }
      float timeLeft = mCallbackTimePassed - deltaTime;
      mCallbackTimePassed = 0;
      return timeLeft;
    });
  }

protected:
  glm::fvec3 mTranslation;
  glm::quat mRotation;
  glm::vec3 mScale;

  /* Parent */
  glm::quat mParentRotation;

  /* MMC - Model Matrix Cache */
  glm::mat4 mMMC;
  glm::mat4 mParentMMC;

  /* Refresh flag */
  bool mRefresh;

  float mCallbackTimePassed;

 private:
  unsigned int mCurrentCallbackIdx;
  unsigned int mCallbackLoops;
  vec<Callback<float(float)>> mTimeCallbacks;

  struct {
    glm::vec3 mTranslation;
    glm::vec3 mScale;
    glm::quat mRotation;
  } mBase;
};

template <class M>
bool isMoveablesRefresh(std::vector<M> &moveables) { //xxx to be removed
  for (auto &it : moveables) {
    if (it->isRefresh()) {
      return true;
    }
  }
  return false;
}

template <class M>
void resetMoveablesRefresh(std::vector<M> &data) {
  for (auto &it : data) {
    it->setRefresh(false);
  }
}

GLboolean isMoveablesRefresh(std::vector<std::shared_ptr<Moveable>> &moveables);

void resetMoveablesRefresh(std::vector<std::shared_ptr<Moveable>> &data);

} /* flf */
} /* flw */
