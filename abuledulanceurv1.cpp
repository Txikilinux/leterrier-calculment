#include "abuledulanceurv1.h"
#include "ui_abuledulanceurv1.h"

AbuleduLanceurV1::AbuleduLanceurV1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbuleduLanceurV1)
{
    qDebug()<<"AbuleduLanceurV1::constructeur (1)";
    ui->setupUi(this);
    setWindowTitle(trUtf8("Lanceur"));
    fillCbExercice();
    associeNomIntitule(ui->cbExercice->currentText());
    fillCbNiveau("sertARien");
    fillCbNombre("sertARien");
    connect(ui->cbExercice, SIGNAL(currentIndexChanged(QString)), this, SLOT(associeNomIntitule(QString)));
    connect(this,SIGNAL(cbExerciceFini(QString)),this, SLOT(fillCbNombre(QString)));
    connect(this,SIGNAL(cbExerciceFini(QString)),this, SLOT(fillCbNiveau(QString)));
    connect(ui->cbExercice, SIGNAL(currentIndexChanged(QString)), this, SLOT(associeNomIntitule(QString)));
    adapte();
}

AbuleduLanceurV1::~AbuleduLanceurV1()
{
    delete ui;
}

void AbuleduLanceurV1::fillCbExercice()
{
    QString locale = qApp->property("langageUtilise").toString();
    qDebug()<<"AbuleduLanceurV1::fillCbExercice(1)";
    ui->cbExercice->clear();
    QString nomFichierConf = "./conf/alacarte.conf";
    if (!QFile(nomFichierConf).exists()) {
        QMessageBox *alertBox=new QMessageBox(QMessageBox::Warning,trUtf8("Problème !!"),trUtf8("Fichier de configuration non trouvé"));
        alertBox->show();
        on_btnAnnuler_clicked();
    }
    else {
        qDebug()<<trUtf8("Conf trouvé");
        m_nomFichierConfExercices = nomFichierConf;
    }

    QSettings configExo(m_nomFichierConfExercices, QSettings::IniFormat);
    configExo.setIniCodec("UTF-8");
    if (!configExo.childKeys().contains("niveaux_"+locale)) {
        QMessageBox *alertBox=new QMessageBox(QMessageBox::Warning,trUtf8("Problème !!"),trUtf8("Le fichier de configuration ne contient pas la clef : niveaux_")+locale);
        alertBox->show();
        this->deleteLater();
        return;
    }
qDebug()<<"dans le général de config on a "<<configExo.childKeys();
    m_listeNiveaux = configExo.value("niveaux_"+locale).toString().split(";");

    configExo.beginGroup("Exercices");
    m_listeExercices = configExo.childGroups();
    QStringList intitulesExercicesProposes;
    QStringListIterator iter(m_listeExercices);
    while (iter.hasNext()){
        QString exerc = iter.next();
        configExo.beginGroup(exerc);
        //qDebug()<<"Je rentre dans "<<exerc<<" et j'ai "<<configExo.childKeys();
        if (!configExo.childKeys().contains("intitule_"+locale)) {
            QMessageBox *alertBox=new QMessageBox(QMessageBox::Warning,trUtf8("Problème !!"),trUtf8("Le fichier de configuration ne contient pas toutes les clefs : intitule_")+locale);
            alertBox->show();
            this->deleteLater();
            return;
        }
        intitulesExercicesProposes.append(QString::fromUtf8((configExo.value("intitule_"+locale).toString()).toStdString().c_str()));
        configExo.endGroup();
    }
    qDebug()<<"fillCbExercice()--m_listeExercices : "<<m_listeExercices;
    qDebug()<<"fillCbExercice()-- intitulesExercicesProposes : "<<intitulesExercicesProposes;
    ui->cbExercice->addItems(intitulesExercicesProposes);
}

