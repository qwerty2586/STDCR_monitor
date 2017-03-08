#include "gtest/gtest.h"
#include "../../stimulator_fileserver/fileserver.h"
#include <QString>
 class HelloFixture : public  ::testing::Test {
    Fileserver *fs;
 protected:
     void SetUp() {
         fs = new Fileserver("test_server","~");
     }


 };

TEST(HelloFixture,test1) {
    EXPECT_EQ(1,1);
}