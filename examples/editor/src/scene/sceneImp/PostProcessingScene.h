#pragma once

#include <fillwave/Fillwave.h>
#include "scene/AScene.h"

namespace scene {
class PostProcessingScene : public AScene {
  Q_OBJECT
  Q_PROPERTY(QMap sceneParameter READ getParameters WRITE setParameters)
public:
  PostProcessingScene(int argc, char **argv, QMap<QString, QVariant> varValues);

  virtual ~PostProcessingScene(){}

  void init() override;

  void perform() override;

private:
  flw::ps<flw::flf::Text> mText;
  int mArgc;
  char **mArgv;
};

}
