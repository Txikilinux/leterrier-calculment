/** Classe de base pour les exercices du logiciel Calcul mental
  * Modification importante par rapport à la première implémentation pour hériter d'AbulEduStateMachineV1
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

#include "exerciceoperation.h"

ExerciceOperation::ExerciceOperation(QString exerciseName,QWidget *parent,int val, int niveau) :
    AbstractExercise(parent),
    m_parent(parent),
    m_minG(0),
    m_maxG(9),
    m_minD(0),
    m_maxD(9)
{
    m_cible = val;
    m_multipleCible = QList<int>();
    factorisation(exerciseName,niveau);
}

ExerciceOperation::ExerciceOperation(QString exerciseName, QWidget *parent, QList<int> values, int niveau) :
    AbstractExercise(parent),
    m_parent(parent),
    m_minG(0),
    m_maxG(9),
    m_minD(0),
    m_maxD(9)
{
    m_cible = -1;
    m_multipleCible = values;
    factorisation(exerciseName,niveau);
}

void ExerciceOperation::factorisation(QString exerciseName, int niveau)
{
    m_localDebug = true;
    m_numberUsed.clear();
    m_operationName = exerciseName;
    m_niveau = niveau;
    if(m_niveau > -1){
        setAbeLevel(QString::number(m_niveau-1));
    }
    m_score = 0;
    chargerParametres();
    setNameAndSkill();
    m_pdfExport = new AbulEduExportPDFV1();
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()<<exerciseName;
        ABULEDU_LOG_DEBUG()<<niveau;
        ABULEDU_LOG_DEBUG()<<m_cible;
        ABULEDU_LOG_DEBUG()<<m_multipleCible;
    }
}

void ExerciceOperation::setNameAndSkill()
{
    if (m_operationName == "complementA")
    {
        setAbeExerciceName(trUtf8("Complément additif à "));
        /* Skill non existant dans les competences Educ Nat */
    }

    if(m_operationName == "complementM")
    {
        setAbeExerciceName(trUtf8("Multiples de "));
        setAbeSkill("multiples-");
    }

    if (m_operationName == "tableM")
    {
        setAbeExerciceName(trUtf8("Table de multiplication par "));
        setAbeSkill("table-multiplication-");
    }

    if (m_operationName == "tableA")
    {
        setAbeExerciceName(trUtf8("Table d'addition de "));
        setAbeSkill("table-addition-");
    }

    if (m_operationName=="addition")
    {
        setAbeExerciceName(trUtf8("Additions de nombres inférieurs à %1 et %2").arg(QString::number(m_maxG)).arg(QString::number(m_maxD)));
        if (((m_maxD == 100) && (m_maxG == 100)) || ((m_maxD == 1000) && (m_maxG == 1000)))
            setAbeSkill("somme-mental-inferieur-"+QString::number(m_maxG));
        /* si je veux que la compétence soit validée, je dois mettre dans l'éditeur la valeur des deux max à 100 ou 1000 */
    }

    if (m_operationName=="soustraction")
    {
        setAbeExerciceName(trUtf8("Soustractions de nombres inférieurs à %1 et %2").arg(QString::number(m_maxG)).arg(QString::number(m_maxD)));
        if (((m_maxD == 100) && (m_maxG == 100)) || ((m_maxD == 1000) && (m_maxG == 1000)))
            setAbeSkill("difference-mental-inferieur-"+QString::number(m_maxG));
        // si je veux que la compétence soit validée, je dois mettre dans l'éditeur la valeur des deux max à 100 ou 1000
    }

    if (m_operationName=="multiplication")
    {
        setAbeExerciceName(trUtf8("Multiplications de nombres inférieurs à %1 et %2").arg(QString::number(m_maxG)).arg(QString::number(m_maxD)));
        if (((m_maxD == 100) && (m_maxG == 100)) || ((m_maxD == 1000) && (m_maxG == 1000)))
            setAbeSkill("produit-mental-inferieur-"+QString::number(m_maxG));
        // si je veux que la compétence soit validée, je dois mettre dans l'éditeur la valeur des deux max à 100 ou 1000
    }

    /** @deprecated : servait quand on mettait un fond personnalisé */
    QString exerciseName;
    if(m_operationName.left(10) =="OdGrandeur"){
        setHelpText("Tu dois trouver l'ordre de grandeur du résultat du calcul proposé.<br/>Pour cela, tu vas arrondir les nombres en ne gardant qu'un seul chiffre significatif, puis faire l'opération sur les nombres arrondis.<br/>Exemple : 372 - 198 -> 400 - 200 = 200<br/>Attention : n'arrondis pas les nombres à 1 seul chiffre");
        if(m_operationName == "OdGrandeur"){
            setAbeExerciceName(trUtf8("Ordres de grandeur sur des "));
            setAbeSkill("ordre-grandeur-");
        }
        else if(m_operationName == "OdGrandeurAddition"){
            setAbeExerciceName(trUtf8("Ordres de grandeur sur des additions"));
            setAbeSkill("ordre-grandeur-addition");
            exerciseName ="OdGrandeur"; /* Pour appel de l'image de fond */
        }
        else if(m_operationName == "OdGrandeurSoustraction"){
            setAbeExerciceName(trUtf8("Ordres de grandeur sur des soustractions"));
            setAbeSkill("ordre-grandeur-soustraction");
            exerciseName ="OdGrandeur";
        }
        else if(m_operationName == "OdGrandeurMultiplication"){
            setAbeExerciceName(trUtf8("Ordres de grandeur sur des multiplications"));
            setAbeSkill("ordre-grandeur-multiplication");
            exerciseName ="OdGrandeur";
        }
        else if(m_operationName == "OdGrandeurDivision"){
            setAbeExerciceName(trUtf8("Ordres de grandeur sur des divisions"));
            setAbeSkill("ordre-grandeur-division");
            exerciseName ="OdGrandeur";
        }
    }

    if (m_operationName == "maisonDesNombres")
    {
        setAbeExerciceName(trUtf8("La maison des nombres"));
        /** @todo A priori Skill non existant dans les competences Educ Nat mais faut vérifier */
    }
