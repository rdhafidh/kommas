#ifndef DECRYPTWIZARD_H
#define DECRYPTWIZARD_H

#include <QtWidgets>
#include <QtConcurrent>
#include <QtCrypto>
#include <QtSql>

class HalamanKeduaDecError: public QWizardPage{
    Q_OBJECT
public:

    HalamanKeduaDecError(QWidget *parent=0);
    ~HalamanKeduaDecError();
    int nextId()const;

signals:
    void ready(int a);

private slots:

private:

};

class HalamanKetigaDec:public QWizardPage{
    Q_OBJECT
public:
    HalamanKetigaDec(QWidget *parent=0);
    ~HalamanKetigaDec();
    int nextId()const;

signals:
    void ready(int a);

private slots:
    void enableDec();
    void doDec();

private:
    bool dec_fail;
    QString dec();
    QTextEdit * teks_pesan;
    QToolButton *tool_dekrip;
    QTextEdit * teks_pesan_dec;
    QCA::SecureArray ciphertext,plaintext,password;
    QCA::PublicKey pb;
    QCA::PrivateKey pv;
    QCA::ConvertResult konversi_dekrip;
	QString pesanasli;
};

class HalamanKeduaDec:public QWizardPage{
    Q_OBJECT
public:

    HalamanKeduaDec(QWidget *parent=0);
    ~HalamanKeduaDec();
    int nextId()const;


signals:
    void ready(int a);
private slots:
    void setPemfile();
    void enablePemfile();
private:
    QLineEdit * line_pem_file;
    QLineEdit *line_pem_password;
    QTextEdit * teks_isi_pemfile;
    QToolButton * tool_pem_file;

    QCA::PrivateKey pv;
    QCA::PublicKey pb;
    QCA::SecureArray pwd;
    bool error_sign_dec;
    QString pemfile;

};

class HalamanPertamaDec:public QWizardPage{
    Q_OBJECT
public:
    HalamanPertamaDec(QWidget *parent=0);
    int nextId()const;

    ~HalamanPertamaDec();
signals:
    void ready(int a);

private slots:

private:


};
class PageKeduaDecSignError:public QWizardPage{
    Q_OBJECT
public:
    PageKeduaDecSignError(QWidget *parent=0);
    int nextId() const;
    ~PageKeduaDecSignError();

signals:
    void ready(int a);

private:


};
class HalamanDecSuccess: public QWizardPage{
    Q_OBJECT
public:
    HalamanDecSuccess(QWidget *parent=0);
    ~HalamanDecSuccess();
    int nextId() const;
    void initializePage();

signals:
    void ready(int a);
    void dta(QString b);

private slots:

private:

};
class PageLoadError:public QWizardPage{
    Q_OBJECT
public:
    PageLoadError(QWidget *parent=0);
    ~PageLoadError(){};
    int nextId() const;
    void initializePage();

signals:
    void ready(int b);

private:

};

QCA::PrivateKey testRSAdec();
class DecryptWizard : public QWizard
{
    Q_OBJECT
public:
    explicit DecryptWizard(QWidget *parent=0);
    enum { Page_pertama,Page_kedua_dec,Page_ketiga,Page_kedua_dec_error,Page_kedua_dec_sign_error,Page_dec_success,Page_load_error};
    ~DecryptWizard();
signals:
    void selesai(QString b);
    void status(int a);

private slots:
    void hideBackButton(int d);
    void showHelp();
    void akhiri(int b);
    void selesaikan(QString c);

private:
    HalamanPertamaDec *hal1;
    HalamanKeduaDec * hal2;
    HalamanKetigaDec * hal3;
    HalamanKeduaDecError * hal4;
    PageKeduaDecSignError * hal5;
    HalamanDecSuccess * hal6;
    PageLoadError * hal7;
    QSqlDatabase m_db;


};

#endif // DECRYPTWIZARD_H
