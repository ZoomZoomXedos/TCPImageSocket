#ifndef TCPSOCK_H
#define TCPSOCK_H

#include <QTcpSocket>
#include "ext.h"

class tcpsocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit tcpsocket(QObject *parent = nullptr);
    ~tcpsocket();

    void connect_tcp_server(const QString& ip, quint16 port);
signals:
    void data_packet_ready(socket_packet socketData);

public slots:
    void send(QByteArray& data, const QByteArray& dataType);
private slots:
    void on_ready_read();

private:
    bool parse_packet(socket_packet *packet);

private:
    socket_packet _data_packet;
};

#endif // TCPSOCK_H
