/** Editeur de paramètres d'exercices pour le logiciel Calcul Mental
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/LeTerrierExercice
  * @author 2010-2014 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
  * @author 2013 Eric Seigne <eric.seigne@ryxeo.com>
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


#include "editeur.h"
#include "ui_editeur.h"
#include "version.h"
#include "interface.h"
#include <QComboBox>
#include <QCloseEvent>
#include <QFile>
#include <QDir>
#include <QMessageBox>


Editeur::Editeur(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::Editeur)
{
    m_abuleduFile.clear();
    this->setWindowModality(Qt::ApplicationModal);
    m_ui->setupUi(this);
    ABULEDU_LOG_TRACE()  << __PRETTY_FUNCTION__;


    m_ui->cbNiveau->addItem("1", 1);
    m_ui->cbNiveau->addItem("2", 2);
    m_ui->cbNiveau->addItem("3", 3);
    m_ui->cbNiveau->addItem(trUtf8("Personnel"),4);

    m_ui->cbMaxG->addItem("10",1);
    m_ui->cbMaxG->addItem("100",2);
    m_ui->cbMaxG->addItem("1000",3);

    m_ui->cbMaxD->addItem("10",1);
    m_ui->cbMaxD->addItem("100",2);
    m_ui->cbMaxD->addItem("1000",3);


    if (associeNomIntitule(m_ui->cbOperation->currentText()).left(10)!="OdGrandeur") {
        m_ui->cbMaxD->hide();
        m_ui->lblDMax_2->hide();
        m_ui->cbMaxG->hide();
        m_ui->lblGMax_2->hide();
    }
    else {
        m_ui->spbDMax->hide();
        m_ui->lblDMax->hide();
        m_ui->spbGMax->hide();
        m_ui->lblGMax->hide();
        m_ui->spbGMin->setEnabled(false);
        m_ui->spbDMin->setEnabled(false);
    }

    connect(m_ui->sldVitesse, SIGNAL(valueChanged(int)), m_ui->pbVitesse, SLOT(setValue(int)));

    //Initialisation attributs de la classe
    //        m_niveauEnCours = new QString(m_ui->cbNiveau->currentText());
    m_niveauEnCours = m_ui->cbNiveau->itemData(m_ui->cbNiveau->currentIndex()).toInt();
    m_operationEnCours = new QString(associeNomIntitule(m_ui->cbOperation->currentText()));
    m_minG=m_maxG=m_minD=m_maxD=0;


    this->chargerNiveau(QString::number(m_niveauEnCours));

    connect(m_ui->cbNiveau, SIGNAL(currentIndexChanged(QString)), this, SLOT(changerNiveau(QString)));
    connect(m_ui->cbOperation, SIGNAL(currentIndexChanged(QString)), this, SLOT(changerOperation(QString)));

    installEventFilters();
    m_ui->lblHelp->setFont(QApplication::font());
}

Editeur::~Editeur()
{
    delete m_ui;
}

int Editeur::getNiveauEnCours()
{
    return m_niveauEnCours;
}

void Editeur::initialiserOperation(QString operation)
{
    QSettings config(m_settingsTempPath, QSettings::IniFormat);

    config.beginGroup(operation);
    config.beginGroup("1");
    config.setValue("MinGauche", 0);
    config.setValue("MaxGauche", 9);
    config.setValue("MinDroite", 0);
    config.setValue("MaxDroite", 9);
    config.setValue("TempsAccorde",8);
    config.endGroup();
    config.beginGroup("2");
    config.setValue("MinGauche", 0);
    config.setValue("MaxGauche", 9);
    config.setValue("MinDroite", 0);
    config.setValue("MaxDroite", 99);
    config.setValue("TempsAccorde",8);
    config.endGroup();
    config.beginGroup("3");
    config.setValue("MinGauche", 0);
    config.setValue("MaxGauche", 99);
    config.setValue("MinDroite", 0);
    config.setValue("MaxDroite", 99);
    config.setValue("TempsAccorde",8);
    config.endGroup();
    config.beginGroup("4");
    config.setValue("MinGauche", 0);
    config.setValue("MaxGauche", 5);
    config.setValue("MinDroite", 0);
    config.setValue("MaxDroite", 5);
    config.setValue("TempsAccorde",8);
    config.endGroup();
    config.setValue("NiveauEnCours"+operation, "1");
    if (operation == "addition")
        config.setValue("NomPourAffichage", trUtf8("Addition"));
    else if (operation == "soustraction")
        config.setValue("NomPourAffichage", trUtf8("Soustraction"));
    else if (operation == "multiplication")
        config.setValue("NomPourAffichage", trUtf8("Multiplication"));
    config.endGroup();
}

void Editeur::initialiserApproche(QString operation)
{
    QSettings config(m_settingsTempPath, QSettings::IniFormat);

    config.beginGroup(operation);
    config.beginGroup("1");
    config.setValue("MaxGauche", 100);
    config.setValue("MaxDroite", 100);
    config.setValue("TempsAccorde",8);
    config.endGroup();
    config.beginGroup("2");
    config.setValue("MaxGauche", 1000);
    config.setValue("MaxDroite", 100);
    config.setValue("TempsAccorde",8);
    config.endGroup();
    config.beginGroup("3");
    config.setValue("MaxGauche", 1000);
    config.setValue("MaxDroite", 1000);
    config.setValue("TempsAccorde",8);
    config.endGroup();
    config.beginGroup("4");
    config.setValue("MaxGauche", 1000);
    config.setValue("MaxDroite", 1000);
    config.setValue("TempsAccorde",8);
    config.endGroup();
    config.setValue("NiveauEnCours"+operation, "1");
    if (operation == "OdGrandeurAddition")
        config.setValue("NomPourAffichage", trUtf8("OdG Additions"));
    else if (operation == "OdGrandeurSoustraction")
        config.setValue("NomPourAffichage", trUtf8("OdG Soustractions"));
    config.endGroup();
}

void Editeur::initialiserApprocheM(QString operation)
{
    QSettings config(m_settingsTempPath, QSettings::IniFormat);

    config.beginGroup(operation);
    config.beginGroup("1");
    config.setValue("MaxGauche", 100);
    config.setValue("MaxDroite", 10);
    config.setValue("TempsAccorde",8);
    config.endGroup();
    config.beginGroup("2");
    config.setValue("MaxGauche", 1000);
    config.setValue("MaxDroite", 10);
    config.setValue("TempsAccorde",8);
    config.endGroup();
    config.beginGroup("3");
    config.setValue("MaxGauche", 100);
    config.setValue("MaxDroite", 100);
    config.setValue("TempsAccorde",8);
    config.endGroup();
    config.beginGroup("4");
    config.setValue("MaxGauche", 1000);
    config.setValue("MaxDroite", 1000);
    config.setValue("TempsAccorde",8);
    config.endGroup();
    config.setValue("NiveauEnCours"+operation, "1");
    config.setValue("NomPourAffichage", trUtf8("OdG Multiplications"));
    config.endGroup();
}

void Editeur::initialiserApprocheD(QString operation)
{
    QSettings config(m_settingsTempPath, QSettings::IniFormat);

    config.beginGroup(operation);
    config.beginGroup("1");
    config.setValue("MaxGauche", 10000);
    config.setValue("MinGauche", 1000);
    config.setValue("MaxDroite", "10;100;100");
    config.setValue("TempsAccorde",8);
    config.endGroup();
    config.beginGroup("2");
    config.setValue("MaxGauche", 10000);
    config.setValue("MinGauche", 1000);
    config.setValue("MaxDroite", "20;200;50;500");
    config.setValue("TempsAccorde",8);
    config.endGroup();
    config.beginGroup("3");
    config.setValue("MaxGauche", 10000);
    config.setValue("MinGauche", 1000);
    config.setValue("MaxDroite", "20;30;40;50");
    config.setValue("TempsAccorde",8);
    config.endGroup();
    config.beginGroup("4");
    config.setValue("MaxGauche", 10000);
    config.setValue("MinGauche", 1000);
    config.setValue("MaxDroite", "20;200;30;300;40;400;50;500");
    config.setValue("TempsAccorde",8);
    config.endGroup();
    config.setValue("NiveauEnCours"+operation, "1");
    config.setValue("NomPourAffichage", trUtf8("OdG Divisions"));
    config.endGroup();
}

void Editeur::initialiserComplement(QString operation)
{
    QSettings config(m_settingsTempPath, QSettings::IniFormat);

    config.beginGroup(operation);
    config.beginGroup("1");
    config.setValue("TempsAccorde",8);
    config.endGroup();
    config.beginGroup("2");
    config.setValue("TempsAccorde",6);
    config.endGroup();
    config.beginGroup("3");
    config.setValue("TempsAccorde",4);
    config.endGroup();
    config.beginGroup("4");
    config.setValue("TempsAccorde",6);
    config.endGroup();
    config.setValue("NiveauEnCours"+operation, "1");
    if (operation[0] == 'c')
    {
        if (operation[10] == 'A')
        {
            if (operation.length() == 13) {
                config.setValue("NomPourAffichage", trUtf8("Compléments à 10"));
                ABULEDU_LOG_DEBUG()<<"-------------- ecriture : Compléments à 10";
            }
            else if (operation.length() == 14)
                config.setValue("NomPourAffichage", trUtf8("Compléments à 100"));
            else if (operation.length() == 15)
                config.setValue("NomPourAffichage", trUtf8("Compléments à 1000"));
        }
        else if (operation[10] == 'M')
        {
            if (operation.remove(0,11) == "5")
                config.setValue("NomPourAffichage", trUtf8("Multiples de 5"));
            else config.setValue("NomPourAffichage", trUtf8("Multiples de ")+operation.right(2));
        }
    }
    else if (operation[0] == 't')
    {
        if (operation[5] == 'A')
            config.setValue("NomPourAffichage", trUtf8("Table d'addition de ")+operation.remove(0,6));
        else if (operation[5] == 'M')
            config.setValue("NomPourAffichage", trUtf8("Table de multiplication par ")+operation.remove(0,6));

    }
    else if (operation[0] == 'm')
    {
        config.setValue("NomPourAffichage", trUtf8("Maison des nombres"));
    }
    config.endGroup();
}

void Editeur::initialiserDivision()
{
    QSettings config(m_settingsTempPath, QSettings::IniFormat);

    config.beginGroup("division");
    config.beginGroup("1");
    config.setValue("MinGauche", 2);
    config.setValue("MaxGauche", 100);
    config.setValue("MinDroite", 2);
    config.setValue("MaxDroite", 2);
    config.setValue("TempsAccorde",8);
    config.endGroup();
    config.beginGroup("2");
    config.setValue("MinGauche", 5);
    config.setValue("MaxGauche", 100);
    config.setValue("MinDroite", 5);
    config.setValue("MaxDroite", 5);
    config.setValue("TempsAccorde",8);
    config.endGroup();
    config.beginGroup("3");
    config.setValue("MinGauche", 2);
    config.setValue("MaxGauche", 100);
    config.setValue("MinDroite", 2);
    config.setValue("MaxDroite", 5);
    config.setValue("TempsAccorde",8);
    config.endGroup();
    config.beginGroup("4");
    config.setValue("MinGauche", 2);
    config.setValue("MaxGauche", 200);
    config.setValue("MinDroite", 2);
    config.setValue("MaxDroite", 5);
    config.setValue("TempsAccorde",8);
    config.endGroup();
    config.setValue("NiveauEnCoursdivision", "1");
    config.setValue("NomPourAffichage", trUtf8("Division"));
    config.endGroup();
}

void Editeur::initComboBoxOperations()
{
    ABULEDU_LOG_TRACE()  << __PRETTY_FUNCTION__;
    QStringList intitulesExercices;
    QSettings* config = new QSettings(m_settingsTempPath, QSettings::IniFormat,this);
    QString operation = QString();
    foreach (QString section,config->childGroups()) {
        config->beginGroup(section);
        intitulesExercices.append(config->value("NomPourAffichage").toString());
        if(operation.isNull()){
            operation = section;
        }
        config->endGroup();
    }
    m_operationEnCours = new QString(operation);
    chargerOperation(*m_operationEnCours);
    m_ui->cbOperation->blockSignals(true);
    m_ui->cbOperation->addItems(intitulesExercices);
    m_ui->cbOperation->blockSignals(false);
}

void Editeur::initialiser()
{
    ABULEDU_LOG_TRACE()  << __PRETTY_FUNCTION__;
    QSettings config(m_settingsTempPath, QSettings::IniFormat);

    config.setValue("NombreBallons", 10);
    config.setValue("version",VER_UNITVERSION_STR);
    initialiserOperation("addition");
    initialiserOperation("multiplication");
    initialiserOperation("soustraction");
    initialiserDivision();
    initialiserApproche("OdGrandeurAddition");
    initialiserApproche("OdGrandeurSoustraction");
    initialiserApprocheM("OdGrandeurMultiplication");
    initialiserApprocheD("OdGrandeurDivision");
    initialiserComplement("complementA10");
    initialiserComplement("complementA100");
    initialiserComplement("complementA1000");
    for (int i=1;i<=5;i++) {
        initialiserComplement("complementM"+QString::number(i*5));
    }
    initialiserComplement("complementM50");
    for (int i=2;i<=9;i++) initialiserComplement("tableM"+QString::number(i));
    for (int i=2;i<=9;i++) initialiserComplement("tableA"+QString::number(i));
    initialiserComplement("maisonDesNombres");

}

QSharedPointer<AbulEduFileV1> Editeur::abeEditeurGetAbulEduFile()
{
    return m_abuleduFile;
}

void Editeur::sauverNiveau(QString niveau)
{
    ABULEDU_LOG_TRACE()  << __PRETTY_FUNCTION__<<niveau<<" dans "<<*m_operationEnCours;

    QSettings config(m_settingsTempPath, QSettings::IniFormat);

    config.setValue("NombreBallons", m_ui->spbNombreBallons->value());
    config.beginGroup(*m_operationEnCours);
    config.beginGroup(niveau);
    if ((m_operationEnCours->left(1) != "t") && (m_operationEnCours->left(1) != "c") && (m_operationEnCours->left(2) != "ma"))
    {
        if (m_operationEnCours->left(10)!="OdGrandeur") {
            config.setValue("MinGauche", m_ui->spbGMin->value());
            config.setValue("MaxGauche", m_ui->spbGMax->value());
            config.setValue("MinDroite", m_ui->spbDMin->value());
            config.setValue("MaxDroite", m_ui->spbDMax->value());
        }
        else {
            config.setValue("MaxGauche",m_ui->cbMaxG->currentText().toInt());
            config.setValue("MaxDroite",m_ui->cbMaxD->currentText().toInt());
        }
    }
    config.setValue("TempsAccorde",m_ui->sldVitesse->value());
    config.endGroup();
    config.endGroup();
    m_niveauEnCours = niveau.toInt();
    disconnect(m_ui->spbGMin, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
    disconnect(m_ui->spbGMax, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
    disconnect(m_ui->spbDMin, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
    disconnect(m_ui->spbDMax, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
}

void Editeur::chargerNiveau(QString niveau)
{
    ABULEDU_LOG_TRACE()  << __PRETTY_FUNCTION__<<niveau<<" dans "<<*m_operationEnCours;

    m_ui->spbDMin->setMaximum(1000);
    m_ui->spbGMin->setMaximum(1000);
    m_ui->spbDMax->setMinimum(0);
    m_ui->spbGMax->setMinimum(0);

    QSettings config(m_settingsTempPath, QSettings::IniFormat);

    m_ui->spbNombreBallons->setValue(config.value("NombreBallons",10).toInt());
    config.beginGroup(*m_operationEnCours);
    config.beginGroup(niveau);
    m_ui->spbGMin->setValue(config.value("MinGauche",0).toInt());
    m_maxG = config.value("MaxGauche",100).toInt();
    m_ui->spbGMax->setValue(m_maxG);
    m_ui->spbDMin->setValue(config.value("MinDroite",0).toInt());
    m_maxD = config.value("MaxDroite",100).toInt();
    m_ui->spbDMax->setValue(m_maxD);
    m_ui->sldVitesse->setValue(config.value("TempsAccorde",10).toInt());
    config.endGroup();
    config.endGroup();

    if (m_operationEnCours->left(10)=="OdGrandeur") {
        switch (m_maxG) {
        case 10 : m_ui->cbMaxG->setCurrentIndex(0);break;
        case 100 : m_ui->cbMaxG->setCurrentIndex(1);break;
        case 1000 : m_ui->cbMaxG->setCurrentIndex(2);break;
        default : m_ui->cbMaxG->setCurrentIndex(2);break;
        }

        switch (m_maxD) {
        case 10 : m_ui->cbMaxD->setCurrentIndex(0);break;
        case 100 : m_ui->cbMaxD->setCurrentIndex(1);break;
        case 1000 : m_ui->cbMaxD->setCurrentIndex(2);break;
        default : m_ui->cbMaxD->setCurrentIndex(2);break;
        }
    }
    connect(m_ui->spbGMin, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
    connect(m_ui->spbGMax, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
    connect(m_ui->spbDMin, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
    connect(m_ui->spbDMax, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
}

void Editeur::changerNiveau(QString chaine)
{
    this->sauverNiveau(QString::number(m_niveauEnCours));
    this->chargerNiveau(chaine);
    m_niveauEnCours = chaine.toInt();
}

void Editeur::sauverOperation(QString operation)
{
    ABULEDU_LOG_TRACE()  << __PRETTY_FUNCTION__<<operation<<" dans "<<m_niveauEnCours;

    if(!operation.isEmpty()){

        QSettings config(m_settingsTempPath, QSettings::IniFormat);

        config.setValue("NombreBallons", m_ui->spbNombreBallons->value());
        config.beginGroup(operation);
        config.beginGroup(QString::number(m_niveauEnCours));
        if ((m_operationEnCours->left(1) != "t") && (m_operationEnCours->left(1) != "c") && (m_operationEnCours->left(2) != "ma"))
        {
            if (m_operationEnCours->left(10)!="OdGrandeur") {
                config.setValue("MinGauche", m_ui->spbGMin->value());
                config.setValue("MaxGauche", m_ui->spbGMax->value());
                config.setValue("MinDroite", m_ui->spbDMin->value());
                config.setValue("MaxDroite", m_ui->spbDMax->value());
            }
            else {
                config.setValue("MaxGauche",m_ui->cbMaxG->currentText().toInt());
                config.setValue("MaxDroite",m_ui->cbMaxD->currentText().toInt());
            }
        }
        config.setValue("TempsAccorde",m_ui->sldVitesse->value());
        config.endGroup();
        config.endGroup();
        /*20150422 Philippe : inutile */
