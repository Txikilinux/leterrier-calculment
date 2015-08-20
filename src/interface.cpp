/** Implémentation de l'interface générale du logiciel, qui permet de lancer
  * les exercices soit par zone cliquable, soit par menu
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/LeTerrierExercice
  * @author 2010-2015 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
  * @author 2010-2013 Eric Seigne <eric.seigne@ryxeo.com>
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

#include "interface.h"
#include "ui_interface.h"



interfaceClass::interfaceClass(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::interfaceClass)
{
    m_localDebug = false;
    m_isEditorRunning = false;
    m_isChangingUser = false;
//    m_abuleduFile.clear();
    setAttribute(Qt::WA_DeleteOnClose);

    //Langue
    //m_locale = QLocale::system().name().section('_', 0, 0);
    m_locale = qApp->property("langageUtilise").toString();
    if(m_localDebug) qDebug()<<"interfaceClass::constructeur (1) - "<<m_locale;
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
    ui->abeLauncher->abeLanceurSetInterface(this);
#ifndef __ABULEDUTABLETTEV1__MODE__
    ui->aboutPage->abeAproposSetMainWindow(this);
#endif
    connect(ui->aboutPage, SIGNAL(signalAbeAproposSetindex(int)),this,SLOT(slotInterfaceShowAboutPage()), Qt::UniqueConnection);
    connect(ui->aboutPage, SIGNAL(signalAbeAproposBtnCloseClicked()), this, SLOT(slotInterfaceShowMainPage()),Qt::UniqueConnection);

    m_signalMapper = new QSignalMapper(this);
    connect(m_signalMapper, SIGNAL(mapped(QString)), this, SLOT(changelangue(QString)) );
    /* il faut gérer les traductions par rapport au fait que c'est lu dans le settings.conf de la page d'accueil */
    ui->menuLangues->setEnabled(false);

//    creeMenuLangue();

    m_activityFilter = new ActivityFilter(abeApp);
    m_activityFilter->setInterval(6000);
    m_activityFilter->setObjectName("demoActivityFilter");
    abeApp->installEventFilter(m_activityFilter);
    QObject::connect(m_activityFilter, SIGNAL(userActive()),   this,  SLOT(slotInterfaceEndDemo()), Qt::UniqueConnection);
    QObject::connect(m_activityFilter, SIGNAL(userInactive()),this,  SLOT(slotInterfaceDemo()), Qt::UniqueConnection);

    /* Création de la page d'accueil et insertion dans la stackedWidget */
    m_abuleduPageAccueil = new AbulEduPageAccueilV1(ui->mainPage);
    connect(m_abuleduPageAccueil, SIGNAL(boutonPressed(int,QString)), this, SLOT(slotInterfaceLaunchExercise(int,QString)), Qt::UniqueConnection);
    connect(m_abuleduPageAccueil->abePageAccueilGetMenu(),SIGNAL(btnAideTriggered()),this, SLOT(slotInterfaceShowAboutPage()), Qt::UniqueConnection);
    connect(m_abuleduPageAccueil, SIGNAL(signalAbePageAccueilMousePressed()), this, SLOT(slotInterfaceDemo()),Qt::UniqueConnection);

    m_messageAide = trUtf8("Clique sur une des zones de lancement des exercices.");
    m_demoMessageBox = new AbulEduMessageBoxV1(trUtf8("On y va ?"),m_messageAide,false,m_abuleduPageAccueil);
    m_demoMessageBox->abeSetModeEnum(AbulEduMessageBoxV1::abeNoButton);
    m_demoMessageBox->setWink();
    //    m_demoMessageBox->hideCloseButton();
    m_demoMessageBox->hide();
    m_demoTimeLine = new QTimeLine(6000,this);
    connect(m_demoTimeLine, SIGNAL(finished()),this, SLOT(slotInterfaceEndDemo()),Qt::UniqueConnection);

    ui->btnInitialise->setGeometry(10,10,170,30);
    ui->btnInitialise->hide();

