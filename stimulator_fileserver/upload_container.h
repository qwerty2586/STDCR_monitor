#ifndef STIMULATOR_CONTROL_UPLOAD_CONTAINER_H
#define STIMULATOR_CONTROL_UPLOAD_CONTAINER_H

#include <QByteArray>
#include <QString>

class UploadContainer {
public:
    QByteArray data;
    QByteArray md5;
    QString target_path;
    int target_size;

    bool testMd5();
};

#endif //STIMULATOR_CONTROL_UPLOAD_CONTAINER_H