//        *m_operationEnCours = operation;

        disconnect(m_ui->spbGMin, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
        disconnect(m_ui->spbGMax, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
        disconnect(m_ui->spbDMin, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
        disconnect(m_ui->spbDMax, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
    }
}

void Editeur::chargerOperation(QString operation)
{
    ABULEDU_LOG_TRACE()  << __PRETTY_FUNCTION__<<operation<<" dans "<<m_niveauEnCours;

    if (operation.left(10)=="OdGrandeur") {
        m_ui->spbGMin->setEnabled(false);
        m_ui->spbDMin->setEnabled(false);
        m_ui->spbGMax->hide();
        m_ui->spbDMax->hide();
        m_ui->lblGMax->hide();
        m_ui->lblDMax->hide();
        m_ui->cbMaxG->show();
        m_ui->cbMaxG->setEnabled(true);
        m_ui->cbMaxD->show();
        m_ui->cbMaxD->setEnabled(true);
        m_ui->lblGMax_2->show();
        m_ui->lblDMax_2->show();
    }
    else if (operation.right(4)=="tion" || operation == "division"){
        m_ui->spbGMin->show();
        m_ui->spbDMin->show();
        m_ui->spbGMin->setEnabled(true);
        m_ui->spbDMin->setEnabled(true);
        m_ui->spbGMax->setMaximum(99);
        m_ui->spbDMax->setMaximum(99);
        m_ui->spbGMax->show();
        m_ui->spbDMax->show();
        m_ui->spbGMax->setEnabled(true);
        m_ui->spbDMax->setEnabled(true);
        m_ui->lblGMax->show();
        m_ui->lblDMax->show();
        m_ui->cbMaxG->hide();
        m_ui->cbMaxD->hide();
        m_ui->lblGMax_2->hide();
        m_ui->lblDMax_2->hide();
    }
    else {
        m_ui->spbGMax->setEnabled(false);
        m_ui->spbDMax->setEnabled(false);
        m_ui->spbGMin->setEnabled(false);
        m_ui->spbDMin->setEnabled(false);
        m_ui->cbMaxG->setEnabled(false);
        m_ui->cbMaxD->setEnabled(false);
    }

    QSettings config(m_settingsTempPath, QSettings::IniFormat);

    m_ui->spbNombreBallons->setValue(config.value("NombreBallons",10).toInt());
    config.beginGroup(operation);
    config.beginGroup(QString::number(m_niveauEnCours));
    m_maxG = config.value("MaxGauche",100).toInt();
    m_ui->spbGMax->setValue(m_maxG);
    m_ui->spbGMin->setValue(config.value("MinGauche",0).toInt());
    m_maxD = config.value("MaxDroite",100).toInt();
    m_ui->spbDMax->setValue(m_maxD);
    m_ui->spbDMin->setValue(config.value("MinDroite",0).toInt());
    m_ui->sldVitesse->setValue(config.value("TempsAccorde",10).toInt());
    config.endGroup();
    config.endGroup();

    if (operation.left(10)=="OdGrandeur") {
        switch (m_maxG) {
        case 10 : m_ui->cbMaxG->setCurrentIndex(0);break;
        case 100 : m_ui->cbMaxG->setCurrentIndex(1);break;
        case 1000 : m_ui->cbMaxG->setCurrentIndex(2);break;
        default : m_ui->cbMaxG->setCurrentIndex(2);break;
        }

        switch (m_maxD) {
        case 10 : m_ui->cbMaxD->setCurrentIndex(0);break;
        case 100 : m_ui->cbMaxD->setCurrentIndex(1);break;
        case 1000 : m_ui->cbMaxD->setCurrentIndex(2);break;
        default : m_ui->cbMaxD->setCurrentIndex(2);break;
        }
    }

    connect(m_ui->spbGMin, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
    connect(m_ui->spbGMax, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
    connect(m_ui->spbDMin, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
    connect(m_ui->spbDMax, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));

}

void Editeur::changerOperation(QString operation)
{
    ABULEDU_LOG_TRACE()  << __PRETTY_FUNCTION__<<operation<<" dans "<<m_niveauEnCours;

    this->sauverOperation(*m_operationEnCours);
    this->chargerOperation(associeNomIntitule(operation));
    *m_operationEnCours = associeNomIntitule(operation);
}

void Editeur::closeEvent(QCloseEvent *event)
{
    this->sauverNiveau(QString::number(m_niveauEnCours));
    this->sauverOperation(*m_operationEnCours);
    event->accept();
    //delete(this);

}

bool Editeur::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::Enter){
        if(obj == m_ui->cbNiveau){
            m_ui->lblHelp->setText(trUtf8("Choisissez dans la liste déroulante le niveau de difficulté à paramétrer."));
        }
        if(obj == m_ui->cbOperation){
            m_ui->lblHelp->setText(trUtf8("Choisissez dans la liste déroulante le type d'exercice."));
        }
        if(obj == m_ui->cbMaxD){
            m_ui->lblHelp->setText(trUtf8("Fixez une valeur maximum pour l'opérande droit."));
        }
        if(obj == m_ui->cbMaxG){
            m_ui->lblHelp->setText(trUtf8("Fixez une valeur maximum pour l'opérande gauche."));
        }
        if(obj == m_ui->spbDMax){
            m_ui->lblHelp->setText(trUtf8("Fixez une valeur maximum pour l'opérande droit."));
        }
        if(obj == m_ui->spbDMin){
            m_ui->lblHelp->setText(trUtf8("Fixez une valeur minimum pour l'opérande droit."));
        }
        if(obj == m_ui->spbGMax){
            m_ui->lblHelp->setText(trUtf8("Fixez une valeur maximum pour l'opérande gauche."));
        }
        if(obj == m_ui->spbGMin){
            m_ui->lblHelp->setText(trUtf8("Fixez une valeur minimum pour l'opérande gauche."));
        }
        if(obj == m_ui->spbNombreBallons){
            m_ui->lblHelp->setText(trUtf8("Indiquez le nombre de questions par exercice."));
        }
        if(obj == m_ui->sldVitesse){
            m_ui->lblHelp->setText(trUtf8("Déterminez le temps imparti pour répondre."));
        }
        if(obj == m_ui->btnEditeurOK){
            m_ui->lblHelp->setText(trUtf8("Enregistrez vos modifications pour retourner à la page d'accueil."));
        }
        if(obj == m_ui->btnEditeurAnnuler){
            m_ui->lblHelp->setText(trUtf8("Abandonnez vos modifications pour retourner à la page d'accueil."));
        }
        return true;
    }
    else if(event->type() == QEvent::Leave){
        QString basicHelp = trUtf8("Survolez les zones actives pour être guidé");
        m_ui->lblHelp->setText(basicHelp);
    }
    return QObject::eventFilter(obj,event);
}

void Editeur::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Editeur::ajusterValeurs(int valeurNouvelle)
{
    Q_UNUSED(valeurNouvelle)
    ABULEDU_LOG_TRACE()  << __PRETTY_FUNCTION__;

    m_ui->spbDMin->setMaximum(m_ui->spbDMax->value());
    m_ui->spbGMin->setMaximum(m_ui->spbGMax->value());
    m_ui->spbDMax->setMinimum(m_ui->spbDMin->value());
    m_ui->spbGMax->setMinimum(m_ui->spbGMin->value());
}

void Editeur::editeurWriteOnAbeBoxPerso()
{
    ABULEDU_LOG_TRACE()  << __PRETTY_FUNCTION__;
    /* Je mets mon fichier de conf dans la liste des trucs à sauver */
    QStringList listeFichiersMots;
    listeFichiersMots.append(m_settingsTempPath);
    ABULEDU_LOG_DEBUG()<<listeFichiersMots;

    /* Je nettoie cette liste avant de créer le abe */
    ABULEDU_LOG_DEBUG()<<m_abuleduFile->abeFileExportPrepare(listeFichiersMots,m_settingsTempPath,QString("abe"));

    /* Je crée un "calculmentSettings.abe" dans le dossier temporaire */
    ABULEDU_LOG_DEBUG()<<m_abuleduFile->abeFileSave("calculmentSettings.abe",
                                                          listeFichiersMots,
                                                          m_abuleduFile->abeFileGetDirectoryTemp().absolutePath(),
                                                          QString("abe"));

    if(abeApp->getAbeNetworkAccessManager()->abeSSOAuthenticationStatus() > 0)
    {
        ABULEDU_LOG_DEBUG()<<abeApp->getAbeIdentite()->abeGetLogin()<<m_boxFileManager->abeBoxFileManagerGetSavingLocation();
        AbulEduLoadingAnimationV1::getInstance()->start(this,trUtf8("Sauvegarde en cours dans abeBoxPerso"));

        /* J'envoie ce fichier sur l'abeBox */
        m_boxFileManager->slotUploadFile(m_abuleduFile->abeFileGetFileName().filePath(),"","calculmentSettings.abe");


        connect(m_boxFileManager, SIGNAL(signalAbeFileSaved(AbulEduBoxFileManagerV1::enumAbulEduBoxFileManagerSavingLocation,QString,bool)),
                this, SLOT(slotAbeFileSaved(AbulEduBoxFileManagerV1::enumAbulEduBoxFileManagerSavingLocation,QString,bool)),Qt::UniqueConnection);
    }
    /* Faut il mettre à jour sur le PC ou prévenir que ça n'a pas marché ? */
//    else
//    {
//        ABULEDU_LOG_DEBUG()<<m_abuleduFile->abeFileSave(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/calculmentSettings.abe",
//                                                        listeFichiersMots,
//                                                        m_abuleduFile->abeFileGetDirectoryTemp().absolutePath(),
//                                                        QString("abe"));
//    }
        else
        {
            ABULEDU_LOG_DEBUG()<<"Impossible de synchroniser : pas d'utilisateur connecté";
    }
}

QString Editeur::associeNomIntitule(QString intitule)
{
    ABULEDU_LOG_TRACE()  << __PRETTY_FUNCTION__;
    QString nomExerciceCorrespondantIntitule;
    //QString locale = QLocale::system().name().section('_', 0, 0);
    QSettings configExo(m_settingsTempPath, QSettings::IniFormat);
    configExo.setIniCodec("UTF-8");
    int i=1;
    bool trouve = false;
    QStringListIterator iterateur(configExo.childGroups());
    while (iterateur.hasNext() && !trouve) {
        QString exercice = iterateur.next();
        configExo.beginGroup(exercice);

        ABULEDU_LOG_DEBUG()<<"exercice"<<exercice;
        ABULEDU_LOG_DEBUG()<<"intitule : "<<QString::fromUtf8((configExo.value("NomPourAffichage").toString()).toStdString().c_str());

        if (QString::fromUtf8((configExo.value("NomPourAffichage").toString()).toStdString().c_str())==intitule) {

            ABULEDU_LOG_DEBUG()<<"Nom : "<<exercice;
            ABULEDU_LOG_DEBUG()<<"Intitule"<<QString::fromUtf8((configExo.value("NomPourAffichage").toString()).toStdString().c_str());
            ABULEDU_LOG_DEBUG()<<"............... variable trouve = true..................";

            trouve = true;
            nomExerciceCorrespondantIntitule = exercice;
            //return;
        }
        configExo.endGroup();
        i++;
    }
    return nomExerciceCorrespondantIntitule;
}

void Editeur::installEventFilters()
{
    m_ui->cbNiveau->installEventFilter(this);
    m_ui->cbOperation->installEventFilter(this);
    m_ui->cbMaxD->installEventFilter(this);
    m_ui->cbMaxG->installEventFilter(this);
    m_ui->spbDMax->installEventFilter(this);
    m_ui->spbDMin->installEventFilter(this);
    m_ui->spbGMax->installEventFilter(this);
    m_ui->spbGMin->installEventFilter(this);
    m_ui->spbNombreBallons->installEventFilter(this);
    m_ui->sldVitesse->installEventFilter(this);
    m_ui->btnEditeurAnnuler->installEventFilter(this);
    m_ui->btnEditeurOK->installEventFilter(this);
    m_ui->gbNbDroite->installEventFilter(this);
    m_ui->gbNbGauche->installEventFilter(this);
}

void Editeur::on_btnEditeurOK_clicked()
{
    sauverNiveau(QString::number(m_niveauEnCours));
    sauverOperation(*m_operationEnCours);
    emit signalEditeurExited();
}

void Editeur::on_btnEditeurAnnuler_clicked()
{
    QFile::rename(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/copieModule.conf",m_settingsTempPath);
    emit signalEditeurExited();
}

void Editeur::slotAbeFileSaved(AbulEduBoxFileManagerV1::enumAbulEduBoxFileManagerSavingLocation location, QString name, bool success)
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__<<location<<name<<success;
    emit signalEditeurSaved();
}

void Editeur::slotOpenSettings(QSharedPointer<AbulEduFileV1> abeFile)
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__<<abeFile->abeFileGetFileName().fileName();
    if(abeFile->abeFileGetFileName().fileName().isEmpty()){
        m_abuleduFile = QSharedPointer<AbulEduFileV1>(new AbulEduFileV1(this), &QObject::deleteLater);
    }
    else{
        m_abuleduFile = abeFile;
    }
//    m_boxFileManager->abeSetFile(m_abuleduFile);
    m_settingsTempPath = m_abuleduFile->abeFileGetDirectoryTemp().absolutePath()+"/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf";
    initComboBoxOperations();

    QFile* fichierConf = new QFile(m_settingsTempPath);
    if (!fichierConf->exists()){
        ABULEDU_LOG_DEBUG()<<"C'est là que je le crée ...";
        initialiser();
    }
    else {
        QSettings conf(m_settingsTempPath, QSettings::IniFormat);
        qDebug()<<conf.value("version");
        if(conf.value("version").toFloat() > 1){
            ABULEDU_LOG_DEBUG()  << trUtf8("Fichier paramètres déjà présent");
        }
        else {
            /* Comme j'ai changé le nom des niveaux dans la version-2.0, si le fichier de paramètres est plus ancien je le supprime et le recrée */
            ABULEDU_LOG_DEBUG()  << trUtf8("Fichier paramètres déjà présent, mais de version inférieure à 2.0 donc à écraser");
            fichierConf->remove();
            initialiser();
        }
    }
}

void Editeur::editeurCreateSettings()
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__;
    m_abuleduFile = QSharedPointer<AbulEduFileV1>(new AbulEduFileV1(this), &QObject::deleteLater);
    m_abuleduFile->setProperty("isRecordable",false);
    m_settingsTempPath = m_abuleduFile->abeFileGetDirectoryTemp().absolutePath()+"/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf";
    ABULEDU_LOG_DEBUG()<<"Je suis déconnecté, je crée un fichier de conf provisoire.";
    initialiser();
    initComboBoxOperations();
}


void Editeur::editeurOpenSettings()
{
    ABULEDU_LOG_TRACE()  << __PRETTY_FUNCTION__;
    m_boxFileManager = new AbulEduBoxFileManagerV1(this);
    m_boxFileManager->setVisible(false);
    m_boxFileManager->abeBoxFileManagerSetSavingLocation(AbulEduBoxFileManagerV1::abeBoxPerso);
    connect(m_boxFileManager,SIGNAL(signalAbeFileSelected(QSharedPointer<AbulEduFileV1>)),this,SLOT(slotOpenSettings(QSharedPointer<AbulEduFileV1>)),Qt::UniqueConnection);
    m_boxFileManager->ouvertureFichier("calculmentSettings.abe");
}
