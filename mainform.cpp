#include "mainform.h"
#include "ui_mainform.h"

#include <QFileDialog>
#include <QPixmap>


MainForm::MainForm(QWidget *parent) :
    QMainWindow(parent),


    ui(new Ui::MainForm)
{

    ui->setupUi(this);


    //隐藏tabWidget的表头
    ui->tabWidget->findChildren<QTabBar*>().at(0)->hide();

    ui->tabWidget->setCurrentIndex(1);

    m_dataThread = new DataThread();

    connect(m_dataThread,SIGNAL(updateChartDataSignalFunction(float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float)),
            this,SLOT(updateListDataSlot(float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float)));


    //程序初始化时，把文件名置空。因为数据采集是通过定时器来控制的，停止数据采集后，再次触发，不想再次输入文件名。如果想每次都输入一次，那就不需要这个步骤
    m_csvFileName.isEmpty();

    m_chart = new QChart();

    m_isStop = false;

    m_isQTimerStart = false;

    m_isSend = true;

    m_isShow = false;

    m_simulationTestTimer = new QTimer(this);
    m_simulationTestTimer->setInterval(1000);

    //    m_tAndHqtimer = new QTimer(this);
    //    m_tAndHqtimer->setInterval(1000);

    m_data1.clear();
    m_data2.clear();
    m_data3.clear();
    m_data4.clear();
    m_data5.clear();
    m_data6.clear();
    m_data7.clear();
    m_data8.clear();
    m_data9.clear();
    m_data10.clear();
    m_data11.clear();
    m_data12.clear();
    m_data13.clear();
    m_data14.clear();
    m_data15.clear();
    m_data16.clear();

    //initChart();

    //connect(m_simulationTestTimer,SIGNAL(timeout()),this,SLOT(simulationTestTimerSlot()));

    //connect(m_tAndHqtimer,SIGNAL(timeout()),this,SLOT(tAndHqtimerSlot()));

//    connect(this,SIGNAL(updateChartDataSignalFunction(float,float,float,float,float,float,float,float)),
//            this,SLOT(updateChartDataSlot(float,float,float,float,float,float,float,float)));


    //m_tAndHqtimer->start();


    m_showForm = new ShowForm();

    //变量初始化
    m_csvFilePath = "";
    //初始为120，表示120秒
    m_countdownFirstTimeInt = 10;
    m_countdownSecondTimeInt = 10;

    m_countdownFirstGenerationTimer = new QTimer(this);
    //设置为1秒触发一次
    m_countdownFirstGenerationTimer->setInterval(1000);
    //连接信号与槽
    connect(m_countdownFirstGenerationTimer,SIGNAL(timeout()),this,SLOT(countdownFirstGenerationTimerSlot()));

    m_countdownSecondGenerationTimer = new QTimer(this);
    //设置为1秒触发一次
    m_countdownSecondGenerationTimer->setInterval(1000);
    //连接信号与槽
    connect(m_countdownSecondGenerationTimer,SIGNAL(timeout()),this,SLOT(countdownSecondGenerationTimerSlot()));



    //csv文件保存路径
    //这个地方把这个路径设置为固定路径，直接放到exe同级目录下
    m_csvFilePath = QCoreApplication::applicationDirPath();
    qDebug() << "m_csvFilePath:" << m_csvFilePath;


}

MainForm::~MainForm()
{
    if(m_showForm->isActiveWindow())
    {
        m_showForm->close();
    }
    delete ui;
}

