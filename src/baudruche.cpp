/** Objet qui porte les opérations pour les exercices du logiciel Calcul mental
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/LeTerrierExercice
  * @author 2010-2014 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
  * @author 2010-2012 Eric Seigne <eric.seigne@ryxeo.com>
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

#include "baudruche.h"
#include "exerciceoperation.h"
#include <QApplication>
 #include <QFontMetrics>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>

const int MULTIPLE_MAX=11;

/**
    * Constructeur.
    * Crée un objet composé d'une image et d'une opération proposée
    * @param intMinG est la plus petite valeur que peut prendre l'opérande gauche
    * @param intMaxG est la plus grande valeur que peut prendre l'opérande gauche
    * @param intMinD est la plus petite valeur que peut prendre l'opérande droit
    * @param intMaxD est la plus grande valeur que peut prendre l'opérande droit
    * @param tempsAccorde est le temps de "vie" de l'instance en secondes
    * @param operation est le nom de l'opération, qui va servir à déterminer le QChar signe opératoire contenu dans la chaine de caractère
    * @param pos est la position à laquelle est créé mon objet
    * @param image -initialisé à 0- est le nom (sans chemin, sans extension) de l'image. Tant qu'image vaut 0, c'est un ballon de baudruche
    */

baudruche::baudruche(int intMinG, int intMaxG, int intMinD, int intMaxD, int tempsAccorde, QString operation, QPoint pos, QObject *parent, QString image)
{
    m_localDebug = false;
    m_nomOperation = operation;
    m_nomImage = image;
    m_dropValeur = QString();
    m_approximation = 0;
    m_parent = parent;
    ExerciceOperation* exoParent = (ExerciceOperation*) parent;
    m_isDetructionPlanified = false;
    if (operation=="addition" || operation=="tableA" || operation==""){
        m_op = "+";
    }
    else if (operation=="soustraction"){
        m_op = "-";
    }
    else if (operation=="multiplication" || operation=="tableM"){
        m_op = "x";
    }
    else if(operation == "division"){
        m_op = ":";
    }
    m_position.setX(pos.x());
    m_position.setY(pos.y());

/* Je déplace la détermination de l'opérande droit avant celle du gauche parce que ça m'arrange pour la division, vérifier si c'est pas le bordail ailleurs */
    if (intMinD == intMaxD){
        d_operande = intMaxD;
    }
    else{
        if(operation == "division"){
            do {
                d_operande = intMinD + rand()%(intMaxD-intMinD);
            }
            while(d_operande == 0);
        }
        else {
                d_operande = intMinD + rand()%(intMaxD-intMinD);
        }
    }

    if (intMinG == intMaxG){
        g_operande = intMaxG;
    }
    else{
        if(operation == "division"){
            float reste;
            do {
            g_operande = intMinG + rand()%(intMaxG-intMinG);
            reste = g_operande/d_operande - static_cast<int>(g_operande/d_operande);
            }
            while(reste != 0);
        }
        else {
            QString classe(parent->metaObject()->className());
            if(classe == "ExerciceOperation"){
                bool foundNew = false;
                do {
                    g_operande = intMinG + rand()%(intMaxG-intMinG+1);
                    if(exoParent->getNumberUsed().count(g_operande) < 1 || exoParent->getNumberUsed().size() >= 10){
                        exoParent->addNumberUsed(g_operande);
                        foundNew = true;
                    }
                }
                while(!foundNew);
            }
            else {
                g_operande = intMinG + rand()%(intMaxG-intMinG+1);
            }
        }
    }
    /* Remarque : il existe sans doute une fonction qui retourne le max mais ça me prendrait plus de temps de chercher que d'écrire 3 lignes... */
    if (d_operande > g_operande && operation.left(5) != "table")
    {
        int tmp=g_operande;
        g_operande=d_operande;
        d_operande=tmp;
    }

    /* Calcul du résultat à émettre (Problème si c'est la multiplication : l'utiliteur veut un "x" alors que le calculateur veut un "*") */
    if (m_op=="x"){
        m_ligne = QString::number(g_operande)+"*"+QString::number(d_operande);
    }
    /* Problème aussi si c'est la division : l'utiliteur veut un ":" alors que le calculateur veut un "/") */
    else if(m_op == ":"){
        m_ligne = QString::number(g_operande)+"/"+QString::number(d_operande);
    }
    else {
        m_ligne = QString::number(g_operande)+m_op+QString::number(d_operande);
    }

    QScriptEngine calculateur;
    QScriptValue resultat = calculateur.evaluate(m_ligne);
    m_resultat = resultat.toNumber();
    m_timer = new QTimeLine(tempsAccorde*1000,this);

    construisAffichage();
    dessineMoi(image);
        
    this->emetRes();
    //qDebug()<<"res emis instanciation "<<m_resultat;
    //qDebug()<<"baudruche::constructeur normal (2) avec parent "<<m_parent;
}

