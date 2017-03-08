#include <QDir>
#include <QtCore/QCryptographicHash>
#include <QImage>
#include <QBuffer>
#include "fileserver.h"
#include "transfer_protocol.h"

Fileserver::Fileserver(const QString &server_name, const QString &path) {
    this->start_path = path;
    if (start_path.right(1).contains('/')) start_path = start_path.left(start_path.size() - 1);
    this->server_name = server_name;
}


void Fileserver::incomingMessage(QByteArray message_data) {
    bool finite = (message_data[INDEX_COMMAND - PREFIX] & SECTION_PART == PART_LAST);
    char iter = message_data[INDEX_ITER - PREFIX];
    char type = (message_data[INDEX_COMMAND - PREFIX] & SECTION_TYPE);

    if (type == TYPE_REQUEST) {
        if (finite) {
            if (unfinished_requests.find(iter) != unfinished_requests.end()) {
                message_data.append(unfinished_requests[iter]);
                unfinished_requests.erase(iter);
            }
            char op = (message_data[INDEX_COMMAND - PREFIX] & SECTION_OP);
            incomingRequest(op, iter, message_data);
        } else {
            if (unfinished_requests.find(iter) == unfinished_requests.end()) {
                unfinished_requests[iter] = QByteArray();
            }
            unfinished_requests[iter].append(message_data.mid(PREFIX));
        }
    }
    if (type == TYPE_UPLOAD) {
        incomingUpload(finite,iter,message_data.mid(PREFIX));
    }
}

void Fileserver::incomingRequest(char op, char iter, QByteArray message_data) {
    switch (op) {
        case OP_HELLO: {
            resetAll();
            client_ver = message_data[0];
            client_name = dataToStr(message_data, 1);
            response(OP_HELLO, iter, RESPONSE_OK, strToData(server_name));
            break;
        }
        case OP_BYE: {
            resetAll();
            response(OP_BYE, iter, RESPONSE_OK);
            break;
        }
        case OP_MD: {
            QString dir_path = dataToStr(message_data);
            dir_path.replace('~', start_path);
            QDir dir(dir_path);
            if (dir.exists()) {
                response(OP_MD, iter, RESPONSE_MD_DIR_EXIST);
            } else {
                if (dir.mkpath(".")) {
                    //sucess
                    response(OP_MD, iter, RESPONSE_OK);
                } else {
                    //fail
                    response(OP_MD, iter, RESPONSE_MD_FAIL);
                }
            }
            break;
        }
        case OP_LS: {
            QString dir_path = dataToStr(message_data);
            dir_path.replace('~', start_path);
            QDir dir(dir_path);
            if (!dir.exists()) {
                response(OP_MD, iter, RESPONSE_LS_DIR_NOT_FOUND);
            }
            // MAGIC
            break;
        }
        case OP_DEL: {
            QString path = dataToStr(message_data);
            path.replace('~', start_path);
            bool result;

            if (QFileInfo(path).isDir()) {
                result = QDir().rmdir(path);
            } else {
                result = QFile(path).remove();
            }

            if (result) {
                response(OP_DEL, iter, RESPONSE_OK);
            } else {
                response(OP_DEL, iter, RESPONSE_DEL_FAIL);
            }
            break;
        }
        case OP_GET: {
            QString path = dataToStr(message_data);
            path.replace('~', start_path);
            QFile file(path);
            QByteArray bytes = file.readAll();
            QByteArray sha1 = QCryptographicHash::hash(bytes,QCryptographicHash::Sha1);
            QByteArray size = intToSizeBytes(bytes.size());

            response(OP_GET,iter,RESPONSE_OK,size.append(sha1));
            send_download(iter, bytes);
            break;
        }
        case OP_GET_PREVIEW: {
            QString path = dataToStr(message_data);
            path.replace('~', start_path);

            QImage image;
            image.load(path);
            image = image.scaled(100,100,Qt::KeepAspectRatio);
            QByteArray bytes;
            QBuffer buffer(&bytes);
            buffer.open(QIODevice::WriteOnly);
            image.save(&buffer, "JPG");

            QByteArray sha1 = QCryptographicHash::hash(bytes,QCryptographicHash::Sha1);
            QByteArray size = intToSizeBytes(bytes.size());

            response(OP_GET_PREVIEW,iter,RESPONSE_OK,size.append(sha1));
            send_download(iter, bytes);
            break;
        }
        case OP_START: {
            QString path = dataToStr(message_data);
            path.replace('~', start_path);
            emit startSdlOutput(path);
            response(OP_START,iter,RESPONSE_OK);
            break;
        }

        case OP_STOP: {
            emit stopSdlOutput();
            response(OP_STOP,iter,RESPONSE_OK);
            break;
        }

        case OP_PUT: {

            UploadContainer upload;
            upload.target_size = sizeBytesToInt(message_data.mid(0,4));
            upload.sha1 = message_data.mid(4,20);
            upload.target_path = dataToStr(message_data,4+20+1);
            unfinished_uploads[iter] = upload;
            break;
        }

    }

}


