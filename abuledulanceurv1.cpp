#include "abuledulanceurv1.h"
#include "ui_abuledulanceurv1.h"

AbuleduLanceurV1::AbuleduLanceurV1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbuleduLanceurV1)
{
    ui->setupUi(this);
    fillCbExercice();
    associeNomIntitule(ui->cbExercice->currentText());
    fillCbNiveau("sertARien");
    fillCbNombre("sertARien");
    connect(ui->cbExercice, SIGNAL(currentIndexChanged(QString)), this, SLOT(associeNomIntitule(QString)));
    connect(this,SIGNAL(cbExerciceFini(QString)),this, SLOT(fillCbNombre(QString)));
    connect(this,SIGNAL(cbExerciceFini(QString)),this, SLOT(fillCbNiveau(QString)));

}

AbuleduLanceurV1::~AbuleduLanceurV1()
{
    delete ui;
}

void AbuleduLanceurV1::fillCbExercice()
{
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
    configExo.beginGroup("Exercices");
    m_listeExercices = configExo.childGroups();
    QStringList intitulesExercicesProposes;
    QStringListIterator iter(m_listeExercices);
    while (iter.hasNext()){
        QString exerc = iter.next();
        configExo.beginGroup(exerc);
        intitulesExercicesProposes.append(QString::fromUtf8((configExo.value("intitule").toString()).toStdString().c_str()));
        configExo.endGroup();
    }
    qDebug()<<"fillCbExercice()--m_listeExercices : "<<m_listeExercices;
    qDebug()<<"fillCbExercice()-- intitulesExercicesProposes : "<<intitulesExercicesProposes;
    ui->cbExercice->addItems(intitulesExercicesProposes);
}

void AbuleduLanceurV1::fillCbNiveau(QString)
{
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
    QString niv = configExo.value("niveaux").toString();
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
    ui->cbNombre->hide();
    ui->cbNombre->clear();
    bool trouve = false;
    QSettings configExo(m_nomFichierConfExercices, QSettings::IniFormat);
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
                ui->cbNombre->hide();
            else {
                ui->cbNombre->addItems(listeVal);
                ui->cbNombre->setSizeAdjustPolicy(QComboBox::AdjustToContents);
                ui->cbNombre->show();
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
    exercice* exerciceLance = new exercice(m_nomExercice,0,ui->cbNombre->currentText().toInt(),ui->cbNiveau->currentText());
    qDebug()<<"Exercice appelé avec "<<m_nomExercice<<", "<<ui->cbNombre->currentText().toInt()<<", "<<ui->cbNiveau->currentText();
    exerciceLance->show();
}

void AbuleduLanceurV1::associeNomIntitule(QString intitule)
{
    QSettings configExo(m_nomFichierConfExercices, QSettings::IniFormat);
    configExo.beginGroup("Exercices");
    int i=1;
    bool trouve = false;
    QStringListIterator iterateur(m_listeExercices);
    while (iterateur.hasNext() && !trouve) {
        QString exercice = iterateur.next();
        configExo.beginGroup(exercice);
        qDebug()<<"exercice"<<exercice;
        qDebug()<<"nom : "<<configExo.value("nom").toString();
        qDebug()<<"intitule : "<<QString::fromUtf8((configExo.value("intitule").toString()).toStdString().c_str());
        if (QString::fromUtf8((configExo.value("intitule").toString()).toStdString().c_str())==intitule) {
            m_nomExercice = configExo.value("nom").toString();
            m_intituleExercice = QString::fromUtf8((configExo.value("intitule").toString()).toStdString().c_str());
            qDebug()<<"Nom : "<<m_nomExercice;
            qDebug()<<"Intitule"<<m_intituleExercice;
            trouve = true;
            qDebug()<<"............... variable trouve = true..................";
            //return;
        }
        configExo.endGroup();
        i++;
    }
        qDebug()<<"Fin du slot associeNomIntitule()";
        qDebug()<<"Nom : "<<m_nomExercice;
        qDebug()<<"Intitule"<<m_intituleExercice;
        emit cbExerciceFini(m_nomExercice);
        qDebug()<<"Signal emis avec "<<m_nomExercice;
}