void MainForm::initChart()
{
    m_lineSeries1 = new QLineSeries();
    m_lineSeries2 = new QLineSeries();
    m_lineSeries3 = new QLineSeries();
    m_lineSeries4 = new QLineSeries();
    m_lineSeries5 = new QLineSeries();
    m_lineSeries6 = new QLineSeries();
    m_lineSeries7 = new QLineSeries();
    m_lineSeries8 = new QLineSeries();
    m_lineSeries9 = new QLineSeries();
    m_lineSeries10 = new QLineSeries();
    m_lineSeries11 = new QLineSeries();
    m_lineSeries12 = new QLineSeries();
    m_lineSeries13 = new QLineSeries();
    m_lineSeries14 = new QLineSeries();
    m_lineSeries15 = new QLineSeries();
    m_lineSeries16 = new QLineSeries();

    m_lineSeries1->setName("Sensor 1");
    m_lineSeries2->setName("Sensor 2");
    m_lineSeries3->setName("Sensor 3");
    m_lineSeries4->setName("Sensor 4");
    m_lineSeries5->setName("Sensor 5");
    m_lineSeries6->setName("Sensor 6");
    m_lineSeries7->setName("Sensor 7");
    m_lineSeries8->setName("Sensor 8");
    m_lineSeries9->setName("Sensor 9");
    m_lineSeries10->setName("Sensor 10");
    m_lineSeries11->setName("Sensor 11");
    m_lineSeries12->setName("Sensor 12");
    m_lineSeries13->setName("Sensor 13");
    m_lineSeries14->setName("Sensor 14");
    m_lineSeries15->setName("Sensor 15");
    m_lineSeries16->setName("Sensor 16");

    m_chart->addSeries(m_lineSeries1);
    m_chart->addSeries(m_lineSeries2);
    m_chart->addSeries(m_lineSeries3);
    m_chart->addSeries(m_lineSeries4);
    m_chart->addSeries(m_lineSeries5);
    m_chart->addSeries(m_lineSeries6);
    m_chart->addSeries(m_lineSeries7);
    m_chart->addSeries(m_lineSeries8);
    m_chart->addSeries(m_lineSeries9);
    m_chart->addSeries(m_lineSeries10);
    m_chart->addSeries(m_lineSeries11);
    m_chart->addSeries(m_lineSeries12);
    m_chart->addSeries(m_lineSeries13);
    m_chart->addSeries(m_lineSeries14);
    m_chart->addSeries(m_lineSeries15);
    m_chart->addSeries(m_lineSeries16);

    m_chart->createDefaultAxes();
    m_chart->axisY()->setRange(0,3.5);
    m_chart->axisX()->setRange(0,20);

    m_chart->axisX()->setTitleFont(QFont("Microsoft YaHei", 10, QFont::Normal, true));
    m_chart->axisY()->setTitleFont(QFont("Microsoft YaHei", 10, QFont::Normal, true));
    m_chart->axisX()->setTitleText("time");
    m_chart->axisY()->setTitleText("Y");

    m_chart->axisX()->setGridLineVisible(false);
    m_chart->axisY()->setGridLineVisible(false);

    m_chartView = new ChartView(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    ui->horizontalLayout->addWidget(m_chartView);
}

void MainForm::writeDatasToCsvFile(QString csvFileName,QString writeDatas)
{
    //对文件名进行是否为空判断
    if(csvFileName.isEmpty())
    {
        qDebug() << "文件打开失败...";
        return;
    }


    QFile file(csvFileName);
    if (!file.exists())		//文件不存在的时候便新建
    {
        file.open(QIODevice::WriteOnly);
        QTextStream txtOutPut(&file);

        txtOutPut << "DateTime,Data1,Data2,Data3,Data4\n";//,Data5,Data6,Data7,Data8,Data9,Data10,Data11,Data12,Data13,Data14,Data15,Data16\n";	//注意，每行数据结束后要加换行符
        file.close();
    }

    file.open(QIODevice::WriteOnly|QIODevice::Append);
    QTextStream txtOutPut(&file);



    txtOutPut << writeDatas;
    file.flush();
    file.close();


}

//void MainForm::print_sensor_data(bme280_data *comp_data)
//{
//#ifdef BME280_FLOAT_ENABLE
//    printf("temperature:%0.2f*C   pressure:%0.2fhPa   humidity:%0.2f%%\r\n",comp_data->temperature, comp_data->pressure/100, comp_data->humidity);

//    //直接显示到状态栏
//    QString str = "当前温度：" + QString::number(comp_data->temperature) + " ,当前湿度：" +  QString::number(comp_data->humidity);
//    ui->statusBar->showMessage(str);

//#else
//    printf("temperature:%ld*C   pressure:%ldhPa   humidity:%ld%%\r\n",comp_data->temperature, comp_data->pressure/100, comp_data->humidity);
//#endif
//}

void MainForm::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0) {
        m_chart->zoom(1.1);
    } else {
        m_chart->zoom(10.0/11);
    }

    QWidget::wheelEvent(event);


}
//模拟测试按钮槽函数
void MainForm::on_simulationTestBtn_clicked()
{
    //弹出框，输入文件名，存为XXX.csv文件
    bool isOk;
    QString csvFileName = QInputDialog::getText(this,"输入文件名","请输入csv文件名",
                                                QLineEdit::Normal,
                                                "比如：xxx",
                                                &isOk);
    //如果没有数据输入，就弹窗提醒，并退出执行
    if(!isOk)
    {
        QMessageBox::information(this,QStringLiteral("提醒"),QStringLiteral("请输入要显示的内容..."),QMessageBox::Ok);
        return;
    }

    //csv文件路径拼接
    m_csvFilePath = m_csvFilePath + "/" + csvFileName + ".csv";
    qDebug() << "m_csvFilePath:" << m_csvFilePath;

    //这个地方打开定时器
    m_countdownFirstGenerationTimer->start();

    m_dataThread->start();

}

//直接采集
void MainForm::on_startBtn_clicked()
{

    //弹出框，输入文件名，存为XXX.csv文件
    bool isOk;
    QString csvFileName = QInputDialog::getText(this,"输入文件名","请输入csv文件名",
                                                QLineEdit::Normal,
                                                "比如：xxx",
                                                &isOk);
    //如果没有数据输入，就弹窗提醒，并退出执行
    if(!isOk)
    {
        QMessageBox::information(this,QStringLiteral("提醒"),QStringLiteral("请输入要显示的内容..."),QMessageBox::Ok);
        return;
    }

    //csv文件路径拼接
    m_csvFilePath = m_csvFilePath + "/" + csvFileName + ".csv";
    qDebug() << "m_csvFilePath:" << m_csvFilePath;


    //这个地方打开定时器
    //m_countdownFirstGenerationTimer->start();

    //m_dataThread->start();



}



