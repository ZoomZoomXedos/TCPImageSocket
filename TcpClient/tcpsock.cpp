#include "tcpsock.h"

static const char* net_packet_start = "PACKET_START";
static const char* net_packet_end = "PACKET_END";

tcpsocket::tcpsocket(QObject *parent)
    : QTcpSocket (parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(on_ready_read()));
}

tcpsocket::~tcpsocket()
{
    this->disconnectFromHost();
    if (this->state() != QAbstractSocket::UnconnectedState) {
        this->waitForDisconnected();
    }
}

void tcpsocket::connect_tcp_server(const QString& ip, quint16 port)
{
    this->connectToHost(ip, port);
    this->waitForConnected();
}

void tcpsocket::on_ready_read()
{
    _data_packet.data += this->readAll();

    bool isOk = false;
    do{
        isOk = parse_packet(&_data_packet);
    }while(isOk);
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

    //длина данных пакета
    tmpPacket.data.remove(0, net_packet_len_byte);//удаление длины данных
    if(tmpPacket.length > tmpPacket.data.count())
    {
        return false;
    }

    //определение конца пакета
    tmpPacket.data.resize(tmpPacket.length);//удалить лишнее
    if(tmpPacket.data.endsWith(net_packet_end) == false)
    {
        packet->data.remove(0, QByteArray(net_packet_start).size());//удалить заголовок
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
                          QByteArray(net_packet_end).count()); //удаление конца пакета

    //какой тип данных
    if(tmpPacket.data.count() < net_packet_byte)
    {
        return false;
    }
    QByteArray dataType = tmpPacket.data.left(net_packet_byte);
    tmpPacket.dataType = dataType;

    tmpPacket.data.remove(0, net_packet_byte);//удаление данных

    //удаление текущего пакета
    packet->data.remove(0,
                        QByteArray(net_packet_start).size()
                        + net_packet_len_byte
                        + tmpPacket.length);

    //сигнал отправки пакета
    emit data_packet_ready(tmpPacket);

    return true;
}

// передача пакета
void tcpsocket::send(QByteArray &data, const QByteArray &dataType)
{
    if(this->state() !=  QTcpSocket::ConnectedState)
    {
        return;
    }

    data.insert(0, dataType); //Тип пакета + данные + заг пакета
    data.append(net_packet_end);

    //Длина занимает 8 байт, а начало заполнено нулями,
    int size = data.size();
    QByteArray length = QByteArray().setNum(size);
    length = QByteArray(net_packet_len_byte, '0') + length;
    length = length.right(net_packet_len_byte);

    data.insert(0, net_packet_start + length);//заголовок пакета + длина данных

    this->write(data);
}
