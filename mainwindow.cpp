#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QObject>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
        ui->setupUi(this);
        this->setWindowTitle("铁路售票系统");
        QObject::connect(ui->main_list,SIGNAL(currentRowChanged(int)),ui->main_widget,SLOT(void on_main_list_clicked())); //点击车票查询或车次查询信号与槽

        ui->main_station_date1->setCalendarPopup(true);
        ui->main_station_date1->setDateTime(QDateTime::currentDateTime());
        ui->main_trainnum_date->setCalendarPopup(true);
        ui->main_trainnum_date->setDateTime(QDateTime::currentDateTime());
        ui->find_station_date1->setCalendarPopup(true);
        ui->find_station_date1->setDateTime(QDateTime::currentDateTime());
        ui->stackedWidget->setCurrentIndex(0);
        ui->main_widget->setCurrentIndex(0);
        ui->find_trainnum_table->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->find_station_table->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->mine_order_table->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->main_logoutbutton->hide();
        ui->station_table->hide();
        activeIndex = -1;
        ui->main_list->setFlow(QListView::LeftToRight);
        ui->label_15->hide();
    }

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::updateUser(QString data, int id){ //登录后显示退出按钮，开启去往个人主页功能
    ui->main_loginbutton->setText(data);
    ui->main_logoutbutton->show();
    activeUserId = id;
}

void MainWindow::Finished(QString data)//过1秒种自动消失的提示信息
{

       ui->label_15->setText(data);
       QTimer::singleShot(1000, ui->label_15, SLOT(hide()));
       ui->label_15->show();

}
void MainWindow::on_main_loginbutton_clicked()//点击登录按钮
{

    LoginDialog = new class LoginDialog;
    if(activeUserId == 0 ) {//未登录则弹出登录框
        LoginDialog->show();
        connect(LoginDialog,SIGNAL(sendUsername(QString, int)),this,SLOT(updateUser(QString, int)));
        if(LoginDialog->exec()==QDialog::Accepted){}
        LoginDialog->close();
    } else {//进入个人主页
        getOrders();
        activeStackIndex = 1;
        ui->stackedWidget->setCurrentIndex(activeStackIndex);
    }
}

void MainWindow::on_main_logoutbutton_clicked()//登出按钮
{
    activeUserId = 0;
    ui->main_loginbutton->setText("登录");
    ui->main_logoutbutton->hide();
    ui->stackedWidget->setCurrentIndex(0);
    ui->main_widget->setCurrentIndex(activeMainIndex);
}

void MainWindow::on_main_list_clicked() {//车票查询和车次查询页面转换
    int current_row = ui->main_list->currentRow();
    for(int i = 0; i < 2; i++) {
        if(current_row == i) {
            activeMainIndex = i;
            ui->main_widget->setCurrentIndex(activeMainIndex);
        }
    }
}

/**************************main>>>>>train_num**************************/
void MainWindow::on_main_trainnum_findbutton_clicked()//查询车次信息首页
{
    if(ui->main_trainnum_edit->text()=="") {
        Finished("请输入车次信息");
        ui->main_trainnum_edit->setFocus();
    } else {
        query.exec("select * from train");
        bool found;
        found = false;
        while(query.next()) {
            if(ui->main_trainnum_date->text() == query.value("date").toString() && ui->main_trainnum_edit->text()==query.value("train_name").toString()) {
                found = true;
                QDate date = ui->main_trainnum_date->date();
                ui->find_trainnum_edit->setText(ui->main_trainnum_edit->text());
                ui->find_trainnum_date->setDate(date);
                int currentId = query.value("train_id").toInt();
                set_trainnum_table(currentId);
                activeMainIndex = 2;
                ui->main_widget->setCurrentIndex(activeMainIndex);
            }
        }
        if(!found) {
            Finished("暂无车次信息");
        }
    }
}

/**************************main>>>>>station**************************/
void MainWindow::on_main_station_findbutton_clicked()//首页点击查询车票信息按钮
{
    bool found = false;
    count = 0;
    if(ui->main_start->text() ==""|| ui->main_end->text()=="") {
        Finished("请将信息填写完整");
    } else {
        //查找车次具体信息
        query.exec(QObject::tr("select c.date as date, a.train_id as train_id, c.train_name, a.stop_id as start_index, b.stop_id as end_index, a.station_name as start, b.station_name as end, a.start_time as start_time, b.arrive_time as end_time from train_stop as a,train_stop as b,train as c where a.city_name = '%1' and b.city_name ='%2' and c.date = '%3' and a.stop_id < b.stop_id and a.train_id = b.train_id and a.train_id = c.train_id").arg(ui->main_start->text()).arg(ui->main_end->text()).arg(ui->main_station_date1->text()));
        while(query.next()) {
            count++;
            found = true;
        }
        if(found) {
            ui->find_station_end->setText(ui->main_end->text());
            ui->find_station_start->setText(ui->main_start->text());
            QDate date1 = ui->main_station_date1->date();
            ui->find_station_date1->setDate(date1);
            set_station_table(ui->find_station_start->text(), ui->find_station_end->text(), ui->find_station_date1->text(), true, true, true, count);
            activeMainIndex = 3;
            ui->main_widget->setCurrentIndex(activeMainIndex);
        }
        if(!found) {
            Finished("暂无信息");
        }
    }
}

