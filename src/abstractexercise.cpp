/** Classe de base pour les exercices du logiciel Calcul mental
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/LeTerrierExercice
  * @author 2014 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
  *
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

#include "abstractexercise.h"

AbstractExercise::AbstractExercise(QWidget *parent):
    AbulEduCommonStatesV1(parent),
    m_localDebug(false),
    m_parent(parent),
    m_AireDeJeu(new QGraphicsView()),
    m_sceneAireDeJeu(new QGraphicsScene(this)),
    m_baudruche(0),
    m_depart(0),
    m_score(0),
    m_total(0),
    m_resultatEnCours(-1),
    m_roundedOperands(QPair<int,int>(-1,-1)),
    m_cible(0),
    m_trace(QString()),
    m_leResultat(0),
    m_niveau(-1)
{
    m_leResultat = new QLineEdit(QString(),getAbeExerciceTelecommandeV1());
    m_leResultat->setObjectName("leResultat");
    QRegExp expressionReguliere("[0-9][0-9]{0,"+QString::number(3)+"}");
    m_leResultat->setValidator(new QRegExpValidator(expressionReguliere, this));
    /** @todo gratter un peu : la ligne ci-dessous n'est plus utile parce que l'eventFilter fait que le clic sur Entrée équivaut à BtnVerification
     *  du coup si on la laisse la vérification déclenche aussitôt l'envoi d'un autre ballon
     *  si on la laissait il faudrait réimplémenter l'eventFilter
     *  par contre il faut trouver pourquoi il faut appuyer deux fois  */
//    connect(m_leResultat, SIGNAL(returnPressed()),getAbeExerciceTelecommandeV1()->ui->btnVerifier, SLOT(click()),Qt::UniqueConnection);
    getAbeExerciceTelecommandeV1()->abeTelecommandeResize();
    /* Création de l'aire de jeu et de sa scène */
    m_AireDeJeu->setScene(m_sceneAireDeJeu);
    m_AireDeJeu->setSceneRect(m_AireDeJeu->rect());
    /* On la place sur l'AbulEduExerciceWidgetAireDeTravailV1 par l'intermédiaire d'un QGraphicsProxyWidget,
       qui va permettre de modifier l'ordre d'affichage des objets. La gv_AireDeJeu sera en dessous des autres objets */
    m_proxyAireDeJeu = getAbeExerciceAireDeTravailV1()->ui->gvPrincipale->scene()->addWidget(m_AireDeJeu);
    m_proxyAireDeJeu->setZValue(-1);
    m_AireDeJeu->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_AireDeJeu->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    /* On lui donne un fond transparent et pas de bordure */
    m_AireDeJeu->setFrameShape(QFrame::NoFrame);
    m_AireDeJeu->setVisible(true);

    connect(getAbeExerciceTelecommandeV1(), SIGNAL(btnTelecommandeAideClicked()), this, SLOT(slotAide()), Qt::UniqueConnection);

    m_numericPad = new AbulEduNumericLinearPadv1(90* abeApp->getAbeApplicationDecorRatio(),true);
    connect(m_numericPad, SIGNAL(signalNumericLinearPadV1Clicked(Qt::Key)), this, SLOT(slotNumericPadKeyPressed(Qt::Key)),Qt::UniqueConnection);
    getAbeExerciceAireDeTravailV1()->ui->gvPrincipale->scene()->addWidget(m_numericPad)->setZValue(1000);
    m_numericPad->setVisible(false);
    sequenceMachine->start();
}

AbstractExercise::~AbstractExercise()
{

}

void AbstractExercise::clearScene()
{
    m_AireDeJeu->scene()->clear();
}

