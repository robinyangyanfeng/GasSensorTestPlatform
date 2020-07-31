
#include "radarform.h"
#include "ui_radarform.h"

RadarForm::RadarForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RadarForm)
{
    ui->setupUi(this);
    isClear = false;
}

RadarForm::~RadarForm()
{
    delete ui;
}

void RadarForm::setRadarType(RadarType radarType)
{
    m_radartype = radarType;
}

void RadarForm::setPointType(PointType pointType)
{
    m_pointstype = pointType;
}

void RadarForm::paintEvent(QPaintEvent *)
{
    QPainter *painter = new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->setRenderHint(QPainter::TextAntialiasing);

    //添加背景颜色
    //      painter->save();
    //      painter->setBrush(Qt::blue);
    //      painter->drawRect(0,0,width(),height());
    //      painter->restore();
    //////////////////////////////////////////////////////////////////////////

    if(!isClear)
    {
        drawRadarType(painter);
        drawAxisLabel(painter);
        drawLabel(painter);
        drawData(painter);
    }
    else
    {
        drawRadarType(painter);
        drawAxisLabel(painter);
        drawLabel(painter);
    }
}

void RadarForm::init(float randFloat, float randFloat2, float randFloat3, float randFloat4, float randFloat5, float randFloat6, float randFloat7, float randFloat8, float randFloat9, float randFloat10, float randFloat11, float randFloat12, float randFloat13, float randFloat14, float randFloat15, float randFloat16)
{
    setRadarType(Radar);
    setPointType(ScatterPoints);

    m_cylindernum = 5; //默认6圈
    m_lineangle = 22.5; //默认30度

    m_showAxisLabel = true;
    m_showLabel = true;

    m_Axis_min = 0;
    m_Axis_max = 2.5;

    m_AxisLabelList << "0" << "0.5" << "1.0" << "1.5" << "2.0" << "2.5";

    m_colorlist << QColor(136,68,255);
    m_colorlist << QColor(102,170,238);
    m_colorlist << QColor(146,39,143);
    m_colorlist << QColor(238,187,34);
    m_colorlist << QColor(238,187,34);
    m_colorlist << QColor(238,187,34);
    m_colorlist << QColor(141,198,63);
    m_colorlist << QColor(255,242,0);
    m_colorlist << QColor(100,68,25);
    m_colorlist << QColor(25,100,255);
    m_colorlist << QColor(167,39,143);
    m_colorlist << QColor(255,187,34);
    m_colorlist << QColor(238,187,255);
    m_colorlist << QColor(238,255,34);
    m_colorlist << QColor(141,255,63);
    m_colorlist << QColor(0,242,0);



    for (int i = 0 ; i < 360; i+= m_lineangle)
    {
        //m_labelList << QString("%1").arg(i);
        m_labelList << QString("%1").arg(i / 22);
    }
    //RadarData tlist1;
    RadarData tlist2;
    //QMap<qreal,qreal> datamap;
    QMap<qreal,qreal> datamap2;

    QList<float> listFloat;
    listFloat.append(randFloat);
    listFloat.append(randFloat2);
    listFloat.append(randFloat3);
    listFloat.append(randFloat4);
    listFloat.append(randFloat5);
    listFloat.append(randFloat6);
    listFloat.append(randFloat7);
    listFloat.append(randFloat8);
    listFloat.append(randFloat9);
    listFloat.append(randFloat10);
    listFloat.append(randFloat11);
    listFloat.append(randFloat12);
    listFloat.append(randFloat13);
    listFloat.append(randFloat14);
    listFloat.append(randFloat15);
    listFloat.append(randFloat16);

    for (int i = 0; i < 4;++i)
    {
        datamap2.insert (i*22,listFloat[i]);
        //datamap2.insert (i*45,i*10 + 4);
        //datamap.insert(i*45,i*5);
    }

    //tlist2.color = QColor(136,68,255);
    tlist2.color = QColor(Qt::red);
    //tlist1.color = QColor(146,39,143);;

    //tlist1.transitionDataMap = datamap;
    tlist2.transitionDataMap = datamap2;

    m_datamap.insert(0,tlist2);
    //m_datamap.insert(1,tlist1);
}

