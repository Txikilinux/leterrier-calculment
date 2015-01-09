/** Lanceur sélectif pour les exercices du logiciel Calcul mental
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/LeTerrierExercice
  * @author 2011-2012 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2013 Icham Sirat <icham.sirat@ryxeo.com>
  * @see The GNU Public License (GPL)
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 2 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful, but
  * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  * for more details.
  *
  * You should have received a copy of the GNU General Public License along
  * with this program; if not, write to the Free Software Foundation, Inc.,
  * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
  */

#include "abuledulanceurv1.h"
#include "ui_abuledulanceurv1.h"

AbuleduLanceurV1::AbuleduLanceurV1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbuleduLanceurV1)
{
    ui->setupUi(this);
    setWindowTitle(trUtf8("Lanceur"));
    fillCbExercice();
    associeNomIntitule(ui->cbExercice->currentText());
    ui->leNom->setText(abeApp->getAbeIdentite()->abeGetNom());
    if (ui->leNom->text().isEmpty())
    {
        ui->leNom->setText(abeApp->getAbeIdentite()->abeGetLogin());
    }
    else if(!abeApp->getAbeIdentite()->abeGetPreNom().isEmpty())
    {
        ui->leNom->setText(ui->leNom->text().prepend(abeApp->getAbeIdentite()->abeGetPreNom()+" "));
    }
    ui->leNom->setReadOnly(true);
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

void AbuleduLanceurV1::abeLanceurSetIdentite(AbulEduIdentitesV1 *identite)
{
    ui->leNom->setText(identite->abeGetNom());
    if (ui->leNom->text().isEmpty())
    {
        ui->leNom->setText(identite->abeGetLogin());
    }
    else if(!identite->abeGetPreNom().isEmpty())
    {
        ui->leNom->setText(ui->leNom->text().prepend(identite->abeGetPreNom()+" "));
    }
}

void AbuleduLanceurV1::fillCbExercice()
{
    QString locale = qApp->property("langageUtilise").toString();
    ui->cbExercice->clear();
    QString nomFichierConf = "./conf/alacarte.conf";
    if (!QFile(nomFichierConf).exists()) {
        QMessageBox *alertBox=new QMessageBox(QMessageBox::Warning,trUtf8("Problème !!"),trUtf8("Fichier de configuration non trouvé"));
        alertBox->show();
        on_btnAnnuler_clicked();
    }
    else {
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
    m_listeNiveaux = configExo.value("niveaux_"+locale,"Niveau1;Niveau2;Niveau3;Personnel").toString().split(";");

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
    ui->cbExercice->addItems(intitulesExercicesProposes);
}

void AbuleduLanceurV1::fillCbNiveau(QString)
{
    QString locale = qApp->property("langageUtilise").toString();
    ui->cbNiveau->clear();
    QString nomFichierConf = "./conf/alacarte.conf";
    if (!QFile(nomFichierConf).exists()) {
        QMessageBox *alertBox=new QMessageBox(QMessageBox::Warning,trUtf8("Problème !!"),trUtf8("Fichier de configuration non trouvé"));
        alertBox->show();
        on_btnAnnuler_clicked();
    }
    else {
        m_nomFichierConfExercices = nomFichierConf;
    }

    QSettings configExo(m_nomFichierConfExercices, QSettings::IniFormat);
     configExo.setIniCodec("UTF-8");
    QString niv = configExo.value("niveaux_"+locale,"Niveau1;Niveau2;Niveau3;Personnel").toString();
    QStringList listeNiv = niv.split(";");
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
    Q_UNUSED(jsaispasquoi)
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
    qApp->setProperty("utilisateur",ui->leNom->text());
    /*  Attention à changer ligne 203 de exercice.cpp si on change le caractère de concaténation dans la ligne au dessus */
//    qApp->setProperty("utilisateur",qApp->property("utilisateur").toString().replace(";"," "));
    if (m_nomExercice == "maisonDesNombres")
    {
        ExerciceMaisonNombres* exerciceLance = new ExerciceMaisonNombres(m_nomExercice, 0, 0,m_listeNiveaux[ui->cbNiveau->currentIndex()]);
    }
    else
    {
//        ExerciceOperation* exerciceLance = new ExerciceOperation(m_nomExercice,0,ui->cbNombre->currentText().toInt(),m_listeNiveaux[ui->cbNiveau->currentIndex()]);
    }
    this->close();
}

void AbuleduLanceurV1::associeNomIntitule(QString intitule)
{
//    qDebug()<<"AbuleduLanceurV1::associeNomIntitule(1) : "<<intitule;
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
        if (QString::fromUtf8((configExo.value("intitule_"+locale).toString()).toStdString().c_str())==intitule) {
            m_nomExercice = configExo.value("nom").toString();
            m_intituleExercice = QString::fromUtf8((configExo.value("intitule_"+locale).toString()).toStdString().c_str());
            trouve = true;
        }
        configExo.endGroup();
        i++;
    }
    emit cbExerciceFini(m_nomExercice);
}

void AbuleduLanceurV1::adapte()
{
    float factX= static_cast<float> (QApplication::desktop()->screenGeometry().width())/1680;
    float factY= static_cast<float> (QApplication::desktop()->screenGeometry().height())/1050;
    QPixmap imageIllustration(":/calculment/backgrounds/backgroundLauncher");
    int larg = qRound((imageIllustration.width())*factX);
    QPixmap imageIllustration2 = imageIllustration.scaledToWidth(larg, Qt::SmoothTransformation);
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
