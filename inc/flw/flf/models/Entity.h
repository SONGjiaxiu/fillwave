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

#include <flw/cmn/IPickable.h>
#include <flw/cmn/scene/ICamera.h>
#include <flw/flc/renderers/IRenderable.h>
#include <flw/cmn/scene/Moveable.h>
#include <flw/flf/models/base/TreePtr.h>

namespace flw {
namespace flf {
class Entity;
class EventHandler;
class Event;
enum class EEventType;
}

namespace flc {
class Program;
}

namespace flf {

/*! \class Entity
 * \brief Base for all Scene nodes.
 */

class Entity : public flc::IRenderable,  public IPickable, public Moveable, public TreePtr<Entity> {
public:
  Entity();

  ~Entity() override;

  Entity& operator=(Entity &&);

  Entity(Entity &&obj);

  /* Flags */
  bool isPSC();

  bool isPSR();

  /* Callbacks */
  void handleEvent(const Event& event);

  /* Model */
  glm::mat4 getPhysicsMMC();

  /* Physics */
  void setTransformation(glm::mat4 modelMatrix);

  /* Callbacks */
  void attachHandler(std::function<void(const Event&)>&& h, EEventType eventType);

  void detachHandlers();

  /* Parent */
  void updateMatrixTree();

  void updateParentMatrix(glm::mat4 &parent);

  void updateParentRotation(glm::quat &rotation);

  /* IPickable */
  void assignColor(const glm::vec3& color) override;

  void unpick() override;

  void stepInTime(float timeSinceLastFrameInSeconds);

  virtual void onPicked() override;

  virtual void onUnpicked() override;

  /* IDrawable */
  void drawFR(ICamera &camera) override;
  void drawPBRP(ICamera &camera) override;
  void drawDR(ICamera &camera) override;
  void drawDepth(ICamera &camera) override;
  void drawDepthColor(ICamera &camera, glm::vec3 &position) override;
  void drawAOG(ICamera &camera) override;
  void drawAOC(ICamera &camera) override;
  void drawOcclusionBox(ICamera &camera) override;
  void drawPicking(ICamera &camera) override;

  /* IRenderable */
  void updateRenderer(flc::IRenderer &renderer) override;
  bool getRenderItem(flc::RenderItem &item) override;

  /* Animations */
  virtual bool isAnimated() const;

  /* Log */
  virtual void log() const;

protected:
  /* MMC - Model Matrix Cache */
  glm::mat4 mPhysicsMMC;

  bool mChildrenPropagateEvent;
  bool mParentRefresh;
  vec<EventHandler> mEventHandlers;

  bool mPSC;
  bool mPSR;
};

} /* flf */
} /* flw */