//    QPixmap imageFond;

//    if(m_localDebug){
//        ABULEDU_LOG_DEBUG()<<":/calculment/backgrounds/"+exerciseName;
//    }
//    imageFond.load(":/calculment/backgrounds/"+exerciseName);
//    m_imageFond = new QPixmap(imageFond.scaledToHeight(m_parent->height()));

    }

ExerciceOperation::~ExerciceOperation()
{
    pushAbulEduLogs(getAbeExerciceName());
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
    AbstractExercise::setDimensionsWidgets(ratio);
    if(m_operationName == "tableA"){
        m_AireDeJeu->setBackgroundBrush(QBrush(QPixmap(":/calculment/backgrounds/multiplication").scaledToWidth(m_AireDeJeu->width())));
    }
    else {
        m_AireDeJeu->setBackgroundBrush(QBrush(QPixmap(":/calculment/backgrounds/"+m_operationName).scaledToWidth(m_AireDeJeu->width())));
    }

}

QList<int> ExerciceOperation::getNumberUsed() const
{
    return m_numberUsed;
}

void ExerciceOperation::setNumberUsed(const QList<int> &numberUsed)
{
    m_numberUsed = numberUsed;
}

void ExerciceOperation::addNumberUsed(int number)
{
    m_numberUsed << number;
}

