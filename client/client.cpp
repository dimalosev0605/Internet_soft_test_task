#include "client.h"

Client::Client(QObject *parent)
    : QObject(parent)
{
    connect(&socket, &QTcpSocket::connected, this, &Client::connected);
    connect(&socket, &QTcpSocket::disconnected, this, &Client::disconnected);
    connect(&socket, &QTcpSocket::readyRead, this, &Client::ready_read);
    connect(&socket, &QTcpSocket::bytesWritten, this, &Client::bytes_written);

    connect_to_server();
}

void Client::connect_to_server()
{
    socket.connectToHost(server_ip, server_port);
}

void Client::send_message()
{
    QJsonArray j_arr;

    insert_obj(j_arr, Protocol_keys::rect_desc_1, "10, 10, 450, 230");
    insert_obj(j_arr, Protocol_keys::rect_desc_2, "50, 12, 40, 204");

    QJsonObject j_obj;
    j_obj.insert(Protocol_keys::data, j_arr);

    QJsonDocument j_doc(j_obj);
    socket.write(j_doc.toJson());
}

void Client::insert_obj(QJsonArray& arr, const QString& key, const QString& str)
{
    QJsonObject obj;
    obj.insert(key, str);
    arr.append(obj);
}

void Client::connected()
{
    qDebug() << this << "connected, thread: " << QThread::currentThreadId();
    send_message();
}

void Client::disconnected()
{
    qDebug() << this << "disconnected, thread: " << QThread::currentThreadId();
}

void Client::ready_read()
{
    qDebug() << this << "ready_read, thread: " << QThread::currentThreadId();
    qint64 bytes_available = socket.bytesAvailable();
    data += socket.read(bytes_available);
    process_data();
}

void Client::process_data()
{
    auto j_doc = QJsonDocument::fromJson(data);
    if(!j_doc.isNull()) {
        auto j_arr = j_doc.array();
        if(!j_arr.isEmpty()) {
            auto response_obj = j_arr[0].toObject();
            auto response_map = response_obj.toVariantMap();
            auto response = response_map[Protocol_keys::result].toString();
            emit response_received(response);
        }
        data.clear();
    }
}

void Client::bytes_written([[maybe_unused]]qint64 bytes)
{
    qDebug() << this << "bytes_written, thread: " << QThread::currentThreadId();
}
