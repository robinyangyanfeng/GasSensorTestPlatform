#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>
#include <QtCharts>
#include <QLineSeries>
#include <QTimer>
#include <QWheelEvent>
#include <QInputDialog>
#include <QMessageBox>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>
#include <QTimer>
#include <QTime>
#include <QFile>

#include "chartview.h"

#include "csvfilereadwirte_xy.h"
#include "showform.h"
#include "datathread.h"
#include "qtclient.h"

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainForm;
}

class MainForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = 0);
    ~MainForm();

    //初始化Chart
    void initChart();

    //把数据写到文件里
    void writeDatasToCsvFile(QString fileName, QString writeDatas);

    //void print_sensor_data(struct bme280_data *comp_data);


protected:
    void wheelEvent(QWheelEvent *event);

private slots:

    //倒计时定时器槽函数
    void countdownFirstGenerationTimerSlot();
    void countdownSecondGenerationTimerSlot();

    //模拟测试按钮槽函数
    void on_simulationTestBtn_clicked();
    //开始按钮槽函数
    void on_startBtn_clicked();
    //重置按钮槽函数
    void on_resetBtn_clicked();

    //更新图表数据槽函数
    void updateChartDataSlot(float floatNum1,float floatNum2,float floatNum3,float floatNum4,float floatNum5,float floatNum6,float floatNum7,float floatNum8,float floatNum9,float floatNum10,float floatNum11,float floatNum12,float floatNum13,float floatNum14,float floatNum15,float floatNum16);

    //模拟测试定时器槽函数
    void simulationTestTimerSlot();

    void updateListDataSlot(float floatNum1, float floatNum2, float floatNum3, float floatNum4, float floatNum5, float floatNum6, float floatNum7, float floatNum8, float floatNum9, float floatNum10, float floatNum11, float floatNum12, float floatNum13, float floatNum14, float floatNum15, float floatNum16);

    void on_stopBtn_clicked();

    void on_actionSimulationTest_triggered();

    void on_actionStart_triggered();

    void on_actionStop_triggered();

    void on_actionReset_triggered();

    void on_actionExit_triggered();

    void on_actionHelp_triggered();

    void on_actionAboutQt_triggered();

    void on_showT_H_Btn_clicked();

    //温湿度定时器槽函数
    void tAndHqtimerSlot();

    void on_saveImageBtn_clicked();



private:
    Ui::MainForm *ui;

    QChart *m_chart;

    QLineSeries *m_lineSeries1;
    QLineSeries *m_lineSeries2;
    QLineSeries *m_lineSeries3;
    QLineSeries *m_lineSeries4;
    QLineSeries *m_lineSeries5;
    QLineSeries *m_lineSeries6;
    QLineSeries *m_lineSeries7;
    QLineSeries *m_lineSeries8;
    QLineSeries *m_lineSeries9;
    QLineSeries *m_lineSeries10;
    QLineSeries *m_lineSeries11;
    QLineSeries *m_lineSeries12;
    QLineSeries *m_lineSeries13;
    QLineSeries *m_lineSeries14;
    QLineSeries *m_lineSeries15;
    QLineSeries *m_lineSeries16;


    QChartView *m_chartView;

    //模拟测试定时器
    QTimer *m_simulationTestTimer;

    //温湿度定时器
    //QTimer *m_tAndHqtimer;

    QVector<QPointF> m_data1;
    QVector<QPointF> m_data2;
    QVector<QPointF> m_data3;
    QVector<QPointF> m_data4;
    QVector<QPointF> m_data5;
    QVector<QPointF> m_data6;
    QVector<QPointF> m_data7;
    QVector<QPointF> m_data8;
    QVector<QPointF> m_data9;
    QVector<QPointF> m_data10;
    QVector<QPointF> m_data11;
    QVector<QPointF> m_data12;
    QVector<QPointF> m_data13;
    QVector<QPointF> m_data14;
    QVector<QPointF> m_data15;
    QVector<QPointF> m_data16;

    bool m_isStop;

    bool m_isQTimerStart;

    bool m_isSend;

    bool m_isShow;


    //保存数据的文件名
    QString m_csvFileName;

    //csv文件保存路径
    QString m_csvFilePath;
    //倒计时定时器
    QTimer *m_countdownFirstGenerationTimer;
    QTimer *m_countdownSecondGenerationTimer;
    //倒计时时间
    int m_countdownFirstTimeInt;
    int m_countdownSecondTimeInt;
    //放置样品窗体
    ShowForm *m_showForm;

    //保存从csv文件里取出来的数据 map
    QMap<int,QStringList> m_csvMap;

    DataThread *m_dataThread;

};

#endif // MAINFORM_H
