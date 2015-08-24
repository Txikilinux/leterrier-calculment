#include "exerciceoperation.h"

ExerciceOperation::ExerciceOperation(QString exerciseName,QWidget *parent) :
    AbstractExercise(parent),
    m_operationName(exerciseName),
    m_parent(parent),
    m_minG(0),
    m_maxG(9),
    m_minD(0),
    m_maxD(9)
{
    m_localDebug = true;
    m_score = 0;
    m_leResultat = new QLineEdit("toto",getAbeExerciceTelecommandeV1());
    m_leResultat->setObjectName("leResultat");
    QRegExp expressionReguliere("[0-9][0-9]{0,"+QString::number(3)+"}");
    m_leResultat->setValidator(new QRegExpValidator(expressionReguliere, this));
    /** @todo gratter un peu : la ligne ci-dessous n'est plus utile parce que l'eventFilter fait que le clic sur Entrée équivaut à BtnVerification
     *  du coup si on la laisse la vérification déclenche aussitôt l'envoi d'un autre ballon
     *  si on la laissait il faudrait réimplémenter l'eventFilter
     *  par contre il faut trouver pourquoi il faut appuyer deux fois  */
//    connect(m_leResultat, SIGNAL(returnPressed()),getAbeExerciceTelecommandeV1()->ui->btnVerifier, SLOT(click()),Qt::UniqueConnection);
    getAbeExerciceTelecommandeV1()->setDimensionsWidget();
}

ExerciceOperation::~ExerciceOperation()
{
    emit signalExerciseExited();
}

int ExerciceOperation::verifieReponse()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    return 0;
}

int ExerciceOperation::donneReponse()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    return -1;
}

void ExerciceOperation::setDimensionsWidgets(float ratio)
{
    Q_UNUSED(ratio)
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    QPixmap imageFond;
    imageFond.load(":/calculment/backgrounds/"+m_operationName);
    m_imageFond = new QPixmap(imageFond.scaledToHeight(m_parent->height()));
    QGraphicsPixmapItem* fond = new QGraphicsPixmapItem();
    fond->setPixmap(*m_imageFond);
    m_sceneAireDeJeu->addItem(fond);
    fond->setZValue(-10);
    m_AireDeJeu->setGeometry(0,0,m_imageFond->width(),m_imageFond->height());
    m_AireDeJeu->setSceneRect(0,0,m_imageFond->width(),m_imageFond->height());
    QPixmap backgr(":/calculment/backgrounds/empty");
    getAbeExerciceAireDeTravailV1()->setImageFond(backgr.scaled(imageFond.width()+75*abeApp->getAbeApplicationDecorRatio(),imageFond.height()+150*abeApp->getAbeApplicationDecorRatio()));
    int ecartAireTelecommande = 0;
    int abscisseAire = (m_parent->width() - (getAbeExerciceAireDeTravailV1()->width() + getAbeExerciceTelecommandeV1()->width() + ecartAireTelecommande))/2;
    int abscisseTelecommande = abscisseAire + getAbeExerciceAireDeTravailV1()->width() + ecartAireTelecommande;
    getAbeExerciceAireDeTravailV1()->move(abscisseAire,0);
    getAbeExerciceTelecommandeV1()->move(abscisseTelecommande,-25*abeApp->getAbeApplicationDecorRatio());
    getAbeExerciceTelecommandeV1()->ui->framePopupQuitter->move(abscisseTelecommande - getAbeExerciceTelecommandeV1()->ui->framePopupQuitter->width()+30*abeApp->getAbeApplicationDecorRatio(),
                                                                getAbeExerciceTelecommandeV1()->ui->framePopupQuitter->y());
    boiteTetes->setPos((getAbeExerciceAireDeTravailV1()->ui->gvPrincipale->width() - boiteTetes->geometry().width())/2,
                       getAbeExerciceAireDeTravailV1()->ui->gvPrincipale->height() - boiteTetes->geometry().height());}

