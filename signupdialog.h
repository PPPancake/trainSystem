#ifndef SIGNUPDIALOG_H
#define SIGNUPDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QRegExp>
#include <QString>
#include "global.h"

namespace Ui {
class SignupDialog;
}

class SignupDialog : public QDialog
{
    Q_OBJECT

signals:
    void sendSignupUser(QString data);

public:
    explicit SignupDialog(QWidget *parent = nullptr);
    ~SignupDialog();

private slots:
    void on_signup_button_clicked();
    void Finished(QString data);

private:
    Ui::SignupDialog *ui;
    QSqlQuery query;
    QString sql;
};

#endif // SIGNUPDIALOG_H
