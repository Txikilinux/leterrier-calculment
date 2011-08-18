/**
  * Implémentation de l'éditeur de niveaux pour les différents exercices
  *
  * @see https://redmine.ryxeo.com/projects/leterrier-calculment/
  * @author 2009-2010 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
  * @see The GNU Public License (GPL)
  */

/*
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

#include <QComboBox>
#include <QSettings>
#include <QCloseEvent>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QMessageBox>


Editeur::Editeur(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::Editeur)
{
    this->setWindowModality(Qt::ApplicationModal);
    m_ui->setupUi(this);
    qDebug()<<"Editeur::constructeur (1)";
    qDebug()<<m_ui->cbOperation->currentText();
    qDebug()<<m_ui->cbNiveau->currentText();

        //Ajout des items dans les comboBox - celles concernant les min et max sont cachées par défaut -
        m_ui->cbNiveau->addItem("Niveau1", 1);
            m_ui->cbNiveau->addItem("Niveau2", 2);
            m_ui->cbNiveau->addItem("Niveau3", 3);
            m_ui->cbNiveau->addItem("Personnel",4);

        m_ui->cbOperation->addItem("Addition", 1);
            m_ui->cbOperation->addItem("Multiplication", 2);
            m_ui->cbOperation->addItem("Soustraction",3);
            m_ui->cbOperation->addItem("OdGrandeurAddition",4);
            m_ui->cbOperation->addItem("OdGrandeurSoustraction",4);
            m_ui->cbOperation->addItem("OdGrandeurMultiplication",4);

        m_ui->cbMaxG->addItem("10",1);
            m_ui->cbMaxG->addItem("100",2);
            m_ui->cbMaxG->addItem("1000",3);
            m_ui->cbMaxG->hide();
            m_ui->lblGMax_2->hide();
        m_ui->cbMaxD->addItem("10",1);
            m_ui->cbMaxD->addItem("100",2);
            m_ui->cbMaxD->addItem("1000",3);
            m_ui->cbMaxD->hide();
            m_ui->lblDMax_2->hide();

        connect(m_ui->sldVitesse, SIGNAL(valueChanged(int)), m_ui->pbVitesse, SLOT(setValue(int)));

        //Initialisation attributs de la classe
        m_niveauEnCours = new QString(m_ui->cbNiveau->currentText());
        m_operationEnCours = new QString(m_ui->cbOperation->currentText());
        m_minG=m_maxG=m_minD=m_maxD=0;

        QFile* fichierConf = new QFile(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres.conf");
             if (!fichierConf->exists()) initialiser();
        this->chargerNiveau(*m_niveauEnCours);

        connect(m_ui->cbNiveau, SIGNAL(currentIndexChanged(QString)), this, SLOT(changerNiveau(QString)));
        connect(m_ui->cbOperation, SIGNAL(currentIndexChanged(QString)), this, SLOT(changerOperation(QString)));
        qDebug()<<"Editeur::constructeur (2)";
        qDebug()<<m_ui->cbOperation->currentText();
        qDebug()<<m_ui->cbNiveau->currentText();
}

Editeur::~Editeur()
{
    delete m_ui;
}

QString Editeur::getNiveauEnCours()
{
    return *m_niveauEnCours;
}

void Editeur::initialiserOperation(QString operation)
{
    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres.conf", QSettings::IniFormat);
    config.beginGroup(operation);
            config.beginGroup("Niveau1");
                config.setValue("MinGauche", 0);
                config.setValue("MaxGauche", 9);
                config.setValue("MinDroite", 0);
                config.setValue("MaxDroite", 9);
                config.setValue("TempsAccorde",8);
            config.endGroup();
            config.beginGroup("Niveau2");
                config.setValue("MinGauche", 0);
                config.setValue("MaxGauche", 9);
                config.setValue("MinDroite", 0);
                config.setValue("MaxDroite", 99);
                config.setValue("TempsAccorde",8);
            config.endGroup();
            config.beginGroup("Niveau3");
                config.setValue("MinGauche", 0);
                config.setValue("MaxGauche", 99);
                config.setValue("MinDroite", 0);
                config.setValue("MaxDroite", 99);
                config.setValue("TempsAccorde",8);
            config.endGroup();
            config.beginGroup("Personnel");
                config.setValue("MinGauche", 0);
                config.setValue("MaxGauche", 5);
                config.setValue("MinDroite", 0);
                config.setValue("MaxDroite", 5);
                config.setValue("TempsAccorde",8);
            config.endGroup();
            config.setValue("NiveauEnCours"+operation, "Niveau1");
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
    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres.conf", QSettings::IniFormat);
    config.beginGroup(operation);
            config.beginGroup("Niveau1");
                config.setValue("MaxGauche", 100);
                config.setValue("MaxDroite", 100);
                config.setValue("TempsAccorde",8);
            config.endGroup();
            config.beginGroup("Niveau2");
                config.setValue("MaxGauche", 1000);
                config.setValue("MaxDroite", 100);
                config.setValue("TempsAccorde",8);
            config.endGroup();
            config.beginGroup("Niveau3");
                config.setValue("MaxGauche", 1000);
                config.setValue("MaxDroite", 1000);
                config.setValue("TempsAccorde",8);
            config.endGroup();
            config.beginGroup("Personnel");
                config.setValue("MaxGauche", 1000);
                config.setValue("MaxDroite", 1000);
                config.setValue("TempsAccorde",8);
            config.endGroup();
            config.setValue("NiveauEnCours"+operation, "Niveau1");
            if (operation == "OdGrandeurAddition")
                    config.setValue("NomPourAffichage", trUtf8("OdG Additions"));
            else if (operation == "OdGrandeurSoustraction")
                    config.setValue("NomPourAffichage", trUtf8("OdG Soustractions"));
    config.endGroup();
}

void Editeur::initialiserApprocheM(QString operation)
{
    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres.conf", QSettings::IniFormat);
    config.beginGroup(operation);
            config.beginGroup("Niveau1");
                config.setValue("MaxGauche", 100);
                config.setValue("MaxDroite", 10);
                config.setValue("TempsAccorde",8);
            config.endGroup();
            config.beginGroup("Niveau2");
                config.setValue("MaxGauche", 1000);
                config.setValue("MaxDroite", 10);
                config.setValue("TempsAccorde",8);
            config.endGroup();
            config.beginGroup("Niveau3");
                config.setValue("MaxGauche", 100);
                config.setValue("MaxDroite", 100);
                config.setValue("TempsAccorde",8);
            config.endGroup();
            config.beginGroup("Personnel");
                config.setValue("MaxGauche", 1000);
                config.setValue("MaxDroite", 1000);
                config.setValue("TempsAccorde",8);
            config.endGroup();
            config.setValue("NiveauEnCours"+operation, "Niveau1");
            config.setValue("NomPourAffichage", trUtf8("OdG Multiplications"));
    config.endGroup();
}

void Editeur::initialiserComplement(QString operation)
{
    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres.conf", QSettings::IniFormat);
    config.setIniCodec("UTF-8");
    config.beginGroup(operation);
            config.beginGroup("Niveau1");
                config.setValue("TempsAccorde",8);
            config.endGroup();
            config.beginGroup("Niveau2");
                config.setValue("TempsAccorde",6);
            config.endGroup();
            config.beginGroup("Niveau3");
                config.setValue("TempsAccorde",4);
            config.endGroup();
            config.beginGroup("Personnel");
                config.setValue("TempsAccorde",6);
            config.endGroup();
            config.setValue("NiveauEnCours"+operation, "Niveau1");
            if (operation[0] == 'c')
            {
                if (operation[10] == 'A')
                {
                    if (operation.length() == 13) {
                        config.setValue("NomPourAffichage", trUtf8("Compléments à 10"));
                        qDebug()<<"-------------- ecriture : Compléments à 10";
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
    config.endGroup();
}


void Editeur::initialiser()
{
    //On aurait pu initialiser dans le répertoire conf de l'application, mais l'utilisateur n'aurait pas eu les droits
    //QSettings config("./conf/parametres.conf", QSettings::IniFormat);

    //On initialise donc directement dans le /home de l'utilisateur
    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres.conf", QSettings::IniFormat);
    config.setValue("NombreBallons", 10);
    initialiserOperation("addition");
    initialiserOperation("multiplication");
    initialiserOperation("soustraction");
    initialiserApproche("OdGrandeurAddition");
    initialiserApproche("OdGrandeurSoustraction");
    initialiserApprocheM("OdGrandeurMultiplication");
    initialiserComplement("complementA10");
    initialiserComplement("complementA100");
    initialiserComplement("complementA1000");
    for (int i=1;i<=5;i++) {
        initialiserComplement("complementM"+QString::number(i*5));
        }
    initialiserComplement("complementM50");
    for (int i=2;i<=9;i++) initialiserComplement("tableM"+QString::number(i));
    for (int i=2;i<=9;i++) initialiserComplement("tableA"+QString::number(i));

}

void Editeur::sauverNiveau(QString niveau)
{
    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres.conf", QSettings::IniFormat);
        config.setValue("NombreBallons", m_ui->spbNombreBallons->value());
        config.beginGroup(*m_operationEnCours);
            config.beginGroup(niveau);
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
                config.setValue("TempsAccorde",m_ui->sldVitesse->value());
            config.endGroup();
        config.endGroup();
        *m_niveauEnCours = niveau;
        disconnect(m_ui->spbGMin, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
        disconnect(m_ui->spbGMax, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
        disconnect(m_ui->spbDMin, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
        disconnect(m_ui->spbDMax, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));

        qDebug() << "Editeur.cpp :fin de sauverniveau";
}

void Editeur::chargerNiveau(QString niveau)
{
       m_ui->spbDMin->setMaximum(1000);
       m_ui->spbGMin->setMaximum(1000);
       m_ui->spbDMax->setMinimum(0);
       m_ui->spbGMax->setMinimum(0);

    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres.conf", QSettings::IniFormat);
    m_ui->spbNombreBallons->setValue(config.value("NombreBallons").toInt());
    config.beginGroup(*m_operationEnCours);
        config.beginGroup(niveau);
            m_ui->spbGMin->setValue(config.value("MinGauche").toInt());
            m_maxG = config.value("MaxGauche").toInt();
            m_ui->spbGMax->setValue(m_maxG);
            m_ui->spbDMin->setValue(config.value("MinDroite").toInt());
            m_maxD = config.value("MaxDroite").toInt();
            m_ui->spbDMax->setValue(m_maxD);
            m_ui->sldVitesse->setValue(config.value("TempsAccorde").toInt());
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
    this->sauverNiveau(*m_niveauEnCours);
    this->chargerNiveau(chaine);
    *m_niveauEnCours = chaine;
}

void Editeur::sauverOperation(QString operation)
{
    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres.conf", QSettings::IniFormat);
        config.setValue("NombreBallons", m_ui->spbNombreBallons->value());
        config.beginGroup(operation);
            config.beginGroup(*m_niveauEnCours);
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
                config.setValue("TempsAccorde",m_ui->sldVitesse->value());
            config.endGroup();
        config.endGroup();
        *m_operationEnCours = operation;

        disconnect(m_ui->spbGMin, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
        disconnect(m_ui->spbGMax, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
        disconnect(m_ui->spbDMin, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
        disconnect(m_ui->spbDMax, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));

}

void Editeur::chargerOperation(QString operation)
{
     if (operation.left(10)=="OdGrandeur") {
            m_ui->spbGMin->setDisabled(true);
            m_ui->spbDMin->setDisabled(true);
            m_ui->spbGMax->hide();
                m_ui->spbDMax->hide();
                m_ui->lblGMax->hide();
                m_ui->lblDMax->hide();
                m_ui->cbMaxG->show();
                m_ui->cbMaxD->show();
                m_ui->lblGMax_2->show();
                m_ui->lblDMax_2->show();
            }
    else {
        m_ui->spbGMin->setEnabled(true);
        m_ui->spbDMin->setEnabled(true);
        m_ui->spbGMax->setMaximum(99);
        m_ui->spbDMax->setMaximum(99);
        m_ui->spbGMax->show();
            m_ui->spbDMax->show();
            m_ui->lblGMax->show();
            m_ui->lblDMax->show();
            m_ui->cbMaxG->hide();
            m_ui->cbMaxD->hide();
            m_ui->lblGMax_2->hide();
            m_ui->lblDMax_2->hide();
        }

    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres.conf", QSettings::IniFormat);
    m_ui->spbNombreBallons->setValue(config.value("NombreBallons").toInt());
    config.beginGroup(operation);
        config.beginGroup(*m_niveauEnCours);
            m_maxG = config.value("MaxGauche").toInt();
            m_ui->spbGMax->setValue(m_maxG);
            m_ui->spbGMin->setValue(config.value("MinGauche").toInt());
            m_maxD = config.value("MaxDroite").toInt();
            m_ui->spbDMax->setValue(m_maxD);
            m_ui->spbDMin->setValue(config.value("MinDroite").toInt());
            m_ui->sldVitesse->setValue(config.value("TempsAccorde").toInt());
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
    this->sauverOperation(*m_operationEnCours);
    this->chargerOperation(operation);
    *m_operationEnCours = operation;
}

void Editeur::closeEvent(QCloseEvent *event)
{
    this->sauverNiveau(*m_niveauEnCours);
    this->sauverOperation(*m_operationEnCours);
    event->accept();
    //delete(this);

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

void Editeur::on_btnQuitter_clicked()
{
    this->close();
    //delete(this);
}

void Editeur::ajusterValeurs(int valeurNouvelle)
{
    valeurNouvelle = 0;
       qDebug() << "Editeur.cpp : appel de ajusterValeurs";
       m_ui->spbDMin->setMaximum(m_ui->spbDMax->value());
       m_ui->spbGMin->setMaximum(m_ui->spbGMax->value());
       m_ui->spbDMax->setMinimum(m_ui->spbDMin->value());
       m_ui->spbGMax->setMinimum(m_ui->spbGMin->value());
}