//    m_exerciceNames.insert("tableM",tru)

    m_editeur = ui->widget;
    connect(m_editeur,SIGNAL(signalEditeurSaved()),this, SLOT(slotSettingsSaved()),Qt::UniqueConnection);

    qApp->setProperty("VerrouNombres",true);
    qApp->setProperty("numericPad",false);

    setTitle(abeApp->getAbeNetworkAccessManager()->abeSSOAuthenticationStatus());
    // On est prêt à démarrer
    createStateMachine();
    m_leterrierStateMachine.start();
//    ui->exercicePage->abePixmapWidgetSetPixmap(":/calculment/backgrounds/backgroundInterface");
//    ui->exercicePage->abePixmapWidgetSetPixmap(QString());

}

interfaceClass::~interfaceClass()
{
    delete ui;
}

void interfaceClass::resizeEvent(QResizeEvent *)
{
    m_abuleduPageAccueil->setDimensionsWidgets();
}

void interfaceClass::creeMenuLangue()
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__;

    QString nomFichierConf = "./conf/abuledulangselector.conf";
    if (!QFile(nomFichierConf).exists())
    {
        qDebug()<<trUtf8("Fichier de configuration des langues non trouvé");
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
     if(configLang.allKeys().size() > 0){
         ui->menuLangues->setEnabled(true);
     }
}
QMap<QString, QString> interfaceClass::abeInterfaceGetExerciceNames() const
{
    return m_exerciceNames;
}

void interfaceClass::createStateMachine()
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__;

    /* Les états */
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
//    m_boxFileManagerState   = new QState();
//    m_boxFileManagerState   ->setObjectName("boxFileManagerState");
    m_finalState        = new QFinalState();
    m_finalState        ->setObjectName("finalState");
    m_leterrierStateMachine.addState        (m_initialState);
    m_leterrierStateMachine.setInitialState (m_initialState);
    m_leterrierStateMachine.addState        (m_globalState);

//    m_leterrierStateMachine.addState        (m_boxFileManagerState);
    m_leterrierStateMachine.addState        (m_finalState);

    m_globalState->setInitialState(m_homeState);

    /* Les transitions */
    m_initialState->addTransition(m_initialState, SIGNAL(entered()), m_globalState);

    m_globalState->addTransition(ui->actionQuitter, SIGNAL(triggered()), m_finalState);
    m_globalState->addTransition(m_abuleduPageAccueil->abePageAccueilGetMenu(), SIGNAL(btnQuitterTriggered()), m_finalState);

    m_homeState->addTransition(ui->actionAfficher_l_diteur, SIGNAL(triggered()), m_editorState);
//    m_homeState->addTransition(m_abuleduPageAccueil->abePageAccueilGetMenu(), SIGNAL(btnBoxTriggered()), m_boxFileManagerState);
    m_homeState->addTransition(m_abuleduPageAccueil->abePageAccueilGetMenu(), SIGNAL(btnQuitterTriggered()), m_finalState);
    m_homeState->addTransition(m_abuleduPageAccueil->abePageAccueilGetBtnRevenirEditeur(), SIGNAL(clicked()), m_editorState);
    m_editorState->addTransition(m_editeur, SIGNAL(signalEditeurExited()), m_homeState);

    /* Les transitions liées à l'éditeur seront installées lors de la création de celui-ci */

    m_exerciseState->addTransition(this, SIGNAL(signalAbeLTMWSMexerciseClosed()), m_homeState);
    /* Transitions gardées */
    LeterrierStringTransition *toExerciseState = new LeterrierStringTransition("launchExercise");
    toExerciseState->setTargetState(m_exerciseState);
    m_homeState->addTransition(toExerciseState);
