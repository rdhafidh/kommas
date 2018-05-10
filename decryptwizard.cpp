#include "decryptwizard.h"

DecryptWizard::DecryptWizard(QWidget *parent) :
    QWizard(parent)
{
    hal1= new HalamanPertamaDec;
    hal2 = new HalamanKeduaDec;
    hal3 = new HalamanKetigaDec;
    hal4 = new HalamanKeduaDecError;
    hal5 = new PageKeduaDecSignError;
    hal6 = new HalamanDecSuccess;
    hal7 = new PageLoadError;

    setPage(Page_pertama,hal1);
    setPage(Page_kedua_dec,hal2);
    setPage(Page_ketiga,hal3);
    setPage(Page_kedua_dec_error,hal4);
    setPage(Page_kedua_dec_sign_error,hal5);
    setPage(Page_dec_success,hal6);
    setPage(Page_load_error,hal7);
    setStartId(Page_pertama);
    setOption(QWizard::HaveHelpButton,true);
    setWindowTitle(tr("Panduan proses dekripsi pesan RSA 2048 bit"));
    setWindowIcon(QIcon(":/lock-26.png"));
    setWizardStyle(	QWizard::ModernStyle);
    connect(this,SIGNAL(helpRequested()),SLOT(showHelp()));
    connect(this,SIGNAL(currentIdChanged(int)),SLOT(hideBackButton(int)));
    connect(hal2,SIGNAL(ready(int)),SLOT(akhiri(int)));
    connect(hal3,SIGNAL(ready(int)),SLOT(akhiri(int)));
    connect(hal7,SIGNAL(ready(int)),SLOT(akhiri(int)));
    connect(hal6,SIGNAL(dta(QString)),SLOT(selesaikan(QString)));


}


DecryptWizard::~DecryptWizard()
{
    hal1->deleteLater();
    hal2->deleteLater();
    hal3->deleteLater();
    hal4->deleteLater();
    hal5->deleteLater();

}

void DecryptWizard::hideBackButton(int d)
{
if (d==DecryptWizard::Page_dec_success){
    button(QWizard::BackButton)->hide();
    button(QWizard::CancelButton)->hide();
    return;
}else if(d==DecryptWizard::Page_kedua_dec_error){
    button(QWizard::BackButton)->hide();
    button(QWizard::CancelButton)->hide();
    return;
}else if (d==DecryptWizard::Page_load_error){
    button(QWizard::BackButton)->hide();
    button(QWizard::CancelButton)->hide();
    return;
}
else if (d==DecryptWizard::Page_kedua_dec_sign_error){
    button(QWizard::BackButton)->hide();
    button(QWizard::CancelButton)->hide();
    return;
}else if(d==DecryptWizard::Page_kedua_dec){
    button(QWizard::BackButton)->hide();
    return;
}else if (d==DecryptWizard::Page_ketiga){
    button(QWizard::BackButton)->hide();

    return;
}
}

void DecryptWizard::showHelp()
{
    QString message;
    if (currentId()==DecryptWizard::Page_pertama){
        message = tr("Anda akan kami pandu untuk memulai dekripsi pesan");

    }else if(currentId()==DecryptWizard::Page_kedua_dec){
        message = tr("Mohon isikan kolom isian berikut untuk kami proses dekripsi");

    }else if(currentId()==DecryptWizard::Page_ketiga){
        message = tr("Selamat, Anda telah melakukan proses dekripsi pesan");

    }else if(currentId()==DecryptWizard::Page_kedua_dec_error){
        message = tr("Maaf,telah terjadi kesalahan dalam proses dekripsi pesan, klik finish lalu bisa diulangi dari proses awal");

    }else if (currentId()==DecryptWizard::Page_load_error){
        message= tr("Maaf, telah terjadi kesalahan dalam memuat plugin sistem, anda kami harapkan untuk melakukan re-installasi aplikasi ini. ");
    }
    QMessageBox::information(0,tr("Bantuan help"),message,QMessageBox::Yes,QMessageBox::Yes);
}

void DecryptWizard::akhiri(int b)
{
    if (b==0){
        emit status(0); //success
    }else if (b==1){
        emit status(1); //casual error dec process
    }else if (b==2){
        emit status(2);//error signing dec pem file
    }
}

