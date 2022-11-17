#include "tcpserv.h"
#include "threat.h"
#include "tcps.h"

tcpserv::tcpserv(int numConnections, QObject *parent)
    : QTcpServer(parent)
{
    qDebug()<<"TcpServer start";
    set_max_pending_connections(numConnections);
    nConnectCount = 0;
}

tcpserv::~tcpserv()
{
    qDebug()<<"TcpServer finish";
    emit this->sent_dis_connect(-1);
    this->close();
}

void tcpserv::set_max_pending_connections(int numConnections)
{
    this->QTcpServer::setMaxPendingConnections(numConnections);//max count of connections
    this->maxConnections = numConnections;
}

void tcpserv::incoming_connection(qintptr socketDescriptor) //new connections
{
    if (tcpClient.size() > maxConnections)
    {
        QTcpSocket tcp;
        tcp.setSocketDescriptor(socketDescriptor);
        tcp.disconnectFromHost();
        return;
    }
    QThread* th = threat::get_class().get_thread();
    tcpsocket* tcpTemp = new tcpsocket(socketDescriptor);
    QString ip =  tcpTemp->peerAddress().toString();
    quint16 port = tcpTemp->peerPort();

    connect(tcpTemp,&tcpsocket::sock_dis_connect,this,&tcpserv::sock_dis_connect_slot);
    connect(this, &tcpserv::sent_dis_connect, tcpTemp, &tcpsocket::dis_con_tcp);//signal off

    tcpTemp->moveToThread(th);
    tcpClient.insert(socketDescriptor,tcpTemp);
    emit connect_client(socketDescriptor,ip,port);
    nConnectCount++;
    qDebug()<<"new connection" <<"socketDescriptor="<<socketDescriptor;
    qDebug()<<"on moment："<<nConnectCount;
}

void tcpserv::sock_dis_connect_slot(qint64 socketDescriptor, const QString & ip, quint16 port, QThread * th)
{
    nConnectCount--;
    qDebug() << "disconnect" <<"socketDescriptor="<<socketDescriptor;
    qDebug()<<"on moment："<<nConnectCount;
    tcpClient.remove(socketDescriptor);
    threat::get_class().remove_thread(th);
    emit sock_dis_connect(socketDescriptor,ip,port);
}

void tcpserv::clear()
{
    emit this->sent_dis_connect(-1);
    threat::get_class().clear();
    tcpClient.clear();
}
