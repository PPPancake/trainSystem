#ifndef COFIRMDELETEDIALOG_H
#define COFIRMDELETEDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include "global.h"

namespace Ui {
class CofirmDeleteDialog;
}

class CofirmDeleteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CofirmDeleteDialog(QWidget *parent = nullptr);
    ~CofirmDeleteDialog();

private slots:
    void on_delete_button_clicked();
    void getInfo();
    void on_delete_cancel_clicked();

private:
    Ui::CofirmDeleteDialog *ui;
    QSqlQuery query;
};

#endif // COFIRMDELETEDIALOG_H