void MainForm::on_resetBtn_clicked()
{
    //重置的情况下

    //先暂停画图，也就是停止传入数据，然后给绘图Vector清空
    m_data1.clear();
    m_data2.clear();
    m_data3.clear();
    m_data4.clear();
    m_data5.clear();
    m_data6.clear();
    m_data7.clear();
    m_data8.clear();
    m_data9.clear();
    m_data10.clear();
    m_data11.clear();
    m_data12.clear();
    m_data13.clear();
    m_data14.clear();
    m_data15.clear();
    m_data16.clear();

    qApp->closeAllWindows();

    QProcess::startDetached(qApp->applicationFilePath(), QStringList());
}
//更新图表数据槽函数
//8个参数 分别是8条曲线的数据
void MainForm::updateChartDataSlot(float floatNum1, float floatNum2, float floatNum3, float floatNum4, float floatNum5, float floatNum6, float floatNum7, float floatNum8,float floatNum9,float floatNum10,float floatNum11,float floatNum12,float floatNum13,float floatNum14,float floatNum15,float floatNum16)
{
    qDebug()  << "zhixingle9XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX..................... ";

    qDebug()  << "zfloatNum1 : .................... "  << floatNum1;
    qDebug()  << "zfloatNum2 : .................... "  << floatNum2;
    qDebug()  << "zfloatNum3 : .................... "  << floatNum3;
    qDebug()  << "zfloatNum4 : .................... "  << floatNum4;
    qDebug()  << "zfloatNum5 : .................... "  << floatNum5;
    qDebug()  << "zfloatNum6 : .................... "  << floatNum6;
    qDebug()  << "zfloatNum7 : .................... "  << floatNum7;
    qDebug()  << "zfloatNum8 : .................... "  << floatNum8;
    qDebug()  << "zfloatNum9 : .................... "  << floatNum9;
    qDebug()  << "zfloatNum10 : .................... "  << floatNum10;
    qDebug()  << "zfloatNum11 : .................... "  << floatNum11;
    qDebug()  << "zfloatNum12 : .................... "  << floatNum12;
    qDebug()  << "zfloatNum13 : .................... "  << floatNum13;
    qDebug()  << "zfloatNum14 : .................... "  << floatNum14;
    qDebug()  << "zfloatNum15 : .................... "  << floatNum15;
    qDebug()  << "zfloatNum16 : .................... "  << floatNum16;

    //触发了
    QVector<QPointF> oldData1 = m_lineSeries1->pointsVector();

    QVector<QPointF> oldData2 = m_lineSeries2->pointsVector();

    QVector<QPointF> oldData3 = m_lineSeries3->pointsVector();

    QVector<QPointF> oldData4 = m_lineSeries4->pointsVector();

    QVector<QPointF> oldData5 = m_lineSeries5->pointsVector();

    QVector<QPointF> oldData6 = m_lineSeries6->pointsVector();

    QVector<QPointF> oldData7 = m_lineSeries7->pointsVector();

    QVector<QPointF> oldData8 = m_lineSeries8->pointsVector();

    QVector<QPointF> oldData9 = m_lineSeries9->pointsVector();

    QVector<QPointF> oldData10 = m_lineSeries10->pointsVector();

    QVector<QPointF> oldData11 = m_lineSeries11->pointsVector();

    QVector<QPointF> oldData12 = m_lineSeries12->pointsVector();

    QVector<QPointF> oldData13 = m_lineSeries13->pointsVector();

    QVector<QPointF> oldData14 = m_lineSeries14->pointsVector();

    QVector<QPointF> oldData15 = m_lineSeries15->pointsVector();

    QVector<QPointF> oldData16 = m_lineSeries16->pointsVector();

    //现在定的存2000个数据  这8条曲线一样，所以共用一个循环和判断就行
    if(oldData1.size() < 2000)
    {
        qDebug()  << "zhixingle1000XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX..................... ";
        m_data1.append(QPointF(oldData1.size(),floatNum1));
        m_data2.append(QPointF(oldData1.size(),floatNum2));
        m_data3.append(QPointF(oldData1.size(),floatNum3));
        m_data4.append(QPointF(oldData1.size(),floatNum4));
        m_data5.append(QPointF(oldData1.size(),floatNum5));
        m_data6.append(QPointF(oldData1.size(),floatNum6));
        m_data7.append(QPointF(oldData1.size(),floatNum7));
        m_data8.append(QPointF(oldData1.size(),floatNum8));
        m_data9.append(QPointF(oldData1.size(),floatNum9));
        m_data10.append(QPointF(oldData1.size(),floatNum10));
        m_data11.append(QPointF(oldData1.size(),floatNum11));
        m_data12.append(QPointF(oldData1.size(),floatNum12));
        m_data13.append(QPointF(oldData1.size(),floatNum13));
        m_data14.append(QPointF(oldData1.size(),floatNum14));
        m_data15.append(QPointF(oldData1.size(),floatNum15));
        m_data16.append(QPointF(oldData1.size(),floatNum16));

    }
    else
    {
        /* 添加之前老的数据到新的vector中，不复制最前的数据，即每次替换前面的数据
         * 由于这里每次只添加1个数据，所以为1，使用时根据实际情况修改
         */
        for (int i = 1; i < oldData1.size(); ++i)
        {
            m_data1.append(QPointF(i - 1, oldData1.at(i).y()));
            m_data2.append(QPointF(i - 1, oldData2.at(i).y()));
            m_data3.append(QPointF(i - 1, oldData3.at(i).y()));
            m_data4.append(QPointF(i - 1, oldData4.at(i).y()));
            m_data5.append(QPointF(i - 1, oldData5.at(i).y()));
            m_data6.append(QPointF(i - 1, oldData6.at(i).y()));
            m_data7.append(QPointF(i - 1, oldData7.at(i).y()));
            m_data8.append(QPointF(i - 1, oldData8.at(i).y()));
            m_data9.append(QPointF(i - 1, oldData9.at(i).y()));
            m_data10.append(QPointF(i - 1, oldData10.at(i).y()));
            m_data11.append(QPointF(i - 1, oldData11.at(i).y()));
            m_data12.append(QPointF(i - 1, oldData12.at(i).y()));
            m_data13.append(QPointF(i - 1, oldData13.at(i).y()));
            m_data14.append(QPointF(i - 1, oldData14.at(i).y()));
            m_data15.append(QPointF(i - 1, oldData15.at(i).y()));
            m_data16.append(QPointF(i - 1, oldData16.at(i).y()));
        }
    }
    qDebug()  << "zhixingle1000XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX11111111111111111111..................... ";
    m_lineSeries1->replace(m_data1);
    m_lineSeries2->replace(m_data2);
    m_lineSeries3->replace(m_data3);
    m_lineSeries4->replace(m_data4);
    m_lineSeries5->replace(m_data5);
    m_lineSeries6->replace(m_data6);
    m_lineSeries7->replace(m_data7);
    m_lineSeries8->replace(m_data8);
    m_lineSeries9->replace(m_data9);
    m_lineSeries10->replace(m_data10);
    m_lineSeries11->replace(m_data11);
    m_lineSeries12->replace(m_data12);
    m_lineSeries13->replace(m_data13);
    m_lineSeries14->replace(m_data14);
    m_lineSeries15->replace(m_data15);
    m_lineSeries16->replace(m_data16);

    m_isShow = false;
}
//模拟测试定时器槽函数
void MainForm::simulationTestTimerSlot()
{

}

