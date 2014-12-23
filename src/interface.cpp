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
#include "abuleduaproposv1.h"
#include "abuledulanceurv1.h"
#include "exerciceoperation.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QDesktopServices>


interface::interface(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::interfaceClass)
{
    m_localDebug = false;
    //Langue
    //m_locale = QLocale::system().name().section('_', 0, 0);
    m_locale = qApp->property("langageUtilise").toString();
    if(m_localDebug) qDebug()<<"interface::constructeur (1) - "<<m_locale;
    /* Un 1er qtTranslator pour prendre les traductions QT Systeme
       c'est d'ailleurs grâce à ça qu'on est en RTL */
    qtTranslator.load("qt_" + m_locale,
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    qApp->installTranslator(&qtTranslator);
    /* Et un second qtTranslator pour les traductions spécifiques du logiciel */
    myappTranslator.load("leterrier-calculment_" + m_locale, "lang");
    if(m_localDebug) qDebug()<<trUtf8("Langue chargée : ")<<m_locale;
    qApp->installTranslator(&myappTranslator);

    ui->setupUi(this);
#ifndef __ABULEDUTABLETTEV1__MODE__
    ui->aboutPage->abeAproposSetMainWindow(this);
#endif
    connect(ui->aboutPage, SIGNAL(signalAbeAproposSetindex(int)),this,SLOT(slotInterfaceShowAboutPage()), Qt::UniqueConnection);
    connect(ui->aboutPage, SIGNAL(signalAbeAproposBtnCloseClicked()), this, SLOT(slotInterfaceShowMainPage()),Qt::UniqueConnection);

    m_signalMapper = new QSignalMapper(this);
    connect(m_signalMapper, SIGNAL(mapped(QString)), this, SLOT(changelangue(QString)) );
    creeMenuLangue();

    m_activityFilter = new ActivityFilter(abeApp);
    m_activityFilter->setInterval(6000);
    m_activityFilter->setObjectName("demoActivityFilter");
    abeApp->installEventFilter(m_activityFilter);
    QObject::connect(m_activityFilter, SIGNAL(userActive()),   this,  SLOT(slotInterfaceEndDemo()), Qt::UniqueConnection);
    QObject::connect(m_activityFilter, SIGNAL(userInactive()),this,  SLOT(slotInterfaceDemo()), Qt::UniqueConnection);

    /* Création de la page d'accueil et insertion dans la stackedWidget */
    m_abuleduPageAccueil = new AbulEduPageAccueilV1(ui->mainPage);
    connect(m_abuleduPageAccueil, SIGNAL(boutonPressed(int,QString)), this, SLOT(slotInterfaceLaunchExercise(int,QString)), Qt::UniqueConnection);
    connect(m_abuleduPageAccueil->abePageAccueilGetMenu(), SIGNAL(btnBoxTriggered()), this, SLOT(slotAskLanceur()),Qt::UniqueConnection);

    m_messageAide = trUtf8("Clique sur une des zones de lancement des exercices.");
    m_demoMessageBox = new AbulEduMessageBoxV1(trUtf8("On y va ?"),m_messageAide,false,m_abuleduPageAccueil);
    m_demoMessageBox->abeSetModeEnum(AbulEduMessageBoxV1::abeNoButton);
    m_demoMessageBox->setWink();
    //    m_demoMessageBox->hideCloseButton();
    m_demoMessageBox->hide();
    m_demoTimeLine = new QTimeLine(6000,this);
    connect(m_demoTimeLine, SIGNAL(finished()),this, SLOT(slotInterfaceEndDemo()),Qt::UniqueConnection);

    QFile* fichierConf = new QFile(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf");
    if (!fichierConf->exists()){
        if(m_localDebug) qDebug()<<trUtf8("Fichier config NON trouvé");
    }
    else {
        if(m_localDebug) qDebug() << trUtf8("Fichier config trouvé");
    }

    ui->btnInitialise->setGeometry(10,10,170,30);
    ui->btnInitialise->hide();

//    m_exerciceNames.insert("tableM",tru)

    m_editeur = ui->widget;

    qApp->setProperty("VerrouNombres",true);

    setTitle(abeApp->getAbeNetworkAccessManager()->abeSSOAuthenticationStatus());
    // On est prêt à démarrer
    createStateMachine();
    m_leterrierStateMachine.start();
    ui->exercicePage->abeWidgetSetBackgroundPixmap(":/calculment/backgrounds/backgroundInterface");
}

interface::~interface()
{
    delete ui;
}

void interface::resizeEvent(QResizeEvent *)
{
    m_abuleduPageAccueil->setDimensionsWidgets();
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

void interface::createStateMachine()
{
    if (m_localDebug) qDebug()<< __FUNCTION__;
    // Les états
    m_initialState      = new QState();
    m_initialState      ->setObjectName("initialState");
    m_globalState       = new QState();
    m_globalState       ->setObjectName("globalState");
    m_homeState         = new QState(m_globalState);
    m_homeState         ->setObjectName("homeState");
    m_exerciseState     = new QState(m_globalState);
    m_exerciseState     ->setObjectName("exerciseState");
    m_editorState       = new QState(m_globalState);
    m_editorState       ->setObjectName("editorState");
    m_historyState      = new QHistoryState(QHistoryState::DeepHistory, m_globalState);
    m_historyState->setDefaultState(m_homeState);
    m_historyState      ->setObjectName("historyState");
    m_boxFileManagerState   = new QState();
    m_boxFileManagerState   ->setObjectName("boxFileManagerState");
    m_finalState        = new QFinalState();
    m_finalState        ->setObjectName("finalState");
    m_leterrierStateMachine.addState        (m_initialState);
    m_leterrierStateMachine.setInitialState (m_initialState);
    m_leterrierStateMachine.addState        (m_globalState);

    m_leterrierStateMachine.addState        (m_boxFileManagerState);
    m_leterrierStateMachine.addState        (m_finalState);

    m_globalState->setInitialState(m_homeState);

    // Les transitions
    m_initialState->addTransition(m_initialState, SIGNAL(entered()), m_globalState);

    m_globalState->addTransition(ui->actionQuitter, SIGNAL(triggered()), m_finalState);
    m_globalState->addTransition(m_abuleduPageAccueil->abePageAccueilGetMenu(), SIGNAL(btnQuitterTriggered()), m_finalState);

    m_homeState->addTransition(ui->actionAfficher_l_diteur, SIGNAL(triggered()), m_editorState);
    m_homeState->addTransition(m_abuleduPageAccueil->abePageAccueilGetMenu(), SIGNAL(btnBoxTriggered()), m_boxFileManagerState);
    m_homeState->addTransition(m_abuleduPageAccueil->abePageAccueilGetMenu(), SIGNAL(btnQuitterTriggered()), m_finalState);
    m_homeState->addTransition(m_abuleduPageAccueil->abePageAccueilGetBtnRevenirEditeur(), SIGNAL(clicked()), m_editorState);
    m_editorState->addTransition(m_editeur, SIGNAL(signalEditeurExited()), m_homeState);

    /* Les transitions liées à l'éditeur seront installées lors de la création de celui-ci */

    m_exerciseState->addTransition(this, SIGNAL(signalAbeLTMWSMexerciseClosed()), m_homeState);
    // Transitions gardées
    LeterrierStringTransition *toExerciseState = new LeterrierStringTransition("launchExercise");
    toExerciseState->setTargetState(m_exerciseState);
    m_homeState->addTransition(toExerciseState);
    LeterrierStringTransition *toAbeBoxFileManager = new LeterrierStringTransition("toAbeBoxFileManager");
    toAbeBoxFileManager->setTargetState(m_boxFileManagerState);
    m_homeState->addTransition(toAbeBoxFileManager);

    // Les connexions
    connect(m_initialState,     SIGNAL(entered()),  this, SLOT(slotInterfaceInitialStateEntered()),  Qt::UniqueConnection);
    connect(m_initialState,     SIGNAL(exited()),   this, SLOT(slotInterfaceInitialStateExited()),   Qt::UniqueConnection);
    connect(m_globalState,      SIGNAL(entered()),  this, SLOT(slotInterfaceGlobalStateEntered()),   Qt::UniqueConnection);
    connect(m_globalState,      SIGNAL(exited()),   this, SLOT(slotInterfaceGlobalStateExited()),    Qt::UniqueConnection);
    connect(m_homeState,        SIGNAL(entered()),  this, SLOT(slotInterfaceHomeStateEntered()),     Qt::UniqueConnection);
    connect(m_homeState,        SIGNAL(exited()),   this, SLOT(slotInterfaceHomeStateExited()),      Qt::UniqueConnection);
    connect(m_exerciseState,    SIGNAL(entered()),  this, SLOT(slotInterfaceExerciseStateEntered()), Qt::UniqueConnection);
    connect(m_exerciseState,    SIGNAL(exited()),   this, SLOT(slotInterfaceExerciseStateExited()),  Qt::UniqueConnection);
    connect(m_editorState,      SIGNAL(entered()),  this, SLOT(slotInterfaceEditorStateEntered()),   Qt::UniqueConnection);
    connect(m_editorState,      SIGNAL(exited()),   this, SLOT(slotInterfaceEditorStateExited()),    Qt::UniqueConnection);
    connect(m_finalState,       SIGNAL(entered()),  this, SLOT(slotInterfaceFinalStateEntered()),    Qt::UniqueConnection);
    connect(m_finalState,       SIGNAL(exited()),   this, SLOT(slotInterfaceFinalStateExited()),     Qt::UniqueConnection);

    //! Les assignProperty

    // globalState

    // homeState
    m_homeState->assignProperty(ui->menuExercices,                  "enabled", true);
    m_homeState->assignProperty(m_abuleduPageAccueil->abePageAccueilGetMenu(), "visible", true);

    // editorState
    m_editorState->assignProperty(ui->menuExercices,                "enabled", false);
    m_editorState->assignProperty(ui->actionAfficher_l_diteur,    "enabled", false);

    // exerciseState
    m_exerciseState->assignProperty(ui->actionAfficher_l_diteur,  "enabled", false);
    m_exerciseState->assignProperty(m_abuleduPageAccueil->abePageAccueilGetMenu(), "visible", false);
    m_exerciseState->assignProperty(m_abuleduPageAccueil->abePageAccueilGetBtnRevenirEditeur(), "visible", false);
    m_exerciseState->assignProperty(ui->menuExercices,              "enabled", false);

    // waitForAbeState

    // FinalState
}

void interface::slotSessionAuthenticated(bool enable)
{
    if(m_localDebug) qDebug()<<__FUNCTION__<<enable;
    if(enable)
        abeApp->getAbeNetworkAccessManager()->abeSSOLogin();

    connect(abeApp->getAbeNetworkAccessManager(), SIGNAL(ssoAuthStatus(int)), this,SLOT(setTitle(int)));
}

void interface::slotInterfaceLaunchExercise(int number,QString name)
{
    qDebug()<<m_leterrierStateMachine.configuration().toList();
    if (m_localDebug){
        ABULEDU_LOG_DEBUG()<<number<<" ------ "<< __PRETTY_FUNCTION__<<" -> "<<name;
    }
    /*" Tables de multiplication"
    " Tables d'addition"
    "Soustractions"
    "Compléments"
     "Multiples"
    "Ordres de grandeur"
    "Divisions"
    "Editeur"
    "Multiplications"
    "Additions" */

    if (name == "Editeur") {
        ui->actionAfficher_l_diteur->trigger();
    }

    /** @todo Gérer les traductions */
    else if (name.simplified().left(6) == "Tables" || name.simplified().left(6) == "Ordres" || name.simplified() == "Compléments" || name.simplified() == "Multiples"){
        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__;
        ui->stackedWidget->setCurrentWidget(ui->transitionalPage);
        InterfaceCompetence* inter = new InterfaceCompetence(m_exerciceNames.key(name.simplified()),ui->transitionalPage);
        connect(inter,SIGNAL(signalInterfaceCompetenceClose()),this, SLOT(slotInterfaceShowMainPage()),Qt::UniqueConnection);
        inter->show();
    }
    /* ça c'est la bonne façon de faire */
    else{
        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__;
        ui->stackedWidget->setCurrentWidget(ui->exercicePage);
        ExerciceOperation* ex = new ExerciceOperation(m_exerciceNames.key(name.simplified()),ui->exercicePage);
        connect(ex,SIGNAL(signalExerciseExited()),this, SLOT(slotInterfaceShowMainPage()),Qt::UniqueConnection);
    }

    //    else if (*m_action=="lanceur") {
    //        if (abeApp->getAbeNetworkAccessManager()->abeSSOAuthenticationStatus() != 1)
    //        {

    //            abeApp->getAbeNetworkAccessManager()->abeOnLoginSuccessGoto(this,SLOT(slotMontreLanceur()));
    //            abeApp->getAbeNetworkAccessManager()->abeOnLoginFailureGoto(this,SLOT(slotMontreErreurId()));
    //            abeApp->getAbeNetworkAccessManager()->abeSSOLogin();
    //        }
    //        else
    //        {
    //            slotMontreLanceur();
    //        }
    //    }

    //    else if (*m_action == "maisonDesNombres") {
    //        ExerciceMaisonNombres* maisonNombres = new ExerciceMaisonNombres(*m_action,0,m_val);
    //        maisonNombres->show();
    //    }
//    else {
//        ui->stackedWidget->setCurrentWidget(ui->exercicePage);
//        exercice* ex = new exercice(m_exerciceNames.key(name.simplified()),ui->widgetContainer,0);
//        ex->show();
//    }

}

void interface::slotInterfaceBackFromExercise()
{
    if (m_localDebug){
        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__;
    }
}

void interface::slotInterfaceDemo()
{
    if (m_localDebug){
//        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__;
    }
    if (m_isDemoAvailable == false || ui->stackedWidget->currentWidget() != ui->mainPage || m_demoTimeLine->state() == QTimeLine::Running){
        return;
    }
    float ratio = abeApp->getAbeApplicationDecorRatio();
    if (m_localDebug) qDebug()<<" ++++++++ "<< __FILE__ <<  __LINE__ << __FUNCTION__;
    m_demoMessageBox->setGeometry(10*ratio,800*ratio,400*ratio,200*ratio);
    m_demoMessageBox->show();
    m_abuleduPageAccueil->abePageAccueilMontreBulles(true,true);
    foreach(AbulEduZoneV1* zone, m_abuleduPageAccueil->abePageAccueilGetZones()){
        zone->abeZoneDrawRect(true,QColor("#dcdcdc"),4);
    }
    /** @todo réactiver pour le passage en tests et en prod
    m_demoTimeLine->start(); */
}

void interface::slotInterfaceEndDemo()
{
    if(sender()->objectName() == "demoActivityFilter"){
        if(m_demoTimeLine->state() != QTimeLine::Running){
            return;
        }
    }
    m_demoTimeLine->stop();
    if(m_demoMessageBox){
            m_demoMessageBox->setVisible(false);
        }
    m_abuleduPageAccueil->abePageAccueilMontreBulles(false,true);
    foreach(AbulEduZoneV1* zone, m_abuleduPageAccueil->abePageAccueilGetZones()){
        zone->abeZoneDrawRect(false);
    }
}

void interface::mousePressEvent(QMouseEvent *event)
{
    if (m_localDebug){
        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__;
    }
    if(event->button() == Qt::RightButton)
    {
        QKeyEvent* pressSpace = new QKeyEvent(QEvent::KeyPress,Qt::Key_Space,Qt::NoModifier);
        QApplication::sendEvent(m_abuleduPageAccueil,pressSpace);
    }
}

void interface::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_localDebug){
        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__;
    }
    if(event->button() == Qt::RightButton)
    {
        QKeyEvent* releaseSpace = new QKeyEvent(QEvent::KeyRelease,Qt::Key_Space,Qt::NoModifier);
        QApplication::sendEvent(m_abuleduPageAccueil,releaseSpace);
    }
}

void interface::slotInterfaceInitialStateEntered()
{
    if (m_localDebug){
        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__;
    }
    m_isDemoAvailable = true;
    // On va mettre à jour les textes dans les bulles car ils sont initialisés après la page d'accueil
    QList<AbulEduZoneV1 *> listeZone = m_abuleduPageAccueil->abePageAccueilGetZones();

    // On crée les entrées du menu exercices
    for(int i = 0; i < listeZone.count(); i++)
    {
        QAction *actionExercice = new QAction(listeZone[i]->abeZoneGetBulle()->abeBulleGetText(), this);
        actionExercice->setObjectName(QString::number(i));
        /* J'ai utilisé le debug ci-dessous et l'ancien code d'appel des boutonsPolygones pour peupler ma QMap */
//        qDebug()<<i<<" ---> "<<listeZone[i]->abeZoneGetBulle()->abeBulleGetText();
        actionExercice->setShortcut(QKeySequence("Ctrl+"+QString::number(i+1)));
        actionExercice->setEnabled(false);
        connect(actionExercice, SIGNAL(triggered()), this, SLOT(slotInterfaceLaunchExercise()), Qt::UniqueConnection);
        ui->menuExercices->addAction(actionExercice);
    }
    m_exerciceNames.insert("tableM","Tables de multiplication");
    m_exerciceNames.insert("addition","Additions");
    m_exerciceNames.insert("multiplication","Multiplications");
    m_exerciceNames.insert("editeur","Editeur");
    m_exerciceNames.insert("complementA","Compléments");
    m_exerciceNames.insert("complementM","Multiples");
    m_exerciceNames.insert("soustraction","Soustractions");
    m_exerciceNames.insert("tableA","Tables d'addition");
    m_exerciceNames.insert("OdGrandeur","Ordres de grandeur");
    m_exerciceNames.insert("lanceur","Lanceur");

    /* Pas de module dans ce logiciel, les zones sensibles sont activées */
}

void interface::slotInterfaceInitialStateExited()
{
    if (m_localDebug){
        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__;
    }

}

void interface::slotInterfaceGlobalStateEntered()
{
    if (m_localDebug){
        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__;
    }

}

void interface::slotInterfaceGlobalStateExited()
{
    if (m_localDebug){
        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__;
    }

}

void interface::slotInterfaceHomeStateEntered()
{
    if (m_localDebug){
        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__;
    }
    ui->stackedWidget->setCurrentWidget(ui->mainPage);
    /* Si on est entrain d'éditer un module, on affiche le bouton Revenir Editeur */
    m_homeState->assignProperty(m_abuleduPageAccueil->abePageAccueilGetBtnRevenirEditeur(), "visible", m_isEditorRunning);
    m_homeState->assignProperty(ui->actionAfficher_l_diteur,                                "enabled", !m_isEditorRunning);
    m_isDemoAvailable = true;
}

void interface::slotInterfaceHomeStateExited()
{
    if (m_localDebug){
        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__;
    }
    m_isDemoAvailable = false;
}

void interface::slotInterfaceExerciseStateEntered()
{
    if (m_localDebug){
        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__;
    }

}

void interface::slotInterfaceExerciseStateExited()
{
    if (m_localDebug){
        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__;
    }

}

void interface::slotInterfaceEditorStateEntered()
{
    ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__;
    if (m_localDebug){
    }
    ui->stackedWidget->setCurrentWidget(ui->editorPage);
}

void interface::slotInterfaceEditorStateExited()
{
        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__;
        if (m_localDebug){
    }
    m_isEditorRunning = false;
    ui->stackedWidget->setCurrentWidget(ui->mainPage);
}

void interface::slotInterfaceFinalStateEntered()
{
    if (m_localDebug){
        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__;
    }
    close();
}

void interface::slotInterfaceFinalStateExited()
{
    if (m_localDebug){
        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__;
    }
}

void interface::changelangue(QString langue)
{
    if(m_localDebug) qDebug()<<"interface::changelangue(1)";
    qApp->setProperty("langageUtilise",langue);
    myappTranslator.load("leterrier-calculment_"+langue, "lang");

    qApp->installTranslator(&myappTranslator);
    interface* nouvelleInterface = new interface();
    nouvelleInterface->showMaximized();
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
    qDebug()<<"je passe là ";

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

void interface::slotInterfaceShowMainPage()
{
    ui->stackedWidget->setCurrentWidget(ui->mainPage);
}

void interface::slotInterfaceShowAboutPage()
{
    ui->stackedWidget->setCurrentWidget(ui->aboutPage);
}

void interface::setTitle(int authStatus)
{
    if(m_localDebug) qDebug()<<__FUNCTION__<<authStatus;
    QString title = abeApp->getAbeApplicationLongName();
    if (authStatus == 1)
    {
        if(abeApp->getAbeIdentite()->abeGetPreNom().isEmpty() && abeApp->getAbeIdentite()->abeGetNom().isEmpty()){
            title.append(" -- "+abeApp->getAbeNetworkAccessManager()->abeGetSSOLogin());
        }
        else{
            title.append(" -- "+abeApp->getAbeIdentite()->abeGetPreNom()+" "+abeApp->getAbeIdentite()->abeGetNom());
        }
    }
    setWindowTitle(title);
}

void interface::slotAskLanceur()
{
        if (abeApp->getAbeNetworkAccessManager()->abeSSOAuthenticationStatus() != 1)
        {

            abeApp->getAbeNetworkAccessManager()->abeOnLoginSuccessGoto(this,SLOT(slotMontreLanceur()));
            abeApp->getAbeNetworkAccessManager()->abeOnLoginFailureGoto(this,SLOT(slotMontreErreurId()));
            abeApp->getAbeNetworkAccessManager()->abeSSOLogin();
        }
        else
        {
            slotMontreLanceur();
        }
}

void interface::slotMontreLanceur()
{
    AbuleduLanceurV1* lanceur = new AbuleduLanceurV1(abeApp->getAbeIdentite());
    lanceur->show();
}

void interface::slotMontreErreurId()
{
    AbulEduMessageBoxV1* msgError = new AbulEduMessageBoxV1(trUtf8("Problème !"),trUtf8("Accès impossible au lanceur d'activité sans identification correcte"));
    msgError->show();
}
