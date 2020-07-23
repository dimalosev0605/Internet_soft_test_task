#include "server.h"

Server::Server(QObject *parent)
    : QTcpServer(parent)
{
    qDebug() << this << "created, thread: " << QThread::currentThreadId();
    threads_count = static_cast<std::size_t>(QThread::idealThreadCount());
    qDebug() << "count of threads: " << threads_count;
}

void Server::listen(const QHostAddress &address, quint16 port)
{
    if(QTcpServer::listen(address, port)) {
        qDebug() << this << "started on port:" << QTcpServer::serverPort();

        thread_connections.reserve(threads_count);
        for(std::size_t i = 0; i < threads_count; ++i) {
            std::unique_ptr<Thread_connections> new_connections = std::make_unique<Thread_connections>(divide_load_mutex);
            thread_connections.push_back(std::move(new_connections));
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
    std::size_t min_number_of_connections_per_thread = thread_connections[0]->get_count_of_connections();
    size_t index = 0;

    for(std::size_t i = 0; i < threads_count; ++i) {
        if(thread_connections[i]->get_count_of_connections() < min_number_of_connections_per_thread) {
            min_number_of_connections_per_thread = thread_connections[i]->get_count_of_connections();
            index = i;
        }
    }

    qDebug() << this << "min load thread index: " << index;
    emit thread_connections[index]->new_connection(socket_descriptor);
}

void Server::set_count_of_threads(const std::size_t number_of_threads)
{
    threads_count = number_of_threads;
}