void MainWindow::on_main_exchangebutton_clicked()//首页交换起始终点站输入框
{
    QString temp = "";
    temp = ui->main_start->text();
    ui->main_start->setText(ui->main_end->text());
    ui->main_end->setText(temp);
}


/**************************main>>>>find_trainnum**************************/
void MainWindow::on_find_trainnum_findbutton_clicked()//详情页查找车次信息按钮
{
    if(ui->find_trainnum_edit->text()=="") {
        Finished("请输入车次信息");
        ui->find_trainnum_edit->setFocus();
    } else {
        query.exec("select * from train");
        bool found = false;
        int currentId = 0;
        while(query.next()) {
            if(ui->find_trainnum_date->text() == query.value("date").toString() && ui->find_trainnum_edit->text()==query.value("train_name").toString()) {
                found = true;
                currentId = query.value("train_id").toInt();
            }
        }
        if(!found) {
            set_trainnum_table(currentId);
            ui->label_9->hide();
            Finished("暂无车次信息");
        } else {
            set_trainnum_table(currentId);
        }
    }
}

void MainWindow::set_trainnum_table(int id)//详情页车次表格渲染
{
    query.exec(QObject::tr("select * from train where train_id = '%1'").arg(id));
    QString train_name="";
    QString train_type="";
    if (query.first()){
        train_name= query.value("train_name").toString();
        train_type= query.value("train_type").toString();
    }
    query.exec(QObject::tr("select COUNT(train_id)count from train_stop where train_id = %1 GROUP BY train_id").arg(id));
    query.first();
    int count = query.value("count").toInt();
    static QSqlQueryModel *model1 = new QSqlQueryModel(ui->find_trainnum_table);
    model1->setQuery(QObject::tr("select stop_id, station_name, arrive_time, start_time, stop_time from train_stop where train_id = '%1'").arg(id));
    model1->setHeaderData(0,Qt::Horizontal,QObject::tr("站序"));
    model1->setHeaderData(1,Qt::Horizontal,QObject::tr("车站"));
    model1->setHeaderData(2,Qt::Horizontal,QObject::tr("到达时间"));
    model1->setHeaderData(3,Qt::Horizontal,QObject::tr("出发时间"));
    model1->setHeaderData(4,Qt::Horizontal,QObject::tr("停留时间"));
    QHeaderView *header = ui->find_trainnum_table->verticalHeader();
    header->setHidden(true);
    ui->find_trainnum_table->setModel(model1);
    ui->label_9->show();
    QModelIndex index3 = model1->index(0,1,QModelIndex());//rowNum,columnNum为行列号
    QString start= index3.data().toString();
    QModelIndex index4 = model1->index(count-1,1,QModelIndex());//rowNum,columnNum为行列号
    QString end= index4.data().toString();
    QString str = QString("%1次列车（%2），始发站：%3；终到站：%4；全程共有%5个停靠站：").arg(train_name).arg(train_type).arg(start).arg(end).arg(count);
    ui->label_9->setText(str);
}

/**************************main>>>find_station**************************/

void MainWindow::on_find_station_exchange_clicked()//详情页交换起始终点站信息
{
    QString temp = "";
    temp = ui->find_station_start->text();
    ui->find_station_start->setText(ui->find_station_end->text());
    ui->find_station_end->setText(temp);
}

//以下为筛选列车类型复选框点击响应
void MainWindow::on_checkBox_T_stateChanged(int arg1)
{
    set_station_table(ui->find_station_start->text(), ui->find_station_end->text(), ui->find_station_date1->text(), ui->checkBox_D->isChecked(), ui->checkBox_T->isChecked(), ui->checkBox_Z->isChecked(), count);
}

void MainWindow::on_checkBox_D_stateChanged(int arg1)
{
    set_station_table(ui->find_station_start->text(), ui->find_station_end->text(), ui->find_station_date1->text(), ui->checkBox_D->isChecked(), ui->checkBox_T->isChecked(), ui->checkBox_Z->isChecked(), count);
}

