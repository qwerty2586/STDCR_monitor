#include <QLabel>
#include <QHBoxLayout>
#include <qstringbuilder.h>
#include "fileserver_test.h"
#include "../../stimulator_fileserver/transfer_protocol.h"

Fileserver *fs;
QByteArray *buffer;
QSignalTester *tester;

#define DIR_PREFIX "./qtest/"

void FileserverTest::initTestCase() {
    fs = new Fileserver("test_server", DIR_PREFIX);
    QVERIFY(fs != nullptr);
    buffer = new QByteArray(62, (char) 0);
    tester = new QSignalTester;
    QVERIFY(tester != nullptr);
    QObject::connect(fs, SIGNAL(outcomingMessage(QByteArray)), tester, SLOT(input(QByteArray)));
    QObject::connect(tester, SIGNAL(output(QByteArray)), fs, SLOT(incomingMessage(QByteArray)));

    QWidget * widget = new QWidget();
    QLabel *label = new QLabel("Ahoj");
    label->setFixedSize(200,50);
    QHBoxLayout *layout = new QHBoxLayout();
    widget->setLayout(layout);
    layout->addWidget(label);
    widget->show();

}

void FileserverTest::helloTest() {
    tester->clear();
    QByteArray hello;
    hello.append(TYPE_REQUEST + OP_HELLO + PART_LAST);
    hello.append((char) 0x01);
    hello.append((char) 0x00); // verze protokolu
    hello.append("hello world!\0"); //zprava zakoncena nulou
    hello.append(QByteArray().fill('\0', 62 - hello.size())); // doplnime na 62

    tester->emit_data(hello);
    QByteArray result = tester->input_buffer[0];
    QVERIFY(result.size() == 62);
    QCOMPARE(result[INDEX_COMMAND - PREFIX] & SECTION_OP, OP_HELLO);
    QCOMPARE(result[INDEX_COMMAND - PREFIX] & SECTION_TYPE, TYPE_RESPONSE);
    QCOMPARE((int) result[INDEX_ITER - PREFIX], 1); // iterator ma bejt 1
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


void FileserverTest::mdTest() {
    tester->clear();

    QString path = QString(DIR_PREFIX) % "testdir";

    {
        QDir dir(path);
        if (dir.exists()) {
            QDir().rmdir(path);
        }
    }

    QByteArray md;
    md.append(TYPE_REQUEST + OP_MD + PART_LAST); // nejdriv mu dame do bufferu nedokoncenou zpravu
    md.append((char) 0x10); //iterator
    md.append("~/testdir\0");
    md.append(QByteArray().fill('\0', 62 - md.size()));
    tester->emit_data(md);

    QVERIFY(tester->input_buffer[0][INDEX_RESPONSE - PREFIX] == (char) RESPONSE_OK);

    QDir dir(path);
    QVERIFY(dir.exists());
    tester->clear();
    tester->emit_data(md);

    QVERIFY(tester->input_buffer[0][INDEX_RESPONSE - PREFIX] == (char) RESPONSE_MD_DIR_EXIST);
}


void FileserverTest::putTest() {
    tester->clear();

    const QString FILENAME = "hello.txt";

    {
        QFile hello_file(FILENAME);
        if (hello_file.exists()) {
            hello_file.remove();
            QVERIFY(!hello_file.exists());
        }
    }

    QByteArray bytes = "hello world!";

    QByteArray hello;
    hello.append(TYPE_REQUEST + OP_PUT + PART_LAST); // nejdriv mu dame do bufferu nedokoncenou zpravu
    hello.append((char) 0x11); //iterator
    hello.append((char) 0x00);          //
    hello.append((char) 0x00);          //
    hello.append((char) 0x00);          //
    hello.append((char) bytes.size());  // < -- delka souboru
    hello.append(QCryptographicHash::hash(bytes, QCryptographicHash::Md5));
    hello.append("~/");
    hello.append(FILENAME);
    hello.append((char) '\0');
    QVERIFY(hello.size() == (2 + 4 + 16 + 3 + FILENAME.size()));
    hello.append(QByteArray().fill('\0', 62 - hello.size())); //prodlouzime na 62 bajtu
    tester->emit_data(hello);
    QByteArray data;
    data.append(TYPE_UPLOAD + PART_LAST);
    data.append((char) 0x11); // interator
    data.append(bytes); // datove bajty
    data.append(QByteArray().fill('\0', 62 - data.size()));
    tester->emit_data(data);

    QFile hello_file(DIR_PREFIX+FILENAME);
    if (hello_file.exists()) {
        QVERIFY(hello_file.size() == bytes.size());
        QVERIFY(tester->input_buffer.last()[INDEX_RESPONSE - PREFIX] == (char) RESPONSE_OK);
    } else
        QFAIL("File not exist");
}

void FileserverTest::putMd5Test() {
    tester->clear();

    const QString FILENAME = "hellosha.txt";

    {
        QFile hello_file(FILENAME);
        if (hello_file.exists()) {
            hello_file.remove();
        }
    }

    QByteArray bytes = "hello world!";

    QByteArray hello;
    hello.append(TYPE_REQUEST + OP_PUT + PART_LAST); // nejdriv mu dame do bufferu nedokoncenou zpravu
    hello.append((char) 0x11);
    hello.append((char) 0x00);          //
    hello.append((char) 0x00);          //
    hello.append((char) 0x00);          //
    hello.append((char) bytes.size());  // < -- delka souboru
    hello.append(QCryptographicHash::hash(bytes, QCryptographicHash::Md5));
    hello.append("~/");
    hello.append(FILENAME);
    hello.append((char) '\0');
    QVERIFY(hello.size() == (2 + 4 + 16 + 3 + FILENAME.size()));
    hello.append(QByteArray().fill('\0', 62 - hello.size()));
    tester->emit_data(hello);
    QByteArray data;
    data.append((char) (TYPE_UPLOAD + PART_LAST));
    data.append((char) 0x11); // iterator
    data.append(QByteArray("hello world?")); // < -- zmeneno
    data.append(QByteArray().fill('\0', 62 - data.size()));
    tester->emit_data(data);
    QVERIFY(tester->input_buffer.last()[INDEX_RESPONSE - PREFIX] == (char) RESPONSE_PUT_MD5_FAIL);
}

void FileserverTest::putLongerTest() {
    tester->clear();

    const QString FILENAME = "hellolong.txt";

    {
        QFile hello_file(FILENAME);
        if (hello_file.exists()) {
            hello_file.remove();
            QVERIFY(!hello_file.exists());
        }
    }

    QByteArray bytes(300, '!'); // soubor s 300 !

    QByteArray hello;
    hello.append(TYPE_REQUEST + OP_PUT + PART_LAST); // nejdriv mu dame do bufferu nedokoncenou zpravu
    hello.append((char) 0x11); // iterator
    hello.append((char) 0x00);                      //
    hello.append((char) 0x00);                      //
    hello.append((char) (bytes.size() / 256));      //
    hello.append((char) bytes.size());              // < -- delka souboru
    hello.append(QCryptographicHash::hash(bytes, QCryptographicHash::Md5));
    hello.append("~/");
    hello.append(FILENAME);
    hello.append((char) '\0');
    QVERIFY(hello.size() == (2 + 4 + 16 + 3 + FILENAME.size()));
    hello.append(QByteArray().fill('\0', 62 - hello.size()));
    tester->emit_data(hello);
    QByteArray head;
    head.append(TYPE_UPLOAD).append(0x11);
    int i;
    for (i = 0; i < (bytes.size() / 60) - 1; ++i) {
        tester->emit_data(bytes.mid(i * 60, 60).prepend(head));

    }
    head[0] = head[0] + PART_LAST;
    head.append(bytes.mid(i * 60, 60));
    tester->emit_data(head);

    QFile hello_file(DIR_PREFIX+FILENAME);
    if (hello_file.exists()) {
        QVERIFY(hello_file.size() == bytes.size());
        QVERIFY(tester->input_buffer.last()[INDEX_RESPONSE - PREFIX] == (char) RESPONSE_OK);
    } else
        QFAIL("File not exist");
}

void FileserverTest::getTest() {
    tester->clear();
    QByteArray bytes = "hello world!";
    const QString FILENAME = "hello.txt";
    {
        QFile hello_file(DIR_PREFIX+FILENAME);
        if (!hello_file.exists()) {
            QSKIP("Run putTest() first!", SkipSingle);
        }
    }

    QByteArray hello;
    hello.append(TYPE_REQUEST + OP_GET + PART_LAST); // nejdriv mu dame do bufferu nedokoncenou zpravu
    hello.append((char) 0xff); // testnem unsigned iterator
    hello.append("~/");
    hello.append(FILENAME);
    hello.append((char) '\0');
    QVERIFY(hello.size() == (2 + 3 + FILENAME.size()));
    hello.append(QByteArray().fill('\0', 62 - hello.size()));
    tester->emit_data(hello);
    QByteArray response = tester->input_buffer[0];
    QVERIFY(response[0] == (char) (TYPE_RESPONSE + OP_GET + PART_LAST));
    QVERIFY(response[1] == (char) (0xff)); //iterator
    QVERIFY((char) response[2] == RESPONSE_OK);
    QByteArray response_data = tester->input_buffer[1];
    QVERIFY(response[6] == (char) bytes.size());
    QByteArray bytes2 = response_data.mid(2, bytes.size());
    QVERIFY(QCryptographicHash::hash(bytes2, QCryptographicHash::Md5) == response.mid(7, 16));
}

void FileserverTest::getLongerTest() {
    tester->clear();
    const QString FILENAME = "hellolong.txt";
    {
        QFile hello_file(DIR_PREFIX+FILENAME);
        if (!hello_file.exists()) {
            QSKIP("Run putLongerTest() first!", SkipSingle);
        }
    }
    QByteArray hello;
    hello.append(TYPE_REQUEST + OP_GET + PART_LAST); // nejdriv mu dame do bufferu nedokoncenou zpravu
    hello.append((uchar) 0xff); // testnem unsigned
    hello.append("~/");
    hello.append(FILENAME);
    hello.append((char) '\0');
    QVERIFY(hello.size() == (2 + 3 + FILENAME.size()));
    hello.append(QByteArray().fill('\0', 62 - hello.size()));
    tester->emit_data(hello);
    QByteArray response = tester->input_buffer[0];
    QVERIFY(response[0] == (char) (TYPE_RESPONSE + OP_GET + PART_LAST));
    QVERIFY(response[1] == (char) (0xff));
    QVERIFY((char) response[2] == RESPONSE_OK);
    QByteArray response_data;
    for (int i = 1; i < tester->input_buffer.size(); ++i) {
        response_data.append(tester->input_buffer[i].mid(2, 60));
    }
    int size = (uchar) response[6] + (256) * (uchar) response[5];
    response_data.truncate(size);
    QVERIFY(QCryptographicHash::hash(response_data, QCryptographicHash::Md5) == response.mid(7, 16));
}

void FileserverTest::delTest() {
    tester->clear();
    const QString FILENAME = "deltest.txt";
    {
        QFile del_file(DIR_PREFIX+FILENAME);
        if (!del_file.exists()) {
            del_file.open(QIODevice::WriteOnly);
            del_file.write("del test");
            del_file.close();
            if (!del_file.exists()) QSKIP("cant create testing file", SkipSingle);
        }
    }

    QByteArray hello;
    hello.append(TYPE_REQUEST + OP_DEL + PART_LAST); // nejdriv mu dame do bufferu nedokoncenou zpravu
    hello.append((uchar) 0xcc); // iterator
    hello.append("~/");
    hello.append(FILENAME);
    hello.append((char) '\0');
    QVERIFY(hello.size() == (2 + 3 + FILENAME.size()));
    hello.append(QByteArray().fill('\0', 62 - hello.size()));
    tester->emit_data(hello);

    QByteArray response = tester->input_buffer[0];
    QVERIFY(response[0] == (char) (TYPE_RESPONSE + OP_DEL + PART_LAST));
    QVERIFY(response[1] == (char) (0xcc)); // iterator
    QVERIFY((char) response[2] == (char) RESPONSE_OK);
    QFile del_file(DIR_PREFIX+FILENAME);
    QVERIFY(!del_file.exists());
}

void FileserverTest::getPreviewTest() {
    tester->clear();
    const QString FILENAME1 = "preview_test_original.jpg";
    const QString FILENAME2 = "preview_test_response.jpg";
    {
        QFile prev_file(DIR_PREFIX+FILENAME1);
        if (!prev_file.exists()) {
            QSKIP(QString("please prepare ").append(FILENAME1).append(
                    " file to test previewing!").toStdString().c_str(), SkipSingle);
        }
    }
    QByteArray hello;
    hello.append(TYPE_REQUEST + OP_GET_PREVIEW + PART_LAST); // nejdriv mu dame do bufferu nedokoncenou zpravu
    hello.append((uchar) 0xbb);
    hello.append("~/");
    hello.append(FILENAME1);
    hello.append((char) '\0');
    QVERIFY(hello.size() == (2 + 3 + FILENAME1.size()));
    hello.append(QByteArray().fill('\0', 62 - hello.size()));
    tester->emit_data(hello);

    QByteArray response = tester->input_buffer[0];
    QVERIFY(response[0] == (char) (TYPE_RESPONSE + OP_GET_PREVIEW + PART_LAST));
    QVERIFY(response[1] == (char) (0xbb));
    QVERIFY((char) response[2] == RESPONSE_OK);
    QByteArray response_data;
    for (int i = 1; i < tester->input_buffer.size(); ++i) {
        response_data.append(tester->input_buffer[i].mid(2, 60));
    }
    int size = (uchar) response[6] + (256) * (uchar) response[5] + (256) * (256) * (uchar) response[4];
    response_data.truncate(size);
    QVERIFY(QCryptographicHash::hash(response_data, QCryptographicHash::Md5) == response.mid(7, 16));
    QFile preview(DIR_PREFIX+FILENAME2);
    preview.open(QIODevice::WriteOnly);
    preview.write(response_data);
    preview.close();
    qDebug(QString("check ").append(FILENAME2).append(" contents if preview of original ").append(
            FILENAME1).toStdString().c_str());
}


void FileserverTest::startTest() {
    tester->clear();
    const QString FILENAME = "dummy_config.xml";

    QSignalSpy spy(fs, SIGNAL(startSdlOutput(QString)));

    QByteArray hello;
    hello.append(TYPE_REQUEST + OP_START + PART_LAST); // nejdriv mu dame do bufferu nedokoncenou zpravu
    hello.append((uchar) 0xaa);
    hello.append("~/");
    hello.append(FILENAME);
    hello.append((char) '\0');
    QVERIFY(hello.size() == (2 + 3 + FILENAME.size()));
    hello.append(QByteArray().fill('\0', 62 - hello.size()));
    tester->emit_data(hello);

    QByteArray response = tester->input_buffer[0];
    QVERIFY(response[0] == (char) (TYPE_RESPONSE + OP_START + PART_LAST));
    QVERIFY(response[1] == (char) (0xaa));
    QVERIFY((char) response[2] == RESPONSE_OK);

    QCOMPARE(spy.count(), 1); // tohle testuje jestli vazne neco zapnul
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toString().contains(FILENAME, Qt::CaseSensitive) == true);
}


