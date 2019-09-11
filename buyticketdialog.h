#ifndef BUYTICKETDIALOG_H
#define BUYTICKETDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include "global.h"

namespace Ui {
class BuyTicketDialog;
}

class BuyTicketDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BuyTicketDialog(QWidget *parent = nullptr);
    ~BuyTicketDialog();
    order oo;

private:
    QSqlQuery query;
    Ui::BuyTicketDialog *ui;
    void getSeatInfo();
    QString str;

private slots:
    void getOrder(order);
    void on_buy_button_clicked();
    void on_buy_radio1_clicked();
    void on_buy_radio2_clicked();
    void on_buy_radio3_clicked();
    void on_A_clicked();
    void on_B_clicked();
    void on_C_clicked();
    void on_D_clicked();
    void on_F_clicked();
};

#endif // BUYTICKETDIALOG_H
