#ifndef STIMULATOR_CONTROL_FILESERVER_TEST_H
#define STIMULATOR_CONTROL_FILESERVER_TEST_H

#include <QtTest/QtTest>
#include "../../stimulator_fileserver/fileserver.h"
#include "qtester.h"

class FileserverTest: public QObject
{
    Q_OBJECT



private slots:
    void initTestCase();

    void helloTest();

    void mySecondTest()
    { QVERIFY(1 != 2); }
    void cleanupTestCase();
};



#endif //STIMULATOR_CONTROL_FILESERVER_TEST_H