//倒计时定时器槽函数 第一阶段 点击开始按钮到弹窗
void MainForm::countdownFirstGenerationTimerSlot()
{
    //完成倒计时部分
    if(m_countdownFirstTimeInt != 0)
    {
        m_countdownFirstTimeInt = m_countdownFirstTimeInt - 1;
        QString showStr = "倒计时：" + QString::number(m_countdownFirstTimeInt) + " 秒";
        ui->statusBar->showMessage(showStr);

        //        if(QObject::sender() == m_countdownFirstGenerationTimer)
        //        {
        //            //模拟产生8组数据用来画图展示
        //            //根据图片上的要求，数据是 8为数字，范围在0-3.5直接。所有从左向右第一位后面肯定有个小数点
        //            QTime time;
        //            time= QTime::currentTime();
        //            qsrand(time.msec() + time.second() * 1000);
        //            int randInt = qrand() % 10000000;    //产生10000000以内的随机数
        //            QString randStr = QString::number(randInt);

        //            qDebug() << "原始randStr:" << randStr;


        //            QString str1 = randStr.left(1);

        //            qDebug() << "str1:" << str1;
        //            QString str2 = randStr.right(7);
        //            qDebug() << "str2:" << str2;

        //            randStr = str1 + "." + str2;

        //            qDebug() << "处理后randStr:" << randStr;

        //            float randFloat = randStr.toFloat();

        //            //因为是模拟，所有产生1个随机数算曲线1的，剩下的2-8，分别减去不同的数得到即可

        //            float randFloat2 = randFloat - 0.0381;
        //            float randFloat3 = randFloat - 0.0181;
        //            float randFloat4 = randFloat - 0.0251;
        //            float randFloat5 = randFloat - 0.1381;
        //            float randFloat6 = randFloat - 0.0321;
        //            float randFloat7 = randFloat - 0.0781;
        //            float randFloat8 = randFloat - 0.0981;

        //            //把数据通过信号与槽发送出去
        //            emit updateChartDataSignalFunction(randFloat,randFloat2,randFloat3,randFloat4,randFloat5,randFloat6,randFloat7,randFloat8);

        //        }

    }
    else
    {
        m_showForm->show();
        m_countdownFirstGenerationTimer->stop();
        //这个地方进行第二次处理
        m_countdownSecondGenerationTimer->start();
    }
}
//倒计时定时器槽函数 第二阶段 弹窗后继续采集2分钟数据
void MainForm::countdownSecondGenerationTimerSlot()
{
    //完成倒计时部分
    if(m_countdownSecondTimeInt != 0)
    {
        m_countdownSecondTimeInt = m_countdownSecondTimeInt - 1;
        QString showStr = "倒计时：" + QString::number(m_countdownSecondTimeInt) + " 秒";
        ui->statusBar->showMessage(showStr);

        //        if(QObject::sender() == m_countdownSecondGenerationTimer)
        //        {
        //            //模拟产生8组数据用来画图展示
        //            //根据图片上的要求，数据是 8为数字，范围在0-3.5直接。所有从左向右第一位后面肯定有个小数点
        //            QTime time;
        //            time= QTime::currentTime();
        //            qsrand(time.msec() + time.second() * 1000);
        //            int randInt = qrand() % 10000000;    //产生10000000以内的随机数
        //            QString randStr = QString::number(randInt);

        //            qDebug() << "原始randStr:" << randStr;


        //            QString str1 = randStr.left(1);

        //            qDebug() << "str1:" << str1;
        //            QString str2 = randStr.right(7);
        //            qDebug() << "str2:" << str2;

        //            randStr = str1 + "." + str2;

        //            qDebug() << "处理后randStr:" << randStr;

        //            float randFloat = randStr.toFloat();

        //            //因为是模拟，所有产生1个随机数算曲线1的，剩下的2-8，分别减去不同的数得到即可

        //            float randFloat2 = randFloat - 0.0381;
        //            float randFloat3 = randFloat - 0.0181;
        //            float randFloat4 = randFloat - 0.0251;
        //            float randFloat5 = randFloat - 0.1381;
        //            float randFloat6 = randFloat - 0.0321;
        //            float randFloat7 = randFloat - 0.0781;
        //            float randFloat8 = randFloat - 0.0981;

        //            //把数据通过信号与槽发送出去
        //            emit updateChartDataSignalFunction(randFloat,randFloat2,randFloat3,randFloat4,randFloat5,randFloat6,randFloat7,randFloat8);

        //        }

    }
    else
    {
        //发送采集完成的信号
        //停止采集
        m_countdownSecondGenerationTimer->stop();

        qDebug() << "开始作图11...";

        //停止采集

        //线程停止
        m_dataThread->threadStop();

        //m_dataThread->terminate();
        m_dataThread->wait();

       // m_dataThread->quit();

//        bcm2835_spi_end();
//        bcm2835_close();

        //定时器停止后，把数据从csv文件里取出来
        QFile file(m_csvFilePath);

        qDebug() << "开始作图22...";

        if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            qDebug() << "打不开CSV文件...";
            return;
        }
        qDebug() << "开始作图33...";
        CsvFileReadWirte_xy csv(&file);
        QStringList res;
        int keyInt = 0;
        while(!csv.atEnd())
        {
            res = csv.readAndAnalysisCsvLine();
            m_csvMap.insert(keyInt,res);
            keyInt++;
            //qDebug().noquote() << res;
        }

        qDebug() << "map最大存储量是：" << keyInt;
        //通过keyInt知道map的最大存储值，
        //获取后10个值

        float valueF1 = 0.0,valueF2 = 0.0,valueF3 = 0.0,valueF4 = 0.0,valueF5 = 0.0,valueF6 = 0.0,valueF7 = 0.0,valueF8 = 0.0,valueF9 = 0.0,valueF10 = 0.0,valueF11 = 0.0,valueF12 = 0.0,valueF13 = 0.0,valueF14 = 0.0,valueF15 = 0.0,valueF16 = 0.0;

        for(int i = 0;i < 10;i++)
        {
            QStringList strList = m_csvMap[keyInt - 1 - i];
            QString valueStr1 = strList[0];
            valueF1 += valueStr1.toFloat();
            QString valueStr2 = strList[1];
            valueF2 += valueStr2.toFloat();
            /*QStringList strList = m_csvMap[keyInt - 2 ];
            QString valueStr2 = strList[0];
            valueF2 = valueStr2.toFloat();
            QString valueStr10 = strList[1];
            valueF10 = valueStr10.toFloat();
            QStringList strList = m_csvMap[keyInt - 3 ];
            QString valueStr3 = strList[0];
            valueF3 = valueStr3.toFloat();
            QString valueStr11 = strList[1];
            valueF11 = valueStr11.toFloat();
            QStringList strList = m_csvMap[keyInt - 4 ];
            QString valueStr4 = strList[0];
            valueF4 = valueStr4.toFloat();
            QString valueStr12 = strList[1];
            valueF12 = valueStr12.toFloat();
            QStringList strList = m_csvMap[keyInt - 5 ];
            QString valueStr5 = strList[0];
            valueF5 = valueStr5.toFloat();
            QString valueStr13 = strList[1];
            valueF13 = valueStr13.toFloat();
            QStringList strList = m_csvMap[keyInt - 6 ];
            QString valueStr6 = strList[0];
            valueF6 = valueStr6.toFloat();
            QString valueStr14 = strList[1];
            valueF14 = valueStr14.toFloat();
            QStringList strList = m_csvMap[keyInt - 7 ];
            QString valueStr7 = strList[0];
            valueF7 = valueStr7.toFloat();
            QString valueStr15 = strList[1];
            valueF15 = valueStr15.toFloat();
            QStringList strList = m_csvMap[keyInt - 8 ];
            QString valueStr8 = strList[0];
            valueF8 = valueStr8.toFloat();
            QString valueStr16 = strList[1];
            valueF16 = valueStr16.toFloat();*/
            QString valueStr3 = strList[2];
            valueF3 += valueStr3.toFloat();
            QString valueStr4 = strList[3];
            valueF4 += valueStr4.toFloat();
            QString valueStr5 = strList[4];
            valueF5 += valueStr5.toFloat();
            QString valueStr6 = strList[5];
            valueF6 += valueStr6.toFloat();
            QString valueStr7 = strList[6];
            valueF7 += valueStr7.toFloat();
            QString valueStr8 = strList[7];
            valueF8 += valueStr8.toFloat();
            QString valueStr9 = strList[8];
            valueF9 += valueStr9.toFloat();
            QString valueStr10 = strList[9];
            valueF10 += valueStr10.toFloat();
            QString valueStr11 = strList[10];
            valueF11 += valueStr11.toFloat();
            QString valueStr12 = strList[11];
            valueF12 += valueStr12.toFloat();
            QString valueStr13 = strList[12];
            valueF13 += valueStr13.toFloat();
            QString valueStr14 = strList[13];
            valueF14 += valueStr14.toFloat();
            QString valueStr15 = strList[14];
            valueF15 += valueStr15.toFloat();
            QString valueStr16 = strList[15];
            valueF16 += valueStr16.toFloat();
        }

        //得到后10位的平均数
        valueF1 = valueF1 / 10;
        valueF2 = valueF2 / 10;
        valueF3 = valueF3 / 10;
        valueF4 = valueF4 / 10;
        valueF5 = valueF5 / 10;
        valueF6 = valueF6 / 10;
        valueF7 = valueF7 / 10;
        valueF8 = valueF8 / 10;
        valueF9 = valueF9 / 10;
        valueF10 = valueF10 / 10;
        valueF11 = valueF11 / 10;
        valueF12 = valueF12 / 10;
        valueF13 = valueF13 / 10;
        valueF14 = valueF14 / 10;
        valueF15 = valueF15 / 10;
        valueF16 = valueF16 / 10;
        qDebug() << "第一组平均数：" << valueF1 << valueF2 << valueF3 << valueF4<< valueF5 << valueF6  << valueF7 << valueF8 << valueF9 << valueF10 << valueF11 << valueF12 << valueF13 << valueF14 << valueF15 << valueF16;


        //取开始1分钟的10组数据
        //这个地方设计个小算法
        //原理是这样的：
        //现在的数据采集为4分钟内的数据，所以把总数据组数除以4就可以得到1分钟的数据量，
        //然后求这个数据量范围的随机数，取出10组数据  考虑小于等10的情况

        float valueF_11 = 0.0, valueF22 = 0.0,valueF33 = 0.0,valueF44 = 0.0,valueF55 = 0.0,valueF66 = 0.0,valueF77 = 0.0,valueF88 = 0.0,valueF99 = 0.0,valueF100 = 0.0,valueF110 = 0.0,valueF120 = 0.0,valueF130 = 0.0,valueF140 = 0.0,valueF150 = 0.0,valueF160 = 0.0;

          for(int i = 0;i < 10;i++)
          {
            QStringList strList = m_csvMap[i];
            QString valueStr1 = strList[0];
            valueF_11 += valueStr1.toFloat();
            QString valueStr2 = strList[1];
            valueF22 += valueStr2.toFloat();
            /*QStringList strList = m_csvMap[1];
            QString valueStr2 = strList[0];
            valueF22 = valueStr2.toFloat();
            QString valueStr10 = strList[1];
            valueF100 = valueStr10.toFloat();
            QStringList strList = m_csvMap[2];
            QString valueStr3 = strList[0];
            valueF33 = valueStr3.toFloat();
            QString valueStr11 = strList[1];
            valueF110 = valueStr11.toFloat();
            QStringList strList = m_csvMap[3];
            QString valueStr4 = strList[0];
            valueF44 = valueStr4.toFloat();
            QString valueStr12 = strList[1];
            valueF120 = valueStr12.toFloat();
            QStringList strList = m_csvMap[4];
            QString valueStr5 = strList[0];
            valueF55 = valueStr5.toFloat();
            QString valueStr13 = strList[1];
            valueF130 = valueStr13.toFloat();
            QStringList strList = m_csvMap[5];
            QString valueStr6 = strList[0];
            valueF66 = valueStr6.toFloat();
            QString valueStr14 = strList[1];
            valueF140 = valueStr14.toFloat();
            QStringList strList = m_csvMap[6];
            QString valueStr7 = strList[0];
            valueF77 = valueStr7.toFloat();
            QString valueStr15 = strList[1];
            valueF150 = valueStr15.toFloat();
            QStringList strList = m_csvMap[7];
            QString valueStr8 = strList[0];
            valueF88 = valueStr8.toFloat();
            QString valueStr16 = strList[1];
            valueF160 = valueStr16.toFloat();*/
            QString valueStr3 = strList[2];
            valueF33 += valueStr3.toFloat();
            QString valueStr4 = strList[3];
            valueF44 += valueStr4.toFloat();
            QString valueStr5 = strList[4];
            valueF55 += valueStr5.toFloat();
            QString valueStr6 = strList[5];
            valueF66 += valueStr6.toFloat();
            QString valueStr7 = strList[6];
            valueF77 += valueStr7.toFloat();
            QString valueStr8 = strList[7];
            valueF88 += valueStr8.toFloat();
            QString valueStr9 = strList[8];
            valueF99 += valueStr9.toFloat();
            QString valueStr10 = strList[9];
            valueF100 += valueStr10.toFloat();
            QString valueStr11 = strList[10];
            valueF110 += valueStr11.toFloat();
            QString valueStr12 = strList[11];
            valueF120 += valueStr12.toFloat();
            QString valueStr13 = strList[12];
            valueF130 += valueStr13.toFloat();
            QString valueStr14 = strList[13];
            valueF140 += valueStr14.toFloat();
            QString valueStr15 = strList[14];
            valueF150 += valueStr15.toFloat();
            QString valueStr16 = strList[15];
            valueF160 += valueStr16.toFloat();
            }

        //得到后10位的平均数
        valueF_11 = valueF_11 / 10;
        valueF22 = valueF22 / 10;
        valueF33 = valueF33 / 10;
        valueF44 = valueF44 / 10;
        valueF55 = valueF55 / 10;
        valueF66 = valueF66 / 10;
        valueF77 = valueF77 / 10;
        valueF88 = valueF88 / 10;
        valueF99 = valueF99 / 10;
        valueF100 = valueF100 / 10;
        valueF110 = valueF110 / 10;
        valueF120 = valueF120 / 10;
        valueF130 = valueF130 / 10;
        valueF140 = valueF140 / 10;
        valueF150 = valueF150 / 10;
        valueF160 = valueF160 / 10;

        qDebug() << "第二组平均数：" << valueF_11 << valueF22 << valueF33 << valueF44<< valueF55 << valueF66  << valueF77 << valueF88 << valueF99 << valueF100 << valueF110 << valueF120 << valueF130 << valueF140 << valueF150 << valueF160;


        //最后10组数平均数减去前面1分钟的10组数据的平均数，得到最后作图值

        valueF1 = valueF1 - valueF_11;
        valueF2 = valueF2 - valueF22;
        valueF3 = valueF3 - valueF33;
        valueF4 = valueF4 - valueF44;
        valueF5 = valueF5 - valueF55;
        valueF6 = valueF6 - valueF66;
        valueF7 = valueF7 - valueF77;
        valueF8 = valueF8 - valueF88;
        valueF9 = valueF9 - valueF99;
        valueF10 = valueF10 - valueF100;
        valueF11 = valueF11 - valueF110;
        valueF12 = valueF12 - valueF120;
        valueF13 = valueF13 - valueF130;
        valueF14 = valueF14 - valueF140;
        valueF15 = valueF15 - valueF150;
        valueF16 = valueF16 - valueF160;
    //    valueF1 = 4.76491;
    //    valueF2 = 4.75639;
    //    valueF3 = 4.5254;
    //    valueF4 = 4.75896;
    //    valueF5 = 4.76193;
    //    valueF6 = 1.7874;
    //    valueF7 = 1.72434;
    //    valueF8 = 4.76923;

        qDebug() << "得到差值：" << valueF1 << valueF2  << valueF3 << valueF4 << valueF5 << valueF6  << valueF7 << valueF8 << valueF9 << valueF10 << valueF11 << valueF12 << valueF13 << valueF14 << valueF15 << valueF16;


        //求绝对值
        valueF1 = qAbs(valueF1);
        valueF2 = qAbs(valueF2);
        valueF3 = qAbs(valueF3);
        valueF4 = qAbs(valueF4);
        valueF5 = qAbs(valueF5);
        valueF6 = qAbs(valueF6);
        valueF7 = qAbs(valueF7);
        valueF8 = qAbs(valueF8);
        valueF9 = qAbs(valueF9);
        valueF10 = qAbs(valueF10);
        valueF11 = qAbs(valueF_11);
        valueF12 = qAbs(valueF12);
        valueF13 = qAbs(valueF13);
        valueF14 = qAbs(valueF14);
        valueF15 = qAbs(valueF15);
        valueF16 = qAbs(valueF16);



        qDebug() << "得到差值取绝对值：" << valueF1 << valueF2 << valueF3 << valueF4 << valueF5 << valueF6  << valueF7 << valueF8 << valueF9 << valueF10 << valueF11 << valueF12 << valueF13 << valueF14 << valueF15 << valueF16;

        //进行绘图
        //根据规则取对应的值，先取后十位数据
        ui->drawWidget->init(valueF1,valueF2,valueF3,valueF4,valueF5,valueF6,valueF7,valueF8,valueF9,valueF10,valueF11,valueF12,valueF13,valueF14,valueF15,valueF16);
        ui->drawWidget->isClear = false;
        ui->drawWidget->update();
    }
}