void Fileserver::incomingUpload(bool finite, char iter, QByteArray message_data) {
    if (unfinished_uploads.find(iter) == unfinished_uploads.end())
        return;
    unfinished_uploads[iter].data.append(message_data);
    if (finite) {
        unfinished_uploads[iter].data.truncate(unfinished_uploads[iter].target_size);
        if (unfinished_uploads[iter].testSha1()) {
            QFile file(unfinished_uploads[iter].target_path);
            file.write(unfinished_uploads[iter].data);
            file.close();
            response(OP_PUT,iter,RESPONSE_OK);
        } else {
            response(OP_PUT,iter,RESPONSE_PUT_SHA1_FAIL);
        }
    }
};

void Fileserver::resetAll() {
    unfinished_requests.clear();
    client_name = "";
    client_ver = 0;
}

void Fileserver::response(char op, char iter, char response) {
    this->response(op, iter, response, QByteArray());
};

void Fileserver::response(char op, char iter, char response, QByteArray data) {
    data.prepend(response);
    if (data.size() % 60 != 0) {
        int new_size = ((data.size() / 60) + 1) * 60;
        data.leftJustified(new_size, '\0');
    }

    int count = data.size() / 60;
    QByteArray two_first_bytes;
    two_first_bytes.append(op + (char)TYPE_RESPONSE);
    two_first_bytes.append(iter);
    for (int i = 0; i < count; ++i) {
        if (i == (count - 1)) two_first_bytes[0] = two_first_bytes[0] + PART_LAST;
        emit outcomingMessage(data.mid(i * 60, 60).prepend(two_first_bytes));
    }
}

QByteArray Fileserver::strToData(QString string) {
    return string.toAscii().append((char) '\0');
};

QString Fileserver::dataToStr(QByteArray data, int start) {
    int end = data.indexOf((char) '\0', start);
    if (end == -1) end = data.size();
    return QString(data.mid(start, end - start));
}

void Fileserver::send_download(char iter, QByteArray data) {
    if (data.size() % 60 != 0) {
        int new_size = ((data.size() / 60) + 1) * 60;
        data.leftJustified(new_size, '\0');
    }

    int count = data.size() / 60;
    QByteArray two_first_bytes;
    two_first_bytes.append((char)TYPE_DOWNLOAD);
    two_first_bytes.append(iter);
    for (int i = 0; i < count; ++i) {
        if (i == (count - 1)) two_first_bytes[0] = two_first_bytes[0] + PART_LAST;
        emit outcomingMessage(data.mid(i * 60, 60).prepend(two_first_bytes));
    }
}

QByteArray Fileserver::intToSizeBytes(int size) {
/*
    // starsi implementace
    qint32 qsize = size;
    QByteArray array;
    QDataStream stream(&array, QIODevice::WriteOnly);
    stream << qsize;
*/
    qint32 qsize = size;
    QByteArray array(4,0);
    array[0] = (qsize << 8*3) & 0xFF;
    array[0] = (qsize << 8*2) & 0xFF;
    array[0] = (qsize << 8*1) & 0xFF;
    array[0] = (qsize << 8*0) & 0xFF;
    return array;
}

int Fileserver::sizeBytesToInt(QByteArray bytes) {
    int size =
    bytes[0] * 0x01000000 +
    bytes[1] * 0x00010000 +
    bytes[2] * 0x00000100 +
    bytes[3] * 0x00000001 ;
    return size;
}