void ExerciceOperation::chargerParametres()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__<<":: niveau "<<m_niveau;
    }
    if(!QFile(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf").exists()){
        Editeur::initialiser();
        if(m_localDebug){
            ABULEDU_LOG_DEBUG()  << "Le fichier de conf n'existait pas : je le crée à la volée ";
        }
    }
    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf", QSettings::IniFormat);
    setAbeNbTotalQuestions(config.value("NombreBallons",10).toInt());
    config.beginGroup(m_operationName);
    if (m_niveau == -1) {
        m_niveau = config.value("NiveauEnCours"+m_operationName,"1").toInt();
        setAbeLevel(QString::number(m_niveau-1));
    }
    else{
        if(m_localDebug){
            ABULEDU_LOG_DEBUG()  << "Dans chargerParametres(), m_level valait déjà "<<m_niveau;
        }
    }
    config.beginGroup(QString::number(m_niveau));
    m_maxG = config.value("MaxGauche",100).toInt();
    m_minG = config.value("MinGauche",0).toInt();
    if(m_operationName == "OdGrandeurDivision"){
        QStringList vL = config.value("MaxDroite").toString().split(";",QString::SkipEmptyParts);
        foreach(QString v,vL){
            m_multipleCible << v.toInt();
        }
        ABULEDU_LOG_DEBUG()<<m_multipleCible;
    }
    m_maxD = config.value("MaxDroite",100).toInt();
    m_minD = config.value("MinDroite",0).toInt();
    m_temps = config.value("TempsAccorde",10).toInt();
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
        ABULEDU_LOG_DEBUG()<<"Lecture des paramètres dans "<<config.fileName()<<" - "<<m_operationName<<" - "<<getAbeLevel();
        ABULEDU_LOG_DEBUG() << "MaxGauche : " << m_maxG << "MinGauche : " << m_minG << "MaxDroite : " << m_maxD << "MinDroite : " << m_minD<< "Mon niveau : "<<getAbeLevel()<<"Tps : "<<m_temps;
    }
    config.endGroup();
    config.endGroup();

    if (m_operationName.left(6)=="tableA" || m_operationName.left(6)=="tableM") {
        m_minD = m_maxD = m_cible;
        m_minG=0;
        m_maxG=9;
    }
    else if(m_operationName.left(11) == "complementA"
            || m_operationName.left(11) == "complementM"){
        m_minG = m_maxG = m_minD = m_maxD = m_cible;
    }
    setAbeNbTotalQuestions(config.value("NombreBallons").toInt());
    getAbeExerciceTelecommandeV1()->ui->lblCustom2->setText(QString::number(m_score)+ " sur "+QString::number(getAbeNbTotalQuestions()));
}

void ExerciceOperation::animeBaudruche()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    float ratio = abeApp->getAbeApplicationDecorRatio();
    QGraphicsItemAnimation *animation = new QGraphicsItemAnimation(m_sceneAireDeJeu);
    animation->setItem(m_baudruche);
    animation->setTimeLine(m_baudruche->m_timer);
    if(m_operationName == "division"){
//        m_AireDeJeu->setStyleSheet("border:2px solid blue");
//        m_AireDeJeu->move(m_AireDeJeu->x()+200,m_AireDeJeu->y());
        for (int i = 1; i < 200; i++){
            /* Code utilisé dans des tests pour faire tomber la voiture comme sur une rampe de grand huit */
//            animation->setPosAt(i/200.0, /*QPointF(m_depart->x(),m_depart->y())+*/QPointF((4*i*ratio) ,(log(i)*100*ratio)));
            animation->setPosAt(i/200.0, QPointF((5.2*i*ratio) ,0 ));
        }
    }
    else if (m_operationName == "addition") {
        for (int i = 0; i < 200; i++)
            animation->setPosAt(i/200.0, QPointF((4.2*i*ratio) ,0 ));
    }
    else if (m_operationName == "multiplication") {
        for (int i = 0; i < 200; i++)
            animation->setPosAt(i/200.0, QPointF(0 , (-2.4*i*ratio)));
    }
    else if(m_operationName.left(6) == "tableA") {
        for (int i = 0; i < 200; i++)
            animation->setPosAt(i/200.0, QPointF(3.8*i*ratio, 2.5*i*ratio));
    }
    else if(m_operationName.left(6) == "tableM") {
        for (int i = 0; i < 200; i++)
            animation->setPosAt(i/200.0, QPointF(0 , 2.8*i*ratio));
    }
    else if (m_operationName.left(11) == "complementA") {
        for (int i = 0; i < 200; i++)
            animation->setPosAt(i/200.0, QPointF(0 , (-2.6*i*ratio)));
    }
    else if (m_operationName.left(11) == "complementM") {
        for (int i = 0; i < 200; i++)
            animation->setPosAt(i/200.0, QPointF(0 , (-2.9*i*ratio)));
    }
    else for (int i = 0; i < 200; i++)
        //animation->setPosAt(i/200.0, QPointF(0 , (-3*i)-(i*0.8)));
        animation->setPosAt(i/200.0, QPointF(0 , (-2.5*i*ratio)));
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
    if(m_operationName == "OdGrandeur"){
        presentationSequence->assignProperty(getAbeExerciceTelecommandeV1()->ui->btnAide, "enabled",true);
        afficheVerificationQuestion->assignProperty(getAbeExerciceTelecommandeV1()->ui->btnAide, "enabled",true);
    }
}