void MainForm::updateListDataSlot(float floatNum1, float floatNum2, float floatNum3, float floatNum4, float floatNum5, float floatNum6, float floatNum7, float floatNum8, float floatNum9, float floatNum10, float floatNum11, float floatNum12, float floatNum13, float floatNum14, float floatNum15, float floatNum16)
{

    QString strText1 =  QString::number(floatNum1) + ","+QString::number(floatNum2)+ ","+QString::number(floatNum3)+ ","+QString::number(floatNum4)
            + ","+QString::number(floatNum5)+ ","+QString::number(floatNum6)+ ","+QString::number(floatNum7)+ ","+QString::number(floatNum8)+ ","+ QString::number(floatNum9) + ","+QString::number(floatNum10)+ ","+QString::number(floatNum11)+ ","+QString::number(floatNum12)
            + ","+QString::number(floatNum13)+ ","+QString::number(floatNum14)+ ","+QString::number(floatNum15)+ ","+QString::number(floatNum16)+ "\n";

    ui->dataDisplayTextEdit->setText(strText1);

    //8组数据写到文件里面
    //数据保存下来

    QFile file(m_csvFilePath);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        return;
    }
    CsvFileReadWirte_xy csv(&file);


    csv << floatNum1 << floatNum2 << floatNum3 << floatNum4 << floatNum5 << floatNum6  << floatNum7 << floatNum8 << floatNum9 << floatNum10 << floatNum11 << floatNum12 << floatNum13 << floatNum14 << floatNum15 << floatNum16 << endl;

    file.close();
}