void MainWindow::on_checkBox_Z_stateChanged(int arg1)
{
    set_station_table(ui->find_station_start->text(), ui->find_station_end->text(), ui->find_station_date1->text(), ui->checkBox_D->isChecked(), ui->checkBox_T->isChecked(), ui->checkBox_Z->isChecked(), count);
}

void MainWindow::on_find_station_find_clicked()//详情页点击查找按钮
{
    bool found = false;
    count = 0;
    if(ui->find_station_start->text() ==""|| ui->find_station_end->text()=="") {
        Finished("请将信息填写完整");
    } else {
        query.exec(QObject::tr("select c.date as date, a.train_id as train_id, c.train_name, a.stop_id as start_index, b.stop_id as end_index, a.station_name as start, b.station_name as end, a.start_time as start_time, b.arrive_time as end_time from train_stop as a,train_stop as b,train as c where a.city_name = '%1' and b.city_name ='%2' and c.date = '%3' and a.stop_id < b.stop_id and a.train_id = b.train_id and a.train_id = c.train_id").arg(ui->find_station_start->text()).arg(ui->find_station_end->text()).arg(ui->find_station_date1->text()));
        while(query.next()) {
            count++;
            found = true;
        }
        if(found) {
            ui->checkBox_D->setCheckState(Qt::Unchecked);
            ui->checkBox_T->setCheckState(Qt::Unchecked);
            ui->checkBox_Z->setCheckState(Qt::Unchecked);
            set_station_table(ui->find_station_start->text(), ui->find_station_end->text(), ui->find_station_date1->text(), true, true, true, count);
        }
        if(!found) {
            set_station_table(ui->find_station_start->text(), ui->find_station_end->text(), ui->find_station_date1->text(), true, true, true, count);
            Finished("暂无信息");
        }
    }
}