//    LeterrierStringTransition *toAbeBoxFileManager = new LeterrierStringTransition("toAbeBoxFileManager");
//    toAbeBoxFileManager->setTargetState(m_boxFileManagerState);
//    m_homeState->addTransition(toAbeBoxFileManager);

    /* Les connexions */
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

    /* Les assignProperty */

    /*     pour globalState */

    /*     pour homeState */
    m_homeState->assignProperty(ui->menuBar,                  "enabled", true);
    m_homeState->assignProperty(m_abuleduPageAccueil->abePageAccueilGetMenu(), "visible", true);
    /* Je ne me tracasse pas pour la gestion intelligente des apparitions de ce bouton tant qu'il n'y a pas de bouton Essayer dans l'éditeur */
    m_homeState->assignProperty(m_abuleduPageAccueil->abePageAccueilGetBtnRevenirEditeur(), "visible", false);

    /*     pour editorState */
    m_editorState->assignProperty(ui->menuBar,                "enabled", false);
    m_editorState->assignProperty(ui->actionAfficher_l_diteur,    "enabled", false);

    /*     pour exerciseState */
    m_exerciseState->assignProperty(ui->actionAfficher_l_diteur,  "enabled", false);
    m_exerciseState->assignProperty(m_abuleduPageAccueil->abePageAccueilGetMenu(), "visible", false);
    m_exerciseState->assignProperty(m_abuleduPageAccueil->abePageAccueilGetBtnRevenirEditeur(), "visible", false);
    m_exerciseState->assignProperty(ui->menuBar,              "enabled", false);

    /*     pour waitForAbeState */

    /*     pour FinalState */
}

void interfaceClass::slotSessionAuthenticated(bool enable)
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__<<enable;

    if(enable){
        abeApp->getAbeNetworkAccessManager()->abeSSOLogin();
        abeApp->getAbeNetworkAccessManager()->abeOnLoginSuccessGoto(this,SLOT(slotSetAbeBoxPersoSettings()));
        abeApp->getAbeNetworkAccessManager()->abeOnLoginFailureGoto(this,SLOT(slotSetPCSettings()));
    }
    else{
        slotSetPCSettings();
    }

    connect(abeApp->getAbeNetworkAccessManager(), SIGNAL(ssoAuthStatus(int)), this,SLOT(setTitle(int)));
}

