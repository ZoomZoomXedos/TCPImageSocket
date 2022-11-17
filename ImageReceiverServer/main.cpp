#include "server.h"
#include "tcpserv.h"
#include <QDir>
#include <QApplication>
Server *w;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    w=new Server;
    w->show();
    qRegisterMetaType<qintptr>();
    QDir dir(dirPath);
    if (!dir.exists()) {
        QDir tmpdir;
        if (!tmpdir.mkpath(dirPath)) {
            qDebug() << "directoty does't exist";
        }
    }
    tcpserv server;
    bool res = server.listen(QHostAddress::Any, port);
    if (res) {
        qDebug() << "ok";
    }
    else {
        qDebug() << "not ok";
    }

    return a.exec();
}
