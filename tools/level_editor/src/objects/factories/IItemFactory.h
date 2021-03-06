#pragma once

#include "common/InternalTypes.h"
#include "objects/items/BaseItem.h"

namespace objects {
class IItemFactory {
 public:

  virtual ~IItemFactory() = default;

  virtual BaseItem*
  create(common::EItemType, QString& name, objects::BaseItem* parent, std::shared_ptr<flw::Engine> engine) = 0;
};

}
