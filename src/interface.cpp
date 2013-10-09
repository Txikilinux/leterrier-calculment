/**
  * Implémentation de l'interface générale du logiciel, qui permet de lancer les exercices soit par zone cliquable, soit par menu
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

#include "interface.h"
#include "ui_interface.h"
#include "exercice.h"
#include "exercicemaisonnombres.h"
#include "ui_exercice.h"
#include "editeur.h"
#include "boutonspolygone.h"
#include "abuleduaproposv0.h"
#include "abuledulanceurv1.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QDesktopServices>


interface::interface(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::interfaceClass)
{

    //Langue
    //m_locale = QLocale::system().name().section('_', 0, 0);
    m_locale = qApp->property("langageUtilise").toString();
    qDebug()<<"interface::constructeur (1) - "<<m_locale;
    //Un 1er qtTranslator pour prendre les traductions QT Systeme
    //c'est d'ailleurs grâce à ça qu'on est en RTL
    qtTranslator.load("qt_" + m_locale,
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    qApp->installTranslator(&qtTranslator);
    //Et un second qtTranslator pour les traductions spécifiques du logiciel
    myappTranslator.load("leterrier-calculment_" + m_locale, "lang");
    qDebug()<<trUtf8("Langue chargée : ")<<m_locale;
    qApp->installTranslator(&myappTranslator);

    ui->setupUi(this);
    AbulEduAproposV0 *monAide=new AbulEduAproposV0(this);
    m_signalMapper = new QSignalMapper(this);
    connect(m_signalMapper, SIGNAL(mapped(QString)), this, SLOT(changelangue(QString)) );
    creeMenuLangue();

    QRect ecran;
    ecran=QApplication::desktop()->availableGeometry();

    QFile* fichierConf = new QFile(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf");
    if (!fichierConf->exists()) qDebug()<<trUtf8("Fichier config NON trouvé");
    else qDebug() << trUtf8("Fichier config trouvé");
    m_hauteurMax = ecran.height();
    qDebug() << "Taille ecran : " << ecran.width()<< " X "<<ecran.height();
    this->resize(ecran.width(),ecran.height());
    ui->fete->resize(ecran.width(),m_hauteurMax);

    QPixmap imgFond("./data/images/fondecran.jpg");
    QPixmap imgFond2=imgFond.scaled(ecran.width(),m_hauteurMax,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    QBrush* fond = new QBrush(imgFond2);
    ui->fete->setBackgroundBrush(*fond);
    this->setFixedSize(imgFond2.width(),imgFond2.height());
    ui->btnInitialise->setGeometry(imgFond2.width()-180,10,170,30);
    ui->btnInitialise->hide();

    double kw=static_cast<double>(imgFond2.width())/static_cast<double>(imgFond.width());
    double kh=static_cast<double>(imgFond2.height())/static_cast<double>(imgFond.height());

    QGraphicsScene* dessin = new QGraphicsScene(this);
    ui->fete->setScene(dessin);
    dessin->setSceneRect(0, 0, ecran.width(),m_hauteurMax);

    m_editeur = new Editeur();

    //Bouton sur les auto-tamponneuses
    boutonsPolygone* btnPoly1 = new boutonsPolygone("addition");
    btnPoly1->deplace(0*kw,440*kh);
    btnPoly1->retaille(630*kw,235*kh);
    //btnPoly1->tourne(-20);
    btnPoly1->QGraphicsItem::setToolTip(trUtf8("Faire des additions"));
    btnPoly1->setTexte(trUtf8("Additions"));
    dessin->addItem(btnPoly1);
    connect(btnPoly1, SIGNAL(clicked()), this, SLOT(close()));

    //Bouton sur le manège
    boutonsPolygone* btnPoly2 = new boutonsPolygone("multiplication");
    btnPoly2->deplace(635*kw,550*kh);
    btnPoly2->retaille(430*kw,410*kh);
    btnPoly2->QGraphicsItem::setToolTip(trUtf8("Faire des multiplications"));
    btnPoly2->setTexte(trUtf8("Multiplications"));
    dessin->addItem(btnPoly2);

    //Bouton sur la barbapapa
    boutonsPolygone* btnPoly3 = new boutonsPolygone("editeur");
    btnPoly3->deplace(60*kw,680*kh);
    btnPoly3->retaille(190*kw,285*kh);
    btnPoly3->QGraphicsItem::setToolTip(trUtf8("Lancer l'éditeur"));
    dessin->addItem(btnPoly3);

    //Bouton sur le panneau sortie
    boutonsPolygone* btnPoly4 = new boutonsPolygone("sortie");
    btnPoly4->setImage(QPixmap("./data/images/texteSortie_"+m_locale+".png"));
    btnPoly4->deplace(1340*kw,723*kh);
    btnPoly4->retaille(170*kw,40*kh);
    btnPoly4->QGraphicsItem::setToolTip(trUtf8("Quitter"));
    connect(btnPoly4, SIGNAL(sortie()), this, SLOT(close()));
    dessin->addItem(btnPoly4);

    //Bouton sur la grande roue
    boutonsPolygone* btnPoly5 = new boutonsPolygone("1tableM");
    btnPoly5->deplace(40*kw,15*kh);
    btnPoly5->retaille(340*kw,365*kh);
    btnPoly5->QGraphicsItem::setToolTip(trUtf8("Tables de multiplication"));
    btnPoly5->setTexte(trUtf8("Tables de multiplication"));
    dessin->addItem(btnPoly5);

    //Bouton sur le train fantôme
    boutonsPolygone* btnPoly6 = new boutonsPolygone("2complementA");
    btnPoly6->deplace(765*kw,275*kh);
    btnPoly6->retaille(395*kw,200*kh);
    btnPoly6->tourne(12);
    btnPoly6->QGraphicsItem::setToolTip(trUtf8("Compléments additifs"));
    btnPoly6->setTexte(trUtf8("Compléments additifs"));
    dessin->addItem(btnPoly6);

    //Bouton sur le stand de tir
    boutonsPolygone* btnPoly7 = new boutonsPolygone("3complementM");
    btnPoly7->deplace(1135*kw,510*kh);
    btnPoly7->retaille(180*kw,120*kh);
    btnPoly7->tourne(10);
    btnPoly7->QGraphicsItem::setToolTip(trUtf8("Multiples"));
    btnPoly7->setTexte(trUtf8("Multiples"));
    dessin->addItem(btnPoly7);

    //Bouton sur la chenille
    boutonsPolygone* btnPoly8 = new boutonsPolygone("soustraction");
    btnPoly8->deplace(515*kw,220*kh);
    btnPoly8->retaille(220*kw,220*kh);
    btnPoly8->QGraphicsItem::setToolTip(trUtf8("Faire des soustractions"));
    btnPoly8->setTexte(trUtf8("Soustractions"));
    dessin->addItem(btnPoly8);

    //Bouton sur le "rooster"
    boutonsPolygone* btnPoly9 = new boutonsPolygone("4tableA");
    btnPoly9->deplace(425*kw,185*kh);
    btnPoly9->retaille(300*kw,50*kh);
    btnPoly9->tourne(-35);
    btnPoly9->QGraphicsItem::setToolTip(trUtf8("Tables d'addition"));
    btnPoly9->setTexte(trUtf8("Tables d'addition"));
    dessin->addItem(btnPoly9);

    //Bouton sur le palais des glaces
    boutonsPolygone* btnPoly10 = new boutonsPolygone("5OdGrandeur",100);
    btnPoly10->deplace(1315*kw,560*kh);
    btnPoly10->retaille(280*kw,150*kh);
    btnPoly10->QGraphicsItem::setToolTip(trUtf8("Ordres de grandeur"));
    dessin->addItem(btnPoly10);

    //Bouton l'avion
    boutonsPolygone* btnPoly11 = new boutonsPolygone("lanceur");
    btnPoly11->deplace(1470*kw, 70*kh);
    btnPoly11->retaille(85*kw,50*kh);
    btnPoly11->QGraphicsItem::setToolTip(trUtf8("Choisir son exercice"));
    dessin->addItem(btnPoly11);

    qApp->setProperty("VerrouNombres",true);
    QDesktopWidget *widget = QApplication::desktop();
    int desktop_width = widget->width();
    int desktop_height = widget->height();
    this->move((desktop_width-this->width())/2, (desktop_height-this->height())/2);
}

interface::~interface()
{
    delete ui;
}

void interface::creeMenuLangue()
{
    QString nomFichierConf = "./conf/abuledulangselector.conf";
    if (!QFile(nomFichierConf).exists())
    {
        QMessageBox *alertBox=new QMessageBox(QMessageBox::Warning,trUtf8("Problème !!"),trUtf8("Fichier de configuration des langues non trouvé"));
        alertBox->show();
        return;
    }
    QSettings configLang(nomFichierConf, QSettings::IniFormat);
     configLang.setIniCodec("UTF-8");
     configLang.beginGroup(m_locale);
     foreach (QString clef, configLang.allKeys())
     {
            QAction* actionLangue = new QAction(configLang.value(clef).toString(),this);
            actionLangue->setCheckable(true);
            if (clef ==m_locale) actionLangue->setChecked(true);
            else actionLangue->setChecked(false);
            m_signalMapper->setMapping(actionLangue, clef );
            connect(actionLangue, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
            ui->menuLangues->addAction(actionLangue);
     }


}

void interface::changelangue(QString langue)
{
    qDebug()<<"interface::changelangue(1)";
    qApp->setProperty("langageUtilise",langue);
    myappTranslator.load("leterrier-calculment_"+langue, "lang");

    qApp->installTranslator(&myappTranslator);
    interface* nouvelleInterface = new interface();
    nouvelleInterface->setGeometry(this->geometry());
    nouvelleInterface->show();
    nouvelleInterface->setWindowTitle(QObject::trUtf8("Calcul Mental"));
    this->close();
    nouvelleInterface->activateWindow();
}

void interface::paintEvent(QPaintEvent* e )
{
    QMainWindow::paintEvent(e);
}
void interface::on_actionQuitter_triggered()
{
    this->close();
}

void interface::on_actionAdditions_triggered()
{
    m_exercice = new exercice("addition",this,0,"");
    m_exercice->show();
}

void interface::on_actionMultiplications_triggered()
{
    m_exercice = new exercice("multiplication",this,0,"");
    m_exercice->show();
}

void interface::on_actionp2_triggered()
{
    m_exercice = new exercice("tableA",this,2,"");
    m_exercice->show();
}

void interface::on_actionp3_triggered()
{
    m_exercice = new exercice("tableA",this,3,"");
    m_exercice->show();
}

void interface::on_actionp4_triggered()
{
    m_exercice = new exercice("tableA",this,4,"");
    m_exercice->show();
}

void interface::on_actionp5_triggered()
{
    m_exercice = new exercice("tableA",this,5,"");
    m_exercice->show();
}

void interface::on_actionp6_triggered()
{
    m_exercice = new exercice("tableA",this,6,"");
    m_exercice->show();
}

void interface::on_actionp7_triggered()
{
    m_exercice = new exercice("tableA",this,7,"");
    m_exercice->show();
}

void interface::on_actionp8_triggered()
{
    m_exercice = new exercice("tableA",this,8,"");
    m_exercice->show();
}

void interface::on_actionp9_triggered()
{
    m_exercice = new exercice("tableA",this,9,"");
    m_exercice->show();
}

void interface::on_actionX_2_triggered()
{
    m_exercice = new exercice("tableM",this,2,"");
    m_exercice->show();
}

void interface::on_actionX_3_triggered()
{
    m_exercice = new exercice("tableM",this,3,"");
    m_exercice->show();
}

void interface::on_actionX_4_triggered()
{
    m_exercice = new exercice("tableM",this,4,"");
    m_exercice->show();
}

void interface::on_actionX_5_triggered()
{
    m_exercice = new exercice("tableM",this,5,"");
    m_exercice->show();
}

void interface::on_actionX_6_triggered()
{
    m_exercice = new exercice("tableM",this,6,"");
    m_exercice->show();
}

void interface::on_actionX_7_triggered()
{
    m_exercice = new exercice("tableM",this,7,"");
    m_exercice->show();
}

void interface::on_actionX_8_triggered()
{
    m_exercice = new exercice("tableM",this,8,"");
    m_exercice->show();
}

void interface::on_actionX_9_triggered()
{
    m_exercice = new exercice("tableM",this,9,"");
    m_exercice->show();
}

void interface::on_actionAfficher_l_diteur_triggered()
{
    m_editeur->show();
}

void interface::on_btnInitialise_clicked()
{
    QFile* fichierConf = new QFile(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf");
    fichierConf->remove();
    m_editeur->initialiser();
}

void interface::on_actionDe_5_triggered()
{
    m_exercice = new exercice("complementM5",this, 5,"");
    m_exercice->show();
}

void interface::on_actionDe_10_triggered()
{
    m_exercice = new exercice("complementM10",this, 10,"");
    m_exercice->show();
}
void interface::on_actionDe_15_triggered()
{
    m_exercice = new exercice("complementM15",this, 15,"");
    m_exercice->show();
}
void interface::on_actionDe_20_triggered()
{
    m_exercice = new exercice("complementM20",this, 20,"");
    m_exercice->show();
}
void interface::on_actionDe_25_triggered()
{
    m_exercice = new exercice("complementM25",this, 25,"");
    m_exercice->show();
}
void interface::on_actionDe_50_triggered()
{
    m_exercice = new exercice("complementM50",this, 50,"");
    m_exercice->show();
}

void interface::on_actionSoustractions_triggered()
{
    m_exercice = new exercice("soustraction",this,0,"");
    m_exercice->show();
}

void interface::on_action_10_triggered()
{
    m_exercice = new exercice("complementA10",this, 10,"");
    m_exercice->show();
}

void interface::on_action_20_triggered()
{
    m_exercice = new exercice("complementA1000",this, 1000,"");
    m_exercice->show();
}

void interface::on_action_100_triggered()
{
    m_exercice = new exercice("complementA100",this, 100,"");
    m_exercice->show();
}

void interface::on_action_un_nombre_al_atoire_triggered()
{

}

void interface::on_actionSur_des_additions_triggered()
{
    m_exercice = new exercice("OdGrandeurAddition",this,100,"");
    m_exercice->show();
}

void interface::on_actionSur_des_soustractions_triggered()
{
    m_exercice = new exercice("OdGrandeurSoustraction",this,100,"");
    m_exercice->show();
}

void interface::on_actionSur_des_multiplications_triggered()
{
    m_exercice = new exercice("OdGrandeurMultiplication",this,100,"");
    m_exercice->show();
}

void interface::on_actionMaison_des_nombres_triggered()
{
    ExerciceMaisonNombres* maisonNombres = new ExerciceMaisonNombres("maisonDesNombres",0,0);
    maisonNombres->show();
}

void interface::on_actionVerrouillage_nombres_changed()
{
    if (ui->actionVerrouillage_nombres->isChecked())
        qApp->setProperty("VerrouNombres",true);
    else
        qApp->setProperty("VerrouNombres",false);
}

void interface::on_action_Journal_de_mes_activit_s_triggered()
{
    abeApp->getAbeIdentite()->abeGetMyLogsAsPDF();
}

void interface::on_action_Changer_d_utilisateur_triggered()
{
    abeApp->getAbeNetworkAccessManager()->abeSSOLogout();
    abeApp->getAbeNetworkAccessManager()->abeSSOLogin();
}
