#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QThread>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class Connection : public QObject
{
    Q_OBJECT

    QTcpSocket socket;
    QByteArray data;

private:
    void process_data();
    void create_response(const QString& response);

private slots:
    void connected();
    void disconnected();
    void ready_read();
    void bytes_written(qint64 bytes);

public:
    explicit Connection(qint64 socket_descriptor, QObject *parent = nullptr);
    ~Connection();
};

#endif // CONNECTION_H