void MainWindow::set_station_table(QString a, QString b, QString c, bool d, bool e, bool f, int count)//详情页渲染车票列表
{
    //确认列车类型
    QString D = "D";
    QString T = "T";
    QString Z = "Z";
    if(!d && !e && !f) {}
    else {
        if (!d) D = "";
        if (!e) T = "";
        if (!f) Z = "";
    }
    static QSqlQueryModel *model = new QSqlQueryModel(ui->find_station_table);
    //查询符合要求的列车信息和剩余票数
    model->setQuery(QObject::tr("select c.train_name, a.station_name as start, b.station_name as end, a.start_time as start_time, b.arrive_time as end_time, b.is_nextday, SUM( case when s.seat_type='硬座' then 1 end)硬座, SUM(case when s.seat_type='硬卧' then 1 end)硬卧, SUM(case when s.seat_type='软卧' then 1 end)软卧, SUM(case when s.seat_type='软卧' then 1 end)操作, c.train_id from train_stop as a,train_stop as b,train as c , train_seat as s where a.city_name = '%1' and b.city_name ='%2' and c.date = '%3' and a.stop_id < b.stop_id and a.train_id = s.train_id and a.train_id = b.train_id and a.train_id = c.train_id and (c.train_t = '%4' or c.train_t = '%5' or c.train_t='%6') and s.seat_id not in (select seat_id	from orders	where train_id = a.train_id and date = '%3' and ((start_station_id >= a.stop_id and start_station_id < b.stop_id) or (end_station_id > a.stop_id and end_station_id <= b.stop_id))) group by c.train_id").arg(a).arg(b).arg(c).arg(D).arg(T).arg(Z));
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("车次"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("出发站"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("到达站"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("出发时间"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("到达时间"));
    model->setHeaderData(5,Qt::Horizontal,QObject::tr("备注"));
    model->setHeaderData(6,Qt::Horizontal,QObject::tr("硬座"));
    model->setHeaderData(7,Qt::Horizontal,QObject::tr("硬卧"));
    model->setHeaderData(8,Qt::Horizontal,QObject::tr("软卧"));
    model->setHeaderData(9,Qt::Horizontal,QObject::tr("操作"));
    ui->find_station_table->setModel(model);
    ui->find_station_table->setColumnHidden(10, true);
    ui->find_station_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    QHeaderView *header = ui->find_station_table->verticalHeader();
    header->setHidden(true);
    for (int i = 0; i < count; i++) {
        QString style = "QPushButton{border: none;color:rgb(0,81,135);text-align: center;text-decoration: underline;display: inline-block;font-size: 15px;cursor: pointer;font-family:微软雅黑;}";
        QString style1 = "QPushButton{border: none;color: white;padding: 5px 18px;text-align: center;text-decoration: none;margin: 4px 2px;background-color: rgb(0,83,135);font-family:微软雅黑, Times, serif;border-radius: 4px;}QPushButton:enabled:hover{background-color: rgb(40,102,173);}QPushButton:enabled:pressed{background-color: rgb(40,102,173);}";
        //订票按钮渲染
        QPushButton * m_button = new QPushButton("订票");
        connect(m_button, SIGNAL(clicked(bool)), this, SLOT(onStationTableBtnClicked()));
        QModelIndex index=model->index(i,10,QModelIndex());//rowNum,columnNum为行列号
        int id= index.data().toInt();
        m_button->setProperty("train_id", id);
        m_button->setStyleSheet(style);
        ui->find_station_table->setIndexWidget(model->index(i, 9), m_button);

        //点击车次可以显示具体车次的经停站信息
        QPushButton * info_button = new QPushButton();
        connect(info_button, SIGNAL(clicked(bool)), this, SLOT(onInfoBtnClicked()));
        QModelIndex index1=model->index(i,0,QModelIndex());//rowNum,columnNum为行列号
        QString button_name = index1.data().toString();
        info_button->setText(button_name);
        info_button->setProperty("train_id", id);
        info_button->setFlat(true);
        info_button->setStyleSheet(style);
        ui->find_station_table->setIndexWidget(model->index(i, 0), info_button);
    }
}

void MainWindow::onInfoBtnClicked()//点击具体车次显示经停站信息
{
    QPushButton *btn = (QPushButton *) sender();  //产生事件的对象
    int id = btn->property("train_id").toInt();
    static QSqlQueryModel *model = new QSqlQueryModel(ui->station_table);
    model->setQuery(QObject::tr("select stop_id, station_name, arrive_time, start_time, stop_time from train_stop where train_id = '%1'").arg(id));
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("站序"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("车站"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("到达时间"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("出发时间"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("停留时间"));
    ui->station_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->station_table->setModel(model);
    QHeaderView *header = ui->station_table->verticalHeader();
    header->setHidden(true);
    int c = model->rowCount();
    if (activeIndex != id) {
        ui->station_table->show();
    } else {
        ui->station_table->setHidden(!ui->station_table->isHidden());
    }
    activeIndex = id;
}

void MainWindow::onStationTableBtnClicked()//点击订票按钮进入订票页面
{
    QPushButton *btn = (QPushButton *)sender();   //产生事件的对象
    if(activeUserId == 0) {
        LoginDialog = new class LoginDialog;
        LoginDialog->show();
        connect(LoginDialog,SIGNAL(sendUsername(QString, int)),this,SLOT(updateUser(QString, int)));
        if(LoginDialog->exec()==QDialog::Accepted){}
        LoginDialog->close();
    } else {
       o.train_id = btn->property("train_id").toInt();
       query.exec(QObject::tr("select c.date as date, c.train_name, a.stop_id as start_id, b.stop_id as end_id, a.station_name as start, b.station_name as end, a.start_time as start_time, b.arrive_time as end_time,(b.miles-a.miles) as miles from train_stop as a,train_stop as b,train as c where a.city_name = '%1' and b.city_name ='%2' and c.date = '%3' and a.stop_id < b.stop_id and a.train_id = b.train_id and a.train_id = c.train_id and a.train_id='%4'").arg(ui->find_station_start->text()).arg(ui->find_station_end->text()).arg(ui->find_station_date1->text()).arg(o.train_id));
       if (query.first()) {
           //设置订单信息
           o.train_name = query.value("train_name").toString();
           o.date = query.value("date").toString();
           o.start_station_id = query.value("start_id").toInt();
           o.start_time = query.value("start_time").toString();
           o.start_station_name = query.value("start").toString();
           o.end_station_id = query.value("end_id").toInt();
           o.end_time = query.value("end_time").toString();
           o.end_station_name = query.value("end").toString();
           o.miles = query.value("miles").toInt();
       }
       BuyTicketDialog = new class BuyTicketDialog;
       QObject::connect(this,SIGNAL(sendToBuy(order)),BuyTicketDialog,SLOT(getOrder(order)));
       emit sendToBuy(o);
       BuyTicketDialog->show();
       if(BuyTicketDialog->exec()==QDialog::Accepted){
            getOrders();
            Finished("订票成功");
            on_find_station_find_clicked();
       }
    }
}

/**************************mine**************************/
void MainWindow::getOrders()//个人页面渲染订单表格
{
    static QSqlQueryModel *model2 = new QSqlQueryModel(ui->mine_order_table);
    model2->setQuery(QObject::tr("select num, date, start_station_name, start_time, end_station_name, end_time, train_name, carriage, seat_type, seat_location, create_date, status, status, order_id from orders where user_id = '%1' ORDER BY create_date DESC").arg(activeUserId));
    model2->setHeaderData(0,Qt::Horizontal,QObject::tr("票号"));
    model2->setHeaderData(1,Qt::Horizontal,QObject::tr("日期"));
    model2->setHeaderData(2,Qt::Horizontal,QObject::tr("出发站"));
    model2->setHeaderData(3,Qt::Horizontal,QObject::tr("出发时间"));
    model2->setHeaderData(4,Qt::Horizontal,QObject::tr("到达站"));
    model2->setHeaderData(5,Qt::Horizontal,QObject::tr("到达时间"));
    model2->setHeaderData(6,Qt::Horizontal,QObject::tr("车次"));
    model2->setHeaderData(7,Qt::Horizontal,QObject::tr("车厢"));
    model2->setHeaderData(8,Qt::Horizontal,QObject::tr("类型"));
    model2->setHeaderData(9,Qt::Horizontal,QObject::tr("座位号"));
    model2->setHeaderData(10,Qt::Horizontal,QObject::tr("创建日期"));
    model2->setHeaderData(11,Qt::Horizontal,QObject::tr("状态"));
    model2->setHeaderData(12,Qt::Horizontal,QObject::tr("操作"));
    ui->mine_order_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->mine_order_table->setModel(model2);
    ui->mine_order_table->setColumnHidden(13, true);
    QHeaderView *header = ui->mine_order_table->verticalHeader();
    header->setHidden(true);
    query.exec(QObject::tr("select count(user_id)count from orders where user_id = '%1' group by user_id").arg(activeUserId));
    int count = 0;
    if(query.first()) {
        count = query.value("count").toInt();
    }
    for (int i = 0; i < count; i++) {
        QPushButton * button_return = new QPushButton("退票");
        QPushButton * button_buy = new QPushButton("支付");
        connect(button_return, SIGNAL(clicked(bool)), this, SLOT(onOrderClicled()));
        connect(button_buy, SIGNAL(clicked(bool)), this, SLOT(onBuyClicled()));

        QModelIndex index = model2->index(i,13,QModelIndex());//rowNum,columnNum为行列号
        int order_id= index.data().toInt();
        button_return->setProperty("order_id", order_id);
        button_buy->setProperty("order_id", order_id);
        QString style = "QPushButton{border: none;color:rgb(0,81,135);text-align: center;text-decoration: none;display: inline-block;font-size: 15px;cursor: pointer;font-family:微软雅黑;}";
        QString style1 = "QPushButton{border: none;color: white;padding: 5px 18px;text-align: center;text-decoration: none;margin: 4px 2px;background-color: rgb(0,83,135);font-family:微软雅黑, Times, serif;border-radius: 4px;}QPushButton:enabled:hover{background-color: rgb(40,102,173);}QPushButton:enabled:pressed{background-color: rgb(40,102,173);}";
        button_return->setStyleSheet(style);
        button_buy->setStyleSheet(style1);
        //根据订单状态显示相应的功能按钮
        QModelIndex index1 = model2->index(i,12,QModelIndex());//rowNum,columnNum为行列号
        QString status= index1.data().toString();
        if(status == "未付款") {
            ui->mine_order_table->setIndexWidget(model2->index(i, 12), button_buy);
        } else {
            ui->mine_order_table->setIndexWidget(model2->index(i, 12), button_return);
        }
    }
}

void MainWindow::on_mine_goIndex_clicked()//从个人页面返回首页
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->main_widget->setCurrentIndex(activeMainIndex);
}

void MainWindow::onOrderClicled()//退票按钮响应
{
    QPushButton *btn = (QPushButton *)sender();  //产生事件的对象
    activeOrder = btn->property("order_id").toInt();
    CofirmDeleteDialog = new class CofirmDeleteDialog;
    CofirmDeleteDialog->show();
    if(CofirmDeleteDialog->exec()==QDialog::Accepted){
        getOrders();
        Finished("退票成功");
    }
}

void MainWindow::onBuyClicled()//支付按钮响应
{
    QPushButton *btn = (QPushButton *)sender();  //产生事件的对象
    activeOrder = btn->property("order_id").toInt();
    confirmBuyDialog = new class confirmBuyDialog;
    confirmBuyDialog->show();
    if(confirmBuyDialog->exec()==QDialog::Accepted){
        getOrders();
        Finished("支付成功");
    }
}
