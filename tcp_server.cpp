#include "tcp_server.h"
#include "ui_tcp_server.h"
#include <QMessageBox>
#include <QDateTime>

TCP_Server::TCP_Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TCP_Server)
{
    ui->setupUi(this);

    //初始化
    TCP_server = new QTcpServer();
    TCP_connectSocket = nullptr;
    connect(ui->pushButton_send,SIGNAL(clicked()),this,SLOT(slot_sendmessage()));

    //调用listen函数监听同时绑定IP和端口号
    if(TCP_server->listen(QHostAddress::LocalHost,10000)) //判断listen是否成功，成功则继续执行，连接新接收信号槽
    {
        this->connect(TCP_server,SIGNAL(newConnection()),this,SLOT(slot_newconnect()));  //将服务器的新连接信号连接到接收新连接的槽
    }
    else
    {
        QMessageBox::critical(this,"错误","IP绑定错误，请关闭其它服务端或更改绑定端口号");
    }
}

TCP_Server::~TCP_Server()
{
    delete ui;
}

//建立新连接的槽
void TCP_Server::slot_newconnect()
{
    if(TCP_server->hasPendingConnections())  //查询是否有新连接
    {
        TCP_connectSocket = TCP_server->nextPendingConnection(); //获取与真实客户端相连的客户端套接字
        ui->textBrowser->append("client login!"); //若有新连接，则提示

        this->connect(TCP_connectSocket,SIGNAL(readyRead()),this,SLOT(slot_recvmessage())); //连接客户端的套接字的有新消息信号到接收消息的槽
        this->connect(TCP_connectSocket,SIGNAL(disconnected()),this,SLOT(slot_disconnect())); //连接客户端的套接字取消连接信号到取消连接槽
    }
}

//发送消息的槽
void TCP_Server::slot_sendmessage()
{
    QString sendMessage = ui->lineEdit->text(); //获取单行文本框内要发送的内容
    if(TCP_connectSocket != nullptr && !sendMessage.isEmpty()) //确保有客户端连接，并且发送内容不为空
    {
        TCP_connectSocket->write(sendMessage.toLatin1());   //发送消息到客户端

        QString localDispalyMessage = "send to client: " + sendMessage \
                        + QDateTime::currentDateTime().toString(" yyyy-M-dd hh:mm:ss") + tr("\n");
        ui->textBrowser->append(localDispalyMessage);   //将要发送的内容显示在listwidget
    }

    ui->lineEdit->clear();
}

//接收消息的槽
void TCP_Server::slot_recvmessage()
{
    if(TCP_connectSocket != nullptr) //与客户端连接的socket，不是nullptr，则说明有客户端存在
    {
        QByteArray array = TCP_connectSocket->readAll();    //接收消息
        QHostAddress clientaddr = TCP_connectSocket->peerAddress(); //获得IP
        int port = TCP_connectSocket->peerPort();   //获得端口号

        QDateTime datetime = QDateTime::currentDateTime();

        QString sendMessage = tr("recv from :") + clientaddr.toString() + tr(" : ") \
                                + QString::number(port) + tr("   ") + datetime.toString("yyyy-M-dd hh:mm:ss") + tr("\n");
        sendMessage += array;

        ui->textBrowser->append(sendMessage);   //将接收到的内容加入到listwidget
    }
}

//取消连接的槽
void TCP_Server::slot_disconnect()
{
    if(TCP_connectSocket != nullptr)
    {
        ui->textBrowser->append("client logout!");
        TCP_connectSocket->close(); //关闭客户端
        TCP_connectSocket->deleteLater();
    }
}