void AbuleduLanceurV1::fillCbNiveau(QString)
{
    qDebug()<<"AbuleduLanceurV1::fillCbNiveau(1)";
    QString locale = qApp->property("langageUtilise").toString();
    ui->cbNiveau->clear();
    QString nomFichierConf = "./conf/alacarte.conf";
    if (!QFile(nomFichierConf).exists()) {
        QMessageBox *alertBox=new QMessageBox(QMessageBox::Warning,trUtf8("Problème !!"),trUtf8("Fichier de configuration non trouvé"));
        alertBox->show();
        on_btnAnnuler_clicked();
    }
    else {
        qDebug()<<trUtf8("Conf trouvé");
        m_nomFichierConfExercices = nomFichierConf;
    }

    QSettings configExo(m_nomFichierConfExercices, QSettings::IniFormat);
     configExo.setIniCodec("UTF-8");
    QString niv = configExo.value("niveaux_"+locale).toString();
    QStringList listeNiv = niv.split(";");
    qDebug()<<"Niveaux : "<<niv;
    qDebug()<<"Niveaux en liste : "<<listeNiv;
    qDebug()<<"Vide ? "<<niv.isEmpty();
    if (niv.isEmpty()) {
        ui->cbNiveau->hide();
        ui->lblNiveau->hide();
    }
    else {
        ui->cbNiveau->addItems(listeNiv);
        ui->cbNiveau->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        ui->cbNiveau->show();
        ui->lblNiveau->show();
    }
}

void AbuleduLanceurV1::fillCbNombre(QString jsaispasquoi)
{
    qDebug()<<"AbuleduLanceurV1::fillCbNombre(1)"<<m_nomExercice ;
    ui->cbNombre->hide();
    ui->lblNombre->hide();
    ui->cbNombre->clear();
    bool trouve = false;
    QSettings configExo(m_nomFichierConfExercices, QSettings::IniFormat);
     configExo.setIniCodec("UTF-8");
    configExo.beginGroup("Exercices");
    QStringListIterator iter(m_listeExercices);
    while (iter.hasNext() && trouve==false){
        QString exerc = iter.next();
        configExo.beginGroup(exerc);
        ui->cbNombre->clear();
        if (configExo.value("nom").toString() == m_nomExercice) {
            trouve=true;
            QString val = configExo.value("liste").toString();
            QStringList listeVal = val.split(";");
            qDebug()<<"Valeurs : "<<val;
            qDebug()<<"Valeurs en liste : "<<listeVal;
            qDebug()<<"Vide ? "<<val.isEmpty();
            if (val.isEmpty())
            {
                ui->cbNombre->hide();
                ui->lblNombre->hide();
            }
            else {
                ui->cbNombre->addItems(listeVal);
                ui->cbNombre->setSizeAdjustPolicy(QComboBox::AdjustToContents);
                ui->cbNombre->show();
                ui->lblNombre->show();
                adapte();
            }
            return;
        }
        configExo.endGroup();
    }
}

void AbuleduLanceurV1::on_btnAnnuler_clicked()
{
    this->close();
}

void AbuleduLanceurV1::on_btnLancer_clicked()
{
    if (!ui->cbNombre->currentText().isNull())
        m_nomExercice.append(ui->cbNombre->currentText());
    qDebug()<<"AbuleduLanceurV1::on_btnLancer_clicked() pour "<<m_nomExercice;
    qApp->setProperty("utilisateur",ui->lePrenom->text() +";"+ui->leNom->text());
    if (m_nomExercice == "maisonDesNombres")
    {
        ExerciceMaisonNombres* exerciceLance = new ExerciceMaisonNombres(m_nomExercice, 0, 0,m_listeNiveaux[ui->cbNiveau->currentIndex()]);
        qDebug()<<"---> Exercice appelé avec "<<m_nomExercice<<" et "<<m_listeNiveaux[ui->cbNiveau->currentIndex()];
        exerciceLance->show();
    }
    else
    {
    exercice* exerciceLance = new exercice(m_nomExercice,0,ui->cbNombre->currentText().toInt(),m_listeNiveaux[ui->cbNiveau->currentIndex()]);
    qDebug()<<"---> Exercice appelé avec "<<m_nomExercice<<", "<<ui->cbNombre->currentText().toInt()<<", "<<m_listeNiveaux[ui->cbNiveau->currentIndex()];
    exerciceLance->show();
    }
    this->close();
}

