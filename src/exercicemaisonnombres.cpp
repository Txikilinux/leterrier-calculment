/** Type particulier d'exercice pour le logiciel Calcul Mental
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/LeTerrierExercice
  * @author 2011-2015 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
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

#include "exercicemaisonnombres.h"
#include "ui_exercice.h"
#include "pixmapmaison.h"

ExerciceMaisonNombres::ExerciceMaisonNombres(QString exo,QWidget *parent,int val, int niveau)
    :ExerciceOperation(exo, parent,val,niveau)
{
    m_localDebug = true;
    if (m_localDebug){
        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__;
    }
    m_AireDeJeu->setInteractive(true);
    m_operationName = exo;
    m_valeurBase = val;
    float ratio = abeApp->getAbeApplicationDecorRatio();
    m_depart = new QPoint(m_AireDeJeu->width()/2-80*ratio,500*ratio);
    chargerParametres();
}

void ExerciceMaisonNombres::dessinePixmapMaisons()
{
    if (m_localDebug){
        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__;
    }
    float ratio = abeApp->getAbeApplicationDecorRatio();
    int ordonneMaison = 0;
    int nombreMaisons = 5;
    for (int i=1;i<=10;i++)
    {
        QPixmap dessinBouton (":/calculment/elements/maison"+QString::number(i+m_valeurBase));
        QPixmap dessinBouton2 = dessinBouton.scaled(dessinBouton.width()*ratio, dessinBouton.height()*ratio, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        PixmapMaison* maison = new PixmapMaison(dessinBouton2);
        maison->setToolTip("Maison du "+QString::number(i+m_valeurBase));
        maison->setProperty("Valeur",i+m_valeurBase);
        ordonneMaison = qFloor((i-1)/2)*(dessinBouton2.height()-1 + ((m_AireDeJeu->height()-nombreMaisons*dessinBouton2.height())/nombreMaisons-1));
        maison->setPos(((1+qPow(-1,i))/2)*(m_AireDeJeu->width() - dessinBouton2.width()),ordonneMaison);
        m_sceneAireDeJeu->addItem(maison);
    }
}

void ExerciceMaisonNombres::on_btn2chance_clicked()
{
    /** @deprecated */
}

void ExerciceMaisonNombres::affichePosBaudruche(QPoint point)
{
    if (m_localDebug){
        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__<<point;
    }

    if (m_sceneAireDeJeu->itemAt(point,QTransform())!=0)
    {
        m_leResultat->setText(QString::number(m_valeurSurvolee));
        /** Je dois remplacer l'appel de on_btnFeu_clicked();
         *  pour pouvoir appeler click(), il faut que je rende le bouton Enabled le temps du clic, puis je le remets comme il était */
        bool isVerifierEnabled = getAbeExerciceTelecommandeV1()->ui->btnVerifier->isEnabled();
        getAbeExerciceTelecommandeV1()->ui->btnVerifier->setEnabled(true);
        getAbeExerciceTelecommandeV1()->ui->btnVerifier->click();
        getAbeExerciceTelecommandeV1()->ui->btnVerifier->setEnabled(isVerifierEnabled);
        //    qDebug()<<"ExerciceMaisonNombres::affichePosBaudruche : "<<point<<" , Valeur recue : "<<m_scene->itemAt(point)->toolTip()<<" Valeur affichee "<<m_ui->leResultat->text();
    }
}

/* sans doute scorie d'un ancien essai, à voir si on peut supprimer */
void ExerciceMaisonNombres::selectionChanged() {
    if (m_localDebug){
        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__;
    }
    // Affiche la position de chaque élément de la sélection
    foreach(QGraphicsItem * item, m_sceneAireDeJeu->selectedItems()) {
        if (m_localDebug){
            qDebug() << item->scenePos();
        }
    }
}