/* constructeur spécifique aux valeurs approchées */
baudruche::baudruche(int intMaxG, int intMaxD, int tempsAccorde, QString operation, QPoint pos, QObject *parent, QString image)
{
    m_localDebug = true;
    m_parent = parent;
    m_nomImage = image;
    m_dropValeur = "";
    m_isDetructionPlanified = false;
    m_nomOperation = operation;
    m_approximation = 0;
        if (operation=="OdGrandeurAddition") m_op = "+";
        else if (operation=="OdGrandeurSoustraction") m_op = "-";
             else if (operation=="OdGrandeurMultiplication") m_op = "x";
                else if (operation=="OdGrandeurDivision") m_op = ":";
    m_position.setX(pos.x());
    m_position.setY(pos.y());

    g_operande = rand()%(intMaxG);
    if(m_op == ":"){
        ExerciceOperation* ex = (ExerciceOperation*) parent;
        int alea = rand()%3;
        d_operande = ex->getMultipleCible().at(alea);
    }
    else {
        d_operande = rand()%(intMaxD);
    }

    /* Remarque : il existe sans doute une fonction qui retourne le max mais ça me prendrait plus de temps de chercher que d'écrire 3 lignes... */
    if (d_operande>g_operande) {
        int tmp=g_operande;
        g_operande=d_operande;
        d_operande=tmp;
        }

    /* Calcul de la valeur approchée à émettre (Problème si c'est la multiplication : l'utiliteur veut un "x" alors que le calculateur veut un "*") */

    if(m_op == ":"){
        float calculExact = g_operande/d_operande;
        if(m_localDebug) qDebug()<<g_operande<<" / "<<d_operande<<" = "<<calculExact;
        int dessous = qFloor(calculExact);
        int dessus = qCeil(calculExact);
        int apeupres;
        if(qAbs(calculExact - dessus) > qAbs(calculExact - dessous)){
            apeupres = dessous;
        }
        else {
            apeupres = dessus;
        }
        if(m_localDebug) qDebug()<<"je retiens "<<apeupres;
        if(apeupres / 1000 > 0){
            int apeupresHaut = ((apeupres/1000)+1)*1000;
            int apeupresBas = (apeupres/1000)*1000;
            if(m_localDebug) qDebug()<<"j'ai un apeupres plus grand que 1000"<<apeupresBas<<" - "<<apeupresHaut;
            if(qAbs(apeupres - apeupresHaut) > qAbs(apeupres - apeupresBas)){
                apeupres = apeupresBas;
            }
            else {
                apeupres = apeupresHaut;
            }
        }
        else if(dessous / 100 > 0){
            int apeupresHaut = ((apeupres/100)+1)*100;
            int apeupresBas = (apeupres/100)*100;
            if(m_localDebug) qDebug()<<"j'ai un apeupres plus grand que 100"<<apeupresBas<<" - "<<apeupresHaut;
            if(qAbs(apeupres - apeupresHaut) > qAbs(apeupres - apeupresBas)){
                apeupres = apeupresBas;
            }
            else {
                apeupres = apeupresHaut;
            }
        }
        else if(dessous / 10 > 0){
            int apeupresHaut = ((apeupres/10)+1)*10;
            int apeupresBas = (apeupres/10)*10;
            if(m_localDebug) qDebug()<<"j'ai un apeupres plus grand que 10"<<apeupresBas<<" - "<<apeupresHaut;
            if(qAbs(apeupres - apeupresHaut) > qAbs(apeupres - apeupresBas)){
                apeupres = apeupresBas;
            }
            else {
                apeupres = apeupresHaut;
            }
        }
        if(m_localDebug) qDebug()<<apeupres;
        m_approximation = apeupres;
    }
    else{
    if (m_op=="x") m_ligne = QString::number(valeurApprochee(g_operande,intMaxG))+"*"+QString::number(valeurApprochee(d_operande,intMaxD));
    else m_ligne = QString::number(valeurApprochee(g_operande,intMaxG))+m_op+QString::number(valeurApprochee(d_operande,intMaxD));

        QScriptEngine calculateur;
        QScriptValue resultat = calculateur.evaluate(m_ligne);
        m_approximation = resultat.toNumber();
    }
    if(m_localDebug) qDebug()<<" gauche : "<<valeurApprochee(g_operande,intMaxG)<<" droite : "<<valeurApprochee(d_operande, intMaxD)<<" valeurAppro : "<<m_approximation;

    m_timer = new QTimeLine(tempsAccorde*1000,this);

    construisAffichage();
    dessineMoi(image);

    this->emetApprox();
    //qDebug()<<"baudruche::constructeur valeurs approchées (2)";
}