void AbuleduLanceurV1::associeNomIntitule(QString intitule)
{
    qDebug()<<"AbuleduLanceurV1::associeNomIntitule(1) : "<<intitule;
    QString locale = qApp->property("langageUtilise").toString();
    QSettings configExo(m_nomFichierConfExercices, QSettings::IniFormat);
    configExo.setIniCodec("UTF-8");
    configExo.beginGroup("Exercices");
    int i=1;
    bool trouve = false;
    QStringListIterator iterateur(m_listeExercices);
    while (iterateur.hasNext() && !trouve) {
        QString exercice = iterateur.next();
        configExo.beginGroup(exercice);
        qDebug()<<"exercice"<<exercice;
        qDebug()<<"nom : "<<configExo.value("nom").toString();
        qDebug()<<"intitule : "<<QString::fromUtf8((configExo.value("intitule_"+locale).toString()).toStdString().c_str());
        if (QString::fromUtf8((configExo.value("intitule_"+locale).toString()).toStdString().c_str())==intitule) {
            m_nomExercice = configExo.value("nom").toString();
            m_intituleExercice = QString::fromUtf8((configExo.value("intitule_"+locale).toString()).toStdString().c_str());
            qDebug()<<"Nom : "<<m_nomExercice;
            qDebug()<<"Intitule"<<m_intituleExercice;
            trouve = true;
            qDebug()<<"............... variable trouve = true..................";
            //return;
        }
        configExo.endGroup();
        i++;
    }
    qDebug()<<"AbuleduLanceurV1::associeNomIntitule(2)";
    qDebug()<<"Nom : "<<m_nomExercice;
    qDebug()<<"Intitule"<<m_intituleExercice;
    emit cbExerciceFini(m_nomExercice);
    qDebug()<<"Signal emis avec "<<m_nomExercice;
}

void AbuleduLanceurV1::adapte()
{
    float factX= static_cast<float> (QApplication::desktop()->screenGeometry().width())/1680;
    float factY= static_cast<float> (QApplication::desktop()->screenGeometry().height())/1050;
//    QFile* fichierImage = new QFile("./data/images/avion.jpg");
//    if (fichierImage->exists()) {
//        qDebug()<<"Image avion OK" << factX << ": " << factY << " :: " << QApplication::desktop()->screenGeometry().width();
//    }
//    else qDebug()<<"Image avion prout !!";
    QPixmap imageIllustration("./data/images/avion.jpg");
    int larg = qRound((imageIllustration.width())*factX);
    QPixmap imageIllustration2 = imageIllustration.scaledToWidth(larg, Qt::SmoothTransformation);
//    qDebug() << "Taille 2 :: " << larg << " -> " << imageIllustration.size() << " --> " << imageIllustration2.size();
    QBrush* fond = new QBrush(imageIllustration2);
    QPalette palette;
    palette.setBrush(this->backgroundRole(),*fond);
    setPalette(palette);
    setGeometry(750*factX,150*factY,imageIllustration2.width(),imageIllustration2.height());
    setFixedSize(imageIllustration2.width(),imageIllustration2.height());
    setWindowModality(Qt::ApplicationModal);
}

void AbuleduLanceurV1::on_chbBilan_clicked()
{
    if (ui->chbBilan->isChecked())
    {
        qApp->setProperty("afficheBilanExercice",true);
    }
    else
    {
        qApp->setProperty("afficheBilanExercice",false);
    }
}