//停止按钮槽函数
void MainForm::on_stopBtn_clicked()
{
    m_isStop = true;

    m_isQTimerStart  = false;

    m_csvFileName.isEmpty();

//    bcm2835_spi_end();
//    bcm2835_close();

    if(m_simulationTestTimer->isActive())
    {
        m_simulationTestTimer->stop();
    }
}

void MainForm::on_actionSimulationTest_triggered()
{
    on_simulationTestBtn_clicked();
}

void MainForm::on_actionStart_triggered()
{
    on_startBtn_clicked();
}

void MainForm::on_actionStop_triggered()
{
    on_stopBtn_clicked();
}

void MainForm::on_actionReset_triggered()
{
    on_resetBtn_clicked();
}

void MainForm::on_actionExit_triggered()
{
    this->close();
}

void MainForm::on_actionHelp_triggered()
{
    QMessageBox::about(this,tr("About E-NOSE CONCEPT DISPLAY PLATFORM Ver 1.07"),
                       tr("<h2>E-NOSE CONCEPT DISPLAY PLATFORM  1.0.7</h2>"
                          "<p>Coptright &copy;2018 Software Inc."
                          "<p>the E-NOSECONCEPTDISPLAYPLATFORM is a samll sofeware"));
}

void MainForm::on_actionAboutQt_triggered()
{
    qApp->aboutQt();
}