void AbstractExercise::setDimensionsWidgets(float ratio)
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    /* Placement de AbulEduExerciceWidgetAireDeTravailV1 et AbulEduExerciceWidgetTelecommandeV1.
   Leur taille est recalculée grâce à leur connect avec le signal dimensionsChangees de l'AbulEduPageAccueilV1 */
    getAbeExerciceAireDeTravailV1()->move(0, 0);
    getAbeExerciceTelecommandeV1()->move(1550*ratio, 0);
    getAbeExerciceMessageV1()->setParent(m_AireDeJeu);
    getAbeExerciceMessageV1()->move(60,0);
    getAbeExerciceMessageV1()->setMaximumSize(800,500);

    /* Dimensionnement et placement de l'aire de jeu
   Les paramètres en dur (250 et 100) sont liés aux contraintes de marges des images de fond */
    int haut  = getAbeExerciceAireDeTravailV1()->ui->gvPrincipale->height() - boiteTetes->geometry().height() - 60 * ratio;
    int large = getAbeExerciceAireDeTravailV1()->ui->gvPrincipale->width();
    if (m_localDebug){
        ABULEDU_LOG_DEBUG()<<"largeur"<<large<<"hauteur"<<haut;
    }
    m_AireDeJeu->setFixedSize(QSize(large - 200*ratio, haut - 110 * ratio));
    m_AireDeJeu->setSceneRect(m_AireDeJeu->rect());
    m_AireDeJeu->move(100 * ratio, 140 * ratio);
    m_AireDeJeu->scene()->setSceneRect(m_AireDeJeu->rect());

    /* Placement des têtes */
    boiteTetes->setPos((getAbeExerciceAireDeTravailV1()->ui->gvPrincipale->width() - boiteTetes->geometry().width())/2,
                       getAbeExerciceAireDeTravailV1()->ui->gvPrincipale->height() - boiteTetes->geometry().height() -30*ratio);
    m_numericPad->move((getAbeExerciceAireDeTravailV1()->ui->gvPrincipale->width() - m_numericPad->width())/2,
                       getAbeExerciceAireDeTravailV1()->ui->gvPrincipale->height() - m_numericPad->height() -30*ratio);
}

void AbstractExercise::slotSequenceEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }

    getAbeExerciceMessageV1()->setParent(0);
    getAbeExerciceAireDeTravailV1()->ui->gvPrincipale->scene()->addWidget(getAbeExerciceMessageV1());

    if(m_cible >= 0){
        abeStateMachineSetOnPeutPresenterSequence(false);
    }
    else {
        abeStateMachineSetOnPeutPresenterSequence(true);
    }
    abeStateMachineSetOnPeutPresenterExercice(false);
    abeStateMachineSetOnPeutPresenterBilanExercice(false);

    /* Choix : le niveau de difficulté est le même pour tous les exercices d'une séquence */
//    setAbeLevel( -> faut voir, quoi ...
//    abeStateMachineSetNbExercices( -> il faudra regarder le nombre de questions

    /* Attention : la ligne ci-dessous peut être déplacée à condition de rester avant l'appel à setDimensionsWidgets(); */
    AbulEduCommonStatesV1::slotSequenceEntered();

    /* Corrections par rapport au fonctionnement général de la machine à états */
    presentationSequence->assignProperty(getAbeExerciceTelecommandeV1()->ui->btnSuivant, "enabled",false);
    question->assignProperty(getAbeExerciceTelecommandeV1()->ui->btnAide, "enabled",false);
    question->assignProperty(getAbeExerciceTelecommandeV1()->ui->btnCorriger, "enabled",false);
    question->assignProperty(m_leResultat, "enabled", true);
    question->assignProperty(m_leResultat, "focus", true);
    question->assignProperty(boiteTetes, "visible", false);
    if(qApp->property("numericPad").toBool()){
        question->assignProperty(m_numericPad, "visible", true);
        question->assignProperty(boiteTetes, "visible", false);
    }
    else {
        question->assignProperty(m_numericPad, "visible", false);
        question->assignProperty(boiteTetes, "visible", true);
    }
    afficheVerificationQuestion->assignProperty(m_numericPad, "visible", false);
    afficheVerificationQuestion->assignProperty(boiteTetes, "visible", true);
    afficheVerificationQuestion->assignProperty(m_leResultat, "enabled", false);

