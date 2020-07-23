#ifndef THREAD_CONNECTIONS_H
#define THREAD_CONNECTIONS_H

#include <QObject>

#include "connection.h"

class Thread_connections : public QObject
{
    Q_OBJECT

    std::size_t count_of_connections = 0;
    std::mutex& divide_load_mutex;

public:
    explicit Thread_connections(std::mutex& divide_lock_mutex, QObject *parent = nullptr);

public slots:
    void accept_socket_descriptor(qintptr socket_descriptor);
    std::size_t get_count_of_connections() const;

private slots:
    void decrease_count_of_connections();

signals:
    void new_connection(qintptr socket_descriptor);
};

#endif // THREAD_CONNECTIONS_H
