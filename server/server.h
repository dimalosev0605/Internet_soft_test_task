#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QDebug>
#include <QThread>

#include <utility>

#include "thread_connections.h"

class Server: public QTcpServer
{
    Q_OBJECT

    std::size_t threads_count;
    std::vector<std::unique_ptr<Thread_connections>> thread_connections;
    std::mutex divide_load_mutex;

private:
    void divide_load(qintptr socket_descriptor);

public:
    explicit Server(QObject* parent = nullptr);
    void listen(const QHostAddress &address = QHostAddress::Any, quint16 port = 0);
    void incomingConnection(qintptr socket_descriptor) override;
    void set_count_of_threads(const std::size_t number_of_threads);
};

#endif // SERVER_H
