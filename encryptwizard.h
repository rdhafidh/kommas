#ifndef ENCRYPTWIZARD_H
#define ENCRYPTWIZARD_H

#include <QWizard>
#include <QtWidgets>
#include <QtConcurrent>
#include <QtCrypto>
class HalamanPertama:public QWizardPage{
    Q_OBJECT
 public:
    HalamanPertama(QWidget *parent=0);
    int nextId() const;
    ~HalamanPertama(){
     }
private:
    QLabel* info;
    QVBoxLayout *layout;
    QRadioButton *radiobutton_enkrpsi;
    QRadioButton *radiobutton_enkrpsi_from_file;


};
class HalamanKedua:public QWizardPage{
    Q_OBJECT
public:
    HalamanKedua(QWidget * parent=0);
    int nextId() const;
    QCA::PublicKey generateRSA();

    ~HalamanKedua(){

    }

signals:
    void ready(int a);

public slots:
    void setPemFile();
    QString enc();
    void doEnc();
    void enableToolPEM(QString b);
    void enableToolEnc();

private:
    bool already_enc;
       QCA::PublicKey  pubkey;
     QCA::SecureArray pwd,result;
    QGridLayout * layout;
    QString pemfile;
     QCA::PrivateKey seckey ;
     QLineEdit *edit_dir;
      QLineEdit * edit_key;
      QTextEdit *teks_teksedit;
      QTextEdit *teksedit_ci;
      QTextEdit * teksdata;
       QToolButton * tool_dir;
        QToolButton *tool_enkrip;

};
class HalamanKeduaB:public QWizardPage{
    Q_OBJECT
public:
    HalamanKeduaB(QWidget *parent=0);

    int nextId()const;
    ~HalamanKeduaB(){

    }

signals:
    void ready(int a);
private slots:
    void setPemfile();
    void enablePemfile();

private:
    QString pemfile,pwdstring;
    bool already_enc;
   QCA::ConvertResult conversionResult;
    QCA::PublicKey  pubkey;
  QCA::SecureArray pwd,result;
  QCA::PrivateKey privkey;
   QLineEdit *pwd_line;
   QTextEdit * teksdata;
   QTextEdit * tekspem;
    QLineEdit * file_edit;
     QToolButton * tool_file_edit ;
      bool error_sign_pem;
};
class HalamanKeduaBENC:public QWizardPage{
    Q_OBJECT
public:
    HalamanKeduaBENC(QWidget *parent=0);
    int nextId()const;
    ~HalamanKeduaBENC(){

    }

signals:

private slots:
    void enableEnc();
    QString enc();
    void doENC();

private:
    bool already_enc;
    bool gagal_enc;
    QToolButton *tool_enkripsi_pesan;
    QTextEdit *teksdata;
    QTextEdit *teksciphertext;
    QString pemfile,password;
    QCA::SecureArray s_pwd,s_result,s_data;
    QCA::PrivateKey pv;
    QCA::PublicKey pb;
};

class HalamanKetiga:public QWizardPage{
    Q_OBJECT
public:
    HalamanKetiga(QWidget*parent=0);
    int nextId()const;
    void initializePage();
    ~HalamanKetiga(){

    }

signals:
    void dta(QString d);
    void ready(int a);

private:

};
class HalamanError:public QWizardPage{
    Q_OBJECT
public:
    HalamanError(QWidget * parent=0);
    int nextId()const;
    ~HalamanError(){}

signals:
    void ready(int b);
private:

};
class HalamanErroPemfile:public QWizardPage{
    Q_OBJECT
public:
    HalamanErroPemfile(QWidget * parent= 0 );
    ~HalamanErroPemfile(){}
    int nextId() const;
    void cleanupPage();
signals:
    void ready(int b);
private:

};

QCA::PrivateKey testRSA();


class EncryptWizard : public QWizard
{
    Q_OBJECT
public:
    enum {Page_pertama,Page_kedua,Page_ketiga,Page_keempat,Page_error,Page_kedua_b,Page_encpem_error,Page_kedua_b_enc };
    explicit EncryptWizard(QWidget *parent = 0);
    ~EncryptWizard();


signals:
    void selesai(QString b);
    void status(int a);


public slots:
    void showHelp();
    void akhiri(int b);
    void selesaikan(QString c);
    void hideBackOnErrorPem(int a);
private:
    HalamanPertama * hal1;
    HalamanKedua *hal2;
    HalamanKeduaB *hal2b;
    HalamanKetiga *hal3;
    HalamanError *hal4;
    HalamanErroPemfile *hal5;
    HalamanKeduaBENC * hal6;

};

#endif // ENCRYPTWIZARD_H
