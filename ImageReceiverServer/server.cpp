#include "server.h"
#include "ui_server.h"
#include <QDebug>

Server::Server(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Server)
{
    ui->setupUi(this);
}

Server::~Server()
{
    delete ui;
}

void Server::set_image(QString file)
{
    QImage image(file);
    float width=image.width();
    float height=image.height();
    float coef;
    if ((width>ui->label->width()) or (height>ui->label->height()))
    {
        if (width>=height)
            coef=width/(ui->label->width());
        else
            coef=height/(ui->label->height());
        width=width/coef;
        height=height/coef;
    }
    image = image.scaled(width, height, Qt::KeepAspectRatio);
    QPixmap mypix=QPixmap::fromImage(image);
    ui->label->setPixmap(mypix);
}
