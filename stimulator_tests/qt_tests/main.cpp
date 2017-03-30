#include <test_test.h>
#include <fileserver_test.h>
#include <QLabel>

int main(int argc, char **argv) {

    QApplication app(argc, argv);

    FileserverTest *fileserverTest = new FileserverTest();

    QTest::qExec(fileserverTest);




}