void interfaceClass::slotInterfaceLaunchExercise(int number, QString name, int level)
{
    slotInterfaceShowMainPage();
    /* Dans le cas d'un appel par le menuExercice ou une zone de l'interface, je mets la valeur number à -1 */
    if(sender()){
        if(sender()->inherits("QAction")){
            name = sender()->objectName();
            number = -1;
        }
        if(sender()->inherits("AbulEduPageAccueilV1")){
            number = -1;
        }
    }
    ABULEDU_LOG_DEBUG()<<number<<" ------ "<< __PRETTY_FUNCTION__<<" -> "<<name;
    /* Prise en compte d'un changement du nom de l'exercice entre les versions */
    if(name == "Compléments additifs"){
        name = "Compléments";
    }
    if (m_localDebug){
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
    else if (m_exerciceNames.key(name.simplified()) == "maisonDesNombres" && !m_leterrierStateMachine.configuration().toList().contains(m_exerciseState)){
        m_leterrierStateMachine.postEvent(new LeterrierStringEvent("launchExercise"));
        ExerciceMaisonNombres* ex = new ExerciceMaisonNombres(m_exerciceNames.key(name.simplified()),m_abuleduPageAccueil,number,level);
        ex->setAbeSettingsDirectory(m_editeur->abeEditeurGetAbulEduFile()->abeFileGetDirectoryTemp().absolutePath());
        connect(ex,SIGNAL(signalExerciseExited()),this, SLOT(slotInterfaceBackFromExercise()),Qt::UniqueConnection);
    }
    /** @todo Gérer les traductions */
    else if (name.simplified().left(6) == "Tables" || name.simplified().left(6) == "Ordres" || name.simplified() == "Compléments" || name.simplified() == "Multiples"){
        m_leterrierStateMachine.postEvent(new LeterrierStringEvent("launchExercise"));
        if(m_exerciceNames.values().contains(name.simplified())&& !m_leterrierStateMachine.configuration().toList().contains(m_exerciseState)){
            ExerciceOperation* ex = new ExerciceOperation(m_exerciceNames.key(name.simplified()),m_abuleduPageAccueil,number,level);
            ex->setAbeSettingsDirectory(m_editeur->abeEditeurGetAbulEduFile()->abeFileGetDirectoryTemp().absolutePath());
            connect(ex,SIGNAL(signalExerciseExited()),this, SLOT(slotInterfaceBackFromExercise()),Qt::UniqueConnection);
        }
        else if(m_exerciceNames.keys().contains(name.simplified())&& !m_leterrierStateMachine.configuration().toList().contains(m_exerciseState)){
            ExerciceOperation* ex = new ExerciceOperation(name.simplified(),m_abuleduPageAccueil,number,level);
            ex->setAbeSettingsDirectory(m_editeur->abeEditeurGetAbulEduFile()->abeFileGetDirectoryTemp().absolutePath());
            connect(ex,SIGNAL(signalExerciseExited()),this, SLOT(slotInterfaceBackFromExercise()),Qt::UniqueConnection);
        }
        else {
            qDebug()<<" Incohérence au lancement";
        }
    }

    /* ça c'est la bonne façon de faire */
    else{
        qDebug()<<name;
        m_leterrierStateMachine.postEvent(new LeterrierStringEvent("launchExercise"));
        if(m_exerciceNames.values().contains(name.simplified()) && !m_leterrierStateMachine.configuration().toList().contains(m_exerciseState)){
            ExerciceOperation* ex = new ExerciceOperation(m_exerciceNames.key(name.simplified()),m_abuleduPageAccueil,0,level);
            ex->setAbeSettingsDirectory(m_editeur->abeEditeurGetAbulEduFile()->abeFileGetDirectoryTemp().absolutePath());
            connect(ex,SIGNAL(signalExerciseExited()),this, SLOT(slotInterfaceBackFromExercise()),Qt::UniqueConnection);
        }
        else if(m_exerciceNames.keys().contains(name.simplified())&& !m_leterrierStateMachine.configuration().toList().contains(m_exerciseState)){
            ExerciceOperation* ex = new ExerciceOperation(name.simplified(),m_abuleduPageAccueil,0,level);
            ex->setAbeSettingsDirectory(m_editeur->abeEditeurGetAbulEduFile()->abeFileGetDirectoryTemp().absolutePath());
            connect(ex,SIGNAL(signalExerciseExited()),this, SLOT(slotInterfaceBackFromExercise()),Qt::UniqueConnection);
        }
        /* Gros cas particulier mais je n'ai pas trouvé pour l'instant de façon de faire élégante : c'est pour le cas de l'appel des ordres de grandeur par AbuleduLanceurV1 */
        else if(name.left(3) == "OdG" && !m_leterrierStateMachine.configuration().toList().contains(m_exerciseState)){
            ExerciceOperation* ex = new ExerciceOperation(name.simplified(),m_abuleduPageAccueil,0,level);
            ex->setAbeSettingsDirectory(m_editeur->abeEditeurGetAbulEduFile()->abeFileGetDirectoryTemp().absolutePath());
            connect(ex,SIGNAL(signalExerciseExited()),this, SLOT(slotInterfaceBackFromExercise()),Qt::UniqueConnection);
        }
        else {
            qDebug()<<" Incohérence au lancement";
        }
    }
}

void interfaceClass::slotInterfaceBackFromExercise()
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__;

    slotInterfaceShowMainPage();
    emit signalAbeLTMWSMexerciseClosed();
}

