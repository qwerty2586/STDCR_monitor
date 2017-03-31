#include <QCryptographicHash>
#include <stimulator_log/log.h>
#include "upload_container.h"

bool UploadContainer::testMd5() {

    QByteArray data_hash = QCryptographicHash::hash(this->data,QCryptographicHash::Md5);
    Log::d("comparing hashes: ");
    Log::d("GIVEN: "+md5.toHex());
    Log::d("REAL : "+data_hash.toHex());
    return data_hash == md5;
}
