#include "widget.h"

static const char* net_pasket_send_file = "01";

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    connect(_Connect, &QPushButton::clicked, this, &Widget::on_connect_tcpserv);
    connect(_DisConnect, &QPushButton::clicked, this, &Widget::on_disconnect_tcpserv);
    connect(_SendFile, &QPushButton::clicked, this, &Widget::on_send_file);

    _socket = new tcpsocket(this);
    connect(_socket, &QTcpSocket::connected, this, &Widget::on_connection);
    connect(_socket, &QTcpSocket::disconnected, this, &Widget::on_disconnection);
}

Widget::~Widget()
{

}

void Widget::setupUi()
{
    /************ widget ****************/
    _IP = new QLabel("IP", this);
    _IPEdit = new QLineEdit("127.0.0.1", this);
    _Port = new QLabel("Port", this);
    _PortSpin = new QSpinBox(this);
    _PortSpin->setMaximum(65535);
    _PortSpin->setValue(static_cast<int>(port));

    _Connect = new QPushButton("Connect", this);
    _DisConnect = new QPushButton("Disconnect", this);
    _DisConnect->setEnabled(false);

    _SendFile = new QPushButton("Send File", this);

    _NetworkStatus = new QLabel(this);
    QString text = tr("Connect：");
    text += QString("<font color=red>%1</font>").arg(tr("wait"));
    _NetworkStatus->setText(text);

    /************ layout ****************/
    QHBoxLayout* topLayout = new QHBoxLayout;
    topLayout->addWidget(_IP);
    topLayout->addWidget(_IPEdit);
    topLayout->addWidget(_Port);
    topLayout->addWidget(_PortSpin);

    QHBoxLayout* top2Layout = new QHBoxLayout;
    top2Layout->addWidget(_Connect);
    top2Layout->addWidget(_DisConnect);

    QHBoxLayout* btmLayout = new QHBoxLayout;
    btmLayout->addWidget(_NetworkStatus);
    btmLayout->addStretch();

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addLayout(topLayout);
    layout->addLayout(top2Layout);
    layout->addWidget(_SendFile);
    layout->addLayout(btmLayout);
}

void Widget::on_connect_tcpserv()
{
    _socket->connect_tcp_server(_IPEdit->text(), static_cast<quint16>(_PortSpin->value()));
}

void Widget::on_disconnect_tcpserv()
{
    _socket->disconnectFromHost();
}

void Widget::on_send_file()
{
    QString filter="Image File (*.png, *.jpg, *.bmp)";
    QString filepath = QFileDialog::getOpenFileName(this, "Send file", tr(""),filter);
    if (filepath.isEmpty())
        return;
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly))
        return;
    QFileInfo info(filepath);
    QString filename = info.fileName();

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << filename << file.readAll();
    file.close();
    _socket->send(block, net_pasket_send_file);
}


void Widget::on_connection()
{
    QString text = tr("Connect：");
    text += QString("<font color=blue>%1</font>").arg(tr("enabled"));
    _NetworkStatus->setText(text);
    _Connect->setEnabled(false);
    _DisConnect->setEnabled(true);
}

void Widget::on_disconnection()
{
    QString text = tr("Connect：");
    text += QString("<font color=red>%1</font>").arg(tr("dis"));
    _NetworkStatus->setText(text);
    _Connect->setEnabled(true);
    _DisConnect->setEnabled(false);
}
