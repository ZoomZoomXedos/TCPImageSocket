#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "ext.h"
#include <QTcpSocket>
#include <QByteArray>
#include <QTime>
#include <QHostAddress>
#include <QDebug>
#include <QThread>
#include <QFile>


class tcpsocket: public QTcpSocket
{
    Q_OBJECT
public:
    explicit tcpsocket(qintptr socketDescriptor, QObject *parent = nullptr);
    ~tcpsocket();

signals:
    void data_packet_ready(socket_packet socketData);
    void sock_dis_connect(qint64, const QString &, quint16, QThread *);

public slots:
    void send(QByteArray& data, const QByteArray& dataType);
    void dis_con_tcp(qint64 socketDescriptor);

private slots:
    void on_ready_read();

private:
    bool parse_packet(socket_packet *packet);
    void deal_data_packet(const socket_packet &tmpPacket);

private:
    qintptr socketID;
    socket_packet m_dataPacket;
    QMetaObject::Connection dis;
};

#endif // TCPSOCKET_H
