#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QDebug>
#include <QThread>
#include <algorithm>

#include "thread_connections.h"

class Server: public QTcpServer
{
    Q_OBJECT

    std::size_t threads_count;
    std::vector<Thread_connections*> connections;

private:
    void divide_load(qintptr socket_descriptor);

public:
    explicit Server(QObject* parent = nullptr);
    void listen(const QHostAddress &address = QHostAddress::Any, quint16 port = 0);
    void incomingConnection(qintptr socket_descriptor) override;
    void set_count_of_threads(const std::size_t number_of_threads);
};

#endif // SERVER_H