/* constructeur spécifique aux compléments */
baudruche::baudruche(int valeurCible, int tempsAccorde, QString operation, QPoint pos, QObject *parent, QString image)
{
    m_localDebug = false;
    if(m_localDebug) qDebug()<<"baudruche::constructeur compléments (1)";
    float factX= static_cast<float> (QApplication::desktop()->screenGeometry().width())/1680;
    m_parent = parent;
    m_nomImage = image;
    m_dropValeur = "";
    m_isDetructionPlanified = false;
    m_nomOperation = operation;
    if (operation=="complementA") m_op = "+";
    else m_op = "x";
    m_position.setX(pos.x());
    m_position.setY(pos.y());
    m_approximation=0;

    int nombreVise;
    if (valeurCible!=0)
        nombreVise=valeurCible;
    else nombreVise=rand()%100;
    g_operande = 0;
    d_operande = 0;

    if (m_op=="x") {
        g_operande = nombreVise;
        int sort = rand()%MULTIPLE_MAX;
        d_operande = nombreVise*sort;
        m_resultat = sort;
        m_ligne = QString::number(g_operande)+" x ? = "+QString::number(d_operande);
        }
    else {
        g_operande = rand()%(nombreVise);
        d_operande = nombreVise;
        m_ligne = QString::number(d_operande)+"-"+QString::number(g_operande);
        QScriptEngine calculateur;
        QScriptValue resultat = calculateur.evaluate(m_ligne);
        m_resultat = resultat.toNumber();
        m_ligne = QString::number(g_operande)+" + ? = "+QString::number(d_operande);
        }

    m_timer = new QTimeLine(tempsAccorde*1000,this);

    QRect ecran;
            ecran=QApplication::desktop()->screenGeometry();

    construisAffichage();
    dessineMoi(image);

    this->emetRes();
    //qDebug()<<"baudruche::constructeur compléments (2)";
}