void ExerciceOperation::chargerParametres()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf", QSettings::IniFormat);
    setAbeNbTotalQuestions(config.value("NombreBallons",10).toInt());
    config.beginGroup(m_operationName);
    if (getAbeLevel().isEmpty()) setAbeLevel(config.value("NiveauEnCours"+m_operationName,1).toString());
    else qDebug()<<"Dans chargerParametres(), m_level valait déjà "<<getAbeLevel();
    config.beginGroup(getAbeLevel());
    qDebug()<<"Lecture des paramètres dans "<<config.fileName()<<" - "<<m_operationName<<" - "<<getAbeLevel();
    m_maxG = config.value("MaxGauche",100).toInt();
    m_minG = config.value("MinGauche",0).toInt();
    m_maxD = config.value("MaxDroite",100).toInt();
    m_minD = config.value("MinDroite",0).toInt();
    m_temps = config.value("TempsAccorde",10).toInt();
    qDebug() << "MaxGauche : " << m_maxG << "MinGauche : " << m_minG << "MaxDroite : " << m_maxD << "MinDroite : " << m_minD<< "Mon niveau : "<<getAbeLevel()<<"Tps : "<<m_temps;
    config.endGroup();
    config.endGroup();

    if (m_operationName.left(6)=="tableA" || m_operationName.left(6)=="tableM") {
        m_minD=m_maxD=m_cible;
        m_minG=0;
        m_maxG=9;
    }
    else if(m_operationName.left(11)=="complementA"
            || m_operationName.left(11)=="complementM"){
        m_minG=m_maxG=m_minD=m_maxD=m_cible;
    }
}

void ExerciceOperation::animeBaudruche()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    float factX= static_cast<float> (QApplication::desktop()->screenGeometry().width())/1680;
    float factY= static_cast<float> (QApplication::desktop()->screenGeometry().height())/1050;
    QGraphicsItemAnimation *animation = new QGraphicsItemAnimation(m_sceneAireDeJeu);
    animation->setItem(m_baudruche);
    animation->setTimeLine(m_baudruche->m_timer);
    if (m_operationName == "addition") {
        for (int i = 0; i < 200; i++)
            animation->setPosAt(i/200.0, QPointF((3.8*i*factX) ,0 ));
    }
    else if(m_operationName.left(6) == "tableA"|| m_operationName.left(6) == "tableM") {
        for (int i = 0; i < 200; i++)
            animation->setPosAt(i/200.0, QPointF(0 , 3.3*i*factY));
    }
    else for (int i = 0; i < 200; i++)
        //animation->setPosAt(i/200.0, QPointF(0 , (-3*i)-(i*0.8)));
        animation->setPosAt(i/200.0, QPointF(0 , (-2.5*i*factY)));
    // animation->setPosAt(i/200.0, QPointF(0 , (3*i)+(i*0.8))); --> pour la faire tomber
    //animation->setPosAt(i/200.0, QPointF((-3*i)-(i*0.8) ,0 )); --> pour la faire aller à gauche
    //animation->setPosAt(i/200.0, QPointF((3*i)+(i*0.8) ,0 )); --> pour la faire aller à droite

    m_baudruche->m_timer->start();
}

void ExerciceOperation::slotSequenceEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
        ABULEDU_LOG_DEBUG() << sequenceMachine->configuration().toList();
    }
    AbstractExercise::slotSequenceEntered();
}

void ExerciceOperation::slotRealisationExerciceEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
        ABULEDU_LOG_DEBUG() << sequenceMachine->configuration().toList();
    }
    AbstractExercise::slotRealisationExerciceEntered();
    chargerParametres();
}

