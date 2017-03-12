#ifndef STIMULATOR_GUI_FILESERVER_H
#define STIMULATOR_GUI_FILESERVER_H

#include <QObject>
#include <map>
#include "upload_container.h"


class Fileserver : public QObject {
Q_OBJECT



public:
    explicit Fileserver(const QString &server_name, const QString &path);


    std::map<int,QByteArray> unfinished_requests;
    std::map<int,UploadContainer> unfinished_uploads;

public slots:

    void incomingMessage(QByteArray message_data);

signals:

    void outcomingMessage(QByteArray messageData);
    void startSdlOutput(QString configfile);
    void stopSdlOutput();


private:

    QString server_name;
    QString start_path;


    QString client_name;
    char client_ver;

    void incomingRequest(char op, char iter, QByteArray message_data);

    void resetAll();

    void response(char op, char iter, char response, QByteArray data);

    void response(char op, char iter, char response);

    QByteArray strToData(QString string);

    QString dataToStr(QByteArray data, int start = 0);

    void send_download(char iter, QByteArray data);

    QByteArray intToSizeBytes(int size);

    int sizeBytesToInt(QByteArray bytes);

    void incomingUpload(bool finite, char iter, QByteArray message_data);

    void ls(char iter, const QString &dir_path, bool dirs, const QStringList &name_filters);
};



#endif //STIMULATOR_GUI_FILESERVER_H
