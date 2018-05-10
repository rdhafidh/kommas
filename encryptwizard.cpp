#include "encryptwizard.h"


EncryptWizard::EncryptWizard(QWidget *parent) :
    QWizard(parent)
{
  //setDefaultProperty("QTextEdit","plainText","textChanged()");
	setWizardStyle(	QWizard::ModernStyle);
     setWindowIcon(QIcon(":/lock-26.png"));

    hal1 = new HalamanPertama;
    hal2 = new HalamanKedua;
    hal2b = new HalamanKeduaB;
    hal3 = new HalamanKetiga;
    hal4 = new HalamanError;
    hal5 = new HalamanErroPemfile;
    hal6 = new HalamanKeduaBENC;

    setPage(Page_pertama,hal1);
    setPage(Page_kedua,hal2);
    setPage(Page_kedua_b,hal2b);
    setPage(Page_ketiga,hal3);
    setPage(Page_error,hal4);
    setPage(Page_encpem_error,hal5);
    setPage(Page_kedua_b_enc,hal6);
    connect(hal3,SIGNAL(ready(int)),SLOT(akhiri(int)));
    connect(hal4,SIGNAL(ready(int)),SLOT(akhiri(int)));
    connect(hal2b,SIGNAL(ready(int)),SLOT(akhiri(int)));

    connect(hal3,SIGNAL(dta(QString)),SLOT(selesaikan(QString)));

    setWindowTitle(tr("Panduan enkrpsi & dekrpsi pesan RSA"));
    setStartId(Page_pertama);
    connect(this,SIGNAL(helpRequested()),SLOT(showHelp()));
    connect(this,SIGNAL(currentIdChanged(int)),SLOT(hideBackOnErrorPem(int)));

    setOption(QWizard::HaveHelpButton,true);

}

EncryptWizard::~EncryptWizard()
{
  hal1->deleteLater();
  hal2->deleteLater();
  hal3->deleteLater();
  hal4->deleteLater();
  hal5->deleteLater();
  hal6->deleteLater();
  hal2b->deleteLater();

}

void EncryptWizard::showHelp()
{
    QString message;
    switch(currentId()){
    case Page_pertama:
        message = tr("Dalam halaman ini kami akan memandu anda untuk melakukan enkripsi dan dekrpsi pesan dengan metode RSA");
        break;
    case Page_kedua:
        message = tr("Mohon isikan password dan data pesan yang anda inginkan untuk enkripsi");
        break;
    case Page_ketiga:
        message = tr("Enkripsi anda berhasill");
        break;
    case Page_kedua_b:
        message = tr("Mohon, autentikasikan terlebih dahulu password dengan .pem private/public key  file");
        break;
    case Page_kedua_b_enc:
        message = tr("Mohon, isikan pesan untuk dilakukan enkripsi pesan");
        break;
    case Page_encpem_error:
        message = tr("Maaf, terjadi kesalahan autentikasi password dengan .pem file oleh input anda. Anda bisa mencobanya lagi dengan klik finish lalu memulai enkripsi dari awal");
        break;
    case Page_error:
        message = tr("Maaf, terjadi kesalahan ketika memuat plugin openssl");
        break;
    default:
        message = tr("");
    }
    QMessageBox::information(this,tr("Informasi bantuan"),message);
}

void EncryptWizard::akhiri(int b)
{
    if (b==0){
        emit status(0);
    }else if (b==1){
        emit status(1); //casual error
    }else if (b==2){
        emit status(2);//error signing enc pem file
    }
}

void EncryptWizard::selesaikan(QString c)
{
    emit selesai(c);
}

void EncryptWizard::hideBackOnErrorPem(int a)
{
    if (a==EncryptWizard::Page_encpem_error){
        button(QWizard::BackButton)->hide();
        button(QWizard::CancelButton)->hide();
        return;
    }
    else if(a==EncryptWizard::Page_ketiga){
        button(QWizard::BackButton)->hide();
         button(QWizard::CancelButton)->hide();
        return;
    }else if(a==EncryptWizard::Page_error){
        button(QWizard::BackButton)->hide();
         button(QWizard::CancelButton)->hide();
        return;
    }
}


