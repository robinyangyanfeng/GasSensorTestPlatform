#ifndef CSVFILEREADWIRTE_XY_H
#define CSVFILEREADWIRTE_XY_H

/*
 * 
 * Csv文件概述
 *
 * 逗号分隔值（Comma-Separated Values，CSV，有时也称为字符分隔值，因为分隔字符也可以不是逗号），其文件以纯文本形式存储表格数据（数字和文本）。
 * 纯文本意味着该文件是一个字符序列，不含必须像二进制数字那样被解读的数据。CSV文件由任意数目的记录组成，记录间以某种换行符分隔；每条记录由字段组成，
 * 字段间的分隔符是其它字符或字符串，最常见的是逗号或制表符。通常，所有记录都有完全相同的字段序列。通常都是纯文本文件。
 * 建议使用WORDPAD或是记事本（NOTE）来开启， 再则先另存新档后用EXCEL开启，也是方法之一。
 *
 * CSV文件格式的通用标准并不存在，但是在RFC 4180中有基础性的描述。使用的字符编码同样没有被指定，但是7-bitASCII是最基本的通用编码。
 *
 * （上述来至百度百科）
 *
 * csv文件作为简单的格式化文本文件，随着数据挖掘和python的普及突然就又火起来了，工作中发现许多数据交互由原来的xml又改为通过csv文件进行交互，
 * csv文件有个最大的缺点是单个单元格里不能出现换行，如果是单纯的数据交互，csv的确是最简单的格式化方式。
 * csv把每个单元数据用逗号隔开，但某些情况下需要注意的是，遇到一个单元内容有包含引号"和逗号,时是需要转义的。
 * 否则会造成格式混乱，本文的目的是封装一个简单好用的csv流式输入输出，类名CsvFileReadWirte_xy
 *
 *
 *
 * Csv的解析
 *
 * 其实导出csv和导出txt唯一的区别就是对csv的一些特殊情况需要转义操作，如遇到逗号遇到引号的情况
 * 在csv中，遇到逗号需要把整个内容用引号扩起来
 * 遇到引号，使用两个引号代表一个引号
 * 如这是一个"复合文本"文本:1,2,4,5,czyt1988，在csv中需要显示为"这是一个""复合文本""文本:1,2,4,5,czyt1988"
 *
 *
 * 参考网址：https://blog.csdn.net/czyt1988/article/details/79402449
 *
 */


class QTextStream;
class QFile;
class CsvStreamPrivate_xy;

#include <QString>
#include <QScopedPointer>

/*
 * 说明：csv文件读写支持
 *
 * 作者：xy
 *
 * 日期：2018年12月10号
*/

class CsvFileReadWirte_xy
{
public:
    CsvFileReadWirte_xy(QTextStream *txt);
    CsvFileReadWirte_xy(QFile *txt);
    virtual ~CsvFileReadWirte_xy();
    //转换为标识csv字符
    static QString toCsvString(const QString &rawStr);
    //把一行要用逗号分隔的字符串转换为一行标准csv字符串
    static QString toCsvStringLine(const QStringList & sectionLine);
    //解析一行csv字符
    static QStringList analysisCsvLine(const QString &lineStr);

    //重载运算符
    CsvFileReadWirte_xy & operator << (const QString &str);
    CsvFileReadWirte_xy & operator << (int d);
    CsvFileReadWirte_xy & operator << (double d);
    CsvFileReadWirte_xy & operator << (float d);

    //另起一行
    void newLine();
    //获取输入输出流
    QTextStream* stream() const;
    //读取并解析一行csv字符串
    QStringList readAndAnalysisCsvLine();
    //判断是否到文件末端
    bool atEnd() const;

private:
    static int advquoted(const QString &s,QString &fld,int i);
    static int advplain(const QString &s,QString &fld,int i);

private:
    QScopedPointer<CsvStreamPrivate_xy> d_p;
};

typedef CsvFileReadWirte_xy & (*CsvFileReadWirteFunction_xy)(CsvFileReadWirte_xy &);
inline CsvFileReadWirte_xy & operator << (CsvFileReadWirte_xy & s,CsvFileReadWirteFunction_xy f)
{
    return (*f)(s);
}

CsvFileReadWirte_xy &endl(CsvFileReadWirte_xy &s);

#endif // CSVFILEREADWIRTE_XY_H
