#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "encryptwizard.h"
#include <QtWidgets>
#include <QtConcurrent>
#include "historyencrypt.h"
#include "decryptwizard.h"
#include "historydecrypt.h"
#include <QtSql>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),con(0),his_already_shown(false),his_dec_alread_shown(false),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupDB();
    QSettings seting("kommas-stimik","kommas");
    if (seting.value("pertama").toString() !="tidak"){
        seting.setValue("pertama","ya");
        QFuture<bool> c1= QtConcurrent::run(this,&MainWindow::setupDBDatas);
        c1.waitForFinished();
        if (c1.result()){
            qDebug()<< "pertama";
        ui->statusBar->showMessage(tr("ok instalasi db selesai"),14000);
        seting.setValue("pertama","tidak");
        }
    }

    connect(ui->actionTambah_enkrpsi_pesan,SIGNAL(triggered()),SLOT(doENC()));
    connect(ui->actionTambah_dekripsi_pesan,SIGNAL(triggered()),SLOT(doDEC()));
    connect(ui->actionKeluar,SIGNAL(triggered()),SLOT(close()));
    connect(ui->actionTentang,SIGNAL(triggered()),SLOT(tentang()));

    connect(ui->actionDaftarPesanEnkripsi,SIGNAL(triggered()),SLOT(showHistoryEnc()));
    connect(ui->actionDaftarPesanDekripsi,SIGNAL(triggered()),SLOT(showHistoryDec()));
    connect(ui->actionHapus_history,SIGNAL(triggered()),SLOT(doHapusHistory()));

}

MainWindow::~MainWindow()
{
    tutupDB();
    delete ui;
}

void MainWindow::doENC()
{
    ec = new EncryptWizard;
    connect(ec,SIGNAL(status(int)),SLOT(saveENC(int)));
    connect(ec,SIGNAL(selesai(QString)),SLOT(inputEncData(QString)));
    ec->setAttribute(Qt::WA_DeleteOnClose);
    ec->setModal(true);
    ec->exec();

}

void MainWindow::doDEC()
{
    dec = new DecryptWizard;
    connect(dec,SIGNAL(status(int)),SLOT(saveDEC(int)));
    connect(dec,SIGNAL(selesai(QString)),SLOT(inputDecData(QString)));
    dec->setAttribute(Qt::WA_DeleteOnClose);
    dec->setModal(true);
    dec->exec();
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::setupColumnView()
{
    model.setHeaderData(0,Qt::Horizontal,"Id");
    model.setHeaderData(1,Qt::Horizontal,"Data enkripsi");
    model.setHeaderData(2,Qt::Horizontal,"Waktu");
}

void MainWindow::setupDB()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE",QString("%1").at(con));
    m_db.setDatabaseName("history.db");
    bool ok =m_db.open();
    if (!ok){
        ui->statusBar->showMessage(tr("<html><head/><body><p><span style='color:#ff1e0a;'>warning: db enkripsi history tidak tersimpan</span></p></body></html>"),20000);
    }

}

bool MainWindow::setupDBDatas()
{
    if (m_db.isOpen()){
        QSqlQuery query(m_db);

        return query.exec(QStringLiteral("CREATE TABLE HISTORY (id integer PRIMARY KEY AUTOINCREMENT,data varchar(2090),waktu varchar(29),action varchar(20) );"));
    }
    else {
        return false;
    }
}

bool MainWindow::lookhistoryEnc(QString datas)
{
   if (m_db.isOpen()){
       QSqlQuery query(m_db);
       query.prepare("insert into history (data,waktu,action) values (?,?,?)");
       query.bindValue(0,datas);
       query.bindValue(1,QDate::currentDate().toString("dd-MM-yyyy"));
       query.bindValue(2,"encrypt");
       qDebug()<< query.lastQuery();
       return query.exec();
   }
       return false;

}

bool MainWindow::lookhistoryDec(QString datas)
{
    if (m_db.isOpen()){
        QSqlQuery query(m_db);
        query.prepare("insert into history(data,waktu,action)values(?,?,?)");
        query.bindValue(0,datas);
        query.bindValue(1,QDate::currentDate().toString("dd-MM-yyyy"));
        query.bindValue(2,"decrypt");
        qDebug()<<query.lastQuery();
        return query.exec();
    }
        return false;
}