void DecryptWizard::selesaikan(QString c)
{
    emit selesai(c);
}


HalamanKeduaDec::HalamanKeduaDec(QWidget *parent):QWizardPage(parent),error_sign_dec(false)
{
setTitle(tr("Proses dekripsi RSA 2048bit"));
setSubTitle(tr("Mohon diisikan dengan lengkap kolom berikut untuk proses autentikasi RSA 2048bit antara .pem file anda dengan passwordnya"));
QGridLayout *l = new QGridLayout;
QLabel *label_pem_file =new QLabel;
label_pem_file->setText(tr("Isikan .pem file anda: "));
line_pem_file = new QLineEdit;
line_pem_file->setReadOnly(true);
tool_pem_file = new QToolButton;
tool_pem_file->setIcon(QIcon(":/folder-26.png"));
tool_pem_file->setEnabled(false);
l->addWidget(label_pem_file,1,0,1,1);
l->addWidget(line_pem_file,1,1,1,1);
l->addWidget(tool_pem_file,1,2,1,1);

QLabel *label_password_pemfile = new QLabel;
label_password_pemfile->setText(tr("Isikan password .pem file anda"));
line_pem_password = new QLineEdit;
l->addWidget(label_password_pemfile,0,0,1,1);
l->addWidget(line_pem_password,0,1,1,1);

QLabel *label_isi_pemfile = new QLabel;
label_isi_pemfile->setText(tr("Isi file .pem file anda"));
teks_isi_pemfile = new QTextEdit;
teks_isi_pemfile->setReadOnly(true);
l->addWidget(label_isi_pemfile,2,0,1,1);
l->addWidget(teks_isi_pemfile,2,1,1,1);

registerField("halamanKeduaDec.line_pem_file*",line_pem_file);
registerField("halamanKeduaDec.line_pem_password*",line_pem_password);

connect(tool_pem_file,SIGNAL(clicked()),SLOT(setPemfile()));
connect(line_pem_password,SIGNAL(textChanged(QString)),SLOT(enablePemfile()));
setLayout(l);

}

HalamanKeduaDec::~HalamanKeduaDec()
{
}

int HalamanKeduaDec::nextId() const
{
    if (error_sign_dec){
        return DecryptWizard::Page_kedua_dec_sign_error;
    }else{
        return DecryptWizard::Page_ketiga;
    }

}

void HalamanKeduaDec::setPemfile()
{
 pemfile = QFileDialog::getOpenFileName(this,tr("Pilih pem file anda"),".",tr("Pem file (*.pem)"));
 line_pem_file->setReadOnly(false);
 line_pem_file->setText(pemfile);
 line_pem_file->setReadOnly(true);


 QFile fl(pemfile);
 QString t_;
 if (fl.open(QFile::ReadOnly|QFile::Text)){
     while(!fl.atEnd()){
         t_+=fl.readLine();
     }
 }
 fl.close();
 teks_isi_pemfile->setReadOnly(false);
 teks_isi_pemfile->setText(t_);
 teks_isi_pemfile->setReadOnly(true);
 QCA::ConvertResult conversionResult;
 pwd = line_pem_password->text().toLatin1();

 pv = QCA::PrivateKey::fromPEMFile(pemfile,pwd,&conversionResult);
 if (!(QCA::ConvertGood ==conversionResult)){
     emit ready(2);
     error_sign_dec=true;
 }

}

void HalamanKeduaDec::enablePemfile()
{
    if (line_pem_password->text().isEmpty()){
        tool_pem_file->setEnabled(false);
        return;
    }else {
        tool_pem_file->setEnabled(true);
        return;
    }
}

HalamanPertamaDec::HalamanPertamaDec(QWidget *parent):QWizardPage(parent){
    setTitle(tr("Panduan dekripsi RSA 2048 bit"));
    setSubTitle(tr("Anda akan kami pandu untuk memulai proses dekripsi pesan RSA 2048 bit"));
    QVBoxLayout *l = new QVBoxLayout;
    QLabel * label_msg= new QLabel;
    label_msg->setText(tr("Anda bisa klik next untuk melanjutkan proses dekripsi pesan RSA 2048 bit"));
    l->addWidget(label_msg);
    setPixmap(QWizard::WatermarkPixmap,QPixmap(":/Picture1.png"));
    setLayout(l);

}
QCA::PrivateKey testRSAdec(){
    return QCA::KeyGenerator().createRSA(2048);
}

