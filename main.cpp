#include "mainwindow.h"
#include "connection.h"
#include "logindialog.h"
#include <QApplication>
#include <iostream>
#include "global.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    if(!Connection())
    {
        QMessageBox::warning(nullptr,"警告","无法打开数据库",QMessageBox::Yes);//无法连接数据库
        return 0;
    } else {
        MainWindow w;
        w.show();//打开主窗口
        return a.exec();
    }
}
