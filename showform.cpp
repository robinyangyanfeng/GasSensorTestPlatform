#include "showform.h"
#include "ui_showform.h"

ShowForm::ShowForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowForm)
{
    ui->setupUi(this);
}

ShowForm::~ShowForm()
{
    delete ui;
}
//ok按钮槽函数
void ShowForm::on_okBtn_clicked()
{
    //都不进行任何操作
    this->close();
}
//取消按钮槽函数
void ShowForm::on_cancelBtn_clicked()
{
    //都不进行任何操作
    this->close();
}