void ExerciceOperation::slotPresenteSequenceEntered()
{
    if(m_operationName == "tableM"){
        m_variations.append(AbulEduLaunchElements("2",":/calculment/elements/aie1",2));
        m_variations.append(AbulEduLaunchElements("3",":/calculment/elements/aie2",3));
        m_variations.append(AbulEduLaunchElements("4",":/calculment/elements/aie3",4));
        m_variations.append(AbulEduLaunchElements("5",":/calculment/elements/aie1",5));
        m_variations.append(AbulEduLaunchElements("6",":/calculment/elements/aie2",6));
        m_variations.append(AbulEduLaunchElements("7",":/calculment/elements/aie3",7));
        m_variations.append(AbulEduLaunchElements("8",":/calculment/elements/aie1",8));
        m_variations.append(AbulEduLaunchElements("9",":/calculment/elements/aie2",9));
        m_variations.append(AbulEduLaunchElements(trUtf8("Choix multiple"),":/calculment/elements/aie2",-1));
        QList<QVariant> chiffres;
        chiffres <<2<<3<<4<<5<<6<<7<<8<<9;
        m_variations.append(AbulEduLaunchElements(trUtf8("Tous ces nombres"),":/calculment/elements/aie3",chiffres));
    }
    else if(m_operationName == "complementA"){
        m_variations.append(AbulEduLaunchElements("10",":/calculment/elements/aie1",10));
        m_variations.append(AbulEduLaunchElements("100",":/calculment/elements/aie2",100));
        m_variations.append(AbulEduLaunchElements("1000",":/calculment/elements/aie3",1000));
        /** @todo Ajouter la maison des nombres */
    }
    else if(m_operationName == "complementM"){
        m_variations.append(AbulEduLaunchElements("5",":/calculment/elements/aie1",5));
        m_variations.append(AbulEduLaunchElements("10",":/calculment/elements/aie2",10));
        m_variations.append(AbulEduLaunchElements("15",":/calculment/elements/aie3",15));
        m_variations.append(AbulEduLaunchElements("20",":/calculment/elements/aie1",20));
        m_variations.append(AbulEduLaunchElements("25",":/calculment/elements/aie2",25));
        m_variations.append(AbulEduLaunchElements("50",":/calculment/elements/aie3",50));
    }
    else if(m_operationName == "tableA"){
        m_variations.append(AbulEduLaunchElements("2",":/calculment/elements/aie1",2));
        m_variations.append(AbulEduLaunchElements("3",":/calculment/elements/aie2",3));
        m_variations.append(AbulEduLaunchElements("4",":/calculment/elements/aie3",4));
        m_variations.append(AbulEduLaunchElements("5",":/calculment/elements/aie1",5));
        m_variations.append(AbulEduLaunchElements("6",":/calculment/elements/aie2",6));
        m_variations.append(AbulEduLaunchElements("7",":/calculment/elements/aie3",7));
        m_variations.append(AbulEduLaunchElements("8",":/calculment/elements/aie1",8));
        m_variations.append(AbulEduLaunchElements("9",":/calculment/elements/aie2",9));
        m_variations.append(AbulEduLaunchElements(trUtf8("Choix multiple"),":/calculment/elements/aie2",-1));
        QList<QVariant> chiffres;
        chiffres <<2<<3<<4<<5<<6<<7<<8<<9;
        m_variations.append(AbulEduLaunchElements(trUtf8("Tous ces nombres"),":/calculment/elements/aie3",chiffres));
    }
    else if(m_operationName == "OdGrandeur"){
        m_variations.append(AbulEduLaunchElements(trUtf8("Additions"),":/calculment/elements/nausee1","Addition"));
        m_variations.append(AbulEduLaunchElements(trUtf8("Soustractions"),":/calculment/elements/nausee2","Soustraction"));
        m_variations.append(AbulEduLaunchElements(trUtf8("Multiplications"),":/calculment/elements/nausee3","Multiplication"));
        m_variations.append(AbulEduLaunchElements(trUtf8("Divisions"),":/calculment/elements/nausee1","Division"));
    }
    else if(m_operationName == "maisonDesNombres"){
        m_variations.append(AbulEduLaunchElements(trUtf8("de 1 à 10"),":/calculment/elements/nausee1",0));
        m_variations.append(AbulEduLaunchElements(trUtf8("de 11 à 20"),":/calculment/elements/nausee2",10));
    }
    else {
        ABULEDU_LOG_DEBUG()  << "Problème : je ne devrais pas pouvoir arriver ici ...";
    }
    AbstractExercise::slotPresenteSequenceEntered();
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
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__<<m_operationName;
//        ABULEDU_LOG_DEBUG() << sequenceMachine->configuration().toList();
        ABULEDU_LOG_DEBUG() <<m_total<<getAbeNbTotalQuestions()<<getAbeNumQuestion()<<m_score;
    }
    AbstractExercise::slotInitQuestionEntered();

    float ratio = abeApp->getAbeApplicationDecorRatio();
    //instanciation d'une baudruche et connexion aux autres objets
    if (m_operationName == "addition"){
        m_depart = new QPoint(0,188*ratio);
    }
    else if(m_operationName.left(6)=="tableM"){
        m_depart = new QPoint(m_AireDeJeu->width()/2-120*ratio,-70*ratio);
    }
    else if(m_operationName.left(6) == "tableA"){
        m_depart = new QPoint(100*ratio,5*ratio);
    }
    else if(m_operationName == "division"){
        m_depart = new QPoint(50*ratio,190*ratio);
    }
    else if(m_operationName == "multiplication"){
        m_depart = new QPoint(m_AireDeJeu->width()/2 - 160*ratio,460*ratio);
    }
    else if(m_operationName.left(11) == "complementA"){
        m_depart = new QPoint(m_AireDeJeu->width()/2 + 80*ratio,500*ratio);
    }
    else if(m_operationName.left(11) == "complementM"){
        m_depart = new QPoint(m_AireDeJeu->width()/2 -250*ratio,550*ratio);
    }
    else m_depart = new QPoint(m_AireDeJeu->width()/2-80*ratio,500*ratio);

    //m_depart = new QPoint(m_ui->vue->width()/2,0); --> pour la faire tomber
    if (m_operationName=="addition"){
        m_baudruche = new baudruche(m_minG,m_maxG,m_minD,m_maxD,m_temps,m_operationName,*m_depart,this,"auto");
    }
    else if(m_operationName==""
            || m_operationName=="soustraction"){
        m_baudruche = new baudruche(m_minG,m_maxG,m_minD,m_maxD,m_temps,m_operationName,*m_depart,this);
    }
    else if(m_operationName == "multiplication"){
        m_baudruche = new baudruche(m_minG,m_maxG,m_minD,m_maxD,m_temps,m_operationName,*m_depart,this,"ovni");
    }
    else if(m_operationName == "division"){
        m_baudruche = new baudruche(m_minG,m_maxG,m_minD,m_maxD,m_temps,m_operationName,*m_depart,this,"voiture");
    }
    else if (m_operationName.left(6)=="tableA"){
        if(!m_multipleCible.isEmpty()){
            /** L'idée est de piocher dans la liste des nombres attendus */
            m_minD = m_maxD = m_multipleCible.at(rand()%m_multipleCible.size());
        }
        m_baudruche = new baudruche(m_minG,m_maxG,m_minD,m_maxD,m_temps,m_operationName.left(6),*m_depart,this,"ovni");
    }
    else if(m_operationName.left(6)=="tableM"){
        if(!m_multipleCible.isEmpty()){
            /** L'idée est de piocher dans la liste des nombres attendus */
            m_minD = m_maxD = m_multipleCible.at(rand()%m_multipleCible.size());
        }
        m_baudruche = new baudruche(m_minG,m_maxG,m_minD,m_maxD,m_temps,m_operationName.left(6),*m_depart,this,"cabine");
    }
    else if (m_operationName.left(11)=="complementA")
        m_baudruche = new baudruche(m_minG,m_temps,m_operationName.left(11), *m_depart,this,"fantome");
    else if (m_operationName.left(11)=="complementM")
        m_baudruche = new baudruche(m_minG,m_temps,m_operationName.left(11), *m_depart,this);

    else if (m_operationName=="OdGrandeurAddition"
             || m_operationName=="OdGrandeurSoustraction"
             || m_operationName=="OdGrandeurMultiplication"
             || m_operationName == "OdGrandeurDivision")
    {
        m_baudruche = new baudruche(m_maxG,m_maxD,m_temps,m_operationName, *m_depart,this);
    }

