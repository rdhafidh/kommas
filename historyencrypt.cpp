#include "historyencrypt.h"
#include "ui_historyencrypt.h"

HistoryEncrypt::HistoryEncrypt(QWidget *parent, QSqlDatabase &db) :
    QFrame(parent),m_db(db),
    ui(new Ui::HistoryEncrypt)
{
    ui->setupUi(this);
    populateDatas();
    m_filter = new QSortFilterProxyModel;
    m_filter->setSourceModel(m_model);

}

HistoryEncrypt::~HistoryEncrypt()
{
    delete ui;
    m_model->deleteLater();
    m_filter->deleteLater();
}

void HistoryEncrypt::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void HistoryEncrypt::on_lineEdit_textChanged(const QString &arg1)
{
    filters=arg1;
    filter();

}

void HistoryEncrypt::on_toolButton_clicked()
{
    m_model->clear();
    populateDatas();
}

void HistoryEncrypt::populateDatas()
{
    if (!m_db.isOpen()){
        emit showError(tr("Maaf, memuat database history gagal,coba cek koneksinya ?"),15000);
        return;
    }
    m_model =new QSqlQueryModel;
    m_model->setQuery("select data,waktu from history where action ='encrypt'",m_db);
    m_model->setHeaderData(0,Qt::Horizontal,QString("Data telah dienkripsi"));
    m_model->setHeaderData(1,Qt::Horizontal,QString("Waktu"));
    ui->tableView->setModel(m_model);
    ui->tableView->resizeColumnsToContents();

}

void HistoryEncrypt::filter()
{
    ui->tableView->setModel(m_filter);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_filter->setFilterKeyColumn(0); //data
    m_filter->setFilterCaseSensitivity(Qt::CaseInsensitive);
   m_filter->setFilterWildcard("*"+filters+"*");

}
