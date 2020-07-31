#ifndef SHOWFORM_H
#define SHOWFORM_H

#include <QWidget>

namespace Ui {
class ShowForm;
}

class ShowForm : public QWidget
{
    Q_OBJECT

public:
    explicit ShowForm(QWidget *parent = 0);
    ~ShowForm();

private slots:
    //ok按钮槽函数
    void on_okBtn_clicked();
    //取消按钮槽函数
    void on_cancelBtn_clicked();

private:
    Ui::ShowForm *ui;
};

#endif // SHOWFORM_H
