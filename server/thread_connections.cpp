#include "thread_connections.h"

Thread_connections::Thread_connections(std::mutex& divide_lock_mutex, QObject *parent)
    : QObject(parent),
      divide_load_mutex(divide_lock_mutex)
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
    {
        std::lock_guard<std::mutex> lock(divide_load_mutex);
        ++count_of_connections;
    }
    qDebug() << this << "increase count of connections to: " << count_of_connections << ", thread: " << QThread::currentThreadId();

    Connection* new_connection = new Connection(socket_descriptor);
    connect(new_connection, &QObject::destroyed, this, &Thread_connections::decrease_count_of_connections);
}

std::size_t Thread_connections::get_count_of_connections() const
{
    std::lock_guard<std::mutex> lock(divide_load_mutex);
    return count_of_connections;
}

void Thread_connections::decrease_count_of_connections()
{
    std::lock_guard<std::mutex> lock(divide_load_mutex);
    --count_of_connections;
    qDebug() << this << "decrease count of connections to: " << count_of_connections << ", thread: " << QThread::currentThreadId();
}
