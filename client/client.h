#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

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
    void stateChanged(QAbstractSocket::SocketState socketState);
    void hostFound();
    void error(QAbstractSocket::SocketError socketError);

public:
    explicit Client(QObject *parent = nullptr);

signals:
    void response_received(const QString& response);
};

#endif // CLIENT_H