void baudruche::dessineMoi(QString image)
{
    float ratio = abeApp->getAbeApplicationDecorRatio();
    const int k = 100;
        int coulAlea = rand()%(5);
        QString illustration;
        QString imageBase="ballon";
        if (image!=0) imageBase=image;
        QFile* fichierImage = new QFile(":/calculment/elements/"+imageBase+"Vert");
        if (fichierImage->exists()) {
                switch (coulAlea) {
                    case 0 : illustration = ":/calculment/elements/"+imageBase+"Vert"; break;
                    case 1 : illustration = ":/calculment/elements/"+imageBase+"Jaune"; break;
                    case 2 : illustration = ":/calculment/elements/"+imageBase+"Rouge"; break;
                    case 3 : illustration = ":/calculment/elements/"+imageBase+"Orange"; break;
                    case 4 : illustration = ":/calculment/elements/"+imageBase+"Bleu"; break;
                    case 5 : illustration = ":/calculment/elements/"+imageBase+"Rose"; break;
                    }
                }
        else illustration=":/calculment/elements/"+imageBase;
        QPixmap imageIllustration(illustration);
        QPixmap imageIllustration2 = imageIllustration.scaledToHeight(imageIllustration.height()*ratio, Qt::SmoothTransformation);
//        qDebug()<<"Taille baudruche : "<<imageIllustration2.width()<<" X "<<imageIllustration2.height();
            m_image.setPixmap(imageIllustration2);
            m_image.setZValue(k);
            m_image.setPos(m_position);
            this->addToGroup(&m_image);

        m_texteAffiche = new QGraphicsTextItem(m_affichage,&m_image);
        QFont fonteUtilisee = abeApp->font();
        fonteUtilisee.setBold(true);
        fonteUtilisee.setPointSize(28*ratio);
        if(m_localDebug) qDebug()<<"&&"<<fonteUtilisee.pointSize();
        QFontMetrics mesureur(fonteUtilisee);
        while(mesureur.boundingRect(m_affichage).width() > imageIllustration2.width()*0.8){
            fonteUtilisee.setPointSize(fonteUtilisee.pointSize()-1);
            mesureur = QFontMetrics(fonteUtilisee);
        }
        if(m_localDebug) qDebug()<<imageIllustration2.width();
        if(m_localDebug) qDebug()<<mesureur.boundingRect(m_affichage).width();
        m_texteAffiche->setFont(fonteUtilisee);
        int longueurAffichage,largeurIllustration,decalageCentrage;
        longueurAffichage=mesureur.width(m_affichage);
        largeurIllustration=imageIllustration2.width();
        decalageCentrage=(largeurIllustration-longueurAffichage)/2;
        if (image=="auto") m_texteAffiche->setPos(50*ratio,170*ratio);
        else m_texteAffiche->setPos(decalageCentrage,75*ratio);
        m_texteAffiche->setZValue(k+1);
        this->addToGroup(m_texteAffiche);
        m_isMaisonSurvolee = false;
}

void baudruche::construisAffichage()
{
    m_affichage = "";
        m_affichage.append(QString::number(g_operande));
        m_affichage.append(" ");
        m_affichage.append(m_op);

        if (m_nomOperation.left(10) == "complement") m_affichage.append(" ? = ");
        else m_affichage.append(" ");
        m_affichage.append(QString::number(d_operande));
        if (m_nomOperation.left(3) == "OdG") m_affichage.append(QString::fromUtf8(" ≈"));
}

QPoint baudruche::getMPosition()
{
    return this->m_position;
}

float baudruche::getMResultat()
{
    return this->m_resultat;
}

float baudruche::getMApproximation()
{
    return this->m_approximation;
}

float baudruche::getMGOperande()
{
    return this->g_operande;
}

float baudruche::getMDOperande()
{
    return this->d_operande;
}

QString baudruche::getMOperation()
{
    return this->m_op;
}

QString baudruche::getMLigne()
{
    return this->m_ligne;
}

QString baudruche::getMAffichage()
{
    return m_affichage;
}

QString baudruche::getMDropValeur()
{
    return m_dropValeur;
}

int baudruche::valeurApprochee(int operande, int maximum)
{
    int apeupres = 0;
    if (maximum == 10 && m_op == "x")
        return operande;
    if (operande > 10 || m_op != "x"){
        if (maximum == 100 || maximum == 1000) {
            if ((operande%(maximum/10))< maximum/20){
                apeupres=(operande/(maximum/10))*(maximum/10);
            }
            else{
                apeupres = ((operande/(maximum/10))+1)*(maximum/10);
            }
        }
        if (maximum == 1000 && operande < 100) {
            if ((operande%(maximum/100))< maximum/200){
                apeupres=(operande/(maximum/100))*(maximum/100);
            }
            else {
                apeupres = ((operande/(maximum/100))+1)*(maximum/100);
            }
        }
    }
    //qDebug()<<"A ce stade, la valeur approchee vaut "<<apeupres;
    return apeupres;
}

void baudruche::detruire()
{
    if (this!=NULL) {
        if (m_approximation == 0) {
            emit valueChanged(m_resultat);//ici le problème
            //qDebug()<<"A la destruction le résultat vaut "<<m_resultat;
        }
        else {emit valueChanged(m_approximation);
            //qDebug()<<"A la destruction l'approximation vaut "<<m_approximation;
        }
        emit destroyed(true);
        emit destroyed();
        delete this;
    }
}