void interfaceClass::slotInterfaceDemo()
{
    if (m_localDebug){
//        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__;
    }
    if (m_isDemoAvailable == false || ui->stackedWidget->currentWidget() != ui->mainPage || m_demoTimeLine->state() == QTimeLine::Running){
        return;
    }
    float ratio = abeApp->getAbeApplicationDecorRatio();
    if (m_localDebug) qDebug()<<" ++++++++ "<< __FILE__ <<  __LINE__ << __FUNCTION__;
    QKeyEvent* pressSpace = new QKeyEvent(QEvent::KeyPress,Qt::Key_Space,Qt::NoModifier);
    QApplication::sendEvent(m_abuleduPageAccueil,pressSpace);
    delete pressSpace;
    m_demoMessageBox->setGeometry(10*ratio,800*ratio,400*ratio,200*ratio);
    m_demoMessageBox->show();
    m_abuleduPageAccueil->abePageAccueilMontreBulles(true,true);
    foreach(AbulEduZoneV1* zone, m_abuleduPageAccueil->abePageAccueilGetZones()){
        zone->abeZoneDrawRect(true,QColor("#dcdcdc"),4);
    }
    m_demoTimeLine->start();
}

void interfaceClass::slotInterfaceEndDemo()
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
    QKeyEvent* releaseSpace = new QKeyEvent(QEvent::KeyRelease,Qt::Key_Space,Qt::NoModifier);
    QApplication::sendEvent(m_abuleduPageAccueil,releaseSpace);
    delete releaseSpace;
}

void interfaceClass::mousePressEvent(QMouseEvent *event)
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__;

    if(event->button() == Qt::RightButton)
    {
        QKeyEvent* pressSpace = new QKeyEvent(QEvent::KeyPress,Qt::Key_Space,Qt::NoModifier);
        QApplication::sendEvent(m_abuleduPageAccueil,pressSpace);
    }
}

void interfaceClass::mouseReleaseEvent(QMouseEvent *event)
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__;

    if(event->button() == Qt::RightButton)
    {
        QKeyEvent* releaseSpace = new QKeyEvent(QEvent::KeyRelease,Qt::Key_Space,Qt::NoModifier);
        QApplication::sendEvent(m_abuleduPageAccueil,releaseSpace);
    }
}

void interfaceClass::slotInterfaceInitialStateEntered()
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__;
    m_exerciceNames.insert("tableM","Tables de multiplication");
    m_exerciceNames.insert("addition","Additions");
    m_exerciceNames.insert("multiplication","Multiplications");
    m_exerciceNames.insert("complementA","Compléments");
    m_exerciceNames.insert("complementM","Multiples");
    m_exerciceNames.insert("soustraction","Soustractions");
    m_exerciceNames.insert("tableA","Tables d'addition");
    m_exerciceNames.insert("OdGrandeur","Ordres de grandeur");
    m_exerciceNames.insert("division", "Divisions");
    m_exerciceNames.insert("maisonDesNombres","Maison des nombres");

    m_isDemoAvailable = true;
    // On va mettre à jour les textes dans les bulles car ils sont initialisés après la page d'accueil
    QList<AbulEduZoneV1 *> listeZone = m_abuleduPageAccueil->abePageAccueilGetZones();
    QStringList operationsSimples;
    operationsSimples << m_exerciceNames.value("addition").simplified() << m_exerciceNames.value("soustraction").simplified() << m_exerciceNames.value("multiplication").simplified() << m_exerciceNames.value("division").simplified();

    // On crée les entrées du menu exercices
    int ctrl = 0;
    for(int i = 0; i < listeZone.count(); i++)
    {
        if(operationsSimples.contains(listeZone[i]->abeZoneGetBulle()->abeBulleGetText().simplified())){
            QAction *actionExercice = new QAction(listeZone[i]->abeZoneGetBulle()->abeBulleGetText().simplified(), this);
            actionExercice->setObjectName(listeZone[i]->abeZoneGetBulle()->abeBulleGetText().simplified());
            /* J'ai utilisé le debug ci-dessous et l'ancien code d'appel des boutonsPolygones pour peupler ma QMap */
    //        qDebug()<<i<<" ---> "<<listeZone[i]->abeZoneGetBulle()->abeBulleGetText();
            actionExercice->setShortcut(QKeySequence("Ctrl+"+QString::number(ctrl)));
            connect(actionExercice, SIGNAL(triggered()), this, SLOT(slotInterfaceLaunchExercise()), Qt::UniqueConnection);
            ui->menuExercices->addAction(actionExercice);
            ctrl++;
        }
    }
    for(int i = 0; i < listeZone.count(); i++)
    {
        if(!operationsSimples.contains(listeZone[i]->abeZoneGetBulle()->abeBulleGetText().simplified())){
            QAction *actionExercice = new QAction(listeZone[i]->abeZoneGetBulle()->abeBulleGetText().simplified(), this);
            actionExercice->setObjectName(listeZone[i]->abeZoneGetBulle()->abeBulleGetText().simplified());
            /* J'ai utilisé le debug ci-dessous et l'ancien code d'appel des boutonsPolygones pour peupler ma QMap */
    //        qDebug()<<i<<" ---> "<<listeZone[i]->abeZoneGetBulle()->abeBulleGetText();
            actionExercice->setShortcut(QKeySequence("Ctrl+"+QString::number(ctrl)));
            connect(actionExercice, SIGNAL(triggered()), this, SLOT(slotInterfaceLaunchExercise()), Qt::UniqueConnection);
            ui->menuExercices->addAction(actionExercice);
            ctrl++;
        }
    }

    /* Pas de module dans ce logiciel, les zones sensibles sont activées */
}