int HalamanPertamaDec::nextId() const
{
 if(!QCA::isSupported("pkey") ||
          !QCA::PKey::supportedIOTypes().contains(QCA::PKey::RSA)){
       return DecryptWizard::Page_load_error;
    }
	 else if (testRSAdec().isNull()){
        return DecryptWizard::Page_load_error;
    }
    return DecryptWizard::Page_kedua_dec;
}
HalamanPertamaDec::~HalamanPertamaDec(){

}
HalamanKetigaDec::HalamanKetigaDec(QWidget *parent):QWizardPage(parent),dec_fail(false){
    setTitle(tr("Proses enkripsi pesan RSA 2048 bit"));
    setSubTitle(tr("Mohon isikan pesan terenkripsi/ciphertext dengan algoritma RSA 2048 bit, dalam kolom pesan yang terbuka anda akan menemui pesan asli setelah dienkripsi."));
    QGridLayout * l = new QGridLayout;
    QLabel *label_msg = new QLabel;
    label_msg->setText(tr("Isikan pesan terenkripsi/ciphertext"));
    teks_pesan = new QTextEdit;
    tool_dekrip = new QToolButton;
    tool_dekrip->setEnabled(false);
    tool_dekrip->setText(tr("dekrip pesan"));
    l->addWidget(label_msg,0,0,1,1);
    l->addWidget(teks_pesan,0,1,1,1);
    l->addWidget(tool_dekrip,0,2,1,1);

    QLabel * label_msg_dec = new QLabel;
    label_msg_dec->setText(tr("Pesan yang terbuka"));
    teks_pesan_dec = new QTextEdit;
    teks_pesan_dec->setReadOnly(true);

    l->addWidget(label_msg_dec,1,0,1,1);
    l->addWidget(teks_pesan_dec,1,1,1,1);

    registerField("halamanKetigaDec.teks_pesan*",teks_pesan,"plainText",SIGNAL(textChanged()));
    registerField("halamanKetigaDec.isi_pesan_asli*",teks_pesan_dec,"plainText",SIGNAL(textChanged()));
    connect(teks_pesan,SIGNAL(textChanged()),SLOT(enableDec()));
    connect(tool_dekrip,SIGNAL(clicked()),SLOT(doDec()));
    setLayout(l);
}
HalamanKetigaDec::~HalamanKetigaDec(){
}

int HalamanKetigaDec::nextId() const
{
    if (dec_fail){
        return DecryptWizard::Page_kedua_dec_error;
    }
    return DecryptWizard::Page_dec_success;
}

void HalamanKetigaDec::enableDec()
{
    if (teks_pesan->toPlainText().isEmpty()){
        tool_dekrip->setEnabled(false);
    }else {
        tool_dekrip->setEnabled(true);
    }
}

void HalamanKetigaDec::doDec()
{

    password = field("halamanKeduaDec.line_pem_password").toByteArray();
    qDebug()<< "password pemfile: "<<password.toByteArray();
    qDebug()<<"pemfile : "<<field("halamanKeduaDec.line_pem_file").toString();
    ciphertext = QCA::hexToArray(teks_pesan->toPlainText());
    qDebug()<<"ciphertext"<< ciphertext.toByteArray();
    pv = QCA::PrivateKey::fromPEMFile(field("halamanKeduaDec.line_pem_file").toString(),password,&konversi_dekrip);
    if (!(QCA::ConvertGood ==konversi_dekrip)){
        qDebug()<<"autentikasi gagal, kenapa bsa?";
        return;//should emmit signal ?
    }

    QFuture<QString> f = QtConcurrent::run(this,&HalamanKetigaDec::dec);
    f.waitForFinished();
    if (f.result().isEmpty()){
        emit ready(1);
        dec_fail=true;
        qDebug()<<"gagal dekripsi pesan?";
        return;
    }

    teks_pesan_dec->setReadOnly(false);
    teks_pesan_dec->setText(f.result());
    teks_pesan_dec->setReadOnly(true);

}
QString HalamanKetigaDec::dec()
{
    if (!pv.canDecrypt()){
        return QString("");
    }
    bool  s_result=pv.decrypt(ciphertext,&plaintext,QCA::EME_PKCS1_OAEP);
    if(!s_result){
        return  QString("");
    }
    return QString(plaintext.toByteArray());
}