void RadarForm::drawRadarType(QPainter *painter)
{
    switch (m_radartype)
    {
    case Radar:
        drawRadar(painter);
        break;
    case Spider:
        drawSpider(painter);
        break;
    default:
        drawRadar(painter);
        break;
    }
}

void RadarForm::drawRadar(QPainter *painter)
{
    painter->save();
    QPen pen;
    QColor color(Qt::lightGray);
    pen.setWidthF(0.4);
    //pen.setColor(color/*.lighter(100)*/); //添加线的颜色
    painter->setPen(pen);

    QPainterPath looppath;
    int w = width();
    int h = height();

    int count = m_cylindernum;
    int radius = qMin(w,h);
    int step = radius/(count+1);  //加1是为了四周留出空间，写标签
    int x = w*0.2;
    int y = h*0.2;

    QRectF outrect;
    for (int i = 1; i < count+1; ++i)
    {
        radius = step*i;
        x = w/2-radius/2;
        y = h/2-radius/2;
        QRectF rect(x,y,radius,radius);
        looppath.addEllipse(rect);
        outrect = rect;
    }
    painter->drawPath(looppath);
    //////////////////////////////////////////////////////////////////////////

    if (m_lineangle == 0)
    {
        m_lineangle =90;
    }
    int linecount = 360 / m_lineangle;
    int x0 = w/2 ;
    int y0 = h/2;
    int newradius = outrect.height()/2;
    for (int i = 0 ; i < linecount; ++i)
    {
        int x1 = x0 + newradius*qSin(PI*2/linecount*i);
        int y1 = y0 + newradius*qCos(PI*2/linecount*i);

        painter->drawLine(x0,y0,x1,y1);
    }

    painter->restore();
}

void RadarForm::drawSpider(QPainter *painter)
{
    painter->save();
    QPen pen;
    QColor color(Qt::red);
    pen.setWidthF(0.4);
    //pen.setColor(color/*.lighter(100)*/); //添加线的颜色
    painter->setPen(pen);

    QPainterPath looppath;
    int w = width();
    int h = height();
    if (m_lineangle == 0)
    {
        m_lineangle =90;
    }

    int  laycount = m_cylindernum;  //默认是几层

    int count = 360/m_lineangle;
    int radius = qMin(w,h)/2;
    int x0 = w/2 ;
    int y0 = h/2;

    for (int j = 1 ; j < laycount+1; ++j)
    {
        int newradius = radius*j/(laycount+1);
        for (int i = 0 ; i < count; ++i)
        {
            int x1 = x0 + newradius*qSin(PI*2/count*i);
            int y1 = y0 + newradius*qCos(PI*2/count*i);
            painter->drawLine(x0,y0,x1,y1);

            int x2;
            int y2;
            if (i == count*2-1)
            {
                x2 = x0 + newradius*qSin(0);
                y2 = y0 + newradius*qCos(0);
            }
            else
            {
                x2 = x0 + newradius*qSin(PI*2/count*(i+1));
                y2 = y0 + newradius*qCos(PI*2/count*(i+1));
            }
            painter->drawLine(x1,y1,x2,y2);
        }
    }
    painter->drawPath(looppath);

    painter->restore();
}

void RadarForm::drawAxisLabel(QPainter *painter)
{
    if (!m_showAxisLabel) return;

    painter->save();

    QPen pen;
    QColor color(Qt::red);
    pen.setColor(color/*.lighter(100)*/); //添加线的颜色
    painter->setPen(pen);


    int  laycount = m_cylindernum;  //默认是几层
    int w = width();
    int h = height();
    //int count = 360 / m_lineangle;
    int radius = qMin(w,h)/2;
    int x0 = w/2 ;
    int y0 = h/2;

    QFont font;
    int fontsize = 10;
    if (h > 500)
        fontsize = 15;
    font.setPointSize(fontsize);
    painter->setFont(font);

    for (int j = 0 ; j < laycount+1; ++j)
    {
        int newradius = radius*j/(laycount+1);
        int x1 = x0 + newradius*qSin(PI);
        int y1 = y0 + newradius*qCos(PI)+20;
        if (j < m_AxisLabelList.count())
        {
            painter->drawText(x1,y1,m_AxisLabelList.at(j));
        }
    }

    painter->restore();
}