void MainForm::on_showT_H_Btn_clicked()
{





//    struct bme280_dev dev;
//    int8_t rslt = BME280_OK;

//    if ((fd = open(IIC_Dev, O_RDWR)) < 0) {
//        //printf("Failed to open the i2c bus %s", argv[1]);
//        exit(1);
//    }
//    if (ioctl(fd, I2C_SLAVE, 0x77) < 0) {
//        printf("Failed to acquire bus access and/or talk to slave.\n");
//        exit(1);
//    }
//    //dev.dev_id = BME280_I2C_ADDR_PRIM;//0x76
//    dev.dev_id = BME280_I2C_ADDR_SEC; //0x77
//    dev.intf = BME280_I2C_INTF;
//    dev.read = user_i2c_read;
//    dev.write = user_i2c_write;
//    dev.delay_ms = user_delay_ms;

//    rslt = bme280_init(&dev);
//    printf("\r\n BME280 Init Result is:%d \r\n",rslt);
//    //stream_sensor_data_forced_mode(&dev);
//    stream_sensor_data_normal_mode(&dev,ui);













}

void MainForm::tAndHqtimerSlot()
{
    ///////

//    struct bme280_dev dev;
//    int8_t rslt = BME280_OK;

//    if ((fd = open(IIC_Dev, O_RDWR)) < 0) {
//        //printf("Failed to open the i2c bus %s", argv[1]);
//        exit(1);
//    }
//    if (ioctl(fd, I2C_SLAVE, 0x77) < 0) {
//        printf("Failed to acquire bus access and/or talk to slave.\n");
//        exit(1);
//    }
//    //dev.dev_id = BME280_I2C_ADDR_PRIM;//0x76
//    dev.dev_id = BME280_I2C_ADDR_SEC; //0x77
//    dev.intf = BME280_I2C_INTF;
//    dev.read = user_i2c_read;
//    dev.write = user_i2c_write;
//    dev.delay_ms = user_delay_ms;

//    rslt = bme280_init(&dev);
//    printf("\r\n BME280 Init Result is:%d \r\n",rslt);
//    //stream_sensor_data_forced_mode(&dev);
//    stream_sensor_data_normal_mode(&dev,ui);

}

void MainForm::on_saveImageBtn_clicked()
{
    QPixmap pix = QPixmap::grabWidget(ui->drawWidget);
    //定义图片名：

    QString strFile =  QDateTime::currentDateTime().toString("yyyyMMddHHmmss") + ".png";



    //再通过 .save() 将pix保存至指定的文件夹中

    QString fileName = QFileDialog::getSaveFileName(this, "保存图片", strFile, "PNG (*.png);;BMP (*.bmp);;JPEG (*.jpg *.jpeg)");
        if (!fileName.isNull())
        {
            pix.save(fileName);
        }

}