//    else  QMessageBox::critical(this, trUtf8("Opération inexistante"), m_operationName.append(trUtf8(", ça n'existe pas comme opération...")));

    //          else {ABULEDU_LOG_DEBUG()<< "Pas d'opération portant le nom de "<<m_operationName;}//Pourquoi quand même erreur de segmentation
    if (QString::compare(m_operationName, "OdGrandeurAddition", Qt::CaseInsensitive) == 0
            || QString::compare(m_operationName, "OdGrandeurSoustraction", Qt::CaseInsensitive) == 0
            || QString::compare(m_operationName, "OdGrandeurMultiplication", Qt::CaseInsensitive) == 0
            || QString::compare(m_operationName, "OdGrandeurDivision", Qt::CaseInsensitive) == 0) this->m_resultatEnCours = m_baudruche->getMApproximation().first();
    else this->m_resultatEnCours = m_baudruche->getMResultat();
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
    if(m_baudruche->getMApproximation().size() == 3){
        m_roundedOperands = QPair<int,int>(m_baudruche->getMApproximation().at(1),m_baudruche->getMApproximation().at(2));
    }
    m_operands = QPair<float,float>(m_baudruche->getMGOperande(),m_baudruche->getMDOperande());
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()<<"Calcul propose : "<<m_trace;
    }

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
//        ABULEDU_LOG_DEBUG() << sequenceMachine->configuration().toList();
    }
    AbstractExercise::slotQuestionEntered();
}

