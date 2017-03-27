#include <QCryptographicHash>
#include "upload_container.h"

bool UploadContainer::testMd5() {
    QByteArray data_hash = QCryptographicHash::hash(this->data,QCryptographicHash::Md5);
    return data_hash == md5;
}