void ExerciceOperation::slotInitQuestionEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
        ABULEDU_LOG_DEBUG() << sequenceMachine->configuration().toList();
        ABULEDU_LOG_DEBUG() <<m_total<<getAbeNbTotalQuestions()<<getAbeNumQuestion()<<m_score;
    }
    AbstractExercise::slotInitQuestionEntered();
    float factX= static_cast<float> (QApplication::desktop()->screenGeometry().width())/1680;
    float factY= static_cast<float> (QApplication::desktop()->screenGeometry().height())/1050;
    //instanciation d'une baudruche et connexion aux autres objets
    if (m_operationName=="addition") m_depart = new QPoint(0*factX,boiteTetes->y());
    else if(m_operationName.left(6)=="tableA"|| m_operationName.left(6)=="tableM") m_depart = new QPoint(m_imageFond->width()/2-80*factY,0*factY);
    else m_depart = new QPoint(m_imageFond->width()/2-80*factY,500*factY);

    //m_depart = new QPoint(m_ui->vue->width()/2,0); --> pour la faire tomber
    qDebug()<<"Creation de baudruche avec temps "<<m_temps;
    if (m_operationName=="addition")
        m_baudruche = new baudruche(m_minG,m_maxG,m_minD,m_maxD,m_temps,m_operationName,*m_depart,m_sceneAireDeJeu,"auto");
    else if(m_operationName==""
            || m_operationName=="soustraction"
            || m_operationName=="multiplication")
        m_baudruche = new baudruche(m_minG,m_maxG,m_minD,m_maxD,m_temps,m_operationName,*m_depart,m_sceneAireDeJeu);

    else if (m_operationName.left(6)=="tableA")
        m_baudruche = new baudruche(m_minG,m_maxG,m_minD,m_maxD,m_temps,m_operationName.left(6),*m_depart,m_sceneAireDeJeu,"nacelle");
    else if(m_operationName.left(6)=="tableM")
        m_baudruche = new baudruche(m_minG,m_maxG,m_minD,m_maxD,m_temps,m_operationName.left(6),*m_depart,m_sceneAireDeJeu,"cabine");
    else if (m_operationName.left(11)=="complementA")
        m_baudruche = new baudruche(m_minG,m_temps,m_operationName.left(11), *m_depart,m_sceneAireDeJeu,"fantome");
    else if (m_operationName.left(11)=="complementM")
        m_baudruche = new baudruche(m_minG,m_temps,m_operationName.left(11), *m_depart,m_sceneAireDeJeu);

    else if (m_operationName=="OdGrandeurAddition"
             || m_operationName=="OdGrandeurSoustraction"
             || m_operationName=="OdGrandeurMultiplication")
        m_baudruche = new baudruche(m_maxG,m_maxD,m_temps,m_operationName, *m_depart,m_sceneAireDeJeu);

//    else  QMessageBox::critical(this, trUtf8("Opération inexistante"), m_operationName.append(trUtf8(", ça n'existe pas comme opération...")));

    //          else {qDebug()<< "Pas d'opération portant le nom de "<<m_operationName;}//Pourquoi quand même erreur de segmentation
    if (m_operationName=="OdGrandeurAddition"
            || m_operationName=="OdGrandeurSoustraction"
            || m_operationName=="OdGrandeurMultiplication") this->m_resultatEnCours=m_baudruche->getMApproximation();
    else this->m_resultatEnCours=m_baudruche->getMResultat();
    if (m_total < getAbeNbTotalQuestions() - 1) {
        /** @todo vérifier que la machine à états gère correctement ça */
//        connect(m_baudruche, SIGNAL(destroyed(bool)), getAbeExerciceTelecommandeV1()->ui->btnSuivant, SLOT(setEnabled(bool)));
//        connect(m_baudruche, SIGNAL(destroyed()), m_ui->btnBallon, SLOT(setFocus()));
    }
//    connect(m_baudruche, SIGNAL(destroyed(bool)), m_ui->btnFeu, SLOT(setDisabled(bool)));
//    connect(m_baudruche, SIGNAL(destroyed(bool)), m_ui->leResultat, SLOT(setDisabled(bool)));
//    connect(m_baudruche, SIGNAL(destroyed()), m_ui->leResultat, SLOT(clear()));
    connect(m_baudruche->m_timer, SIGNAL(finished()),m_baudruche, SLOT(detruireTps()));
    connect(m_baudruche, SIGNAL(tempsFini(QString)), this, SLOT(ajouteErreur(QString)));
