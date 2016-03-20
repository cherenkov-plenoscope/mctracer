#include "gtest/gtest.h"
#include "Xml/Factory/SceneryFactory.h"
using std::cout;
using std::string;

class SceneryFactoryTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(SceneryFactoryTest, call_SceneryFactory) {

    Xml::SceneryFactory xml2frame("test_scenery/tree_epoch_20160320.xml");
    EXPECT_EQ(xml2frame.scenery->get_name(), "scenery");
}