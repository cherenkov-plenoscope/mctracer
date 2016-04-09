#include "gtest/gtest.h"
#include "Xml/Factory/SceneryFactory.h"
//#include "Cameras/FlyingCamera.h"
using std::cout;
using std::string;

class SceneryFactoryTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(SceneryFactoryTest, call_SceneryFactory) {

    Xml::SceneryFactory xml2frame("test_scenery/tree_epoch_20160320.xml");
    EXPECT_EQ(xml2frame.scenery->get_name(), "scenery");

    //TracerSettings settings;
    //FlyingCamera free(xml2frame.scenery, &settings);
}
//------------------------------------------------------------------------------
TEST_F(SceneryFactoryTest, author_and_comment) {

    Xml::SceneryFactory xml2frame("test_scenery/tree_epoch_20160320.xml");
    EXPECT_EQ(xml2frame.author, "Sebastian");
    EXPECT_EQ(xml2frame.comment, "First novel xml format.");
}
//------------------------------------------------------------------------------
TEST_F(SceneryFactoryTest, no_author_and_no_comment) {

    Xml::SceneryFactory xml2frame("test_scenery/tree_epoch_20160320_no_author_no_comment.xml");
    EXPECT_TRUE(xml2frame.author.empty());
    EXPECT_TRUE(xml2frame.comment.empty());
}
//------------------------------------------------------------------------------