HalamanPertama::HalamanPertama(QWidget *parent):QWizardPage(parent)
{
    setTitle(tr("Panduan RSA"));
    setSubTitle(tr("Anda akan kami pandu untuk melakukan penguncia RSA"));
    setPixmap(QWizard::WatermarkPixmap,QPixmap(":/Picture1.png"));
    info = new QLabel;
    info->setText(tr("Anda akan kami pandu untuk melakuan proses enkripsi pesan RSA"));
    info->setWordWrap(true);
    radiobutton_enkrpsi = new QRadioButton(tr("Lakukan enkripsi awal dengan input password"));
    radiobutton_enkrpsi_from_file = new QRadioButton(tr("Lakukan enkrpsi dengan input public key file"));
    radiobutton_enkrpsi->setChecked(true);

    layout = new QVBoxLayout;
    layout->addWidget(info);
    layout->addWidget(radiobutton_enkrpsi);
    layout->addWidget(radiobutton_enkrpsi_from_file);
    setLayout(layout);

}

int HalamanPertama::nextId() const
{
    if(!QCA::isSupported("pkey") ||
          !QCA::PKey::supportedIOTypes().contains(QCA::PKey::RSA)){
       return EncryptWizard::Page_error;
    }
    if (testRSA().isNull()){
        return EncryptWizard::Page_error;
    }
    if (radiobutton_enkrpsi->isChecked()){
        return EncryptWizard::Page_kedua;
    }
    else if (radiobutton_enkrpsi_from_file->isChecked()){
        return EncryptWizard::Page_kedua_b;
    }
	return EncryptWizard::Page_error;
}


HalamanKedua::HalamanKedua(QWidget *parent):QWizardPage(parent)
{
    already_enc=false;
    setTitle(tr("Isikan kunci key RSA anda"));
    setSubTitle(tr("Mohon dilengkapi isian berikut"));
    layout = new QGridLayout;

    QLabel * teks_key= new QLabel;
    teks_key->setText(tr("Isikan password"));
    edit_key = new QLineEdit;
    teks_key->setBuddy(edit_key);
    layout->addWidget(teks_key,0,0,1,1);
    layout->addWidget(edit_key,0,1,1,1);


    QLabel * teks_public_file = new QLabel;
    teks_public_file->setText(tr("Public key file"));
   edit_dir = new QLineEdit;
    edit_dir->setReadOnly(true);
   tool_dir = new QToolButton;
    tool_dir->setIcon(QIcon(":/save-26.png"));
    layout->addWidget(teks_public_file,1,0,1,1);
    layout->addWidget(edit_dir,1,1,1,1);
    layout->addWidget(tool_dir,1,2,1,1);

    QLabel * teks_public = new QLabel;
   teks_public->setText(tr("Public key text"));
   teks_teksedit  = new QTextEdit;
    teks_teksedit->setReadOnly(true);
    layout->addWidget(teks_public,2,0,1,1);
    layout->addWidget(teks_teksedit,2,1,1,1);

    QLabel *teks_data = new QLabel;
    teks_data->setText(tr("Input pesan yang akan di enkripsi"));
    teksdata = new QTextEdit;
    tool_enkrip = new QToolButton;
    tool_enkrip->setText(tr("Enkripsi"));
    layout->addWidget(teks_data,3,0,1,1);
    layout->addWidget(teksdata,3,1,1,1);
    layout->addWidget(tool_enkrip,3,2,1,1);
    connect(tool_enkrip,SIGNAL(clicked()),SLOT(doEnc()));

    QLabel * teks_ci = new QLabel;
    teks_ci->setText(tr("Ciphertext/teks yang telah di enkripsi"));
    teksedit_ci = new QTextEdit;
    teksedit_ci->setReadOnly(true);
    layout->addWidget(teks_ci,4,0,1,1);
    layout->addWidget(teksedit_ci,4,1,1,1);

    connect(edit_key,SIGNAL(textChanged(QString)),SLOT(enableToolPEM(QString)));
    connect(teksdata,SIGNAL(textChanged()),SLOT(enableToolEnc()));
    connect(tool_dir,SIGNAL(clicked()),SLOT(setPemFile()));
    connect(tool_enkrip,SIGNAL(clicked()),SLOT(doEnc()));
    setLayout(layout);
    registerField("halamanKedua.editkey*",edit_key);
    registerField("halamanKedua.teksdata*",teksdata,"plainText",SIGNAL(textChanged()));
}

