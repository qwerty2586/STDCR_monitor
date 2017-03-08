#include "gtest/gtest.h"
#include "../../stimulator_fileserver/fileserver.h"
#include "../../stimulator_fileserver/transfer_protocol.h"
#include "qtester.h"
#include <QObject>

Fileserver *fs;
QByteArray *buffer;
QTester *tester;

class HelloFixture : public  ::testing::Test {

 protected:
     void SetUp() {
         fs = new Fileserver("test_server","~");
         buffer = new QByteArray(62,(char)0);
         tester = new QTester;
         QObject::connect(fs,SIGNAL(outcomingMessage(QByteArray)),tester,SLOT(input(QByteArray)));
         QObject::connect(tester,SIGNAL(output(QByteArray)),fs,SLOT(incommingMessage(QByteArray)));
     }
     void TearDown() {
         delete fs;
         delete buffer;
         delete tester;
     }
 };

TEST(HelloFixture,test1) {
    EXPECT_EQ(1,1);
}

TEST_F(HelloFixture,test_hello) {
    tester->clear();
    QByteArray hello;
    hello.append(TYPE_REQUEST+OP_HELLO);
    hello.append((char)0x00);
    hello.append((char)0x00);
    hello.append("hello world!\0");
    hello.leftJustified(62,'\0');

    tester->emit_data(hello);
    QByteArray result = tester->input_buffer;
    EXPECT_EQ(result & SECTION_OP,OP_HELLO);
    EXPECT_EQ(result & SECTION_TYPE,TYPE_RESPONSE);

}