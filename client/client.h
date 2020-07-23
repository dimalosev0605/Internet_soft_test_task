#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

namespace Protocol_keys {
    const QString data = "data";
    const QString rect_desc_1 = "rectDescription1";
    const QString rect_desc_2 = "rectDescription2";
    const QString result = "result";
}

class Client : public QObject
{
    Q_OBJECT

    QTcpSocket socket;
    QString server_ip = "127.0.0.1";
    quint16 server_port = 1234;
    QByteArray data;

private:
    void connect_to_server();
    void send_message();
    void insert_obj(QJsonArray& arr, const QString& key, const QString& str);
    void process_data();

private slots:
    void connected();
    void disconnected();
    void ready_read();
    void bytes_written(qint64 bytes);

public:
    explicit Client(QObject *parent = nullptr);

signals:
    void response_received(const QString& response);
};

#endif // CLIENT_H
