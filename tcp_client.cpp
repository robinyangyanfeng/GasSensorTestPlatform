nclude "tcp_client.h"
#include "ui_tcp_client.h"

TCP_Client::TCP_Client(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TCP_Client)
{
    ui->setupUi(this);
}

TCP_Client::~TCP_Client()
{
    delete ui;
}
