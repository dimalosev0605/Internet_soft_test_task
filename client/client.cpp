#include "client.h"

Client::Client(QObject *parent)
    : QObject(parent)
{

    connect(&socket, &QTcpSocket::hostFound, this, &Client::hostFound);
    connect(&socket, &QTcpSocket::connected, this, &Client::connected);
    connect(&socket, &QTcpSocket::stateChanged, this, &Client::stateChanged);
    connect(&socket, &QTcpSocket::readyRead, this, &Client::ready_read);
    connect(&socket, &QTcpSocket::bytesWritten, this, &Client::bytes_written);
    connect(&socket, &QTcpSocket::disconnected, this, &Client::disconnected);
    connect(&socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Client::error);

    connect_to_server();
}

void Client::connect_to_server()
{
    socket.connectToHost(server_ip, server_port);
}

void Client::send_message()
{
    QJsonObject j_obj;
    QJsonArray j_arr;

    const QString data_key = "data";
    const QString rect_desc_1_key = "rectDescription1";
    const QString rect_desc_2_key = "rectDescription2";

    insert_obj(j_arr, rect_desc_1_key, "10, 10, 450, 230");
    insert_obj(j_arr, rect_desc_2_key, "50, 12, 40, 204");

    j_obj.insert(data_key, j_arr);

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

        qDebug() << "j_arr.size() = " << j_arr.size();

        auto response_obj = j_arr[0].toObject();
        auto response_map = response_obj.toVariantMap();

        const QString res_key = "result";
        auto response = response_map[res_key].toString();

        qDebug() << "RESPONSE = " << response;
        emit response_received(response);
        data.clear();
    }
}

void Client::bytes_written(qint64 bytes)
{
    qDebug() << this << "bytes_written, thread: " << QThread::currentThreadId();
}

void Client::stateChanged(QAbstractSocket::SocketState socketState)
{
    qDebug() << this << "stateChanged, thread: " << QThread::currentThreadId();
}

void Client::hostFound()
{
    qDebug() << this << "hostFound, thread: " << QThread::currentThreadId();
}

void Client::error(QAbstractSocket::SocketError socketError)
{
    qDebug() << this << "error, thread: " << QThread::currentThreadId();
}
