#include <QCryptographicHash>
#include "upload_container.h"

bool UploadContainer::testSha1() {
    QByteArray data_hash = QCryptographicHash::hash(this->data,QCryptographicHash::Sha1);
    return data_hash == sha1;
}