void ExerciceOperation::slotAfficheVerificationQuestionEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
//        ABULEDU_LOG_DEBUG() << sequenceMachine->configuration().toList();
    }
    AbstractExercise::slotAfficheVerificationQuestionEntered();
    if(m_leResultat->text().simplified().isEmpty()){
        setAbeExerciceEvaluation(abe::evalZ);
        boiteTetes->setEtatTete(m_numExercice, getAbeExerciceEvaluation(),false,getAbeNbTotalQuestions()-getAbeNumQuestion()+1);
        getAbeExerciceTelecommandeV1()->ui->btnCorriger->setEnabled(true);
        /* On va logger l'appui sur Vérification en cas de champ de saisie vide comme une absence de réponse (cf temps écoulé)
         * On pourra peut-être plutôt empêcher la vérification si le champs est vide mais pour l'instant on fait comme ça */
        m_answers.append(QPair<QVariant,QVariant>(trUtf8("Non répondu"),m_resultatEnCours));
    }
    else {
        float proposition = m_leResultat->text().toFloat();
        m_answers.append(QPair<QVariant,QVariant>(proposition,m_resultatEnCours));
        if(m_localDebug){
            ABULEDU_LOG_DEBUG()<<"Valeur du ballon : "<<m_resultatEnCours<<", lache sur "<<proposition;
        }
        QString demande = "";
        demande = m_baudruche->getMGOperande()+m_baudruche->getMOperation()+m_baudruche->getMDOperande();
        if (proposition == m_resultatEnCours) {
            m_score++;
            setAbeExerciceEvaluation(abe::evalA);
        }
        else {
            ajouteErreur("Erreur calcul");
            setAbeExerciceEvaluation(abe::evalD);
            getAbeExerciceTelecommandeV1()->ui->btnCorriger->setEnabled(true);
        }
    }
    getAbeExerciceTelecommandeV1()->ui->lblCustom2->setText(QString::number(m_score)+ " sur "+QString::number(getAbeNbTotalQuestions()));

    if (m_baudruche && !m_baudruche->getBaudrucheIsDetructionPlanified()){
            m_baudruche->detruire();
    }

    if (m_total == getAbeNbTotalQuestions()) {
        //erics 20110209 sinon coredump en cas de "aucune erreur" ou "que des erreurs"
        //if (m_baudruche) delete m_baudruche;
        /** @todo remplacer la ligne en dessous par le bilan d'exercice */
        if(qApp->property("afficheBilanExercice").toBool())
        {
            ABULEDU_LOG_DEBUG()<<qApp->property("utilisateur").toString();
            m_pdfExport->abeExportPDFSetLogin(qApp->property("utilisateur").toString());
            m_pdfExport->abeExportPDFSetSoftware("Calcul Mental");
            m_pdfExport->abeExportPDFSetLogs(getPluginLogs());
            m_pdfExport->abeExportPDFFile();
        }
        qApp->setProperty("afficheBilanExercice",false);

        /** @todo discuter de la pertinence de l'augmentation automatique de niveau */
        //mise à jour ou pas du niveau
        QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf", QSettings::IniFormat);
        config.beginGroup(m_operationName);
        if (m_score == m_total) {
            if (getAbeLevel() == "1") config.setValue("NiveauEnCours"+m_operationName, "2");
            else if (getAbeLevel() == "2") config.setValue("NiveauEnCours"+m_operationName, "3");
            else if (getAbeLevel() == "3") config.setValue("NiveauEnCours"+m_operationName, "4");
        }


        //m_level = config.value("NiveauEnCours"+opCourante).toString();

        config.endGroup();
    }
    boiteTetes->setEtatTete(m_numExercice, getAbeExerciceEvaluation(),false,getAbeNbTotalQuestions()-getAbeNumQuestion()+1);
    /* On ajoute une ligne de log */
    if (m_answers.count() > 0)
    {
        setAbeLineLog(m_trace,
                      m_answers.last().first.toString(),
                      m_score,
                      m_total,
                      getAbeExerciceEvaluation(),
                      m_answers.last().second.toString());
    }
    if (m_localDebug)
    {
        if (m_answers.count() > 0)
        {
            ABULEDU_LOG_DEBUG() <<__PRETTY_FUNCTION__<<" --> Correction : Envoi ligne de resultats ---------- par setAbeLineLog  ------------------";
            ABULEDU_LOG_DEBUG()<<m_trace;
            ABULEDU_LOG_DEBUG()<<m_answers.last().first.toString();
            ABULEDU_LOG_DEBUG()<<m_score;
            ABULEDU_LOG_DEBUG()<<m_total;
            ABULEDU_LOG_DEBUG()<<abe::ABE_DONNEEVAL().value(getAbeExerciceEvaluation());
            ABULEDU_LOG_DEBUG()<<m_answers.last().second.toString();
        }
        else
            ABULEDU_LOG_DEBUG()<<"Pas de log à envoyer..."<<abe::ABE_DONNEEVAL().value(getAbeExerciceEvaluation());
    }
}

