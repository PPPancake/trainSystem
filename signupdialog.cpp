#include "signupdialog.h"
#include "ui_signupdialog.h"
#include <QRegExpValidator>
#include <QDebug>
#include <QString>
#include <QTimer>

SignupDialog::SignupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignupDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("用户注册");
    ui->signup_username->setMaxLength(10);

    ui->signup_password->setEchoMode(QLineEdit::Password);

    ui->signup_confirmpass->setEchoMode(QLineEdit::Password);
    ui->label_15->hide();
}

SignupDialog::~SignupDialog()
{
    delete ui;
}
void SignupDialog::Finished(QString data)//过1秒种自动消失的提示信息
{
    ui->label_15->setText(data);
    QTimer::singleShot(1000, ui->label_15, SLOT(hide()));
    ui->label_15->show();
}

void SignupDialog::on_signup_button_clicked() {//点击注册按钮
    query.exec("select * from user");

    //检查用户填写的数据
    if(ui->signup_username->text()=="" || ui->signup_password->text()=="" || ui->signup_confirmpass->text()=="" || ui->signup_tele->text()=="" || ui->signup_idcard->text()=="") {
        Finished("信息不能为空");
    } else {
        bool UserExist = false;
        bool IdcardExist = false;
        while(query.next()) {
            if(query.value("username").toString() == ui->signup_username->text()) {
                UserExist = true;
            }
            if(query.value("idcard").toString() == ui->signup_idcard->text()) {
                IdcardExist = true;
            }
        }
        if (UserExist) {
            Finished("用户名已存在");
            ui->signup_username->setFocus();
        } else if (IdcardExist) {
            Finished("身份证信息已存在");
            ui->signup_idcard->setFocus();
        }
        if(!UserExist && !IdcardExist) {
            bool passwordMatch;//正则表达式验证密码：8-16位同时含有数字和字母
            QRegExp PasswordRegExp{"^(?![0-9]+$)(?![a-zA-Z]+$)[0-9A-Za-z]{8,16}$"};
            passwordMatch = PasswordRegExp.exactMatch(ui->signup_password->text());

            bool idcardMatch;//正则验证身份证信息
            QRegExp IdcardRegExp{"(^[1-9]\\d{5}(18|19|([23]\\d))\\d{2}((0[1-9])|(10|11|12))(([0-2][1-9])|10|20|30|31)\\d{3}[0-9Xx]$)|(^[1-9]\\d{5}\\d{2}((0[1-9])|(10|11|12))(([0-2][1-9])|10|20|30|31)\\d{2}[0-9Xx]$)"};
            idcardMatch = IdcardRegExp.exactMatch(ui->signup_idcard->text());

            bool teleMatch;//正则验证电话信息
            QRegExp teleRegExp{"^((13[0-9])|(14[5|7])|(15([0-3]|[5-9]))|(18[0,5-9]))\\d{8}$"};
            teleMatch = teleRegExp.exactMatch(ui->signup_tele->text());

            if(!passwordMatch) {
                Finished("密码不符合要求");
                ui->signup_password->clear();
                ui->signup_confirmpass->clear();
                ui->signup_password->setFocus();
            } else if (ui->signup_password->text() != ui->signup_confirmpass->text()) {
                Finished("两次密码不一致");
                ui->signup_confirmpass->clear();
                ui->signup_confirmpass->setFocus();
            } else if (!idcardMatch) {
                Finished("身份证格式不正确");
                ui->signup_idcard->setFocus();
            } else if(!teleMatch) {
                Finished("请输入正确的电话号码");
                ui->signup_tele->setFocus();
            } else {
                QString str1=QString("insert into user(username,password,telephone,idcard) values(\"%1\",\"%2\",\"%3\",\"%4\")").arg(ui->signup_username->text()).arg(ui->signup_password->text()).arg(ui->signup_tele->text()).arg(ui->signup_idcard->text());
                if(query.exec(str1)) {
                    emit sendSignupUser(ui->signup_username->text());
                    accept();
                }
            }
        }
    }
}
