#include "cofirmdeletedialog.h"
#include "ui_cofirmdeletedialog.h"

CofirmDeleteDialog::CofirmDeleteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CofirmDeleteDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("退票");
    getInfo();

}

CofirmDeleteDialog::~CofirmDeleteDialog()
{
    delete ui;
}

void CofirmDeleteDialog::on_delete_button_clicked()//点击退票按钮，删除相关订单
{
    query.exec(QObject::tr("DELETE FROM orders WHERE order_id='%1'").arg(activeOrder));
    accept();
}

void CofirmDeleteDialog::getInfo()//获取票号
{
    query.exec(QObject::tr("select * from orders where order_id = '%1'").arg(activeOrder));
    if(query.first()) {
        ui->label_2->setText(query.value("num").toString());
    }
}

void CofirmDeleteDialog::on_delete_cancel_clicked()//取消按钮
{
    reject();
}
