#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include "logindialog.h"
#include <QSqlTableModel>
#include <QTableView>
#include "global.h"
#include "buyticketdialog.h"
#include "cofirmdeletedialog.h"
#include "confirmbuydialog.h"
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    LoginDialog *LoginDialog;
    BuyTicketDialog *BuyTicketDialog;
    CofirmDeleteDialog * CofirmDeleteDialog;
    confirmBuyDialog * confirmBuyDialog;
    int count;

public slots:
    void updateUser(QString data, int id);

private slots:
    void on_main_loginbutton_clicked();
    void on_main_list_clicked();
    void on_main_trainnum_findbutton_clicked();
    void on_main_station_findbutton_clicked();
    void on_main_exchangebutton_clicked();
    void on_find_trainnum_findbutton_clicked();
    void on_find_station_exchange_clicked();
    void onStationTableBtnClicked();
    void on_find_station_find_clicked();
    void set_station_table(QString a, QString b, QString c, bool d, bool e, bool f, int count);
    void set_trainnum_table(int id);
    void on_checkBox_T_stateChanged(int arg1);
    void on_checkBox_D_stateChanged(int arg1);
    void on_checkBox_Z_stateChanged(int arg1);
    void getOrders();
    void on_mine_goIndex_clicked();
    void on_main_logoutbutton_clicked();
    void onOrderClicled();
    void Finished(QString data);
    void onBuyClicled();
    void onInfoBtnClicked();

private:
    Ui::MainWindow *ui;
    QSqlQuery query;
    QSqlTableModel *trainnum_model;
    QTableView *trainnum_view;
    QSqlTableModel *station_model;
    QTableView *station_view;
    QSqlTableModel *order_model;
    QTableView *order_view;
    order o;
    int activeIndex;

signals:
    void sendToBuy(order o);
};
#endif // MAINWINDOW_H