void FileserverTest::stopTest() {
    tester->clear();

    QSignalSpy spy(fs, SIGNAL(stopSdlOutput()));

    QByteArray hello;
    hello.append(TYPE_REQUEST + OP_STOP + PART_LAST); // nejdriv mu dame do bufferu nedokoncenou zpravu
    hello.append((uchar) 0xaa);
    QVERIFY(hello.size() == 2);
    hello.append(QByteArray().fill('\0', 62 - hello.size()));
    tester->emit_data(hello);

    QByteArray response = tester->input_buffer[0];
    QVERIFY(response[0] == (char) (TYPE_RESPONSE + OP_STOP + PART_LAST));
    QVERIFY(response[1] == (char) (0xaa));
    QVERIFY((char) response[2] == RESPONSE_OK);

    QCOMPARE(spy.count(), 1);
}

void FileserverTest::lsTest() {
    tester->clear();
    QByteArray hello;
    QString MASK = "*.jpg;*.gif;*.png";
    hello.append((char)(TYPE_REQUEST + OP_LS + PART_LAST)); // nejdriv mu dame do bufferu nedokoncenou zpravu
    hello.append((char) 0x35); // iterator
    hello.append((char) 0x00); //defaultni flagy ,kdyby 1 tak je to s adresarema
    hello.append("~/");
    hello.append((char)0);//vypis domovskej adresar
    hello.append(MASK);
    hello.append((char)0); //maska
    QVERIFY(hello.size() == 2 + 1 + 3 + MASK.size() + 1);
    hello.append(QByteArray().fill('\0', 62 - hello.size())); //doplnit na 62 bajtu
    tester->emit_data(hello);

    QByteArray response = tester->input_buffer[0];
    QVERIFY(response[0] == (char) (TYPE_RESPONSE + OP_LS + PART_LAST));
    QVERIFY(response[1] == (char) (0x35)); //iterator
    QVERIFY((char) response[2] == RESPONSE_OK);


    QByteArray response_data;
    for (int i = 1; i < tester->input_buffer.size(); ++i) {
        response_data.append(tester->input_buffer[i].mid(2, 60));
    }
    int size = (uchar) response[6] + (256) * (uchar) response[5] + (256) * (256) * (uchar) response[4];
    response_data.truncate(size);
    QVERIFY(QCryptographicHash::hash(response_data, QCryptographicHash::Md5) == response.mid(7, 16));
    int count = (uchar)response_data[0] * (256) + (uchar)response_data[1];

    QList<int> sizes;
    QList<QString> names;
    QList<QByteArray> hashs;

    int i = 0;
    int c = 2; // preskakujem prvni dva byty
    while (i<count) {
        int size = (int) (
                (unsigned char) response_data[c+0] * 0x01000000 +
                (unsigned char) response_data[c+1] * 0x00010000 +
                (unsigned char) response_data[c+2] * 0x00000100 +
                (unsigned char) response_data[c+3] * 0x00000001);
        c +=4;
        sizes.append(size);
        hashs.append(response_data.mid(c,16));
        c+=16;
        int end = response_data.indexOf((char)'\0',c);
        names.append(response_data.mid(c,end-c));
        c = end+1;
        i++;
    }
    qDebug(QString("directory listing by mask ").append(MASK).toStdString().c_str());
    for (i = 0; i < count; i++) {
        qDebug() << names[i] << " " << hashs[i].toHex() << " " << sizes[i];
    }




}


void FileserverTest::cleanupTestCase() {
    delete fs;
    delete buffer;
    delete tester;
}



