#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include "signupdialog.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

signals:
    void sendUsername(QString data, int id);

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    SignupDialog *SignupDialog;

private slots:
    void on_login_button_clicked();
    void on_login_signupbutton_clicked();

public slots:
    void AfterSignup(QString data);
    void Finished(QString data);

private:
    Ui::LoginDialog *ui;
    QSqlQuery query;
    bool hasUsername;
};

#endif // LOGINDIALOG_H
