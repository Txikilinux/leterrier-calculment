/**
  * Implémentation de l'objet qui portera les opérations proposées
  *
  * @warning l'attribut m_timer est public, son passage en privé avec accesseur est laissé en commentaire
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


#include "baudruche.h"
#include <QApplication>
 #include <QDesktopWidget>
 #include <QFontMetrics>

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

baudruche::baudruche(int intMinG, int intMaxG, int intMinD, int intMaxD, int tempsAccorde, QString operation,QPoint pos,QObject *parent,QString image)
{
    float factX= static_cast<float> (QApplication::desktop()->screenGeometry().width())/1600;

    qDebug()<<"Parent de baudruche: "<<parent->objectName();
    m_nomImage = image;
    m_approximation=0;
    if (operation=="addition" || operation=="tableA" || operation=="") m_op = "+";
    else if (operation=="soustraction") m_op = "-";
        else if (operation=="multiplication" || operation=="tableM") m_op = "x";
    m_position.setX(pos.x());
    m_position.setY(pos.y());

    if (intMinG==intMaxG) g_operande=intMaxG;
    else g_operande = intMinG + rand()%(intMaxG-intMinG);

    if (intMinD==intMaxD) d_operande=intMaxD;
    else d_operande = intMinD + rand()%(intMaxD-intMinD);
    //Remarque : il existe sans doute une fonction qui retourne le max mais ça me prendrait plus de temps de chercher que d'écrire 3 lignes...
    if (d_operande>g_operande) {
        int tmp=g_operande;
        g_operande=d_operande;
        d_operande=tmp;
        }

    //Calcul du résultat à émettre (Problème si c'est la multiplication : l'utiliteur veut un "x" alors que le calculateur veut un "*")
    if (m_op=="x") m_ligne = QString::number(g_operande)+"*"+QString::number(d_operande);
    else m_ligne = QString::number(g_operande)+m_op+QString::number(d_operande);
        QScriptEngine calculateur;
        QScriptValue resultat = calculateur.evaluate(m_ligne);
        m_resultat = resultat.toNumber();

    //QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres.conf", QSettings::IniFormat);
    m_timer = new QTimeLine(tempsAccorde*1000,this);

    //Je dois convertir mes entiers en QString pour les concatener
    QString aGauche, aDroite;
        aGauche = aGauche.setNum(g_operande);
        aDroite = aDroite.setNum(d_operande);
    //Je peux maintenant construire mon opération en ligne
    m_affichage = "";
        m_affichage.append(aGauche);
        m_affichage.append(" ");
        m_affichage.append(m_op);
        m_affichage.append(" ");
        m_affichage.append(aDroite);

    dessineMoi(image,16*factX);
    qDebug()<<" Taille police "<<16*factX;
        
    this->emetRes();
    qDebug()<<"res emis instanciation "<<m_resultat;
}

//constructeur spécifique aux valeurs approchées
baudruche::baudruche(int intMaxG, int intMaxD,int tempsAccorde, QString operation,QPoint pos,QObject *parent,QString image)
{
    float factX= static_cast<float> (QApplication::desktop()->screenGeometry().width())/1600;
    m_nomImage = image;
    m_approximation=0;
        if (operation=="approcheA") m_op = "+";
        else if (operation=="approcheS") m_op = "-";
             else if (operation=="approcheM") m_op = "x";
    m_position.setX(pos.x());
    m_position.setY(pos.y());

    g_operande = rand()%(intMaxG);
    d_operande = rand()%(intMaxD);

    //Remarque : il existe sans doute une fonction qui retourne le max mais ça me prendrait plus de temps de chercher que d'écrire 3 lignes...
    if (d_operande>g_operande) {
        int tmp=g_operande;
        g_operande=d_operande;
        d_operande=tmp;
        }

    //Calcul de la valeur approchée à émettre (Problème si c'est la multiplication : l'utiliteur veut un "x" alors que le calculateur veut un "*")
    if (m_op=="x") m_ligne = QString::number(valeurApprochee(g_operande,intMaxG))+"*"+QString::number(valeurApprochee(d_operande,intMaxD));
    else m_ligne = QString::number(valeurApprochee(g_operande,intMaxG))+m_op+QString::number(valeurApprochee(d_operande,intMaxD));

        QScriptEngine calculateur;
        QScriptValue resultat = calculateur.evaluate(m_ligne);
        m_approximation = resultat.toNumber();
qDebug()<<" gauche : "<<valeurApprochee(g_operande,intMaxG)<<" droite : "<<valeurApprochee(d_operande, intMaxD)<<" valeurAppro : "<<m_approximation;

    //QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres.conf", QSettings::IniFormat);
    m_timer = new QTimeLine(tempsAccorde*1000,this);

    //Je dois convertir mes entiers en QString pour les concatener
    QString aGauche, aDroite;
        aGauche = aGauche.setNum(g_operande);
        aDroite = aDroite.setNum(d_operande);
    //Je peux maintenant construire mon opération en ligne
    m_affichage = "";
        m_affichage.append(aGauche);
        m_affichage.append(" ");
        m_affichage.append(m_op);
        m_affichage.append(" ");
        m_affichage.append(aDroite);
        m_affichage.append(QString::fromUtf8(" ≈"));

    dessineMoi(image,16*factX);

    this->emetApprox();
}

//constructeur spécifique aux compléments
baudruche::baudruche(int valeurCible, int tempsAccorde,QString operation,QPoint pos,QObject *parent,QString image)
{
    float factX= static_cast<float> (QApplication::desktop()->screenGeometry().width())/1600;
    m_nomImage = image;
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
        }
    else {
        g_operande = rand()%(nombreVise);
        d_operande = nombreVise;
        m_ligne = QString::number(d_operande)+"-"+QString::number(g_operande);
        QScriptEngine calculateur;
        QScriptValue resultat = calculateur.evaluate(m_ligne);
        m_resultat = resultat.toNumber();
        }

    //QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres.conf", QSettings::IniFormat);
    m_timer = new QTimeLine(tempsAccorde*1000,this);

    //Je dois convertir mes entiers en QString pour les concatener
    QString aGauche, aDroite;
        aGauche = aGauche.setNum(g_operande);
        aDroite = aDroite.setNum(d_operande);
    //Je peux maintenant construire mon opération en ligne
    m_affichage = "";
        m_affichage.append(aGauche);
        m_affichage.append(" ");
        m_affichage.append(m_op);
        m_affichage.append(" ? = ");
        m_affichage.append(aDroite);

    QRect ecran;
            ecran=QApplication::desktop()->screenGeometry();
    dessineMoi(image,12*factX);

    this->emetRes();
}

//constructeur spécifique à l'affichage du résultat
baudruche::baudruche(int pts, QPoint pos,QObject *parent,QString image)
{
    float factX= static_cast<float> (QApplication::desktop()->screenGeometry().width())/1600;
    qDebug()<<"Fact X vaut "<< factX;
    m_nomImage = image;
    const int k=100;
    g_operande = 0;
    d_operande = 0;
    m_op = "";
    m_resultat=0;
    m_position.setX(pos.x());
    m_position.setY(pos.y());
    QString resultStr;
    resultStr = resultStr.setNum(pts);
    QString* msg = new QString(tr("Ton score est de "));
    msg->append(resultStr);
    if (pts==0 || pts==1) msg->append(tr(" point."));
    else msg->append(tr(" points."));
    QGraphicsPixmapItem* pixmap = new QGraphicsPixmapItem(this);
        QString illustration;
        QString imageBase="ballon";
        if (image!=0) imageBase=image;
        illustration = "./data/images/"+image+"Rose.png";
        pixmap->setPixmap(illustration);
        pixmap->setZValue(k);
        pixmap->setPos(pos);
        this->addToGroup(pixmap);
    QGraphicsTextItem* affichage = new QGraphicsTextItem("",pixmap);
        affichage->setFont( QFont( "dejaVuSans",16*factX ) );
        affichage->setHtml(*msg);
        affichage->setZValue(k+2);
        affichage->setPos(40/factX,100/factX);
        this->addToGroup(affichage);
        //m_timer = new QTimeLine(TPS*1000,this);

}

void baudruche::dessineMoi(QString image, int taillePolice)
{
    float factX= static_cast<float> (QApplication::desktop()->screenGeometry().width())/1600;
    qDebug()<<"FactX = "<<factX;
    float factY= static_cast<float> (QApplication::desktop()->screenGeometry().height())/1050;
    qDebug()<<"FactY = "<<factY;
    const int k=100;
//    QGraphicsPixmapItem* m_image = new QGraphicsPixmapItem(this);
        int coulAlea = rand()%(5);
        QString illustration;
        QString imageBase="ballon";
        if (image!=0) imageBase=image;
        QFile* fichierImage = new QFile("./data/images/"+imageBase+"Vert.png");
        if (fichierImage->exists()) {
                switch (coulAlea) {
                    case 0 : illustration = "./data/images/"+imageBase+"Vert.png"; break;
                    case 1 : illustration = "./data/images/"+imageBase+"Jaune.png"; break;
                    case 2 : illustration = "./data/images/"+imageBase+"Rouge.png"; break;
                    case 3 : illustration = "./data/images/"+imageBase+"Orange.png"; break;
                    case 4 : illustration = "./data/images/"+imageBase+"Bleu.png"; break;
                    case 5 : illustration = "./data/images/"+imageBase+"Rose.png"; break;
                    }
                }
        else illustration="./data/images/"+imageBase;
        QPixmap imageIllustration(illustration);
        QPixmap imageIllustration2 = imageIllustration.scaledToHeight(imageIllustration.height()*factY, Qt::SmoothTransformation);
//avant//QPixmap imageIllustration2 = imageIllustration.scaled(imageIllustration.width()*factX, imageIllustration.height()*factY, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        qDebug()<<"Taille baudruche : "<<imageIllustration2.width()<<" X "<<imageIllustration2.height();
            m_image.setPixmap(imageIllustration2);
            m_image.setZValue(k);
            m_image.setPos(m_position);
            this->addToGroup(&m_image);

    QGraphicsTextItem* affichage = new QGraphicsTextItem("",&m_image);
        affichage->setFont( QFont( "dejaVuSans",taillePolice,QFont::Bold ) );
        QFontMetrics mesureur(QFont("dejaVuSans",taillePolice));
        int longueurAffichage,largeurIllustration,decalageCentrage;
        longueurAffichage=mesureur.width(m_affichage);
        largeurIllustration=imageIllustration2.width();
        decalageCentrage=(largeurIllustration-longueurAffichage)/2;
        affichage->setHtml(m_affichage);
        if (image=="auto") affichage->setPos(50*factX,190*factY);
        else affichage->setPos(decalageCentrage,75*factY);
        affichage->setZValue(k+1);
        this->addToGroup(affichage);
        m_texteAffiche = new QGraphicsTextItem();
        m_texteAffiche = affichage;

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

int baudruche::valeurApprochee(int operande, int maximum)
{
    int apeupres=0;
    if (maximum==10 && m_op=="x")
        return operande;
    if (operande>10 || m_op!="x"){
        if (maximum==100 || maximum==1000) {
            if ((operande%(maximum/10))< maximum/20) apeupres=(operande/(maximum/10))*(maximum/10);
            else apeupres=((operande/(maximum/10))+1)*(maximum/10);
            }
        if (maximum==1000 && operande<100) {
            if ((operande%(maximum/100))< maximum/200) apeupres=(operande/(maximum/100))*(maximum/100);
            else apeupres=((operande/(maximum/100))+1)*(maximum/100);
            }
        }
    qDebug()<<"A ce stade, la valeur approchee vaut "<<apeupres;
    return apeupres;
}

//QTimeLine baudruche::getMTimer()
//{
//    return m_timer;
//}

void baudruche::detruire()
{
    if (this!=NULL) {
        if (m_approximation==0) {
            emit valueChanged(m_resultat);//ici le problème
            qDebug()<<"A la destruction le résultat vaut "<<m_resultat;
        }
        else {emit valueChanged(m_approximation);
            qDebug()<<"A la destruction l'approximation vaut "<<m_approximation;
        }
        emit destroyed(true);
        emit destroyed();
        delete this;
        }
}

void baudruche::detruireTps()
{
    float factY= static_cast<float> (QApplication::desktop()->screenGeometry().height())/1050;
    if (this!=NULL) {
        emit valueChanged(m_resultat);//pas sûr utile si baudruche termine temps
        //Résolution bug Hayat : les signaux sont émits par detruire() connecté à la fin de tiptip
//        emit destroyed(true);
//        emit destroyed();
        emit tempsFini(tr("TROP TARD..."));
        QPixmap image("./data/images/will-let.png");
        QPixmap imageRetaillee = image.scaledToHeight(image.height()*factY);
        emit tempsFini(imageRetaillee);
        if (m_nomImage == "auto")
            changeImage("./data/images/boum.png");
        else if (m_nomImage == "fantome")
            changeImage("./data/images/pop.png");
        else changeImage("./data/images/paf.png");
        removeFromGroup(m_texteAffiche);
        delete m_texteAffiche;
        QTimeLine* tiptip = new QTimeLine(1000,this);
        connect(tiptip, SIGNAL(finished()),this, SLOT(detruire()));
        tiptip->start();
        //delete this;
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

//Inutilisé ?
//void baudruche::emetMort()
//{
//    emit destroyed(true);
//}

void baudruche::changeImage(QString nomNouvelleImage)
{
    float factY= static_cast<float> (QApplication::desktop()->screenGeometry().height())/1050;
    QPixmap nouvelleImage(nomNouvelleImage);
    QPixmap nouvelleImage2 = nouvelleImage.scaledToHeight(nouvelleImage.height()*factY, Qt::SmoothTransformation);
    m_position.setX(m_position.x()-(nouvelleImage2.width()-m_image.pixmap().width())/2);
    m_position.setY(m_position.y()-((nouvelleImage2.height()-m_image.pixmap().height())/2));    //P... de Justin
    qDebug()<<"Taille image avant changement : "<<m_image.pixmap().width()<<" X "<<m_image.pixmap().height();
    qDebug()<<"Position image avant changement : "<<m_image.x()<<" X "<<m_image.y();
    m_image.setPixmap(nouvelleImage2);
    qDebug()<<"Taille image après changement : "<<m_image.pixmap().width()<<" X "<<m_image.pixmap().height();
    m_image.setPos(m_position);
    qDebug()<<"Position image après changement : "<<m_image.x()<<" X "<<m_image.y();

}
