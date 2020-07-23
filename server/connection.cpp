#include "connection.h"

Connection::Connection(qint64 socket_descriptor, QObject *parent)
    : QObject(parent)
{
    qDebug() << this << "created, thread: " << QThread::currentThreadId();

    connect(&socket, &QTcpSocket::hostFound, this, &Connection::hostFound);
    connect(&socket, &QTcpSocket::connected, this, &Connection::connected);
    connect(&socket, &QTcpSocket::stateChanged, this, &Connection::stateChanged);
    connect(&socket, &QTcpSocket::readyRead, this, &Connection::ready_read);
    connect(&socket, &QTcpSocket::bytesWritten, this, &Connection::bytes_written);
    connect(&socket, &QTcpSocket::disconnected, this, &Connection::disconnected);
    connect(&socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Connection::error);

    socket.setSocketDescriptor(socket_descriptor);
}

void Connection::process_data()
{
    auto j_doc = QJsonDocument::fromJson(data);
    if(!j_doc.isNull()) {
        auto j_obj = j_doc.object();
        auto j_map = j_obj.toVariantMap();

        const QString data_key = "data";
        const QString rect_desc_1_key = "rectDescription1";
        const QString rect_desc_2_key = "rectDescription2";

        auto j_arr = j_map[data_key].toJsonArray();

        auto rect_desc_1_obj = j_arr[0].toObject();
        auto rect_desc_1_map = rect_desc_1_obj.toVariantMap();
        auto rect_desc_1 = rect_desc_1_map[rect_desc_1_key].toString();

        auto rect_desc_2_obj = j_arr[1].toObject();
        auto rect_desc_2_map = rect_desc_2_obj.toVariantMap();
        auto rect_desc_2 = rect_desc_2_map[rect_desc_2_key].toString();

        qDebug() << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$";
        qDebug() << "rect_desc_1 = " << rect_desc_1;
        qDebug() << "rect_desc_2 = " << rect_desc_2;
        qDebug() << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$";

        create_response(rect_desc_2);

        data.clear();
    }
}

void Connection::create_response(const QString& response)
{
    QJsonArray j_arr;

    QJsonObject response_j_arr_obj;
    const QString res_key = "result";
    response_j_arr_obj.insert(res_key, response);

    j_arr.append(response_j_arr_obj);

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

void Connection::bytes_written(qint64 bytes)
{
    qDebug() << this << "bytes_written, thread: " << QThread::currentThreadId();
}

void Connection::stateChanged(QAbstractSocket::SocketState socketState)
{
    qDebug() << this << "stateChanged, thread: " << QThread::currentThreadId();
}

void Connection::hostFound()
{
    qDebug() << this << "hostFound, thread: " << QThread::currentThreadId();
}

void Connection::error(QAbstractSocket::SocketError socketError)
{
    qDebug() << this << "error, thread: " << QThread::currentThreadId();
}
