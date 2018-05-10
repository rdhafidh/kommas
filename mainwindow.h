#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>;
#include <QSqlQuery>
#include <QSqlDatabase>

namespace Ui {
class MainWindow;
}
class EncryptWizard;
class HistoryEncrypt;
class DecryptWizard;
class HistoryDecrypt;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void doENC();
     void doDEC();
	 void saveENC(int a);
     void saveDEC(int b);
     void inputEncData(QString b);
     void inputDecData(QString a);
     void tentang();
     void tutupDB();

      void showHistoryEnc();
      void showHistoryDec();
      void resetShownHistoryEnc();
      void resetShownHistoryDec();

      void doHapusHistory();

protected:
    void changeEvent(QEvent *e);

private:

    bool hapusHistory();
    void setupColumnView();
    void setupDB();
    bool setupDBDatas();

    bool lookhistoryEnc(QString datas);
    bool lookhistoryDec(QString datas);


    HistoryEncrypt *his_enc();
    HistoryEncrypt * m_his_enc;
    HistoryDecrypt *his_dec();
    HistoryDecrypt * m_his_dec;

    bool his_already_shown;
    bool his_dec_alread_shown;

   

    int con;
    Ui::MainWindow *ui;
    EncryptWizard *ec;
    DecryptWizard *dec;
    QSqlQueryModel model,dummyencrypt,dummydecrypt;
    QSqlQuery query;
    QSqlDatabase m_db;
};

#endif // MAINWINDOW_H

