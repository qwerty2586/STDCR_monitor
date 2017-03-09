#include "fileserver_test.h"
#include "../../stimulator_fileserver/transfer_protocol.h"

Fileserver *fs;
QByteArray *buffer;
QTester *tester;

void FileserverTest::initTestCase() {
    fs = new Fileserver("test_server", "~");
    QVERIFY(fs != nullptr);
    buffer = new QByteArray(62, (char) 0);
    tester = new QTester;
    QVERIFY(tester != nullptr);
    QObject::connect(fs, SIGNAL(outcomingMessage(QByteArray)), tester, SLOT(input(QByteArray)));
    QObject::connect(tester, SIGNAL(output(QByteArray)), fs, SLOT(incomingMessage(QByteArray)));


}

void FileserverTest::helloTest() {
    tester->clear();
    QByteArray hello;
    hello.append(TYPE_REQUEST + OP_HELLO + PART_LAST);
    hello.append((char) 0x01);
    hello.append((char) 0x00); // verze protokolu
    hello.append("hello world!\0");
    hello.append(QByteArray().fill('\0', 62 - hello.size()));

    tester->emit_data(hello);
    QByteArray result = tester->input_buffer;
    QVERIFY(result.size() == 62);
    QCOMPARE(result[INDEX_COMMAND - PREFIX] & SECTION_OP, OP_HELLO);
    QCOMPARE(result[INDEX_COMMAND - PREFIX] & SECTION_TYPE, TYPE_RESPONSE);
    QCOMPARE((int)result[INDEX_ITER - PREFIX] , 1); // iterator ma bejt 1
}

void FileserverTest::byeTest() {
    tester->clear();

    QByteArray bye;
    bye.append(TYPE_REQUEST + OP_BYE + PART_CONTINUE); // nejdriv mu dame do bufferu nedokoncenou zpravu
    bye.append((char) 0x02);
    bye.append(QByteArray().fill('\0', 62 - bye.size()));
    tester->emit_data(bye);
    fs->unfinished_requests.size();
    QVERIFY(fs->unfinished_requests.size() == 1);



    QByteArray bye2;
    bye2.append(TYPE_REQUEST + OP_BYE + PART_LAST);
    bye2.append((char) 0x03);
    bye2.append(QByteArray().fill('\0', 62 - bye.size()));
    tester->emit_data(bye2);

    fs->unfinished_requests.size();
    QVERIFY(fs->unfinished_requests.size() == 0);
}


void FileserverTest::cleanupTestCase() {
    delete fs;
    delete buffer;
    delete tester;
}


