#ifndef HISTORYENCRYPT_H
#define HISTORYENCRYPT_H

#include <QFrame>
#include <QSqlDatabase>
#include <QtConcurrent>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSortFilterProxyModel>

namespace Ui {
class HistoryEncrypt;
}

class HistoryEncrypt : public QFrame
{
    Q_OBJECT

public:
    explicit HistoryEncrypt(QWidget *parent ,QSqlDatabase & db);
    ~HistoryEncrypt();

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

    Ui::HistoryEncrypt *ui;
    QSqlDatabase m_db;
    QSortFilterProxyModel *m_filter;
    QSqlQueryModel * m_model;
    QString filters;

};

#endif // HISTORYENCRYPT_H
