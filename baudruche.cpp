#include "baudruche.h"

const float FLOATMIN = 0.0;
const float FLOATMAX = 9.9;
//const int INTMIN = 0;
//const int INTMAX = 9;
const int TPS = 6;

baudruche::baudruche(int intMinG, int intMaxG, int intMinD, int intMaxD,QString op,QPoint pos)
{
    g_operande = intMinG + rand()%(intMaxG-intMinG);
            //J'aurais bien aimé mettre des réels mais à une décimale...
            //    g_operande = ((FLOATMAX-FLOATMIN)*((float)rand()/RAND_MAX))+FLOATMIN;
            //    int arr_g_operande=floor(g_operande*10);
            //    g_operande=(arr_g_operande/10);
            //    //debug valeur réel
            //        qDebug() << "avant :" << g_operande << " et après:" << arr_g_operande ;
            //d_operande = b;
    d_operande = intMinD + rand()%(intMaxD-intMinD);
    //Remarque : il existe sans doute une fonction qui retourne le max mais ça me prendrait plus de temps de chercher que d'écrire 3 lignes...
    if (d_operande>g_operande) {
        int tmp=g_operande;
        g_operande=d_operande;
        d_operande=tmp;
        }
    m_op = op;
    m_position.setX(pos.x());
    m_position.setY(pos.y());

    //Problème si c'est la multiplication : l'utiliteur veut un "x" alors que le calculateur veut un "*"
    if (m_op=="x") m_ligne = QString::number(g_operande)+"*"+QString::number(d_operande);
    else m_ligne = QString::number(g_operande)+m_op+QString::number(d_operande);
    QScriptEngine calculateur;
    QScriptValue resultat = calculateur.evaluate(m_ligne);
    m_resultat = resultat.toNumber();
    m_timer = new QTimeLine(TPS*1000,this);

    //à réfléchir la place de cette constante : ici ? dans exempledessin1.cpp où on va instancier des baudruche, dans le main ?
    const int k=100;

    //Je dois convertir mes entiers en QString pour les concatener
    QString aGauche, aDroite;
        aGauche = aGauche.setNum(g_operande);
        aDroite = aDroite.setNum(d_operande);
    //Je peux maintenant construire mon opération en ligne
    QString* operation = new QString("");
        operation->append(aGauche);
        operation->append(" ");
        operation->append(op);
        operation->append(" ");
        operation->append(aDroite);

    QGraphicsPixmapItem* pixmap = new QGraphicsPixmapItem(this);
        int coulAlea = rand()%(5);
        QString illustration;
        switch (coulAlea) {
            case 0 : illustration = "./images/ballonVert.png"; break;
            case 1 : illustration = "./images/ballonJaune.png"; break;
            case 2 : illustration = "./images/ballonRouge.png"; break;
            case 3 : illustration = "./images/ballonOrange.png"; break;
            case 4 : illustration = "./images/ballonBleu.png"; break;
            case 5 : illustration = "./images/ballonRose.png"; break;
            }

        pixmap->setPixmap(illustration);
        pixmap->setZValue(k);
        pixmap->setPos(pos);
        this->addToGroup(pixmap);

    QGraphicsTextItem* affichage = new QGraphicsTextItem("",pixmap);
        affichage->setFont( QFont( "dejaVuSans",16 ) );
        affichage->setHtml(*operation);
        affichage->setZValue(k+1);
        affichage->setPos(40,60);
        this->addToGroup(affichage);
        
    emit valueChanged(m_resultat);
}

baudruche::baudruche(int pts, QPoint pos)
{
    //à réfléchir la place de cette constante : ici ? dans exempledessin1.cpp où on va instancier des baudruche, dans le main ?
    const int k=100;
    g_operande = 0;
    d_operande = 0;
    m_op = "";
    m_resultat=0;
    m_position.setX(pos.x());
    m_position.setY(pos.y());
    QString resultStr;
    resultStr = resultStr.setNum(pts);
    QString* msg = new QString("Ton score est de ");
    msg->append(resultStr);
    if (pts==0 || pts==1) msg->append(" point.");
    else msg->append(" points.");
    QGraphicsPixmapItem* pixmap = new QGraphicsPixmapItem(this);
        QString illustration = "./images/ballonRose.png";
        pixmap->setPixmap(illustration);
        pixmap->setZValue(k);
        pixmap->setPos(pos);
        this->addToGroup(pixmap);
    QGraphicsTextItem* affichage = new QGraphicsTextItem("",pixmap);
        affichage->setFont( QFont( "dejaVuSans",16 ) );
        affichage->setHtml(*msg);
        affichage->setZValue(k+1);
        affichage->setPos(40,100);
        //affichage->setPos(m_position.x()+30, m_position.y()+60);
        this->addToGroup(affichage);
        m_timer = new QTimeLine(TPS*1000,this);

}

QPoint baudruche::getMPosition()
{
    return this->m_position;
}

float baudruche::getMResultat()
{
    return this->m_resultat;
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
//QTimeLine baudruche::getMTimer()
//{
//    return m_timer;
//}

void baudruche::detruire()
{
    if (this!=NULL) {
        emit valueChanged(m_resultat);
        emit destroyed(true);
        emit destroyed();
        delete this;
        }
}

void baudruche::detruireTps()
{
    if (this!=NULL) {
        emit valueChanged(m_resultat);//pas sûr utile si baudruche termine temps
        emit destroyed(true);
        emit destroyed();
        emit tempsFini("TROP TARD...");
        delete this;
        }
}

void baudruche::emetRes()
{
       emit valueChanged(m_resultat);
}

void baudruche::emetMort()
{
    emit destroyed(true);
}