//    afficheVerificationQuestion->assignProperty(getAbeExerciceTelecommandeV1()->ui->btnCorriger, "enabled", true);
    finVerificationQuestion->addTransition(getAbeExerciceTelecommandeV1()->ui->btnSuivant,SIGNAL(clicked()),initQuestion);

    setDimensionsWidgets();
}

void AbstractExercise::slotPresenteSequenceEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__<<m_variations.size();
    }
    getAbeExerciceMessageV1()->setVisible(false);
    getAbeExerciceMessageV1()->abeWidgetMessageShowImageFond(false);
    getAbeExerciceMessageV1()->abeWidgetMessageSetZoneTexteVisible(true);
    QListIterator<AbulEduLaunchElements> iter(m_variations);
    m_boutonsChoix.clear();
    QGridLayout* grid = new QGridLayout();

    if(m_variations.count() > 0 && m_variations.count() < 6){
        getAbeExerciceMessageV1()->abeWidgetMessageGetFrmCustomLayout()->setDirection(QBoxLayout::TopToBottom);
        getAbeExerciceMessageV1()->abeWidgetMessageGetFrmCustomLayout()->addSpacerItem(new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Expanding));
    }
    else{
        getAbeExerciceMessageV1()->abeWidgetMessageGetFrmCustomLayout()->setDirection(QBoxLayout::LeftToRight);
        getAbeExerciceMessageV1()->abeWidgetMessageGetFrmCustomLayout()->addSpacerItem(new QSpacerItem(40,20,QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding));
        getAbeExerciceMessageV1()->abeWidgetMessageGetFrmCustomLayout()->addLayout(grid);
        getAbeExerciceMessageV1()->abeWidgetMessageGetFrmCustomLayout()->addSpacerItem(new QSpacerItem(40,20,QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding));

    }
    while(iter.hasNext()){
        AbulEduLaunchElements abeElement = iter.next();
        AbulEduFlatBoutonV1* btn = new AbulEduFlatBoutonV1(getAbeExerciceMessageV1());
        btn->setTexteAlignement(Qt::AlignLeft);
        btn->setMinimumWidth(300);
        btn->setText(abeElement.abeLaunchElementGetButtonText());
        btn->setIcon(QIcon(abeElement.abeLaunchElementGetIconPath()));
        btn->setProperty("peculiarity",abeElement.abeLaunchElementGetPeculiarity());
        btn->setIconSize(QSize(64*abeApp->getAbeApplicationDecorRatio(),64*abeApp->getAbeApplicationDecorRatio()));
        if(abeElement.abeLaunchElementGetPeculiarity().toInt() == -1){
            /* Je m'appuie pour savoir si on est dans le cas du choix multiple sur cette condition : c'est dans ce seul cas que ça vaut -1 et s'appuyer sur le texte risquerait de poser des problèmes lors de traductions */
            connect(btn, SIGNAL(clicked()), this, SLOT(slotOnBtnMultipleChoiceClicked()), Qt::UniqueConnection);
        }
        else{
            connect(btn, SIGNAL(clicked()), this, SLOT(slotSetPeculiarity()), Qt::UniqueConnection);
        }
        if(m_variations.count() < 6){
            btn->setTextePaddingLeft(300*abeApp->getAbeApplicationDecorRatio());
            getAbeExerciceMessageV1()->abeWidgetMessageGetFrmCustomLayout()->addWidget(btn);
            getAbeExerciceMessageV1()->abeWidgetMessageGetFrmCustomLayout()->addSpacerItem(new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Expanding));
            qDebug()<<"Ajout en bas au layout";
        }
        else{
            getAbeExerciceMessageV1()->abeWidgetMessageGetFrmCustomLayout()->addLayout(grid);
            grid->addWidget(btn,m_boutonsChoix.size()/2,m_boutonsChoix.size()%2);
            qDebug()<<"Ajout au layout en "<<m_boutonsChoix.size()/2<<m_boutonsChoix.size()%2;
        }
        btn->setVisible(true);
        m_boutonsChoix << btn;
    }
    getAbeExerciceMessageV1()->ui->gvPrincipale->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    getAbeExerciceMessageV1()->ui->gvPrincipale->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    getAbeExerciceMessageV1()->repaint();
    AbulEduCommonStatesV1::slotPresenteSequenceEntered();
    getAbeExerciceMessageV1()->abeWidgetMessageSetTitre(trUtf8("Choisis : ")+getAbeExerciceName());
    getAbeExerciceMessageV1()->abeWidgetMessageResize();
    getAbeExerciceMessageV1()->setVisible(true);
    getAbeExerciceMessageV1()->setMaximumHeight(m_AireDeJeu->height());
    getAbeExerciceMessageV1()->move(m_AireDeJeu->pos());
}

