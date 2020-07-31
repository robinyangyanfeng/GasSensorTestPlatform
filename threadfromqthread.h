#ifndef THREADFROMQTHREAD_H
#define THREADFROMQTHREAD_H
#include <QThread>

class ThreadFromQThread : public QThread
{
    Q_OBJECT
signals:
    void message(const QString& info);
    void progress(int present);
public:
    ThreadFromQThread(QObject* par);
    ~ThreadFromQThread();
    void setSomething();
    void getSomething();
    void setRunCount(int count);
    void run();
    void doSomething();
private:
    int m_runCount;
};

#endif // THREADFROMQTHREAD_H
