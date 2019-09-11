#include "buyticketdialog.h"
#include "ui_buyticketdialog.h"
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QButtonGroup>
#include <QMessageBox>

BuyTicketDialog::BuyTicketDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BuyTicketDialog)
{
    ui->setupUi(this);
    ui->buy_radio1->setChecked(true);
    this->setWindowTitle("车票预定");
}

BuyTicketDialog::~BuyTicketDialog()
{
    delete ui;
}


void BuyTicketDialog::getSeatInfo()
{
    // 单选按钮组：选择座位位置
    QString seat_type = "硬座";
    QButtonGroup *bg1=new QButtonGroup(this);
    bg1->addButton(ui->A,0);
    bg1->addButton(ui->B,1);
    bg1->addButton(ui->C,2);
    bg1->addButton(ui->D,3);
    bg1->addButton(ui->F,4);

    //单选按钮组：座位类型
    QButtonGroup *bg2=new QButtonGroup(this);
    bg2->addButton(ui->buy_radio1,6);//一个值为0
    bg2->addButton(ui->buy_radio2,7);
    bg2->addButton(ui->buy_radio3,8);

    //对单选按钮选择进行判断
    if(ui->buy_radio1->isChecked()){
        seat_type = "硬座";
    }
    if(ui->buy_radio2->isChecked()) {
        seat_type = "硬卧";
    }
    if(ui->buy_radio3->isChecked()) {
        seat_type = "软卧";
    }
    if(ui->A->isChecked()) {
        query.prepare(QObject::tr("SELECT t.train_id, s.seat_id, s.carriage, s.seat_type, s.seat_location FROM train AS t, train_seat AS s WHERE t.train_id = '%1' AND t.train_id = s.train_id AND s.seat_type = '%2' AND s.seat_id NOT IN (SELECT seat_id FROM orders WHERE train_id = '%3' AND date = '%4' AND ( ( start_station_id >= '%5' AND start_station_id < '%6' ) OR ( end_station_id > '%7' AND end_station_id <='%8')))and s.seat_location like '%A%'").arg(oo.train_id).arg(seat_type).arg(oo.train_id).arg(oo.date).arg(oo.start_station_id).arg(oo.end_station_id).arg(oo.start_station_id).arg(oo.end_station_id));
    } else if(ui->B->isChecked()) {
        query.prepare(QObject::tr("SELECT t.train_id, s.seat_id, s.carriage, s.seat_type, s.seat_location FROM train AS t, train_seat AS s WHERE t.train_id = '%1' AND t.train_id = s.train_id AND s.seat_type = '%2' AND s.seat_id NOT IN (SELECT seat_id FROM orders WHERE train_id = '%3' AND date = '%4' AND ( ( start_station_id >= '%5' AND start_station_id < '%6' ) OR ( end_station_id > '%7' AND end_station_id <='%8')))and s.seat_location like '%B%'").arg(oo.train_id).arg(seat_type).arg(oo.train_id).arg(oo.date).arg(oo.start_station_id).arg(oo.end_station_id).arg(oo.start_station_id).arg(oo.end_station_id));
    } else if(ui->C->isChecked()) {
        query.prepare(QObject::tr("SELECT t.train_id, s.seat_id, s.carriage, s.seat_type, s.seat_location FROM train AS t, train_seat AS s WHERE t.train_id = '%1' AND t.train_id = s.train_id AND s.seat_type = '%2' AND s.seat_id NOT IN (SELECT seat_id FROM orders WHERE train_id = '%3' AND date = '%4' AND ( ( start_station_id >= '%5' AND start_station_id < '%6' ) OR ( end_station_id > '%7' AND end_station_id <='%8')))and s.seat_location like '%C%'").arg(oo.train_id).arg(seat_type).arg(oo.train_id).arg(oo.date).arg(oo.start_station_id).arg(oo.end_station_id).arg(oo.start_station_id).arg(oo.end_station_id));
    } else if(ui->D->isChecked()) {
        query.prepare(QObject::tr("SELECT t.train_id, s.seat_id, s.carriage, s.seat_type, s.seat_location FROM train AS t, train_seat AS s WHERE t.train_id = '%1' AND t.train_id = s.train_id AND s.seat_type = '%2' AND s.seat_id NOT IN (SELECT seat_id FROM orders WHERE train_id = '%3' AND date = '%4' AND ( ( start_station_id >= '%5' AND start_station_id < '%6' ) OR ( end_station_id > '%7' AND end_station_id <='%8')))and s.seat_location like '%D%'").arg(oo.train_id).arg(seat_type).arg(oo.train_id).arg(oo.date).arg(oo.start_station_id).arg(oo.end_station_id).arg(oo.start_station_id).arg(oo.end_station_id));
    } else if(ui->F->isChecked()) {
        query.prepare(QObject::tr("SELECT t.train_id, s.seat_id, s.carriage, s.seat_type, s.seat_location FROM train AS t, train_seat AS s WHERE t.train_id = '%1' AND t.train_id = s.train_id AND s.seat_type = '%2' AND s.seat_id NOT IN (SELECT seat_id FROM orders WHERE train_id = '%3' AND date = '%4' AND ( ( start_station_id >= '%5' AND start_station_id < '%6' ) OR ( end_station_id > '%7' AND end_station_id <='%8')))and s.seat_location like '%F%'").arg(oo.train_id).arg(seat_type).arg(oo.train_id).arg(oo.date).arg(oo.start_station_id).arg(oo.end_station_id).arg(oo.start_station_id).arg(oo.end_station_id));
    } else {
        query.prepare(QObject::tr("SELECT t.train_id, s.seat_id, s.carriage, s.seat_type, s.seat_location FROM train AS t, train_seat AS s WHERE t.train_id = '%1' AND t.train_id = s.train_id AND s.seat_type = '%2' AND s.seat_id NOT IN (SELECT seat_id FROM orders WHERE train_id = '%3' AND date = '%4' AND ( ( start_station_id >= '%5' AND start_station_id < '%6' ) OR ( end_station_id > '%7' AND end_station_id <='%8')))").arg(oo.train_id).arg(seat_type).arg(oo.train_id).arg(oo.date).arg(oo.start_station_id).arg(oo.end_station_id).arg(oo.start_station_id).arg(oo.end_station_id));
    }
    query.exec();
    if (query.first()) {
        oo.carriage = query.value("carriage").toString();
        oo.seat_type = query.value("seat_type").toString();
        oo.seat_id = query.value("seat_id").toInt();
        oo.seat_location = query.value("seat_location").toString();
    } else {
        // 查询用户筛选后的剩余座位
        query.exec(QObject::tr("SELECT t.train_id, s.seat_id, s.carriage, s.seat_type, s.seat_location FROM train AS t, train_seat AS s WHERE t.train_id = '%1' AND t.train_id = s.train_id AND s.seat_type = '%2' AND s.seat_id NOT IN (SELECT seat_id FROM orders WHERE train_id = '%3' AND date = '%4' AND ( ( start_station_id >= '%5' AND start_station_id < '%6' ) OR ( end_station_id > '%7' AND end_station_id <='%8')))").arg(oo.train_id).arg(seat_type).arg(oo.train_id).arg(oo.date).arg(oo.start_station_id).arg(oo.end_station_id).arg(oo.start_station_id).arg(oo.end_station_id));
        if (query.first()) {
            QMessageBox::warning(this,tr("Warning"),("当前座位已满，请选择其他座位"),QMessageBox::Yes);
        } else {
            query.exec(QObject::tr("SELECT t.train_id, s.seat_id, s.carriage, s.seat_type, s.seat_location FROM train AS t, train_seat AS s WHERE t.train_id = '%1' AND t.train_id = s.train_id AND s.seat_id NOT IN (SELECT seat_id FROM orders WHERE train_id = '%2' AND date = '%3' AND ( ( start_station_id >= '%4' AND start_station_id < '%5' ) OR ( end_station_id > '%6' AND end_station_id <='%7')))").arg(oo.train_id).arg(oo.train_id).arg(oo.date).arg(oo.start_station_id).arg(oo.end_station_id).arg(oo.start_station_id).arg(oo.end_station_id));
            if(query.first()){
                QMessageBox::warning(this,tr("Warning"),("当前座位类型没有余票，请选择其他座位类型"),QMessageBox::Yes);
            } else {
                QMessageBox::warning(this,tr("Warning"),("当前车次没有余票"),QMessageBox::Yes);
            }
        }
    }

    //设置界面信息
    ui->label_3->setText(oo.date);
    ui->label_5->setText(oo.train_name);
    ui->label_6->setText(oo.start_station_name);
    ui->label_7->setText(oo.start_time);
    ui->label_8->setText(oo.end_station_name);
    ui->label_9->setText(oo.end_time);
    if(oo.seat_type == "硬座") {
        ui->label_13->setText(QString::number(oo.miles/10));
    } else if (oo.seat_type == "硬卧") {
        ui->label_13->setText(QString::number((oo.miles/10)*1.2));
    } else {
        ui->label_13->setText(QString::number((oo.miles/10)*1.5));
    }
    ui->label_17->setText(oo.username);
    ui->label_18->setText(oo.idcard);
    ui->label_20->setText(oo.tele);
    oo.num = QString::number(oo.train_id) + QString::number(oo.seat_id);
}