void interfaceClass::slotInterfaceInitialStateExited()
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__;
}

void interfaceClass::slotInterfaceGlobalStateEntered()
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__;
}

void interfaceClass::slotInterfaceGlobalStateExited()
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__;
}

void interfaceClass::slotInterfaceHomeStateEntered()
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__;

    ui->stackedWidget->setCurrentWidget(ui->mainPage);
    /* Si on est entrain d'éditer un module, on affiche le bouton Revenir Editeur */
    m_homeState->assignProperty(m_abuleduPageAccueil->abePageAccueilGetBtnRevenirEditeur(), "visible", m_isEditorRunning);
    m_homeState->assignProperty(ui->actionAfficher_l_diteur,                                "enabled", !m_isEditorRunning);
    m_isDemoAvailable = true;
}

void interfaceClass::slotInterfaceHomeStateExited()
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__;

    m_isDemoAvailable = false;
}

void interfaceClass::slotInterfaceExerciseStateEntered()
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__;

}

void interfaceClass::slotInterfaceExerciseStateExited()
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__;

}

void interfaceClass::slotInterfaceEditorStateEntered()
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__;

    ui->stackedWidget->setCurrentWidget(ui->editorPage);
    QFile* fichierConf = new QFile(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf");
    fichierConf->copy(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/copieModule.conf");
    m_isEditorRunning = true;
}

void interfaceClass::slotInterfaceEditorStateExited()
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__;

    m_isEditorRunning = false;
    QFile* copieConf = new QFile(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/copieModule.conf");
    if (copieConf->exists()){
        QFile::remove(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/copieModule.conf");
    }
    ui->stackedWidget->setCurrentWidget(ui->mainPage);
}

void interfaceClass::slotInterfaceFinalStateEntered()
{
    ABULEDU_LOG_DEBUG()<<__PRETTY_FUNCTION__;

    /* OKAZOU */
    QFile::remove(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/copieModule.conf");
    if(abeApp->getAbeNetworkAccessManager()->abeSSOAuthenticationStatus() > 0)
    {
        m_editeur->editeurWriteOnAbeBoxPerso();
    }
    else{
        close();
    }
}

void interfaceClass::slotInterfaceFinalStateExited()
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__;

}

void interfaceClass::changelangue(QString langue)
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__;

    qApp->setProperty("langageUtilise",langue);
    myappTranslator.load("leterrier-calculment_"+langue, "lang");

    qApp->installTranslator(&myappTranslator);
    interfaceClass* nouvelleInterface = new interfaceClass();
    nouvelleInterface->showMaximized();
    nouvelleInterface->setWindowTitle(QObject::trUtf8("Calcul Mental"));
//    this->close();
    nouvelleInterface->activateWindow();
}

void interfaceClass::paintEvent(QPaintEvent* e )
{
    QMainWindow::paintEvent(e);
}
void interfaceClass::on_actionQuitter_triggered()
{

    this->close();
}

void interfaceClass::on_btnInitialise_clicked()
{
    QFile* fichierConf = new QFile(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf");
    fichierConf->remove();
    m_editeur->initialiser();
}

void interfaceClass::on_actionVerrouillage_nombres_changed()
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__;

    if (ui->actionVerrouillage_nombres->isChecked())
        qApp->setProperty("VerrouNombres",true);
    else
        qApp->setProperty("VerrouNombres",false);
}

void interfaceClass::on_action_Journal_de_mes_activit_s_triggered()
{
    abeApp->getAbeIdentite()->abeGetMyLogsAsPDF();
}

void interfaceClass::on_action_Changer_d_utilisateur_triggered()
{
    m_isChangingUser = true;
    if(abeApp->getAbeNetworkAccessManager()->abeSSOAuthenticationStatus() > 0)
    {
        m_editeur->editeurWriteOnAbeBoxPerso();
        abeApp->getAbeNetworkAccessManager()->abeSSOLogout();
    }
    else
    {
        abeApp->getAbeNetworkAccessManager()->abeSSOLogin();
        abeApp->getAbeNetworkAccessManager()->abeOnLoginSuccessGoto(this,SLOT(slotSetAbeBoxPersoSettings()));
        abeApp->getAbeNetworkAccessManager()->abeOnLoginFailureGoto(this,SLOT(slotSetPCSettings()));
    }
}

void interfaceClass::slotInterfaceShowMainPage()
{
    ui->stackedWidget->setCurrentWidget(ui->mainPage);
    if(!ui->menuBar->isEnabled()){
        ui->menuBar->setEnabled(true);
    }
}

void interfaceClass::slotInterfaceShowAboutPage()
{
    ui->stackedWidget->setCurrentWidget(ui->aboutPage);
    ui->menuBar->setEnabled(false);
}

void interfaceClass::setTitle(int authStatus)
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__<<authStatus;

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

void interfaceClass::slotMontreLanceur()
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__;

    ui->abeLauncher->abeLanceurSetIdentite(abeApp->getAbeIdentite());
    ui->stackedWidget->setCurrentWidget(ui->launcherPage);
}