//    connect(m_baudruche, SIGNAL(tempsFini(QString)), m_ui->lblMsg, SLOT(setText(QString)));
//    connect(m_baudruche, SIGNAL(tempsFini(QPixmap)), m_ui->lblImgMsg, SLOT(setPixmap(QPixmap)));
    //        connect(m_baudruche, SIGNAL(tempsFini(QString)), this, SLOT(afficheResultat(QString)));
    connect(m_baudruche, SIGNAL(tempsFini(QString)), this, SLOT(pousseLogs(QString)));
    m_baudruche->emetRes();
    m_sceneAireDeJeu->addItem(m_baudruche);
    if (m_operationName == "addition") {
        QPixmap cache(":/calculment/elements/cache");
        int bordure=20;
        QRect ecran;
        ecran=QApplication::desktop()->screenGeometry();
        QPixmap cache2 = cache.scaledToHeight(ecran.height()-60 - 2*bordure, Qt::SmoothTransformation);
        m_sceneAireDeJeu->addPixmap(cache2);
    }

    m_trace = m_baudruche->getMAffichage();
    qDebug()<<"Calcul propose : "<<m_trace;

    //affichage du nombre de ballons déjà instanciés
    /** @todo voir ça, dessous */
//    m_total = m_ui->lblTotal->text().toInt();
    m_total++;

    /* animation du ballon */
    animeBaudruche();
    boiteTetes->setEtatTete(m_numExercice, abe::evalY,false,getAbeNbTotalQuestions()-getAbeNumQuestion()+1);
}

void ExerciceOperation::slotQuestionEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
        ABULEDU_LOG_DEBUG() << sequenceMachine->configuration().toList();
    }
    AbstractExercise::slotQuestionEntered();
    m_leResultat->clear();
    m_leResultat->setFocus();
}

void ExerciceOperation::slotAfficheVerificationQuestionEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
        ABULEDU_LOG_DEBUG() << sequenceMachine->configuration().toList();
    }
    AbstractExercise::slotAfficheVerificationQuestionEntered();
    if(m_leResultat->text().simplified().isEmpty()){
        setAbeExerciceEvaluation(abe::evalZ);
        boiteTetes->setEtatTete(m_numExercice, getAbeExerciceEvaluation(),false,getAbeNbTotalQuestions()-getAbeNumQuestion()+1);
        setAbeTeteForResult(1,1);
    }
    else {

        float proposition = m_leResultat->text().toFloat();
        qDebug()<<"Valeur du ballon : "<<m_resultatEnCours<<", lache sur "<<proposition;
        QString demande = "";
        demande = m_baudruche->getMGOperande()+m_baudruche->getMOperation()+m_baudruche->getMDOperande();
        //    m_score =

        if (proposition == m_resultatEnCours) {
            m_score++;
            /** @todo Mettre la tête de William très bien dans la boiteTetes */
            //        QPixmap* imgO = new QPixmap(":/calculment/elements/win");
            //        imgO->scaledToHeight(imgO->height()*factY);
            //        m_ui->lblImgMsg->setPixmap(*imgO);
            setAbeExerciceEvaluation(abe::evalA);
        }
        else {
            /** @todo Mettre la tête de William mal dans la boiteTetes */
            ajouteErreur("Erreur calcul");
            setAbeExerciceEvaluation(abe::evalD);
        }
    }
    getAbeExerciceTelecommandeV1()->ui->lblCustom2->setText(QString::number(m_score)+ " sur "+QString::number(getAbeNbTotalQuestions()));

    //sauvegardeLog* envoieRes = new sauvegardeLog(QDate::currentDate(), QTime::currentTime(), utilisateur, m_baudruche->getMLigne(), m_ui->leResultat->text(), reponseAttendueEnString);
    setAbeLineLog(m_baudruche->getMLigne(),m_leResultat->text().simplified(),m_score, m_total,getAbeExerciceEvaluation(),QString::number(m_resultatEnCours));
    qDebug()<<getPluginLogs();

    if (m_baudruche) m_baudruche->detruire();// <<<<-------------------------------------- Merdier