void ExerciceMaisonNombres::mousePressEvent(QMouseEvent *)
{
    /** @todo ce mousePressEvent() n'est jamais appelé... l'événement est-il capté ailleurs ?! Mais est-ce utile qu'il soit appelé ? */
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    if (getAbeExerciceTelecommandeV1()->ui->btnSuivant->isEnabled()){
        sequenceMachine->postEvent(new StringEvent("finAfficheVerification"));
    }
}

void ExerciceMaisonNombres::ajouteErreur(QString msg)
{
    if (m_localDebug){
        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__;
    }
    ExerciceOperation::ajouteErreur(msg);
}

void ExerciceMaisonNombres::trouveMaisonSurvolee(QString bulleAide)
{
    if (m_localDebug){
        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__;
    }
    float ratio = abeApp->getAbeApplicationDecorRatio();
    zeroMaisonSurvolee();
    foreach(QGraphicsItem * item, m_sceneAireDeJeu->items())
    {
        PixmapMaison* itemMaison = static_cast<PixmapMaison*>(item);
        if (itemMaison->toolTip() == bulleAide)
        {
            QPixmap dessinBouton (":/calculment/elements/maison"+QString::number(itemMaison->property("Valeur").toInt())+"b");
            QPixmap dessinBouton2 = dessinBouton.scaled(dessinBouton.width()*ratio, dessinBouton.height()*ratio, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            itemMaison->setPixmap(dessinBouton2);
            m_valeurSurvolee = itemMaison->property("Valeur").toInt();
        }
    }
}

void ExerciceMaisonNombres::zeroMaisonSurvolee()
{
    if (m_localDebug){
        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__<<m_sceneAireDeJeu->items().size();
    }
    m_valeurSurvolee = 0;
    foreach(QGraphicsItem * item, m_sceneAireDeJeu->items())
    {
        qDebug()<<item->toolTip();
        if (item->toolTip().left(6) == "Maison"){
            PixmapMaison* itemMaison = static_cast<PixmapMaison*>(item);
            itemMaison->setPixmap(itemMaison->getMPixmapInitial());
            qDebug()<<"redessin de "<<item->toolTip();
        }
    }
}

void ExerciceMaisonNombres::slotSequenceEntered()
{
    if (m_localDebug){
        ABULEDU_LOG_DEBUG()<<" ------ "<< __PRETTY_FUNCTION__;
    }
    ExerciceOperation::slotSequenceEntered();
    question->assignProperty(getAbeExerciceTelecommandeV1()->ui->btnVerifier, "enabled",false);
    question->assignProperty(m_leResultat, "enabled", false);
}

void ExerciceMaisonNombres::slotRealisationExerciceEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
        ABULEDU_LOG_DEBUG() << getAbeExerciceName();
        ABULEDU_LOG_DEBUG() <<getAbeSkill();
    }
    dessinePixmapMaisons();
    ExerciceOperation::slotRealisationExerciceEntered();
}

void ExerciceMaisonNombres::slotInitQuestionEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
        ABULEDU_LOG_DEBUG() << sequenceMachine->configuration().toList();
        ABULEDU_LOG_DEBUG() <<m_total<<getAbeNbTotalQuestions()<<getAbeNumQuestion()<<m_score;
    }
    /* Attention, ici je shunte AbstractExercise::slotInitQuestionEntered() qui me fait un clearScene() et supprime les maisons. Je copie donc son contenu moins le clearScene() */
    AbulEduCommonStatesV1::slotInitQuestionEntered();
    setAbeExerciceEvaluation(abe::evalY);
    boiteTetes->setEtatTete(m_numExercice, abe::evalY,false,getAbeNbTotalQuestions()-getAbeNumQuestion()+1);

    bool inferieurA11 = false;
    while (!inferieurA11) {
        m_baudruche = new baudruche(0,9,0,9,m_temps,"addition",*m_depart,m_sceneAireDeJeu,"fantome");
        m_valeurSurvolee = 0;
        this->m_resultatEnCours=m_baudruche->getMResultat();
//        qDebug()<<" Ballon créé avec comme résultat "<<m_resultatEnCours<<" et comme parent "<<m_scene<<" euh "<<m_baudruche.data()->parent();
        if (m_resultatEnCours > (10+m_valeurBase) || m_resultatEnCours < (1+m_valeurBase)) {
            m_baudruche->deleteLater();
        }
        else inferieurA11 = true;
    }
    emit baudrucheLancee();
    m_baudruche->setFlag(QGraphicsItem::ItemIsMovable);
