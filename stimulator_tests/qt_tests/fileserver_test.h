#ifndef STIMULATOR_CONTROL_FILESERVER_TEST_H
#define STIMULATOR_CONTROL_FILESERVER_TEST_H

#include <QtTest/QtTest>
#include "../../stimulator_fileserver/fileserver.h"
#include "qsignaltester.h"

class FileserverTest: public QObject
{
    Q_OBJECT
public:



private slots:
    void initTestCase();

    void helloTest();
    void byeTest();
    void mdTest();
    void putTest();
    void putLongerTest();
    void putMd5Test();
    void getTest();
    void getLongerTest();
    void delTest();
    void getPreviewTest();
    void startTest();
    void stopTest();
    void lsTest();

    void cleanupTestCase();
};



#endif //STIMULATOR_CONTROL_FILESERVER_TEST_H
