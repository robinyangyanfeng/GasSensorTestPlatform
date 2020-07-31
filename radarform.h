#ifndef RADARFORM_H
#define RADARFORM_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QMap>
#include <qmath.h>
#include <QDebug>

enum RadarType
{
    Radar = 0, //雷达图
    Spider //蜘蛛网图
};

enum PointType
{
    ScatterPoints = 0, //散点图
    LinePoints //连线图
};

typedef struct RadarData
{
    QColor color;
    QMap<qreal,qreal> dataMap; //key:角度  value：值
    QMap<qreal,qreal> transitionDataMap; //key：角度  value：转换的值

}RadarData;

#define PI 3.14159

namespace Ui {
class RadarForm;
}

class RadarForm : public QWidget
{
    Q_OBJECT

public:
    explicit RadarForm(QWidget *parent = 0);
    ~RadarForm();
    void init(float randFloat,float randFloat2,float randFloat3,float randFloat4,float randFloat5,float randFloat6,float randFloat7,float randFloat8,float randFloat9,float randFloat10,float randFloat11,float randFloat12,float randFloat13,float randFloat14,float randFloat15,float randFloat16);
    void setRadarType(RadarType radarType);
    void setPointType(PointType pointType);

    bool isClear;

protected:
    void paintEvent(QPaintEvent *);

private:

    void drawRadarType(QPainter *painter);
    void drawRadar(QPainter *painter);
    void drawSpider(QPainter *painter);
    void drawAxisLabel(QPainter *painter);
    void drawLabel(QPainter *painter);
    void drawData(QPainter *painter);
    void drawScatterPoints(QPainter *painter);
    void drawLinePoints(QPainter *painter);


private:
    Ui::RadarForm *ui;

    RadarType m_radartype;
    PointType m_pointstype;
    int m_cylindernum;  //圈数
    int m_lineangle;    //角度，决定多少。默认是30
    int m_Axis_max,m_Axis_min; //最大最小
    QStringList m_AxisLabelList; //标签 刻度标签
    QStringList m_labelList;     //外围标签
    bool m_showAxisLabel;
    bool m_showLabel;

    QMap<int,RadarData> m_datamap;  //key 表示组，value表示对于的值
    QList<QColor> m_colorlist;
};

#endif // RADARFORM_H