void AbstractExercise::slotPresenteSequenceExited()
{
    foreach(AbulEduFlatBoutonV1* bouton,m_boutonsChoix){
        bouton->deleteLater();
    }
    getAbeExerciceMessageV1()->abeWidgetMessageShowImageFond(true);
    getAbeExerciceMessageV1()->abeWidgetMessageSetZoneTexteVisible(false);
    /* On peut bien lui fixer une hauteur fixe : comme on n'a qu'un seul exercice et une seule question on ne s'en ressert pas pour autre chose que le bilan */
    getAbeExerciceMessageV1()->abeWidgetMessageGetFrmCustomLayout()->parentWidget()->setFixedHeight(10);
    getAbeExerciceMessageV1()->abeWidgetMessageResize();
}

void AbstractExercise::slotOnBtnMultipleChoiceClicked()
{
    AbulEduMessageBoxV1* choice = new AbulEduMessageBoxV1(trUtf8("Choisir les nombres"),trUtf8("Coche les cases des nombres sur lesquels doivent porter les questions."),false,getAbeExerciceAireDeTravailV1());
    choice->abeSetModeEnum(AbulEduMessageBoxV1::abeAcceptCancelButton);
    choice->setWink(true);
    for(int i = 2;i <= 9;i++){
        QCheckBox* chk = new QCheckBox(QString::number(i),getAbeExerciceMessageV1());
        chk->setProperty("value",i);
        chk->setMinimumHeight(40);
        connect(chk, SIGNAL(toggled(bool)),this, SLOT(slotStackTablesNumbers(bool)),Qt::UniqueConnection);
        QGridLayout* lay = (QGridLayout*) choice->abeMessageBoxGetCustomFrame()->layout();
        lay->addWidget(chk,(i-2)/4,(i-2)%4);
    }
    choice->updateGeometry();
    connect(choice, SIGNAL(signalAbeMessageBoxOK()), SLOT(slotAcceptMultipleChoice()),Qt::UniqueConnection);
    connect(choice, SIGNAL(signalAbeMessageBoxCancel()), SLOT(slotCancelMultipleChoice()),Qt::UniqueConnection);
    choice->show();
    getAbeExerciceMessageV1()->setEnabled(false);
    getAbeExerciceTelecommandeV1()->setEnabled(false);
}

void AbstractExercise::slotStackTablesNumbers(bool checked)
{
    if(checked){
        m_multipleCible.append(sender()->property("value").toInt());
    }
    else {
        m_multipleCible.removeOne(sender()->property("value").toInt());
    }
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__<<m_multipleCible;
    }
}

void AbstractExercise::slotAcceptMultipleChoice()
{
        getAbeExerciceMessageV1()->setEnabled(true);
        getAbeExerciceTelecommandeV1()->setEnabled(true);
    if(m_multipleCible.size() > 0){
        bool isBtnSuivantEnable = getAbeExerciceTelecommandeV1()->ui->btnSuivant->isEnabled();
        getAbeExerciceTelecommandeV1()->ui->btnSuivant->setEnabled(true);
        getAbeExerciceTelecommandeV1()->ui->btnSuivant->click();
        getAbeExerciceTelecommandeV1()->ui->btnSuivant->setEnabled(isBtnSuivantEnable);
    }
}

void AbstractExercise::slotCancelMultipleChoice()
{
    getAbeExerciceMessageV1()->setEnabled(true);
    getAbeExerciceTelecommandeV1()->setEnabled(true);
}

