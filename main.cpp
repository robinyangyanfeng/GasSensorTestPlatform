#include "mainform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainForm w;
    QtClient Qc;
    w.show();
    Qc.show();

    return a.exec();
}