int HalamanKedua::nextId() const
{
    return EncryptWizard::Page_ketiga;
}
QCA::PrivateKey testRSA(){
    return QCA::KeyGenerator().createRSA(2048);
}

QCA::PublicKey  HalamanKedua::generateRSA()
{
    seckey = testRSA();
    QCA::PublicKey pubkey = seckey.toPublicKey();
    return pubkey;
}


void HalamanKedua::setPemFile()
{
    pubkey = generateRSA();
    pwd =edit_key->text().toLatin1();
    pemfile =QFileDialog::getSaveFileName(this,tr("pilih direktori save Public key file"),".",tr("Pem file (*.pem)"));
    seckey.toPEMFile(pemfile,pwd);
    edit_dir->setReadOnly(false);
    edit_dir->setText(pemfile);
    edit_dir->setReadOnly(true);
    teks_teksedit->setReadOnly(false);
    QFile fl(pemfile);
    QString t_;
    if (fl.open(QFile::ReadOnly|QFile::Text)){
        while(!fl.atEnd()){
            t_+=fl.readLine();
        }
    }
    fl.close();

    teks_teksedit->setText(t_);
    teks_teksedit->setReadOnly(true);

}

QString HalamanKedua::enc()
{

    if (!pubkey.canEncrypt()){
        return QString("");
    }
    result=pubkey.encrypt(pwd,QCA::EME_PKCS1_OAEP);
    if(result.isEmpty()){
        return  QString("");
    }
    return QCA::arrayToHex(result.toByteArray());
}

void HalamanKedua::doEnc()
{
    if(!already_enc){
    QFuture<QString> hasil = QtConcurrent::run(this,&HalamanKedua::enc);
    hasil.waitForFinished();
    if (!hasil.result().isEmpty()){
        teksedit_ci->setReadOnly(false);
        teksedit_ci->setText(hasil.result());
        teksedit_ci->setReadOnly(true);
        already_enc=true;
    }
    }
}

void HalamanKedua::enableToolPEM(QString b)
{
    if (!b.isEmpty()){
        tool_dir->setEnabled(true);
    }
    else{
        tool_dir->setEnabled(false);
    }
}

void HalamanKedua::enableToolEnc()
{
    if (!teksdata->toPlainText().isEmpty()){
        tool_enkrip->setEnabled(true);
    }else{
        tool_enkrip->setEnabled(false);
    }
}


HalamanKetiga::HalamanKetiga(QWidget *parent):QWizardPage(parent)
{
    setTitle("Selesai");
    setSubTitle(tr("Selamat proses enkripsi anda berhasil"));
    setPixmap(QWizard::WatermarkPixmap,QPixmap(":/Picture1.png"));
    QLabel * l = new QLabel;
    l->setText(tr("Anda telah melakukan proses enkripsi pesan dengan RSA 2048 bit key"));
    l->setWordWrap(true);
    QVBoxLayout * la= new QVBoxLayout;
    la->addWidget(l);
    setLayout(la);

}

int HalamanKetiga::nextId() const
{

    return -1;
}

void HalamanKetiga::initializePage()
{
    if (wizard()->hasVisitedPage(EncryptWizard::Page_kedua)){
    emit dta(field("halamanKedua.teksdata").toString());
        emit ready(0);
        return;
    }
    else if(wizard()->hasVisitedPage(EncryptWizard::Page_kedua_b_enc)){
        emit dta(field("halamanKeduaBENC.teksdata").toString());
       emit ready(0);
        return;
    }
}


HalamanError::HalamanError(QWidget *parent):QWizardPage(parent)
{
 setTitle(tr("maaf, terjadi kesalahan"));
 setSubTitle(tr("Ooops, terjadi kesalahan"));
 QVBoxLayout * l=new QVBoxLayout;
 QLabel * a=new QLabel;
 a->setText(tr("Maaf, terjadi kesalahan saat memuat openssl plugin untuk RSA"));
 l->addWidget(a);
 setLayout(l);
 emit ready(0);
}

int HalamanError::nextId() const
{
    return -1;
}


