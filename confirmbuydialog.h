#ifndef CONFIRMBUYDIALOG_H
#define CONFIRMBUYDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include "global.h"

namespace Ui {
class confirmBuyDialog;
}

class confirmBuyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit confirmBuyDialog(QWidget *parent = nullptr);
    ~confirmBuyDialog();

private slots:
    void on_pay_clicked();
    void on_cancel_clicked();
    void getInfo();

private:
    Ui::confirmBuyDialog *ui;
    QSqlQuery query;
};

#endif // CONFIRMBUYDIALOG_H
