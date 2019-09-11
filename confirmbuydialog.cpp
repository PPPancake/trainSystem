#include "confirmbuydialog.h"
#include "ui_confirmbuydialog.h"
#include <QDebug>

confirmBuyDialog::confirmBuyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::confirmBuyDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("支付");
    getInfo();
}

confirmBuyDialog::~confirmBuyDialog()
{
    delete ui;
}

void confirmBuyDialog::getInfo()//获取票号
{
    query.exec(QObject::tr("select * from orders where order_id = '%1'").arg(activeOrder));
    if(query.first()) {
        ui->label_2->setText(query.value("num").toString());
    }
}

void confirmBuyDialog::on_pay_clicked()//点击支付按钮，更改订单状态
{
    query.exec(QObject::tr("UPDATE orders SET status = '订票成功' WHERE order_id ='%1'").arg(activeOrder));
    accept();
}

void confirmBuyDialog::on_cancel_clicked()//取消
{
    reject();
}