void baudruche::detruireTps()
{
    float factY= static_cast<float> (QApplication::desktop()->screenGeometry().height())/1050;
    if (this!=NULL)
    {
        if (m_approximation == 0) {
            emit valueChanged(m_resultat);
            //qDebug()<<"A la destruction le résultat vaut "<<m_resultat;
        }
        else {emit valueChanged(m_approximation);
            //qDebug()<<"A la destruction l'approximation vaut "<<m_approximation;
        }

        emit tempsFini(tr("TROP TARD..."));
        QPixmap image(":/calculment/elements/let");
        QPixmap imageRetaillee = image.scaledToHeight(image.height()*factY);
        emit tempsFini(imageRetaillee);
        if (m_nomImage == "auto")
            changeImage(":/calculment/elements/boum");
        else if (m_nomImage == "fantome")
            changeImage(":/calculment/elements/pop");
        else changeImage(":/calculment/elements/paf");
        removeFromGroup(m_texteAffiche);
        delete m_texteAffiche;
        QTimeLine* tiptip = new QTimeLine(1000,this);
        connect(tiptip, SIGNAL(finished()),this, SLOT(detruire()));
        tiptip->start();
        m_isDetructionPlanified = true;
   }
}

void baudruche::emetRes()
{
       emit valueChanged(m_resultat);
}

void baudruche::emetApprox()
{
    emit valueChanged(m_approximation);
}

void baudruche::changeImage(QString nomNouvelleImage)
{
    float factY= static_cast<float> (QApplication::desktop()->screenGeometry().height())/1050;
    QPixmap nouvelleImage(nomNouvelleImage);
    QPixmap nouvelleImage2 = nouvelleImage.scaledToHeight(nouvelleImage.height()*factY, Qt::SmoothTransformation);
    m_position.setX(m_position.x()-(nouvelleImage2.width()-m_image.pixmap().width())/2);
    m_position.setY(m_position.y()-((nouvelleImage2.height()-m_image.pixmap().height())/2));    //P... de Justin
//    qDebug()<<"Taille image avant changement : "<<m_image.pixmap().width()<<" X "<<m_image.pixmap().height();
//    qDebug()<<"Position image avant changement : "<<m_image.x()<<" X "<<m_image.y();
    m_image.setPixmap(nouvelleImage2);
//    qDebug()<<"Taille image après changement : "<<m_image.pixmap().width()<<" X "<<m_image.pixmap().height();
    m_image.setPos(m_position);
//    qDebug()<<"Position image après changement : "<<m_image.x()<<" X "<<m_image.y();
}

void baudruche::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug()<<" --- Release ---"<<m_parent;
        QList<QGraphicsItem *>listeItems = static_cast<QGraphicsScene *>(m_parent)->items(event->scenePos());
        listeItems.removeAt(listeItems.indexOf(this)); // On enleve cette piece de la liste
        foreach(QGraphicsItem* elt, listeItems)
        {
            if (elt->parentItem() == this) listeItems.removeOne(elt);
            else {
                //qDebug()<<"J'ai ete lachee sur l'objet "<<elt->toolTip()<<"->"<<elt;
                m_dropValeur = elt->toolTip();
            }
        }
        if(listeItems.size() == 0){ // On essaie de déposer la pièce en dehors d'une cellule ou d'un autre objet(autre pièce)
            //qDebug()<<"Rien ici...";
        }
    emit lacheIci(event->scenePos().toPoint());
}

void baudruche::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
    QList<QGraphicsItem *>listeItems = collidingItems();
    bool isMaisonSurvolee = false;
    QString nomMaisonSurvolee="";
    foreach (QGraphicsItem* itemExamine, listeItems) {
        if (itemExamine->toolTip().left(6) == "Maison")
        {
            isMaisonSurvolee = true ;
            nomMaisonSurvolee = itemExamine->toolTip();
        }
    }
    if (isMaisonSurvolee)
    {
        emit baudrucheSurvole(nomMaisonSurvolee);
        //qDebug()<<"baudrucheSurvolee("<<nomMaisonSurvolee<<") emis";
    }
    else
    {
        emit baudrucheSurvoleRien();
        //qDebug()<<"baudrucheSurvoleRien() émis";
    }
}
