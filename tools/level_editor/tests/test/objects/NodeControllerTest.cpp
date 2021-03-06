#include "NodeControllerFixture.h"

TEST_F(NodeControllerFixture, insertingRow) {
  EXPECT_CALL(*mTreeModelMock, insertToModel(testing::_, testing::_)).WillOnce(testing::Return(true));
  auto addResult = sut->addNode(common::EItemType::object, "Test");
  EXPECT_EQ(addResult, true);
}