void RadarForm::drawLabel(QPainter *painter)
{
    if (!m_showLabel) return;

    painter->save();

    QPen pen;
    QColor color(Qt::green);
    pen.setColor(color/*.lighter(100)*/); //添加线的颜色
    painter->setPen(pen);


    int  laycount = m_cylindernum;  //默认是几层
    int w = width();
    int h = height();
    int count = 360 / m_lineangle;
    int radius = qMin(w,h)/2;
    int x0 = w/2 ;
    int y0 = h/2;

    QFont font;
    int fontsize = 10;
    if (h > 500)
        fontsize = 15;
    font.setPointSize(fontsize);
    painter->setFont(font);


    for (int i = 0 ; i < count; ++i)
    {
        int newradius = radius*laycount/(laycount+1)+10;
        newradius = newradius+10*qSin(PI*2/count*i);
        int x1 = x0 - newradius*qSin(PI*2/count*i);
        int y1 = y0 - newradius*qCos(PI*2/count*i);

        if (y0 < y1 && x0 < x1)
        {
            y1 -= fontsize*qSin(PI*2/count*i);
        }
        else
        {
            if (x1 < x0)
                x1-=fontsize;
            y1 += fontsize*qSin(PI*2/count*i);
        }


        if (i < m_labelList.count())
        {
            painter->drawText(x1,y1,m_labelList.at(i));
            qDebug() << "坐标：" << x1 << y1 << m_labelList.at(i);
            //painter->drawText(x1,y1,"xxxx");
        }

    }

    painter->restore();
}

void RadarForm::drawData(QPainter *painter)
{
    switch(m_pointstype)
    {
    case ScatterPoints:drawScatterPoints(painter);break;
    case LinePoints:drawLinePoints(painter);break;
    default:drawLinePoints(painter);break;
    }
}

void RadarForm::drawScatterPoints(QPainter *painter)
{
    painter->save();

    int w = width();
    int h = height();
    //int count = 360 / m_lineangle;
    int radius = qMin(w,h)/2 * m_cylindernum/(m_cylindernum+1);

    QList<int> keyilist = m_datamap.keys();

    qreal step = 1.0*radius/(m_Axis_max-m_Axis_min);
    foreach (qreal key, keyilist)
    {
        RadarData rdata = m_datamap.value(key);
        QColor color = rdata.color;
        painter->setPen(color.darker(200));
        painter->setBrush(color.darker(200));
        int  i = 0;
        QPointF startp;
        foreach (qreal angle ,rdata.transitionDataMap.keys())
        {
            qreal length = step*rdata.transitionDataMap.value(angle);
            angle+=90; //因为转90
            QPointF certp(w/2 + length*qCos(angle*PI/180),h/2 - length*qSin(angle*PI/180));
            //painter->drawEllipse(certp,2,2);
            if (i != 0)
            {
                painter->drawLine(startp.x(),startp.y(),certp.x(),certp.y());
            }

            if (i == 0)
            {
                //path.moveTo(certp);
                startp = certp;
            }
            else
            {
                startp = certp;
            }

            ++i;
        }
    }

    painter->restore();
}

void RadarForm::drawLinePoints(QPainter *painter)
{
    painter->save();

    int w = width();
    int h = height();
    //int count = 360 / m_lineangle;
    int radius = qMin(w,h)/2*m_cylindernum/(m_cylindernum+1);

    QList<int> keyilist = m_datamap.keys();

    qreal step = 1.0*radius/(m_Axis_max - m_Axis_min);

    foreach (qreal key, keyilist)
    {
        RadarData rdata = m_datamap.value(key);
        QPainterPath path;
        int  i = 0;
        QPointF startp;

        QColor color = rdata.color;
        painter->setPen(color.darker(200));
        painter->setBrush(color.darker(200));
        foreach (qreal angle ,rdata.transitionDataMap.keys())
        {
            qreal length = step*rdata.transitionDataMap.value(angle);
            angle+=90; //因为转90
            QPointF certp(w/2 + length*qCos(angle*PI/180),h/2 - length*qSin(angle*PI/180));
            //painter->drawEllipse(certp,2,2);
            painter->drawLine(startp,certp);
            if (i == 0)
            {
                path.moveTo(certp);
                startp = certp;
            }
            else
                path.lineTo(certp);
            ++i;
        }
        path.lineTo(startp);
        painter->setBrush(color.lighter(100));
        painter->drawPath(path);
    }


    painter->restore();
}
