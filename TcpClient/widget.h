#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets>
#include "tcpsock.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void setupUi();

private slots:
    void on_connect_tcpserv();
    void on_disconnect_tcpserv();
    void on_send_file();

    void on_connection();
    void on_disconnection();
private:
    QLabel* _IP;
    QLineEdit* _IPEdit;
    QLabel* _Port;
    QSpinBox* _PortSpin;

    QPushButton* _Connect;
    QPushButton* _DisConnect;

    QPushButton* _SendFile;

    QLabel* _NetworkStatus;

    tcpsocket* _socket;
};

#endif // WIDGET_H