void BuyTicketDialog::getOrder(order o) //获取订单信息中的用户相关信息
{
    oo = o;
    query.exec(QObject::tr("select * from user where user_id = '%1'").arg(activeUserId));
    while(query.next()){
        oo.idcard = query.value("idcard").toString();
        oo.username = query.value("username").toString();
        oo.tele = query.value("telephone").toString();
    }
    getSeatInfo();
}

void BuyTicketDialog::on_buy_button_clicked()// 点击购买车票按钮
{

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    query.exec(QObject::tr("insert into orders (user_id, idcard, username, train_id, train_name, carriage, seat_type, seat_id, seat_location, start_time, start_station_id, start_station_name,  end_time, end_station_id, end_station_name, date, create_date, status, num) values ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10', '%11', '%12', '%13', '%14', '%15', '%16', '%17', '%18', '%19')").arg(activeUserId).arg(oo.idcard).arg(oo.username).arg(oo.train_id).arg(oo.train_name).arg(oo.carriage).arg(oo.seat_type).arg(oo.seat_id).arg(oo.seat_location).arg(oo.start_time).arg(oo.start_station_id).arg(oo.start_station_name).arg(oo.end_time).arg(oo.end_station_id).arg(oo.end_station_name).arg(oo.date).arg(current_date).arg("未付款").arg(oo.num));
    accept();
}

/**************************以下为单选点击响应**************************/
void BuyTicketDialog::on_buy_radio1_clicked()
{
    getSeatInfo();
}

void BuyTicketDialog::on_buy_radio2_clicked()
{
    getSeatInfo();
}

void BuyTicketDialog::on_buy_radio3_clicked()
{
    getSeatInfo();
}

void BuyTicketDialog::on_A_clicked()
{
    getSeatInfo();
}

void BuyTicketDialog::on_B_clicked()
{
    getSeatInfo();
}

void BuyTicketDialog::on_C_clicked()
{
    getSeatInfo();
}

void BuyTicketDialog::on_D_clicked()
{
    getSeatInfo();
}

void BuyTicketDialog::on_F_clicked()
{
    getSeatInfo();
}
