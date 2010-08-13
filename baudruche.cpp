#include "baudruche.h"
 #include <QFontMetrics>

const int MULTIPLE_MAX=11;

//à faire : réfléchir à une fonction permettant de factoriser dans les constructeurs

baudruche::baudruche(int intMinG, int intMaxG, int intMinD, int intMaxD,QString op,QPoint pos,QString image)
{
    if (intMinG==intMaxG) g_operande=intMaxG;
    else g_operande = intMinG + rand()%(intMaxG-intMinG);
            //J'aurais bien aimé mettre des réels mais à une décimale...
            //    g_operande = ((FLOATMAX-FLOATMIN)*((float)rand()/RAND_MAX))+FLOATMIN;
            //    int arr_g_operande=floor(g_operande*10);
            //    g_operande=(arr_g_operande/10);
            //    //debug valeur réel
            //        qDebug() << "avant :" << g_operande << " et après:" << arr_g_operande ;
            //d_operande = b;
    if (intMinD==intMaxD) d_operande=intMaxD;
    else d_operande = intMinD + rand()%(intMaxD-intMinD);
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
    QSettings config("./maConfig.ini", QSettings::IniFormat);
    m_timer = new QTimeLine(config.value(tr("TempsAccorde")).toInt()*1000,this);

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
        QString imageBase="ballon";
        if (image!=0) imageBase=image;
        switch (coulAlea) {
            case 0 : illustration = "./images/"+imageBase+"Vert.png"; break;
            case 1 : illustration = "./images/"+imageBase+"Jaune.png"; break;
            case 2 : illustration = "./images/"+imageBase+"Rouge.png"; break;
            case 3 : illustration = "./images/"+imageBase+"Orange.png"; break;
            case 4 : illustration = "./images/"+imageBase+"Bleu.png"; break;
            case 5 : illustration = "./images/"+imageBase+"Rose.png"; break;
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

//constructeur spécifique aux compléments
baudruche::baudruche(int valeurCible,QString op,QPoint pos,QString image)
{
    //1 ajout de 4 lignes
        int nombreVise;
    if (valeurCible!=0)
        nombreVise=valeurCible;
    else nombreVise=rand()%100;
    g_operande = 0;
    d_operande = 0;
    m_op = op;
    m_position.setX(pos.x());
    m_position.setY(pos.y());

    //Problème si c'est la multiplication : l'utiliteur veut un "x" alors que le calculateur veut un "*"
    if (m_op=="x") {
        g_operande = nombreVise;
        int sort = rand()%MULTIPLE_MAX;
        d_operande = nombreVise*sort;
        //m_ligne = QString::number(nombreVise)+"/"+QString::number(g_operande);
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

    QSettings config("./maConfig.ini", QSettings::IniFormat);
    m_timer = new QTimeLine(config.value(tr("TempsAccorde")).toInt()*1000,this);

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
        operation->append(" ? = ");
        operation->append(aDroite);

    QGraphicsPixmapItem* pixmap = new QGraphicsPixmapItem(this);
        int coulAlea = rand()%(5);
        QString illustration;

        QString imageBase="ballon";
        if (image!=0) imageBase=image;
        switch (coulAlea) {
            case 0 : illustration = "./images/"+imageBase+"Vert.png"; break;
            case 1 : illustration = "./images/"+imageBase+"Jaune.png"; break;
            case 2 : illustration = "./images/"+imageBase+"Rouge.png"; break;
            case 3 : illustration = "./images/"+imageBase+"Orange.png"; break;
            case 4 : illustration = "./images/"+imageBase+"Bleu.png"; break;
            case 5 : illustration = "./images/"+imageBase+"Rose.png"; break;
            }
        QPixmap imageIllustration(illustration);
        pixmap->setPixmap(imageIllustration);
        pixmap->setZValue(k);
        pixmap->setPos(pos);
        this->addToGroup(pixmap);

    QGraphicsTextItem* affichage = new QGraphicsTextItem("",pixmap);
        affichage->setFont( QFont( "dejaVuSans",14 ) );
        QFontMetrics mesureur(QFont("dejaVuSans",14));
        int longueurAffichage,largeurIllustration,decalageCentrage;
        longueurAffichage=mesureur.width(*operation);
        largeurIllustration=imageIllustration.width();
        decalageCentrage=(largeurIllustration-longueurAffichage)/2;
        affichage->setHtml(*operation);
      //affichage->setPos(QFontMetrics::width(operation),0);
        affichage->setPos(decalageCentrage,80);
        affichage->setZValue(k+1);
        this->addToGroup(affichage);

    emit valueChanged(m_resultat);
}

//constructeur spécifique à l'affichage du résultat
baudruche::baudruche(int pts, QPoint pos,QString image)
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
    QString* msg = new QString(tr("Ton score est de "));
    msg->append(resultStr);
    if (pts==0 || pts==1) msg->append(tr(" point."));
    else msg->append(tr(" points."));
    QGraphicsPixmapItem* pixmap = new QGraphicsPixmapItem(this);
        QString illustration;
        QString imageBase="ballon";
        if (image!=0) imageBase=image;
        illustration = "./images/"+image+"Rose.png";
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
        //m_timer = new QTimeLine(TPS*1000,this);

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
        emit tempsFini(tr("TROP TARD..."));
        QPixmap image("./images/will-let.png");
        emit tempsFini(image);
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