HalamanKeduaDecError::HalamanKeduaDecError(QWidget *parent):QWizardPage(parent)
{
setTitle(tr("Proses dekrip pesan gagal"));
setSubTitle(tr("Maaf, proses dekrip pesan gagal"));
QVBoxLayout *l= new QVBoxLayout;
QLabel * info = new QLabel;
info->setText(tr("Diagnosa kami, kemungkinan masalah yang terjadi:\n"
                 "1. Hal ini jarang sekali terjadi, proses dekrip pesan sedang overload cpu di memori, mohon diulangi lagi proses ini dari awal\n"
                 "2. Pesan ciphertext/yang telah dienkripsi anda masukan korup?\n"
                 "Anda bisa mengulangi proses dekrip pesan dari awal. terima kasih"));
l->addWidget(info);
setLayout(l);

}

HalamanKeduaDecError::~HalamanKeduaDecError()
{}

int HalamanKeduaDecError::nextId() const
{
return -1;
}

PageKeduaDecSignError::PageKeduaDecSignError(QWidget *parent):QWizardPage(parent)
{
    setTitle(tr("Autentikasi gagal"));
    setSubTitle(tr("Maaf proses autentikasi .pem file dengan password sebenarnya gagal"));
    QVBoxLayout *l = new QVBoxLayout;
    QLabel * label_info = new QLabel;
    label_info->setText(tr("Diagnosa kami, kemungkinan masalah yang terjadi:\n"
                           "1. File .pem private/pubkey korup\n"
                           "2. Anda salah menginputkan password sesuai dengan passwords .pem tersebut, ketika dibuat pertama kali\n\n"
                           "Anda bisa menutup panduan ini, dengan klik 'Finish', lalu ulangi proses enkripsi dari awal panduan atau tekan ctrl+n "));
    l->addWidget(label_info);

    setLayout(l);
}

int PageKeduaDecSignError::nextId() const
{
return -1;
}

PageKeduaDecSignError::~PageKeduaDecSignError()
{}


HalamanDecSuccess::HalamanDecSuccess(QWidget *parent):QWizardPage(parent)
{
setSubTitle(tr("Selamat anda telah melakukan proses dekripsii pesan"));
setTitle(tr("Proses dekrip pesan berhasil"));
QVBoxLayout *l =new QVBoxLayout;
QLabel * info = new QLabel;
info->setText(tr("Anda telah menyelesaikan proses dekrip pesan RSA "));
l->addWidget(info);
 setPixmap(QWizard::WatermarkPixmap,QPixmap(":/Picture1.png"));

setLayout(l);
}

HalamanDecSuccess::~HalamanDecSuccess(){}

int HalamanDecSuccess::nextId() const
{
    return -1;
}

void HalamanDecSuccess::initializePage()
{
    if (wizard()->hasVisitedPage(DecryptWizard::Page_ketiga)){
		
        emit dta(field("halamanKetigaDec.isi_pesan_asli").toString());
        return;
    }
}


PageLoadError::PageLoadError(QWidget *parent):QWizardPage(parent)
{
setTitle(tr("Maaf gagal memuat openssl plugin"));
setSubTitle(tr("Kami mendeteksi kerusakan dalam instalasi aplikasi ini"));
QVBoxLayout * l = new QVBoxLayout;
QLabel *info = new QLabel;
info->setText(tr("Maaf, kami mendeteksi kerusakan instalasi pada aplikasi ini, anda bisa melakukan re-instalasi dari awal untuk bisa bekerja dengan baik."));
l->addWidget(info);

setLayout(l);
}

int PageLoadError::nextId() const
{
    return -1;
}

void PageLoadError::initializePage()
{
    emit ready(2);
}