void MainWindow::showHistoryEnc()
{
    if (!his_already_shown){
        his_already_shown=true;
    HistoryEncrypt * m_h_c = his_enc();
    connect(m_h_c,SIGNAL(destroyed()),SLOT(resetShownHistoryEnc()));
    m_h_c->show();
    }

}

void MainWindow::showHistoryDec()
{
if (!his_dec_alread_shown){
    his_dec_alread_shown=true;
    HistoryDecrypt * m_h_dec = his_dec();
    connect(m_h_dec,SIGNAL(destroyed()),SLOT(resetShownHistoryDec()));
    m_h_dec->show();
}
}

void MainWindow::resetShownHistoryEnc()
{
    his_already_shown=false;
}

void MainWindow::resetShownHistoryDec()
{
    his_dec_alread_shown=false;
}

void MainWindow::doHapusHistory()
{
    int ret= QMessageBox::warning(this,tr("Yakin hapus semua histori"),tr("Apakah anda yakin untuk menghapus semua data history ?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if (ret==QMessageBox::Yes){
        QFuture<bool>f= QtConcurrent::run(this,&MainWindow::hapusHistory);
        f.waitForFinished();
        if (!f.result()){
            ui->statusBar->showMessage(tr("Maaf, gagal menghapus histori"),15000);
            return;
        }
        ui->statusBar->showMessage(tr("Data histori telah dihapus"),15000);
        return;
    }
    else if(ret==QMessageBox::No){
        return;
    }
}

bool MainWindow::hapusHistory()
{
    QSqlQuery query(m_db);
    return query.exec(QStringLiteral("delete from history"));
}

HistoryEncrypt *MainWindow::his_enc()
{
    m_his_enc = new HistoryEncrypt(this,m_db);
    connect(m_his_enc,SIGNAL(showError(QString,int)),ui->statusBar,SLOT(showMessage(QString,int)));
    m_his_enc->setAttribute(Qt::WA_DeleteOnClose);
    ui->mdiArea->addSubWindow(m_his_enc);
    return m_his_enc;
}

HistoryDecrypt *MainWindow::his_dec()
{
    m_his_dec = new HistoryDecrypt(this,m_db);
    connect(m_his_dec,SIGNAL(showError(QString,int)),ui->statusBar,SLOT(showMessage(QString,int)));
    m_his_dec->setAttribute(Qt::WA_DeleteOnClose);
    ui->mdiArea->addSubWindow(m_his_dec);
    return m_his_dec;
}


void MainWindow::saveENC(int a)
{
    qDebug()<< "status enc:"<<a;
}

void MainWindow::saveDEC(int b)
{
    qDebug()<< "status dec: "<< b;
}

void MainWindow::inputEncData(QString b)
{
    qDebug()<< "data yang dienkripsi:" <<b;
    QFuture<bool> f=QtConcurrent::run(this,&MainWindow::lookhistoryEnc,b);
    f.waitForFinished();
    if (!f.result()){
        ui->statusBar->showMessage(tr("Save history gagal."),15000);
        return;
    }
    ui->statusBar->showMessage(tr("Data enkripsi history telah disave"),15000);
}

void MainWindow::inputDecData(QString a)
{
qDebug()<< "data yang terdekripsi:"<< a;
QFuture<bool> f = QtConcurrent::run(this,&MainWindow::lookhistoryDec,a);
f.waitForFinished();
if (!f.result()){
    ui->statusBar->showMessage(tr("Save history gagal."),15000);
    return;
}
ui->statusBar->showMessage(tr("Data dekripsi history telah disave"),15000);
}

void MainWindow::tentang()
{
    QMessageBox::information(this,tr("Tentang Program ini"),tr("Program kommas. \nMenggunakan enkripsi RSA 2048 bit\n Author: Ridwan Abdul Hafidh\nEmail: Ridwan Abdul Hafidh "),QMessageBox::Ok,QMessageBox::Ok);
}

void MainWindow::tutupDB()
{
    ui->mdiArea->closeAllSubWindows();
    if (m_db.isOpen()){
        m_db.close();
    }
}