HalamanKeduaB::HalamanKeduaB(QWidget *parent):QWizardPage(parent)
{
    setTitle(tr("Autentikas password Public key file"));
    setSubTitle(tr("Mohon isikan password beserta private/public key file sebelum melakukan enkripsi pesan"));
    QGridLayout * l = new QGridLayout;

    QLabel *pwd_edit = new QLabel;
    pwd_line = new QLineEdit;
    pwd_edit->setText("Isikan password dari pem file berikut:");
    l->addWidget(pwd_edit,0,0,1,1);
    l->addWidget(pwd_line,0,1,1,1);

    QLabel *file_key = new QLabel;
    file_key->setText(tr("Pilih file .pem private/public key anda"));
    file_edit= new QLineEdit;
    file_edit->setReadOnly(true);
    tool_file_edit= new QToolButton;
    tool_file_edit->setEnabled(false);
    tool_file_edit->setIcon(QIcon(":/folder-26.png"));
    tool_file_edit->setText("");
    l->addWidget(file_key,1,0,1,1);
    l->addWidget(file_edit,1,1,1,1);
    l->addWidget(tool_file_edit,1,2,1,1);

    QLabel *file_teks = new QLabel;
    file_teks->setText(tr("Data .pem private/public key anda "));
    tekspem = new QTextEdit;
    tekspem->setReadOnly(true);
    l->addWidget(file_teks,2,0,1,1);
    l->addWidget(tekspem,2,1,1,1);


    connect(pwd_line,SIGNAL(textChanged(QString)),SLOT(enablePemfile()));

    connect(tool_file_edit,SIGNAL(clicked()),SLOT(setPemfile()));
    error_sign_pem=false;
    registerField("halamanKeduaB.pemfile*",file_edit);
    registerField("halamanKeduaB.pwd*",pwd_line);



    setLayout(l);

}

int HalamanKeduaB::nextId() const
{
    if (!error_sign_pem ){
        return EncryptWizard::Page_kedua_b_enc;
    }else{
        return EncryptWizard::Page_encpem_error;
    }
}

void HalamanKeduaB::setPemfile()
{
    file_edit->setReadOnly(false);
    pemfile = QFileDialog::getOpenFileName(this,tr("Pilih file"),".",tr("Pem file (*.pem)"));
    file_edit->setText(pemfile);
    file_edit->setReadOnly(true);
    QFile fl(pemfile);
    QString t_;
    if (fl.open(QFile::ReadOnly|QFile::Text)){
        while(!fl.atEnd()){
            t_+=fl.readLine();
        }
    }
    fl.close();
    tekspem->setReadOnly(false);
    tekspem->setText(t_);
    tekspem->setReadOnly(true);

    privkey = QCA::PrivateKey::fromPEMFile(pemfile,pwd,&conversionResult);
    if (!(QCA::ConvertGood ==conversionResult)){
        emit ready(2);
        error_sign_pem=true;
    }
    if (!error_sign_pem){
//        wizard()->re
     //   wizard()->setField("halamanKeduaB.password",pwd_line->text());
  //      wizard()->setField("halamanKeduaB.privatefile",pemfile);
    }
}


void HalamanKeduaB::enablePemfile()
{
    pwd =pwd_line->text().toLatin1();
    if (pwd_line->text().isEmpty()){
        tool_file_edit->setEnabled(false);
    }else{
        tool_file_edit->setEnabled(true);
    }
}


HalamanErroPemfile::HalamanErroPemfile(QWidget *parent):QWizardPage(parent)
{
    setTitle(tr("Terjadi kesalahan"));
    setSubTitle(tr("terjadi kesalahan autentikasi"));
    QVBoxLayout * l=new QVBoxLayout;
    QLabel * label = new QLabel;
    label->setText(tr("Maaf, telah terjadi kesalahan dalam mengautentikasi private .pem file dengan password yang sebenarnya"));
    label->setWordWrap(true);
    QLabel *labeldesc = new QLabel;
    labeldesc->setText(tr("Diagnosa kami, kemungkinan masalah yang terjadi:\n"
                          "1. File .pem private/pubkey korup\n"
                          "2. Anda salah menginputkan password sesuai dengan passwords .pem tersebut, ketika dibuat pertama kali\n\n"
                          "Anda bisa menutup panduan ini, dengan klik 'Finish', lalu ulangi proses enkripsi dari awal panduan atau tekan ctrl+n "));

    l->addWidget(label);
    l->addWidget(labeldesc);
    setLayout(l);

}

