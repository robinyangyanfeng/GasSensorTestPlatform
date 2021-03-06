#ifndef QTCLIENT__H
#define QTCLIENT__H

#include <QWidget>
#include <QFile>
#include <QGlobalStatic>
#include <QDateTime>
#include <QtNetwork>
#include <QFileDialog>

namespace Ui {
class QtClient;
}

class QtClient : public QWidget
{
    Q_OBJECT
public:
    QtClient(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~QtClient();

    void initTCP();
    void newConnect();

signals:


    private slots:
        //连接服务器
        void connectServer();
        //与服务器断开连接
        void disconnectServer();
        //接收服务器发送的数据
        void receiveData();
        //向服务器发送数据
        void sendData();

        //浏览文件
        void selectFile();
        //发送文件
        void sendFile();
        //更新文件发送进度
        void updateFileProgress(qint64);
        //更新文件接收进度
        void updateFileProgress();
        //swap window
        void on_pushButton_clicked();

signals:
        void sendsignal();

private:
    Ui::QtClient *ui;
    QTcpSocket *tcpSocket;
    QTcpSocket *fileSocket;

    //文件传送
    QFile *localFile;
    //文件大小
    qint64 totalBytes;      //文件总字节数
    qint64 bytesWritten;    //已发送的字节数
    qint64 bytestoWrite;    //尚未发送的字节数
    qint64 filenameSize;    //文件名字的字节数
    qint64 bytesReceived;   //接收的字节数
    //每次发送数据大小
    qint64 perDataSize;
    QString filename;
    //数据缓冲区
    QByteArray inBlock;
    QByteArray outBlock;

    //系统时间
    QDateTime current_date_time;
    QString str_date_time;
};

#endif