void interfaceClass::slotMontreErreurId()
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__;

    AbulEduMessageBoxV1* msgError = new AbulEduMessageBoxV1(trUtf8("Problème !"),trUtf8("Accès impossible au lanceur d'activité sans identification correcte"),true,this);
    msgError->show();
}

void interfaceClass::on_actionAfficher_le_lanceur_d_exercice_triggered()
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__;

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

void interfaceClass::on_actionPav_num_rique_lin_aire_toggled(bool checked)
{
    if (m_localDebug){
        ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__<<checked;
    }
    qApp->setProperty("numericPad",checked);
}

void interfaceClass::slotSetAbeBoxPersoSettings()
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__;
    m_editeur->editeurOpenSettings();
}

void interfaceClass::slotSetPCSettings()
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__;
    m_editeur->editeurCreateSettings();
}

void interfaceClass::slotSettingsSaved()
{
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__;
    /* Je dois fermer si j'ai demandé la fermeture, mais pas si j'ai demandé le changement d'utilisateur */
    if(m_isChangingUser){
        m_isChangingUser = false;
        abeApp->getAbeNetworkAccessManager()->abeSSOLogin();
        abeApp->getAbeNetworkAccessManager()->abeOnLoginSuccessGoto(this,SLOT(slotSetAbeBoxPersoSettings()));
        abeApp->getAbeNetworkAccessManager()->abeOnLoginFailureGoto(this,SLOT(slotSetPCSettings()));
    }
    else{
        close();
    }
}
