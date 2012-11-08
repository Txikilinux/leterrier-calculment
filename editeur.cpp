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
    m_settings = new QSettings(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf", QSettings::IniFormat);
    m_settings->setIniCodec("UTF-8");

    QFile* fichierConf = new QFile(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf");
         if (!fichierConf->exists()) initialiser();
         else qDebug()<<trUtf8("Fichier paramètres déjà présent");

    QStringList intitulesExercices;
    foreach (QString section,m_settings->childGroups()) {
        m_settings->beginGroup(section);
        intitulesExercices.append(m_settings->value("NomPourAffichage").toString());
        m_settings->endGroup();
    }

        m_ui->cbNiveau->addItem(trUtf8("Niveau1"), 1);
            m_ui->cbNiveau->addItem(trUtf8("Niveau2"), 2);
            m_ui->cbNiveau->addItem(trUtf8("Niveau3"), 3);
            m_ui->cbNiveau->addItem(trUtf8("Personnel"),4);

//        m_ui->cbOperation->addItem("Addition", 1);
//            m_ui->cbOperation->addItem("Multiplication", 2);
//            m_ui->cbOperation->addItem("Soustraction",3);
//            m_ui->cbOperation->addItem("OdGrandeurAddition",4);
//            m_ui->cbOperation->addItem("OdGrandeurSoustraction",4);
//            m_ui->cbOperation->addItem("OdGrandeurMultiplication",4);

        m_ui->cbOperation->addItems(intitulesExercices);//remplace le bloc au-dessus

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
        QString niveauLu = m_ui->cbNiveau->currentText();
        if (niveauLu.right(1).toInt() == 0) {
            m_niveauEnCours = new QString("Personnel");
            }
        else m_niveauEnCours = new QString("Niveau"+niveauLu.right(1));
        m_operationEnCours = new QString(associeNomIntitule(m_ui->cbOperation->currentText()));
        m_minG=m_maxG=m_minD=m_maxD=0;


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
    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf", QSettings::IniFormat);
    config.setIniCodec("UTF-8");
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
    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf", QSettings::IniFormat);
    config.setIniCodec("UTF-8");
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
    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf", QSettings::IniFormat);
    config.setIniCodec("UTF-8");
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
    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf", QSettings::IniFormat);
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
            else if (operation[0] == 'm')
            {
                config.setValue("NomPourAffichage", trUtf8("Maison des nombres"));
            }
    config.endGroup();
}


void Editeur::initialiser()
{
    //On aurait pu initialiser dans le répertoire conf de l'application, mais l'utilisateur n'aurait pas eu les droits
    //QSettings config("./conf/parametres_"+qApp->property("langageUtilise").toString()+".conf", QSettings::IniFormat);

    //On initialise donc directement dans le /home de l'utilisateur
    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf", QSettings::IniFormat);
    config.setIniCodec("UTF-8");
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
    initialiserComplement("maisonDesNombres");

}

void Editeur::sauverNiveau(QString niveau)
{
    qDebug()<<"Editeur::sauverNiveau(1): "<<niveau<<" dans "<<*m_operationEnCours;
    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf", QSettings::IniFormat);
    config.setIniCodec("UTF-8");
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
        *m_niveauEnCours = niveau;
        disconnect(m_ui->spbGMin, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
        disconnect(m_ui->spbGMax, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
        disconnect(m_ui->spbDMin, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
        disconnect(m_ui->spbDMax, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));

        qDebug()<<"Editeur::sauverNiveau(2)";
}

void Editeur::chargerNiveau(QString niveau)
{
    qDebug()<<"chargerNiveau(1):"<<niveau<<" dans "<<*m_operationEnCours;
       m_ui->spbDMin->setMaximum(1000);
       m_ui->spbGMin->setMaximum(1000);
       m_ui->spbDMax->setMinimum(0);
       m_ui->spbGMax->setMinimum(0);

    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf", QSettings::IniFormat);
    config.setIniCodec("UTF-8");
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
    QString chaineEnParametre = chaine;
    if (chaineEnParametre.right(1).toInt() == 0) {
        chaine = "Personnel";
        }
    else chaine = "Niveau"+chaineEnParametre.right(1);
    this->sauverNiveau(*m_niveauEnCours);
    this->chargerNiveau(chaine);
    *m_niveauEnCours = chaine;
}

void Editeur::sauverOperation(QString operation)
{
    qDebug()<<"Editeur::sauverOperation(1):"<<operation<<" pour "<<*m_niveauEnCours;
    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf", QSettings::IniFormat);
    config.setIniCodec("UTF-8");
        config.setValue("NombreBallons", m_ui->spbNombreBallons->value());
        config.beginGroup(operation);
            config.beginGroup(*m_niveauEnCours);
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
        *m_operationEnCours = operation;

        disconnect(m_ui->spbGMin, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
        disconnect(m_ui->spbGMax, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
        disconnect(m_ui->spbDMin, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));
        disconnect(m_ui->spbDMax, SIGNAL(valueChanged(int)), this, SLOT(ajusterValeurs(int)));

}

void Editeur::chargerOperation(QString operation)
{
    qDebug()<<"Editeur::chaRgerOperation(1):"<<operation<<" pour "<<*m_niveauEnCours;
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
     else if (operation.right(4)=="tion"){
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

    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf", QSettings::IniFormat);
    config.setIniCodec("UTF-8");
    m_ui->spbNombreBallons->setValue(config.value("NombreBallons",10).toInt());
    config.beginGroup(operation);
        config.beginGroup(*m_niveauEnCours);
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
    qDebug()<<"Editeur::chaNgerOperation(1):"<<operation<<" pour "<<*m_niveauEnCours;
    this->sauverOperation(*m_operationEnCours);
    this->chargerOperation(associeNomIntitule(operation));
    *m_operationEnCours = associeNomIntitule(operation);
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

QString Editeur::associeNomIntitule(QString intitule)
{
    qDebug()<<"Editeur::associeNomIntitule(1)";
    QString nomExerciceCorrespondantIntitule;
    //QString locale = QLocale::system().name().section('_', 0, 0);
    QSettings configExo(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf", QSettings::IniFormat);
    configExo.setIniCodec("UTF-8");
    int i=1;
    bool trouve = false;
    QStringListIterator iterateur(configExo.childGroups());
    while (iterateur.hasNext() && !trouve) {
        QString exercice = iterateur.next();
        configExo.beginGroup(exercice);
        qDebug()<<"exercice"<<exercice;
        qDebug()<<"intitule : "<<QString::fromUtf8((configExo.value("NomPourAffichage").toString()).toStdString().c_str());
        if (QString::fromUtf8((configExo.value("NomPourAffichage").toString()).toStdString().c_str())==intitule) {
            qDebug()<<"Nom : "<<exercice;
            qDebug()<<"Intitule"<<QString::fromUtf8((configExo.value("NomPourAffichage").toString()).toStdString().c_str());
            trouve = true;
            nomExerciceCorrespondantIntitule = exercice;
            qDebug()<<"............... variable trouve = true..................";
            //return;
        }
        configExo.endGroup();
        i++;
    }
    qDebug()<<"Editeur::associeNomIntitule(2)";
    qDebug()<<"Nom : "<<nomExerciceCorrespondantIntitule;
    qDebug()<<"Intitule"<<intitule;
//    emit cbExerciceFini(m_nomExercice);
//    qDebug()<<"Signal emis avec "<<m_nomExercice;
    return nomExerciceCorrespondantIntitule;
}
