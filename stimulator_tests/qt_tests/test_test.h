#ifndef STIMULATOR_CONTROL_TEST_TEST_H
#define STIMULATOR_CONTROL_TEST_TEST_H

#include <QtTest/QtTest>

class TestTest: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    { qDebug("called before everything else"); }
    void myFirstTest()
    { QVERIFY(1 == 1); }
    void mySecondTest()
    { QVERIFY(1 != 2); }
    void cleanupTestCase()
    { qDebug("called after myFirstTest and mySecondTest"); }
};

#endif //STIMULATOR_CONTROL_TEST_TEST_H
