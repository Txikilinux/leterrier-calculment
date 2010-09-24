#include "baudruche.h"
 #include <QFontMetrics>

const int MULTIPLE_MAX=11;

baudruche::baudruche(int intMinG, int intMaxG, int intMinD, int intMaxD, int tempsAccorde, QString operation,QPoint pos,QString image)
{
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
    m_affichage = new QString("");
        m_affichage->append(aGauche);
        m_affichage->append(" ");
        m_affichage->append(m_op);
        m_affichage->append(" ");
        m_affichage->append(aDroite);

    dessineMoi(image,16);
        
    this->emetRes();
    qDebug()<<"res emis instanciation "<<m_resultat;
}

//constructeur spécifique aux valeurs approchées
baudruche::baudruche(int intMaxG, int intMaxD,int tempsAccorde, QString operation,QPoint pos,QString image)
{
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
    m_affichage = new QString("");
        m_affichage->append(aGauche);
        m_affichage->append(" ");
        m_affichage->append(m_op);
        m_affichage->append(" ");
        m_affichage->append(aDroite);
        m_affichage->append(QString::fromUtf8(" ≈"));

    dessineMoi(image,16);

    this->emetApprox();
}

//constructeur spécifique aux compléments
baudruche::baudruche(int valeurCible, int tempsAccorde,QString operation,QPoint pos,QString image)
{
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
    m_affichage = new QString("");
        m_affichage->append(aGauche);
        m_affichage->append(" ");
        m_affichage->append(m_op);
        m_affichage->append(" ? = ");
        m_affichage->append(aDroite);

    dessineMoi(image,12);

    this->emetRes();
}

//constructeur spécifique à l'affichage du résultat
baudruche::baudruche(int pts, QPoint pos,QString image)
{
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
        illustration = QCoreApplication::applicationDirPath()+"/data/images/"+image+"Rose.png";
        pixmap->setPixmap(illustration);
        pixmap->setZValue(k);
        pixmap->setPos(pos);
        this->addToGroup(pixmap);
    QGraphicsTextItem* affichage = new QGraphicsTextItem("",pixmap);
        affichage->setFont( QFont( "dejaVuSans",16 ) );
        affichage->setHtml(*msg);
        affichage->setZValue(k+1);
        affichage->setPos(40,100);
        this->addToGroup(affichage);
        //m_timer = new QTimeLine(TPS*1000,this);

}

void baudruche::dessineMoi(QString image, int taillePolice)
{
    const int k=100;
    QGraphicsPixmapItem* pixmap = new QGraphicsPixmapItem(this);
        int coulAlea = rand()%(5);
        QString illustration;
        QString imageBase="ballon";
        if (image!=0) imageBase=image;
        QFile* fichierImage = new QFile(QCoreApplication::applicationDirPath()+"/data/images/"+imageBase+"Vert.png");
        if (fichierImage->exists()) {
                switch (coulAlea) {
                    case 0 : illustration = QCoreApplication::applicationDirPath()+"/data/images/"+imageBase+"Vert.png"; break;
                    case 1 : illustration = QCoreApplication::applicationDirPath()+"/data/images/"+imageBase+"Jaune.png"; break;
                    case 2 : illustration = QCoreApplication::applicationDirPath()+"/data/images/"+imageBase+"Rouge.png"; break;
                    case 3 : illustration = QCoreApplication::applicationDirPath()+"/data/images/"+imageBase+"Orange.png"; break;
                    case 4 : illustration = QCoreApplication::applicationDirPath()+"/data/images/"+imageBase+"Bleu.png"; break;
                    case 5 : illustration = QCoreApplication::applicationDirPath()+"/data/images/"+imageBase+"Rose.png"; break;
                    }
                }
        else illustration=QCoreApplication::applicationDirPath()+"/data/images/"+imageBase;
        QPixmap imageIllustration(illustration);
            pixmap->setPixmap(imageIllustration);
            pixmap->setZValue(k);
            pixmap->setPos(m_position);
            this->addToGroup(pixmap);

    QGraphicsTextItem* affichage = new QGraphicsTextItem("",pixmap);
        affichage->setFont( QFont( "dejaVuSans",taillePolice ) );
        QFontMetrics mesureur(QFont("dejaVuSans",taillePolice));
        int longueurAffichage,largeurIllustration,decalageCentrage;
        longueurAffichage=mesureur.width(*m_affichage);
        largeurIllustration=imageIllustration.width();
        decalageCentrage=(largeurIllustration-longueurAffichage)/2;
        affichage->setHtml(*m_affichage);
        affichage->setPos(decalageCentrage,80);
        affichage->setZValue(k+1);
        this->addToGroup(affichage);
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

int baudruche::valeurApprochee(int operande, int maximum)
{
    int apeupres=0;
    if (maximum==10 && m_op=="x") return apeupres;
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
    return apeupres;
}

//QTimeLine baudruche::getMTimer()
//{
//    return m_timer;
//}

void baudruche::detruire()
{
    if (this!=NULL) {
        if (m_approximation==0) {emit valueChanged(m_resultat);//ici le problème
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
    if (this!=NULL) {
        emit valueChanged(m_resultat);//pas sûr utile si baudruche termine temps
        emit destroyed(true);
        emit destroyed();
        emit tempsFini(tr("TROP TARD..."));
        QPixmap image("./data/images/will-let.png");
        emit tempsFini(image);
        delete this;
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

void baudruche::emetMort()
{
    emit destroyed(true);
}
