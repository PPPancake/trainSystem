/**************************与数据库建立联系**************************/
#ifndef CONNECTION_H
#define CONNECTION_H
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>

static bool Connection() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("c:\\libingxuan\\sqlite\\train.db");
    if(!db.open())
    {
        QMessageBox::warning(nullptr,QString("警告"),QString("数据库无法打开!!!"),QMessageBox::Ok);
        return false;
    }
    QSqlQuery query(db);
    return true;
}

#endif // CONNECTION_H
