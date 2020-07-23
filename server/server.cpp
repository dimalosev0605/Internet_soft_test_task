#include "server.h"

Server::Server(QObject *parent)
    : QTcpServer(parent)
{
    qDebug() << this << "created, thread: " << QThread::currentThreadId();
    threads_count = QThread::idealThreadCount();
    qDebug() << "count of threads: " << threads_count;
}

void Server::listen(const QHostAddress &address, quint16 port)
{
    if(QTcpServer::listen(address, port)) {
        qDebug() << this << "started on port:" << QTcpServer::serverPort();

        connections.reserve(threads_count);
        for(std::size_t i = 0; i < threads_count; ++i) {
            Thread_connections* new_connections = new Thread_connections;
            connections.push_back(new_connections);
        }

    }
    else {
        qDebug() << this << "did no start";
    }
}

void Server::incomingConnection(qintptr socket_descriptor)
{
    divide_load(socket_descriptor);
}

void Server::divide_load(qintptr socket_descriptor)
{
    int min_c = connections[0]->get_count_of_connections();
    size_t n = 0;

    for(std::size_t i = 0; i < threads_count; ++i) {
        if(connections[i]->get_count_of_connections() < min_c) {
            min_c = connections[i]->get_count_of_connections();
            n = i;
        }
    }

    qDebug() << this << " min load thread number is: " << n;
    emit connections[n]->new_connection(socket_descriptor);
}

void Server::set_count_of_threads(const std::size_t number_of_threads)
{
    threads_count = number_of_threads;
}
