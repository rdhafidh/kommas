#ifndef HISTORYDECRYPT_H
#define HISTORYDECRYPT_H

#include <QFrame>
#include <QSqlDatabase>
#include <QtConcurrent>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSortFilterProxyModel>

namespace Ui {
class HistoryDecrypt;
}

class HistoryDecrypt : public QFrame
{
    Q_OBJECT

public:
    explicit HistoryDecrypt(QWidget *parent,QSqlDatabase & db);
    ~HistoryDecrypt();
signals:
    void showError(QString a,int b);
protected:
    void changeEvent(QEvent *e);

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_toolButton_clicked();

private:
    void populateDatas();
    void filter();
    Ui::HistoryDecrypt *ui;
    QSqlDatabase m_db;
    QSortFilterProxyModel *m_filter;
    QSqlQueryModel * m_model;
    QString filters;
};

#endif // HISTORYDECRYPT_H