/** @todo assignProperty */
    //    m_ui->btnFeu->setDisabled(true);

    if (m_total == getAbeNbTotalQuestions()) {
        //erics 20110209 sinon coredump en cas de "aucune erreur" ou "que des erreurs"
        //if (m_baudruche) delete m_baudruche;
        /** @todo remplacer la ligne en dessous par le bilan d'exercice */
//        afficheResultat("peutImporteCeQuiEstEcritIci");

        /** @todo discuter de la pertinence de l'augmentation automatique de niveau */
        //mise à jour ou pas du niveau
        QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf", QSettings::IniFormat);
        config.beginGroup(m_operationName);
        if (m_score == m_total) {
            if (getAbeLevel() == "Niveau1") config.setValue("NiveauEnCours"+m_operationName, "Niveau2");
            else if (getAbeLevel() == "Niveau2") config.setValue("NiveauEnCours"+m_operationName, "Niveau3");
            else if (getAbeLevel() == "Niveau3") config.setValue("NiveauEnCours"+m_operationName, "Personnel");
        }


        //m_level = config.value("NiveauEnCours"+opCourante).toString();

        config.endGroup();
    }
    boiteTetes->setEtatTete(m_numExercice, getAbeExerciceEvaluation(),false,getAbeNbTotalQuestions()-getAbeNumQuestion()+1);
}

void ExerciceOperation::slotFinVerificationQuestionEntered()
{
    AbstractExercise::slotFinVerificationQuestionEntered();
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
        ABULEDU_LOG_DEBUG() << sequenceMachine->configuration().toList();
        qDebug()<<" ---------------------------------------------------- ";
        qDebug()<<" Je sors de "<<__PRETTY_FUNCTION__;
        qDebug()<<" m_score "<<m_score;
        qDebug()<<" m_total "<<m_total;
        qDebug()<<" m_resultatEnCours "<<m_resultatEnCours;
        qDebug()<<" m_trace "<<m_trace;
        qDebug()<<" getAbeNumQuestion "<<getAbeNumQuestion();
        qDebug()<<" getAbeNbTotalQuestions "<<getAbeNbTotalQuestions();
        qDebug()<<"getAbeExerciceEvaluation" <<getAbeExerciceEvaluation();
        qDebug()<<" ---------------------------------------------------- ";
    }
}

void ExerciceOperation::slotAfficheCorrectionQuestionEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
        ABULEDU_LOG_DEBUG() << sequenceMachine->configuration().toList();
    }
    AbstractExercise::slotAfficheCorrectionQuestionEntered();
}

void ExerciceOperation::slotFinCorrectionQuestionEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
        ABULEDU_LOG_DEBUG() << sequenceMachine->configuration().toList();
    }
    AbstractExercise::slotFinCorrectionQuestionEntered();
}

void ExerciceOperation::slotBilanSequenceEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
        ABULEDU_LOG_DEBUG() << sequenceMachine->configuration().toList();
    }
    AbstractExercise::slotBilanSequenceEntered();
}

bool ExerciceOperation::eventFilter(QObject *obj, QEvent *event)
{
    return AbstractExercise::eventFilter(obj,event);
}

void ExerciceOperation::slotAide()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    AbstractExercise::slotAide();
}

void ExerciceOperation::ajouteErreur(QString msg)
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
        ABULEDU_LOG_DEBUG() << sequenceMachine->configuration().toList();
    }
    if(m_leResultat->text().simplified().isEmpty()){
        sequenceMachine->postEvent(new StringEvent("QuestionVerifieEmpty"));
        qDebug()<<" post que questionverifieempty";
    }
    else{
        qDebug()<<" pas de post que questionverifieempty";

    }
    qDebug()<<getAbeNumQuestion()<<getAbeNbTotalQuestions();
    m_listeEchecs.append(QString::number(m_baudruche->getMGOperande())+";"+m_baudruche->getMOperation()+";"+QString::number(m_baudruche->getMDOperande())+";"+QString::number(m_resultatEnCours)+";"+m_baudruche->m_nomImage);
}