//    qDebug()<<"Creation de baudruche avec temps "<<m_temps;
//    qDebug()<<" attribut résultat de l'exercice "<<m_resultatEnCours;
    /** @todo machine à états ... */
//    if (m_total<m_nbTotalQuestions - 1) {
//        connect(m_baudruche, SIGNAL(destroyed(bool)), m_ui->btnBallon, SLOT(setEnabled(bool)));
//        connect(m_baudruche, SIGNAL(destroyed()), m_ui->btnBallon, SLOT(setFocus()));
//    }
//    connect(m_baudruche, SIGNAL(destroyed(bool)), m_ui->btnFeu, SLOT(setDisabled(bool)));
    connect(m_baudruche, SIGNAL(destroyed()), m_leResultat, SLOT(clear()));
    connect(m_baudruche, SIGNAL(destroyed(bool)), m_leResultat, SLOT(setDisabled(bool)));
    connect(m_baudruche->m_timer, SIGNAL(finished()),m_baudruche, SLOT(detruireTps()));
    connect(m_baudruche, SIGNAL(tempsFini(QString)), this, SLOT(ajouteErreur(QString)));
    ///connect(m_baudruche, SIGNAL(tempsFini(QString)), m_ui->lblMsg, SLOT(setText(QString)));
    ///connect(m_baudruche, SIGNAL(tempsFini(QPixmap)), m_ui->lblImgMsg, SLOT(setPixmap(QPixmap)));
    connect(m_baudruche, SIGNAL(tempsFini(QString)), this, SLOT(afficheResultat(QString)));
    connect(m_baudruche, SIGNAL(tempsFini(QString)), this, SLOT(pousseLogs(QString)));
    connect(m_baudruche, SIGNAL(lacheIci(QPoint)), this, SLOT(affichePosBaudruche(QPoint)));

    connect(m_baudruche, SIGNAL(baudrucheSurvole(QString)), this, SLOT(trouveMaisonSurvolee(QString)));
    connect(m_baudruche, SIGNAL(baudrucheSurvoleRien()), this, SLOT(zeroMaisonSurvolee()));

    m_baudruche->emetRes();
    m_sceneAireDeJeu->addItem(m_baudruche);

    m_trace = m_baudruche->getMAffichage();
    qDebug()<<"Calcul propose : "<<m_trace;

    //affichage du nombre de ballons déjà instanciés
    ///m_total = m_ui->lblTotal->text().toInt();
    m_total++;
    /** QString monTotal = "";
    monTotal = monTotal.setNum(m_total);
    m_ui->lblTotal->setText(monTotal);
    m_ui->lblMsg->setText("");
    QPixmap* rien = new QPixmap("");
    m_ui->lblImgMsg->setPixmap(*rien); */

    m_baudruche->m_timer->start();
    ///if (m_baudruche!=NULL) m_ui->btnBallon->setDisabled(true);
}

void ExerciceMaisonNombres::slotSetPeculiarity()
{
    AbulEduFlatBoutonV1* fromBtn = (AbulEduFlatBoutonV1*) sender();
    if(fromBtn->property("peculiarity").type() == QVariant::Int){
        m_valeurBase = fromBtn->property("peculiarity").toInt();
        setAbeExerciceName(getAbeExerciceName()+trUtf8(" de ")+QString::number(m_valeurBase+1)+trUtf8(" à ")+QString::number(m_valeurBase+10));
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
