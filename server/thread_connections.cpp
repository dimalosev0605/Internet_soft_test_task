#include "thread_connections.h"

Thread_connections::Thread_connections(QObject *parent)
    : QObject(parent)
{
    qDebug() << this << "created, thread: " << QThread::currentThreadId();
    QThread* thread = new QThread(this);
    connect(this, &Thread_connections::new_connection, this, &Thread_connections::accept_socket_descriptor,
            Qt::QueuedConnection);
    this->moveToThread(thread);
    thread->start();
}

void Thread_connections::accept_socket_descriptor(qintptr socket_descriptor)
{
    ++count_of_connections;
    qDebug() << this << "increase count of connections to: " << count_of_connections << ", thread: " << QThread::currentThreadId();

    Connection* new_connection = new Connection(socket_descriptor);
    connect(new_connection, &QObject::destroyed, this, &Thread_connections::decrease_count_of_connections);
}

std::size_t Thread_connections::get_count_of_connections() const
{
    return count_of_connections;
}

void Thread_connections::decrease_count_of_connections()
{
    --count_of_connections;
    qDebug() << this << "decrease count of connections to: " << count_of_connections << ", thread: " << QThread::currentThreadId();
}
