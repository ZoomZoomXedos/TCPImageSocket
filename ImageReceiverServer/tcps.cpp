#include "tcps.h"
#include "ext.h"
#include <QPixmap>
#include <QFileInfo>
#include <server.h>
#include <ui_server.h>

static const char* net_packet_start = "PACKET_START";
static const char* net_packet_end = "PACKET_END";
static const char* net_packet_send_file = "01";

tcpsocket::tcpsocket(qintptr socketDescriptor, QObject *parent)
    : QTcpSocket(parent), socketID(socketDescriptor)
{
    this->setSocketDescriptor(socketDescriptor);
    connect(this, &QTcpSocket::readyRead, this, &tcpsocket::on_ready_read);
    qDebug()<<"connect";
    dis = connect(this,&QTcpSocket::disconnected,
        [&](){       //lambda
            emit sock_dis_connect(socketID,this->peerAddress().toString(),this->peerPort(),QThread::currentThread());
            this->deleteLater();
        });
}

tcpsocket::~tcpsocket()
{

}

void tcpsocket::on_ready_read()
{
    m_dataPacket.data += this->readAll();
    bool isOk = false;
    do
    {
        isOk = parse_packet(&m_dataPacket);
    }
    while(isOk);
}

bool tcpsocket::parse_packet(socket_packet *packet)
{
    int pIndexStart = packet->data.indexOf(net_packet_start);
    if(pIndexStart < 0)
    {
        return false;
    }
    packet->data = packet->data.mid(pIndexStart);
    socket_packet tmpPacket;
    tmpPacket.data = packet->data;
    tmpPacket.data.remove(0, QByteArray(net_packet_start).size());
    if(tmpPacket.data.count() < net_packet_len_byte)
    {
        return false;
    }
    bool isOk;
    tmpPacket.length = tmpPacket.data.mid(0, net_packet_len_byte).toLong(&isOk);
    if(isOk == false)
    {
        packet->data.remove(0, QByteArray(net_packet_start).size());
        if(packet->data.indexOf(net_packet_start) >= 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    tmpPacket.data.remove(0, net_packet_len_byte);
    if(tmpPacket.length > tmpPacket.data.count())
    {
        return false;
    }

    tmpPacket.data.resize(tmpPacket.length);
    if(tmpPacket.data.endsWith(net_packet_end) == false)
    {
        packet->data.remove(0, QByteArray(net_packet_start).size());
        if(packet->data.indexOf(net_packet_start) >= 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    tmpPacket.data.resize(tmpPacket.length -
                          QByteArray(net_packet_end).count());

    if(tmpPacket.data.count() < net_packet_byte)
    {
        return false;
    }
    QByteArray dataType = tmpPacket.data.left(net_packet_byte);
    tmpPacket.dataType = dataType;

    tmpPacket.data.remove(0, net_packet_byte);

    packet->data.remove(0, QByteArray(net_packet_start).size()
                        + net_packet_len_byte
                        + tmpPacket.length);

    emit data_packet_ready(tmpPacket);
    deal_data_packet(tmpPacket);
    return true;
}
extern Server *w;
void tcpsocket::deal_data_packet(const socket_packet &tmpPacket)
{
    if (tmpPacket.dataType == net_packet_send_file)
    {
        QByteArray block = tmpPacket.data;
        QDataStream in(&block, QIODevice::ReadOnly);
        net_send_file stru;
        in >> stru;

        QFile file(dirPath+'/'+stru.m_filename);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(stru.m_bytearray);
            qDebug() << "socketDescriptor=" << socketID <<"receive file" << stru.m_filename;
        QFileInfo fileinf(file);
        w->set_image(fileinf.absoluteFilePath());
        }
    }
}

void tcpsocket::send(QByteArray &data, const QByteArray &dataType)
{
    if(this->state() !=  QTcpSocket::ConnectedState)
    {
        return;
    }
    data.insert(0, dataType);
    data.append(net_packet_end);
    int size = data.size();
    QByteArray length = QByteArray().setNum(size);
    length = QByteArray(net_packet_len_byte, '0') + length;
    length = length.right(net_packet_len_byte);
    data.insert(0, net_packet_start + length);
    this->write(data);
}

void tcpsocket::dis_con_tcp(qint64 socketDescriptor)
{
    if (socketDescriptor == socketID)
    {
        this->disconnectFromHost();
    }
    else if (socketDescriptor == -1) //-1 - flag of off
    {
        disconnect(dis); //disconnest signal slot
        this->disconnectFromHost();
        this->deleteLater();
    }
}