void ExerciceOperation::slotFinVerificationQuestionEntered()
{
    AbstractExercise::slotFinVerificationQuestionEntered();

}

void ExerciceOperation::slotAfficheCorrectionQuestionEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
//        ABULEDU_LOG_DEBUG() << sequenceMachine->configuration().toList();
    }
    AbstractExercise::slotAfficheCorrectionQuestionEntered();
}

void ExerciceOperation::slotFinCorrectionQuestionEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
//        ABULEDU_LOG_DEBUG() << sequenceMachine->configuration().toList();
    }
    AbstractExercise::slotFinCorrectionQuestionEntered();
}

void ExerciceOperation::slotBilanSequenceEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
//        ABULEDU_LOG_DEBUG() << sequenceMachine->configuration().toList();
    }
    AbstractExercise::slotBilanSequenceEntered();

}

bool ExerciceOperation::eventFilter(QObject *obj, QEvent *event)
{
    return AbstractExercise::eventFilter(obj,event);
}

void ExerciceOperation::ajouteErreur(QString msg)
{
    Q_UNUSED(msg)
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
//        ABULEDU_LOG_DEBUG() << sequenceMachine->configuration().toList();
    }
    if(m_leResultat->text().simplified().isEmpty()){
        sequenceMachine->postEvent(new StringEvent("QuestionVerifieEmpty"));
    }
    m_listeEchecs.append(QString::number(m_baudruche->getMGOperande())+";"+m_baudruche->getMOperation()+";"+QString::number(m_baudruche->getMDOperande())+";"+QString::number(m_resultatEnCours)+";"+m_baudruche->m_nomImage);
}