int HalamanErroPemfile::nextId() const
{
    return -1;
}

void HalamanErroPemfile::cleanupPage()
{
    qDebug()<< "pwd:"<<field("halamanKeduaB.pwd").toString();
}


HalamanKeduaBENC::HalamanKeduaBENC(QWidget *parent):QWizardPage(parent)
{
    setTitle(tr("Proses enkripsi pesan"));
    setSubTitle(tr("Lakukan proses enkripsi pesan"));

    QGridLayout *l = new QGridLayout;
    QLabel *label_data=new QLabel;
    label_data->setText(tr("Isikan pesan anda"));
    teksdata = new QTextEdit;
    tool_enkripsi_pesan = new QToolButton;
    tool_enkripsi_pesan->setText("Enkripsi");
    tool_enkripsi_pesan->setEnabled(false);
    l->addWidget(label_data,0,0,1,1);
    l->addWidget(teksdata,0,1,1,1);
    l->addWidget(tool_enkripsi_pesan,0,2,1,1);

    QLabel *label_ci = new QLabel;
    label_ci->setText(tr("Ciphertext/pesan yang terenkripsi"));
    teksciphertext = new QTextEdit;
    teksciphertext->setReadOnly(true);
    l->addWidget(label_ci,1,0,1,1);
    l->addWidget(teksciphertext,1,1,1,1);


    already_enc=false;
    gagal_enc=false;

    registerField("halamanKeduaBENC.teksdata*",teksdata,"plainText",SIGNAL(textChanged()));
    connect(teksdata,SIGNAL(textChanged()),SLOT(enableEnc()));
    connect(tool_enkripsi_pesan,SIGNAL(clicked()),SLOT(doENC()));

    setLayout(l);

}

int HalamanKeduaBENC::nextId() const
{
    int  ret;
   if (gagal_enc){
       ret = QMessageBox::warning(0,tr("Gagal mengenkripsi pesan"),tr("Apakah anda ingin mengulangi proses setting password & private/public key file?"),QMessageBox::Ok|QMessageBox::No,QMessageBox::Ok);

       if (ret ==QMessageBox::Ok){
            return EncryptWizard::Page_kedua_b;
       }
       else if (ret ==QMessageBox::No){
           return -1;
       }
   }
   return EncryptWizard::Page_ketiga;
}

void HalamanKeduaBENC::enableEnc()
{
    if (teksdata->toPlainText().isEmpty()){
        tool_enkripsi_pesan->setEnabled(false);
    }else{
        tool_enkripsi_pesan->setEnabled(true);
    }
}

QString HalamanKeduaBENC::enc()
{
    if (!pb.canEncrypt()){
        return QString("");
    }
    s_result=pb.encrypt(s_data,QCA::EME_PKCS1_OAEP);
    if(s_result.isEmpty()){
        return  QString("");
    }
    return QCA::arrayToHex(s_result.toByteArray());

}

void HalamanKeduaBENC::doENC()
{
   password= field("halamanKeduaB.pwd").toString();
   pemfile = field("halamanKeduaB.pemfile").toString();
   s_data = teksdata->toPlainText().toLatin1();
   qDebug()<<"password: "<<password;
   qDebug()<<"pemfile: "<<pemfile;
   s_pwd=password.toLatin1();
   QCA::ConvertResult result;
   pv = QCA::PrivateKey::fromPEMFile(pemfile,s_pwd,&result);

   if (!(QCA::ConvertGood==result)){
       qDebug()<<"autentikasi gagal, kenapa bsa?";
       return;
   }
   pb=pv.toPublicKey();
   QFuture<QString> f=QtConcurrent::run(this,&HalamanKeduaBENC::enc);
   f.waitForFinished();
   if (f.result().isEmpty()){
       gagal_enc=true;
       qDebug()<<"enkripsi gagal, kenapa bisa?";
       return;
   }

   already_enc=true;
   teksciphertext->setReadOnly(false);
   teksciphertext->setText(f.result());
   teksciphertext->setReadOnly(true);

}
