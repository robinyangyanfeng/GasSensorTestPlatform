#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>

namespace Ui {
class TCP_Server;
}

class TCP_Server : public QWidget
{
    Q_OBJECT

public:
    explicit TCP_Server(QWidget *parent = nullptr);
    ~TCP_Server();

private slots:
    void slot_newconnect(); //建立新连接的槽
    void slot_sendmessage(); //发送消息的槽
    void slot_recvmessage(); //接收消息的槽
    void slot_disconnect(); //取消连接的槽

private:
    Ui::m_tcpServer *ui;

    QTcpServer *TCP_server; //QTcpServer服务器
    QTcpSocket *TCP_connectSocket; //与客户端连接套接字
};

#endif // TCP_SERVER_H
