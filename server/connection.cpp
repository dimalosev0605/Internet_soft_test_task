#include "connection.h"

Connection::Connection(qint64 socket_descriptor, QObject *parent)
    : QObject(parent)
{
    qDebug() << this << "created, thread: " << QThread::currentThreadId();

    connect(&socket, &QTcpSocket::connected, this, &Connection::connected);
    connect(&socket, &QTcpSocket::disconnected, this, &Connection::disconnected);
    connect(&socket, &QTcpSocket::readyRead, this, &Connection::ready_read);
    connect(&socket, &QTcpSocket::bytesWritten, this, &Connection::bytes_written);

    socket.setSocketDescriptor(socket_descriptor);
}

void Connection::process_data()
{
    auto j_doc = QJsonDocument::fromJson(data);
    if(!j_doc.isNull()) {
        auto j_obj = j_doc.object();
        auto j_map = j_obj.toVariantMap();

        auto j_arr = j_map[Protocol_keys::data].toJsonArray();

        const int magic_number = 2;
        if(j_arr.size() == magic_number) {
            auto rect_desc_2_obj = j_arr[1].toObject();
            auto rect_desc_2_map = rect_desc_2_obj.toVariantMap();
            auto rect_desc_2 = rect_desc_2_map[Protocol_keys::rect_desc_2].toString();
            create_response(rect_desc_2);
        }
        data.clear();
    }
}

void Connection::create_response(const QString& response)
{
    QJsonArray j_arr;

    QJsonObject j_obj;
    j_obj.insert(Protocol_keys::result, response);

    j_arr.append(j_obj);

    QJsonDocument j_doc(j_arr);
    socket.write(j_doc.toJson());
}

Connection::~Connection()
{
    qDebug() << this << "destroyed, thread: " << QThread::currentThreadId();
}

void Connection::connected()
{
    qDebug() << this << "connected, thread: " << QThread::currentThreadId();
}

void Connection::disconnected()
{
    qDebug() << this << "disconnected, thread: " << QThread::currentThreadId();
}

void Connection::ready_read()
{
    qDebug() << this << "ready_read, thread: " << QThread::currentThreadId();
    qint64 bytes_available = socket.bytesAvailable();
    data += socket.read(bytes_available);
    process_data();
}

void Connection::bytes_written([[maybe_unused]]qint64 bytes)
{
    qDebug() << this << "bytes_written, thread: " << QThread::currentThreadId();
}
