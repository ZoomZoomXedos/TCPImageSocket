#ifndef TCPSERV_H
#define TCPSERV_H

#include <QHostAddress>
#include <QTcpServer>
#include <QHash>
#include "tcps.h"

class tcpserv: public QTcpServer
{
    Q_OBJECT
public:
    explicit tcpserv(int numConnections = 10000, QObject *parent = nullptr);
    ~tcpserv();
    void set_max_pending_connections(int numConnections);
    void incoming_connection(qintptr socketDescriptor);

signals:
    void connect_client(qint64, const QString &, quint16);
    void sock_dis_connect(qint64, const QString &, quint16);
    void sent_dis_connect(qint64);

public slots:
    void clear();

private slots:
    void sock_dis_connect_slot(qint64 socketDescriptor,const QString &ip, quint16 port, QThread *th);

private:
    QHash<qint64, tcpsocket*> tcpClient;
    int maxConnections;
    int nConnectCount;
};
#endif // TCPSERV_H
