#ifndef DATATHREAD_H
#define DATATHREAD_H

#include <QThread>
#include <QDebug>

class DataThread : public QThread
{
    Q_OBJECT
public:
    DataThread();

    void threadStop();

signals:
    void updateChartDataSignalFunction(float floatNum1,float floatNum2,float floatNum3,float floatNum4,float floatNum5,float floatNum6,float floatNum7,float floatNum8,float floatNum9,float floatNum10,float floatNum11,float floatNum12,float floatNum13,float floatNum14,float floatNum15,float floatNum16);

protected:
    void run();

    bool m_isStop;

    bool m_isQTimerStart;

    bool m_isSend;

    bool m_isShow;

    int m_isStopThread;
};

#endif // DATATHREAD_H