void AbstractExercise::slotDecreaseLevel()
{
    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf", QSettings::IniFormat);
    m_niveau--;
    config.beginGroup(m_operationName);
    config.setValue("NiveauEnCours"+m_operationName,m_niveau);
    config.endGroup();
}

void AbstractExercise::slotRealisationExerciceEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    getAbeExerciceMessageV1()->setVisible(false);
    AbulEduCommonStatesV1::slotRealisationExerciceEntered();
}

void AbstractExercise::slotInitQuestionEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    AbulEduCommonStatesV1::slotInitQuestionEntered();
    setAbeExerciceEvaluation(abe::evalY);
    boiteTetes->setEtatTete(m_numExercice, abe::evalY,false,getAbeNbTotalQuestions()-getAbeNumQuestion()+1);
    m_sceneAireDeJeu->clear();
}

void AbstractExercise::slotQuestionEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    m_leResultat->clear();
    m_leResultat->setFocus();
    AbulEduCommonStatesV1::slotQuestionEntered();
}

void AbstractExercise::slotAfficheVerificationQuestionEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    /* Je commente l'appel à la fonction de la classe mère afin d'empêcher le passage automatique à la question suivante */
//    AbulEduCommonStatesV1::slotAfficheVerificationQuestionEntered();
    m_leResultat->clearFocus();
    m_numericPad->setVisible(false);
}

void AbstractExercise::slotFinVerificationQuestionEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__<<getAbeNumQuestion()<<getAbeNbTotalQuestions();
    }
    if(getAbeNumQuestion() == getAbeNbTotalQuestions()) {
        sequenceMachine->postEvent(new StringEvent("Questionsdone"));
    }
    else {
        sequenceMachine->postEvent(new StringEvent("Questionsloop"));
    }
    /* Je commente l'appel à la fonction de la classe mère afin d'empêcher le passage automatique à la question suivante */
//    AbulEduCommonStatesV1::slotFinVerificationQuestionEntered();
}

void AbstractExercise::slotAfficheCorrectionQuestionEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__<<m_roundedOperands;
    }
    QString affichage;
    if(m_roundedOperands.first != -1){
        if(m_operands.first != m_roundedOperands.first){
            affichage.append(QString::number(m_operands.first)+" "+QString::fromUtf8("≈")+" "+QString::number(m_roundedOperands.first));
            affichage.append("<br/>");
        }
        if(m_operands.second != m_roundedOperands.second){
            affichage.append(QString::number(m_operands.second)+" "+QString::fromUtf8("≈")+" "+QString::number(m_roundedOperands.second));
            affichage.append("<br/>");
        }
    }
    if(!affichage.isEmpty()){
        affichage.append(trUtf8("donc "));
    }
    if(m_trace.contains("?")){
        affichage.append(m_trace.replace("?","<b>"+QString::number(m_resultatEnCours)+"</b>"));
    }
    else if(m_trace.contains(QString::fromUtf8("≈"))) {
        affichage.append(m_trace+" "+"<b>"+QString::number(m_resultatEnCours)+"</b>");
    }
    else {
        affichage.append(m_trace+" = "+"<b>"+QString::number(m_resultatEnCours)+"</b>");
    }

    AbulEduMessageBoxV1* msgCorrection = new AbulEduMessageBoxV1(trUtf8("Correction"),affichage,true,m_AireDeJeu);
    QFont f = msgCorrection->font();
    f.setPointSizeF(20*abeApp->getAbeApplicationDecorRatio());
    msgCorrection->abeMessageBoxSetTextFont(f);
    msgCorrection->abeMessageBoxSetTextAlignment(Qt::AlignCenter);
    msgCorrection->setTextColor(QColor(255,128,128));
    msgCorrection->show();
}

void AbstractExercise::slotFinCorrectionQuestionEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    if(getAbeNumQuestion() == getAbeNbTotalQuestions()) {
        sequenceMachine->postEvent(new StringEvent("QuestionsDoneCorrect"));
    }
    else {
        sequenceMachine->postEvent(new StringEvent("QuestionsLoopCorrect"));
    }
}

