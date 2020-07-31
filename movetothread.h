#ifndef MOVETOTHREAD_H
#define MOVETOTHREAD_H
#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QDebug>

class Dummy:public QObject
{
    Q_OBJECT
public:
    Dummy(QObject* parent=0):QObject(parent)     {}
public slots:
    void updateChartDataSignalFunction()
    {
        emit updateChartDataSignalFunction(float floatNum4,float floatNum5,float floatNum6,float floatNum7,float floatNum8);
    }
signals:
    void updateChartDataSignalFunction(float floatNum5,float floatNum6,float floatNum7,float floatNum8);
};

class Object:public QObject
{
    Q_OBJECT
public:
    Object(){}
public slots:
    void slot()
    {
           uint8_t id;
            int32_t adc[8];
            int32_t volt[8];
            uint8_t i;

            uint8_t ch_num;
            int32_t iTemp;
            uint8_t buf[3];


            if(!m_isQTimerStart)
            {
                if (!bcm2835_init())
                    return;

                qDebug()  << "zhixingle1111111111111111111111111111111111111111111....................... ";

                /*
              bcm2835_spi_begin();
              bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_LSBFIRST );      // The default
              bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);                   // The default
              bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_1024); // The default
            */

                bcm2835_spi_begin();
                qDebug()  << "zhixingle3....................... ";
                bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);   //default
                qDebug()  << "zhixingle4....................... ";

                bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);                //default
                qDebug()  << "zhixingle5...................... ";
                bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256);//default
                qDebug()  << "zhixingle6...................... ";
                bcm2835_gpio_fsel(SPICS, BCM2835_GPIO_FSEL_OUTP);//
                bcm2835_gpio_write(SPICS, HIGH);
                bcm2835_gpio_fsel(DRDY, BCM2835_GPIO_FSEL_INPT);
                bcm2835_gpio_set_pud(DRDY, BCM2835_GPIO_PUD_UP);

                //qDebug()  << "zhixingle222222222222....................... ";

                //ADS1256_WriteReg(REG_MUX,0x01);
                //ADS1256_WriteReg(REG_ADCON,0x20);
                // ADS1256_CfgADC(ADS1256_GAIN_1, ADS1256_15SPS);
                id = ADS1256_ReadChipID();
                //printf("\r\n");
                //printf("ID=\r\n");
                if (id != 3)
                {
                    //printf("Error, ASD1256 Chip ID = 0x%d\r\n", (int)id);
                }
                else
                {
                    //printf("Ok, ASD1256 Chip ID = 0x%d\r\n", (int)id);
                }
                ADS1256_CfgADC(ADS1256_GAIN_1, ADS1256_15SPS);
                ADS1256_StartScan(0);
                //qDebug()  << "zhixingle7...................... ";
                ch_num = 4;
                //if (ADS1256_Scan() == 0)
                //{
                //continue;

                //}

                m_isQTimerStart = true;
            }



            //定义个数组，用来存当前的数据
            QList<float> intList;
            QStringList strLsit;
            //QString str5 ;
            float str4Flaot ;
            while(m_isStopThread)
            {
                //是否停止，如果停止，那么就直接跳出循环
                if(m_isStop)
                {
                    // break;
                }

                while((ADS1256_Scan() == 0));
                for (i = 0; i < ch_num; i++)
                {
                    adc[i] = ADS1256_GetAdc(i);
                    volt[i] = (adc[i] * 100) / 167;

                }


                        for(i = 3; i < ch_num; i++)
                        {
                            qDebug()  << "volt[i]::"  << i << " ge ::" << volt[i];
                        }

                for (i = 0; i < ch_num; i++)
                {
                    buf[0] = ((uint32_t)adc[i] >> 16) & 0xFF;
                    buf[1] = ((uint32_t)adc[i] >> 8) & 0xFF;
                    buf[2] = ((uint32_t)adc[i] >> 0) & 0xFF;
                    //            printf("%d=%02X%02X%02X, %8ld", (int)i, (int)buf[0],
                    //                    (int)buf[1], (int)buf[2], (long)adc[i]);

                    iTemp = volt[i];	/* uV  */
                    if (iTemp < 0)
                    {
                        iTemp = -iTemp;
                        //printf(" (-%ld.%03ld %03ld Vaaaxxx) \r\n", iTemp /1000000, (iTemp%1000000)/1000, iTemp%1000);

                        QString str1 = QString::number(iTemp /1000000);
                        QString str2 = QString::number((iTemp%1000000)/1000);
                        QString str3 = QString::number(iTemp%1000);

                        QString str4 = str1 + "."  + str2 + str3;

                        str4 = str4.left(7);

                        //str5 = str4 + "::" +QString::number((int)ch_num);

                        str4Flaot  = str4.toFloat();

                    }
                    else
                    {
                        //printf(" ( %ld.%03ld %03ld Vaaaxxx) \r\n", iTemp /1000000, (iTemp%1000000)/1000, iTemp%1000);

                        QString str1 = QString::number(iTemp /1000000);
                        QString str2 = QString::number((iTemp%1000000)/1000);
                        QString str3 = QString::number(iTemp%1000);

                        QString str4 = str1 + "."  + str2 + str3;

                        str4 = str4.left(7);

                        //str5 = str4 + "::" +QString::number((int)ch_num);

                        str4Flaot = str4.toFloat();
                    }
                    //把数据放到list容器里
                    if(intList.count() < 9)
                    {
                        intList.append(str4Flaot);
                        qDebug()  << "zhixingle7...................... ";
                        //strLsit.append(str5);
                    }

                    //如果说存够8个了，那就直接把数据发送出去
                    if(intList.count() == 4)
                    {
                        qDebug()  << "zhixingle8..................... ";

                        for(int i = 0;i < intList.count();i++)
                        {
                            qDebug()  << "shuju:::" << i << "  xxxx"        << intList[i];
                            //在这个里面进行判断，如果里面有数据等于0，那就不进行数据绘图
                            if(intList[i] < 0.1 || intList[i] > 5.5)
                            {
                                qDebug()  << "zhixingl9..................... ";
                                intList.clear();
                                strLsit.clear();
                                m_isSend = false;
                                break; //跳出循环
                            }
                            else
                            {
                                m_isSend = true;
                            }
                        }

                        if( m_isSend)
                        {
                            qDebug()  << "zhixingle10000..................... ";
                            m_isShow = true;
                            emit updateChartDataSignalFunction(intList[4],intList[5],intList[6],intList[7]);//,intList[8]);//,intList[9],intList[10],intList[11],intList[12],intList[13],intList[14],intList[15]);


                            //把容器清空
                            intList.clear();
                            strLsit.clear();
                            m_isSend = true;
                        }

                    }
                }
                // printf("\33[%dA", (int)ch_num);
                bsp_DelayUS(100000);
            }

}
};

#endif // MOVETOTHREAD_H
