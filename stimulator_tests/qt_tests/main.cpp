#include <QApplication>
#include <test_test.h>
#include <fileserver_test.h>

int main(int argc, char **argv) {


//    TestTest *testTest = new TestTest();
//    QTest::qExec(testTest);

    FileserverTest *fileserverTest = new FileserverTest();
    QTest::qExec(fileserverTest);



}