void AbstractExercise::slotBilanSequenceEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }

    /* On affiche la bonne tete en fonction du nombre de bonnes réponses */
    QString imagetete = ":/bilan/";
    setAbeTeteForResult(getAbeNbTotalQuestions()-m_score,getAbeNbTotalQuestions());
    boiteTetes->setEtatTete(abeStateMachineGetNumExercice(), getAbeExerciceEvaluation());
    imagetete.append(boiteTetes->suffixe->value(getAbeExerciceEvaluation()));
    AbulEduCommonStatesV1::slotBilanExerciceEntered();
    QString motReponse;
    QString motQuestion;
    if(m_score > 1){
        motReponse = trUtf8("réponses correctes");
    }
    else{
        motReponse = trUtf8(("réponse correcte"));
    }
    if(getAbeNbTotalQuestions() > 1){
        motQuestion = trUtf8("questions");
    }
    else {
        motQuestion = trUtf8("question");
    }
    QString message = trUtf8("%1 %2 sur %3 %4.").arg(QString::number(m_score)).arg(motReponse).arg(QString::number(getAbeNbTotalQuestions())).arg(motQuestion);
    getAbeExerciceMessageV1()->abeWidgetMessageSetTitre(trUtf8("Bilan de l'exercice"));

    //    En attendant d'avoir trouvé le souci sur get ou setEtatTete, je construis imagetete plus haut
    //    QString imagetete = QString(":/"+m_prefixeTetes+"/"+boiteTetes->suffixe->value(boiteTetes->getEtatTete(abeStateMachineGetNumExercice())));
    getAbeExerciceMessageV1()->abeWidgetMessageSetConsigne(QString("<center><img src=")+imagetete+QString("/><br>")
                                                           + message + QString("</center>"));
    getAbeExerciceMessageV1()->abeWidgetMessageResize();
    getAbeExerciceMessageV1()->abeWidgetMessageSetZoneTexteVisible(false);

    getAbeExerciceMessageV1()->move((getAbeExerciceAireDeTravailV1()->width() - getAbeExerciceMessageV1()->width())/2,
                                    ((getAbeExerciceAireDeTravailV1()->height() - getAbeExerciceMessageV1()->height())/2) - 200*abeApp->getAbeApplicationDecorRatio());
    getAbeExerciceMessageV1()->setVisible(true);

    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf", QSettings::IniFormat);
    config.beginGroup(m_operationName);
    if (m_score == m_total) {
        m_niveau++;
        config.setValue("NiveauEnCours"+m_operationName,m_niveau);
    }
    else if(m_score < m_total*0.2 && m_niveau > 1){
        AbulEduMessageBoxV1* goDown = new AbulEduMessageBoxV1(trUtf8("Trop difficile ?"),trUtf8("Tu as fait beaucoup d'erreurs. Veux-tu redescendre au niveau en dessous ?"),true,getAbeExerciceAireDeTravailV1());
        goDown->abeSetModeEnum(AbulEduMessageBoxV1::abeYesNoButton);
        connect(goDown, SIGNAL(signalAbeMessageBoxYES()), SLOT(slotDecreaseLevel()),Qt::UniqueConnection);
        goDown->show();
    }

    //m_level = config.value("NiveauEnCours"+opCourante).toString();

    config.endGroup();
}

