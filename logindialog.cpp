#include "logindialog.h"
#include "ui_logindialog.h"
#include <QDebug>
#include <QTimer>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog) {
        ui->setupUi(this);
        ui->login_password->setEchoMode(QLineEdit::Password);
        ui->label_15->hide();
        ui->login_username->setPlaceholderText("姓名");
        ui->login_password->setPlaceholderText("密码");
    }

LoginDialog::~LoginDialog() {
    delete ui;
}

void LoginDialog::Finished(QString data)//过1秒种会自动消失的提示信息
{
    ui->label_15->setText(data);
    QTimer::singleShot(1000, ui->label_15, SLOT(hide()));
    ui->label_15->show();
}

void LoginDialog::on_login_button_clicked() {//登录
    query.exec("select * from user");
    hasUsername = false;
    while(query.next()) {
        if(query.value("username").toString()==ui->login_username->text()) {//查询是否有对应用户
            hasUsername = true;
            if (query.value("password") == ui->login_password->text()) {//登陆成功
                emit sendUsername(ui->login_username->text(), query.value("user_id").toInt());
                accept();
            } else {
                Finished("密码错误");
                ui->login_password->clear();//清除密码
                ui->login_username->setFocus();  //鼠标回到用户名栏
            }
        }
    }
    if(!hasUsername) {
        Finished("用户名不存在");
        ui->login_password->clear();
        ui->login_username->setFocus();
    }
}

void LoginDialog::on_login_signupbutton_clicked()//点击注册按钮
{
    SignupDialog = new class SignupDialog;
    SignupDialog->show();//打开注册页面
    connect(SignupDialog,SIGNAL(sendSignupUser(QString)),this,SLOT(AfterSignup(QString)));//接受注册后返回的用户名
    if(SignupDialog->exec()==QDialog::Accepted){
        Finished("成功注册账号");
    }
    SignupDialog->close();
}

void LoginDialog::AfterSignup(QString data)//注册后自动填写用户名
{
    ui->login_username->setText(data);
}
