#ifndef EXT_H
#define EXT_H

#include <QByteArray>
#include <QDataStream>

static const QString dirPath = "C:/Users/Parrot/Desktop";
static const quint16 port = 6000;
static const long number_init = -1;
static const int net_packet_byte = 2;
static const int net_packet_len_byte = 8;


class socket_packet
{
public:
    long       length;
    QByteArray data;
    QByteArray dataType;

    socket_packet()
    {
        length = number_init;
    }

    void clear()
    {
        length = number_init;
        data.clear();
        dataType.clear();
    }
};

struct net_send_file
{
    QString m_filename;
    QByteArray m_bytearray;

    friend QDataStream& operator>>(QDataStream& in, net_send_file& stru)
    {
        in >> stru.m_filename >> stru.m_bytearray;
        return in;
    }

    friend QDataStream& operator<<(QDataStream& out, const net_send_file& stru)
    {
        out << stru.m_filename << stru.m_bytearray;
        return out;
    }
};

#endif // EXT_H