bool AbstractExercise::eventFilter(QObject *obj, QEvent *event)
{
    /* Pas localDebug car il y a trop de message */
    if (event->type() == QEvent::KeyRelease && !m_isAdjourned)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        /* Protection contre les clics frénétiques */
        if(!keyEvent->isAutoRepeat())
        {
            /* Navigation avec la touche Entrée : l'appui sur la touche Entrée provoque : */
            switch(keyEvent->key())
            {
            case Qt::Key_Enter:
            case Qt::Key_Return:
                /* Dans les états question et continueQuestion la Vérification ou la Solution si Ctrl est appuyé conjointement */
                if(sequenceMachine->configuration().contains(question))
                {
                        getAbeExerciceTelecommandeV1()->ui->btnVerifier->click();
                }
                /* La sortie en deux temps dans l'état bilanSequence */
                else if(sequenceMachine->configuration().contains(bilanSequence))
                        {
                            if(getAbeExerciceTelecommandeV1()->ui->framePopupQuitter->isVisible())
                                getAbeExerciceTelecommandeV1()->ui->btnQuitterRetourMenuPrincipal->click();
                            else
                                getAbeExerciceTelecommandeV1()->ui->btnQuitter->click();
                        }
                else if(sequenceMachine->configuration().contains(afficheVerificationQuestion))
                {
                    if(keyEvent->modifiers() & Qt::ControlModifier ){
                        getAbeExerciceTelecommandeV1()->ui->btnCorriger->click();
                    }
                    else{
                        getAbeExerciceTelecommandeV1()->ui->btnSuivant->click();
                    }
                }
                else
                /* Le passage à la Suite dans les autres états */
                {
                    getAbeExerciceTelecommandeV1()->ui->btnSuivant->click();
                }
                break;
            default:
                break;
            }
        }
        return true;
    }
    else
    {
        /* On fait suivre l'évènement, sinon tout est bloqué */
        return QObject::eventFilter(obj, event);
    }
}

void AbstractExercise::slotSetPeculiarity()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    m_niveau = -1;
    AbulEduFlatBoutonV1* fromBtn = (AbulEduFlatBoutonV1*) sender();
    if(fromBtn->property("peculiarity").type() == QVariant::String){
        m_operationName = m_operationName.append(fromBtn->property("peculiarity").toString());
        setAbeExerciceName(getAbeExerciceName()+fromBtn->text());
        setAbeSkill(getAbeSkill()+fromBtn->text());
    }
    else if(fromBtn->property("peculiarity").type() == QVariant::Int){
        m_cible = fromBtn->property("peculiarity").toInt();
        m_operationName = m_operationName.append(fromBtn->property("peculiarity").toString());
        setAbeExerciceName(getAbeExerciceName()+QString::number(m_cible));
        setAbeSkill(getAbeSkill()+QString::number(m_cible));
    }
    else if(fromBtn->property("peculiarity").type() == QVariant::List){
        QVariantList vList = fromBtn->property("peculiarity").toList();
        m_multipleCible.clear();
        foreach(QVariant v,vList){
            m_multipleCible << v.toInt();
        }
        /** @todo voir là dessous */
//        m_operationName = m_operationName.append(fromBtn->property("peculiarity").toString());
//        setAbeExerciceName(getAbeExerciceName()+QString::number(m_cible));
//        setAbeSkill(getAbeSkill()+QString::number(m_cible));
    }
    else{
        ABULEDU_LOG_DEBUG() << "Problème : le paramètre transmis n'est pas conforme...";
        return;
    }
    bool isBtnSuivantEnable = getAbeExerciceTelecommandeV1()->ui->btnSuivant->isEnabled();
    getAbeExerciceTelecommandeV1()->ui->btnSuivant->setEnabled(true);
    getAbeExerciceTelecommandeV1()->ui->btnSuivant->click();
    getAbeExerciceTelecommandeV1()->ui->btnSuivant->setEnabled(isBtnSuivantEnable);
}

void AbstractExercise::slotNumericPadKeyPressed(Qt::Key key)
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__<<key;
    }
    /* Astuce : X-48 donne la valeur du nombre envoyé par Qt::Key_X */
    QString num = QString::number(key-48);
    /* J'aurais bien aimé simplement poster un événement avec la key mais ça ne marche pas, je pense parce que le QLineEdit perd le focus */
    //    abeApp->postEvent(abeApp,new QKeyEvent(QEvent::KeyRelease,key,Qt::NoModifier));
    QString value = m_leResultat->text().simplified();
    if(key == Qt::Key_Enter){
        abeApp->postEvent(abeApp,new QKeyEvent(QEvent::KeyRelease,key,Qt::NoModifier));
    }
    else if(key == Qt::Key_Backspace){
        if(!value.isEmpty()){
            value.chop(1);
        }
        m_leResultat->setText(value);
    }
    else{
        m_leResultat->setText(value+num);
    }
}
