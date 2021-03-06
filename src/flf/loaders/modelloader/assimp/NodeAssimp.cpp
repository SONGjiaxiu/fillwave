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

#include <flw/flf/loaders/modelloader/assimp/AnimatorAssimp.h>

namespace flw {
namespace flf {

AnimatorAssimp::NodeAssimp::NodeAssimp(aiNode *node)
  : mName(node->mName.C_Str())
  , mTransformation(convert(node->mTransformation)) {
  // nothing
}

void AnimatorAssimp::NodeAssimp::update(float deltaTime, glm::mat4 parent, AnimatorAssimp* animator, GLint activeAnim) {
  std::string nodeName(mName);
  Animation* a = animator->getAnimation(activeAnim);
  if (nullptr == a) {
    return;
  }
  glm::mat4 transformation = mTransformation;
  Channel *channel = animator->findChannel(a, nodeName);

  if (channel) {
    glm::vec3 scaling = animator->getCurrentScale(deltaTime, channel);
    glm::mat4 scale = glm::scale(glm::mat4(1.0), scaling);

    glm::quat rotation = animator->getCurrentRotation(deltaTime, channel);
    glm::mat4 rotate = glm::mat4_cast(rotation);

    glm::vec3 translation = animator->getCurrentTranslation(deltaTime, channel);
    glm::mat4 translate = glm::translate(glm::mat4(1.0), translation);

    transformation = translate * rotate * scale;
  }

  glm::mat4 m = parent * transformation;

  if (mBone) {
    mBone->setGlobalOffsetMatrix(m * mBone->getOffsetMatrix());
  }

  for (auto &it : mChildren) {
    it->update(deltaTime, m, animator, activeAnim);
  }
}

AnimatorAssimp::NodeAssimp::~NodeAssimp() {
  for (auto &it : mChildren) {
    delete it;
  }
}

glm::mat4 AnimatorAssimp::NodeAssimp::convert(aiMatrix4x4 matrix) {
  glm::mat4 out;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      out[i][j] = matrix[j][i];
    }
  }
  return out;
}

glm::vec3 AnimatorAssimp::NodeAssimp::convert(aiVector3D vec) {
  return glm::vec3(vec.x, vec.y, vec.z);
}

glm::vec4 AnimatorAssimp::NodeAssimp::convert(aiColor4D vec) {
  return glm::vec4(vec.r, vec.g, vec.b, vec.a);
}

glm::quat AnimatorAssimp::NodeAssimp::convert(aiQuaternion vec) {
  return glm::quat(vec.w, vec.x, vec.y, vec.z);
}

} /* flf */
} /* flw */
