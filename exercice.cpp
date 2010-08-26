#include "exercice.h"
#include "ui_exercice.h"
#include "sauvegardelog.h"
#include "editeur.h"
#include <QDate>
#include <QTime>

#include <QGraphicsItemAnimation>
 #include <QMessageBox>
#include <QApplication>
 #include <QDesktopWidget>

const int NBCHIFFRE = 2;
const float SEUIL_NON_ACQUIS=0.4;
const float SEUIL_ACQUIS=0.8;

exercice::exercice(QString exo,int val, QString niveau,QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::exercice)
{
    m_ui->setupUi(this);
    QString nomImage = "./images/";
        nomImage.append(exo);
        nomImage.append(".png");
        m_imgFond = new QPixmap(nomImage);
    QRect fenetre(0,0,m_imgFond->width(),m_imgFond->height());
    qDebug()<<"Image fond "<<fenetre.width()<<" X "<<fenetre.height()<<" dans l'exercice";
    //fenetre=QA
    m_ui->vue->setGeometry(fenetre);
    qDebug()<<"Vue "<<fenetre.width()<<" X "<<fenetre.height()<<" dans l'exercice";
    //l'ouverture de la fenêtre exercice doit empêcher qu'on accède à la fenêtre interface
    this ->setWindowModality( Qt::ApplicationModal ) ;
    //le drapeau DeleteOnClose fait que l'objet créé sera détruit lors de sa fermeture
    //this->setAttribute(Qt::WA_DeleteOnClose);
    setPalette(QPalette(QColor(250, 250, 200)));
    this->setWindowTitle(tr("Mon ballon !!"));

    this->setImgFond();

    m_scene = new QGraphicsScene();
    m_ui->vue->setScene(m_scene);
    m_scene->setSceneRect(0, 100, 700, 700);
    this->m_score=0;
    this->m_total=0;
    m_ui->lblPoints->setText("0");
    m_ui->lblTotal->setText("0");
    m_ui->lblArg->setText(exo);

    m_operation=exo;

    QSettings config("./maConfig.ini", QSettings::IniFormat);
    m_nbMaxBallons = config.value("NombreBallons").toInt();

    m_niveau = new QString(niveau);

    QChar initialeExo = exo[0];
    initialeExo = initialeExo.toUpper();
    QString opCourante = exo;
    opCourante.remove(0,1);
    opCourante.push_front(initialeExo);

//    QHash<QChar, int> hash;
//        hash['+']=1;
//        hash['-']=2;
//        hash['x']=3;
//    switch (hash.value(m_operation)) {
//        case 1 : opCourante = "Addition"; break;
//        case 3 : opCourante = "Multiplication"; break;
//        }
    qDebug() <<"L'opération en cours est une "<<opCourante<<" et m_niveau valait "<<*m_niveau;

    config.beginGroup(opCourante);
        if (*m_niveau=="") *m_niveau = config.value("NiveauEnCours"+opCourante).toString();
        config.beginGroup(*m_niveau);
            m_maxG = config.value(tr("MaxGauche")).toInt();
            m_minG = config.value(tr("MinGauche")).toInt();
            m_maxD = config.value(tr("MaxDroite")).toInt();
            m_minD = config.value(tr("MinDroite")).toInt();
            qDebug() << "MaxGauche : " << m_maxG << "MinGauche : " << m_minG << "MaxDroite : " << m_maxD << "MinDroite : " << m_minD<< "Mon niveau : "<<*m_niveau;
        config.endGroup();
    config.endGroup();

    if (*m_niveau=="Personnel") m_ui->btnEditeur->setEnabled(true);
    else m_ui->btnEditeur->setDisabled(true);
    m_ui->btnBallon->setFocus();
    m_ui->btnFeu->setDisabled(true);

    if (exo=="tableA" || exo=="tableM") {
        m_minD=m_maxD=val;
        m_minG=0;
        m_maxG=9;
        }

    if (exo=="complementA"
        || exo=="complementM"
        || exo=="approcheA"
        || exo=="approcheS"
        || exo=="approcheM") {
        m_minG=m_maxG=m_minD=m_maxD=val;
        }
}

exercice::~exercice()
{
    delete m_ui;
}

void exercice::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

// void exercice::resizeEvent(QResizeEvent *event)
// {
//     if (width() > image.width() || height() > image.height()) {
//         int newWidth = qMax(width() + 128, image.width());
//         int newHeight = qMax(height() + 128, image.height());
//         resizeImage(&image, QSize(newWidth, newHeight));
//         update();
//     }
//     QWidget::resizeEvent(event);
// }


void exercice::setImgFond()
{
         QBrush* fond = new QBrush(*m_imgFond);
         m_ui->vue->setBackgroundBrush(*fond);
}

void exercice::on_btnQuitter_clicked()
{
    this->close();
}

void exercice::on_btnBallon_clicked()
{
    //instanciation d'une baudruche et connexion aux autres objets
    QPoint* depart = new QPoint(m_ui->vue->width()/2,400);

    if (m_operation=="addition"
        || m_operation==""
        || m_operation=="tableA")
                m_baudruche = new baudruche(m_minG,m_maxG,m_minD,m_maxD,"+",*depart);

    else    if (m_operation=="soustraction")
                    m_baudruche = new baudruche(m_minG,m_maxG,m_minD,m_maxD,"-",*depart);

            else if (m_operation=="multiplication"
                     || m_operation=="tableM")
                            m_baudruche = new baudruche(m_minG,m_maxG,m_minD,m_maxD,"x",*depart);

                 else if (m_operation=="complementA")
                            m_baudruche = new baudruche(m_minG,"+", *depart,"fantome");

                      else if(m_operation=="complementM")
                                m_baudruche = new baudruche(m_minG, "x", *depart);

                            else if(m_operation=="approcheA")
                                      m_baudruche = new baudruche(m_maxG,m_maxD,"+", *depart);
                                 else  QMessageBox::critical(this, "Opération inexistante", m_operation.append(QString::fromUtf8(", ça n'existe pas comme opération...")));

                  //          else {qDebug()<< "Pas d'opération portant le nom de "<<m_operation;}//Pourquoi quand même erreur de segmentation

        connect(m_baudruche, SIGNAL(valueChanged(int)),m_ui->lcdNumber, SLOT(display(int)));
        qDebug()<<" lcdNumb : "<<m_ui->lcdNumber->value();
        if (m_total<m_nbMaxBallons - 1) {
            connect(m_baudruche, SIGNAL(destroyed(bool)), m_ui->btnBallon, SLOT(setEnabled(bool)));
            connect(m_baudruche, SIGNAL(destroyed()), m_ui->btnBallon, SLOT(setFocus()));
            }
        connect(m_baudruche, SIGNAL(destroyed(bool)), m_ui->btnFeu, SLOT(setDisabled(bool)));
        connect(m_baudruche, SIGNAL(destroyed()), m_ui->leResultat, SLOT(clear()));
        connect(m_baudruche->m_timer, SIGNAL(finished()),m_baudruche, SLOT(detruireTps()));
        connect(m_baudruche, SIGNAL(tempsFini(QString)), m_ui->lblMsg, SLOT(setText(QString)));
        connect(m_baudruche, SIGNAL(tempsFini(QPixmap)), m_ui->lblImgMsg, SLOT(setPixmap(QPixmap)));
        m_baudruche->emetRes();
        m_scene->addItem(m_baudruche);

    //affichage du nombre de ballons déjà instanciés
    m_total = m_ui->lblTotal->text().toInt();
    m_total++;
    QString monTotal = "";
    monTotal = monTotal.setNum(m_total);
    m_ui->lblTotal->setText(monTotal);
    m_ui->lblMsg->setText("");
    QPixmap* rien = new QPixmap("");
    m_ui->lblImgMsg->setPixmap(*rien);

    //accessibilité des boutons
    m_ui->btnFeu->setEnabled(true);
    if (m_baudruche!=NULL) m_ui->btnBallon->setDisabled(true);
    m_ui->leResultat->setFocus(Qt::OtherFocusReason);
        //à régler si le résultat est zéro...
       // QString expressRegul = "
        QRegExp expressionReguliere("[0-9][0-9]{0,"+QString::number(NBCHIFFRE)+"}");
        m_ui->leResultat->setValidator(new QRegExpValidator(expressionReguliere, this));

    //animation du ballon
    QGraphicsItemAnimation *animation = new QGraphicsItemAnimation(m_scene);
        animation->setItem(m_baudruche);
        animation->setTimeLine(m_baudruche->m_timer);
        for (int i = 0; i < 200; i++) {
            animation->setPosAt(i/200.0, QPointF(0 , (-3*i)-(i*0.8)));
           }
        m_baudruche->m_timer->start();


}

void exercice::on_btnFeu_clicked()
{
    float proposition = m_ui->leResultat->text().toFloat();
    float reponse = m_ui->lcdNumber->value();
    QString demande = "";
        demande = m_baudruche->getMGOperande()+m_baudruche->getMOperation()+m_baudruche->getMDOperande();
    m_score = m_ui->lblPoints->text().toInt();

    if (proposition==reponse) {
        m_score++;
        m_ui->lblMsg->setText(tr("GAGNE"));
        QPixmap* imgO = new QPixmap("./images/will-win.png");
        m_ui->lblImgMsg->setPixmap(*imgO);
        }
    else {
        m_ui->lblMsg->setText(tr("PERDU"));
        QPixmap* imgN = new QPixmap("./images/will-lose.png");
        m_ui->lblImgMsg->setPixmap(*imgN);
        }
    QString monScore = "";
    monScore = monScore.setNum(m_score);
    m_ui->lblPoints->setText(monScore);

   //envoi au journal de log
        #ifdef Q_WS_WIN
        QString utilisateur("bill.gates" );
        #endif

        #ifdef Q_WS_X11
        QString utilisateur( getenv("USER") );
        #endif
        #ifdef Q_WS_MAC
            QString utilisateur( "steve.jobs" );
        #endif
    QString reponseAttendueEnString;
        reponseAttendueEnString.setNum(reponse);
    QString propositionEnString;
        propositionEnString.setNum(proposition);
    sauvegardeLog* envoieRes = new sauvegardeLog(QDate::currentDate(), QTime::currentTime(), utilisateur, m_baudruche->getMLigne(), m_ui->leResultat->text(), reponseAttendueEnString);

    if (m_baudruche!=NULL) m_baudruche->detruire();
    m_ui->btnFeu->setDisabled(true);

    if (m_total==m_nbMaxBallons) {
        //debug eric
        qDebug() << "m_total:" << m_total << " et NBTOTAL:" << m_nbMaxBallons << "et score :: " << m_score;

        QPoint* depart = new QPoint(200,400);
        m_baudruche = new baudruche(m_score,*depart);
        m_ui->vue->setScene(m_scene);

        QGraphicsPixmapItem* fondProf = new QGraphicsPixmapItem();
        QPixmap* prof = new QPixmap("./images/bof.png");
        if (m_score<m_total*SEUIL_NON_ACQUIS)
            prof = new QPixmap("./images/rate.png");
        else if (m_score>=m_total*SEUIL_ACQUIS)
                prof = new QPixmap("./images/bien.png");
            fondProf->setPixmap(*prof);
            m_scene->addItem(fondProf);
            fondProf->setPos(depart->x(),depart->y()-prof->height()/1.5);
            fondProf->setZValue(m_nbMaxBallons-1-10);

        QString tabBallons[] = {"./images/ballonBleu.png","./images/ballonJaune.png","./images/ballonRouge.png","./images/ballonVert.png","./images/ballonOrange.png"};
        for (int i=0;i<5;i++) {
            QGraphicsPixmapItem* image = new QGraphicsPixmapItem();
            QPixmap* img = new QPixmap(tabBallons[i]);
            image->setPixmap(*img);
            m_scene->addItem(image);
            image->setPos(depart->x()+35*(i+1),depart->y()+10*(i+1));
            image->setZValue(m_nbMaxBallons-1-i);
            }

    m_scene->addItem(m_baudruche);
        m_baudruche->setZValue(m_nbMaxBallons);

    QString scoreEnString;
        scoreEnString.setNum(m_score);
    QString totalEnString;
        totalEnString.setNum(m_total);
    //écriture du SCORE et du NBTOTAL dans le journal des logs
    sauvegardeLog* envoieScore = new sauvegardeLog(QDate::currentDate(), QTime::currentTime(), utilisateur, "score", totalEnString, scoreEnString);

    //mise à jour ou pas du niveau
    QSettings config("./maConfig.ini", QSettings::IniFormat);
    QChar initialeExo = m_operation[0];
    initialeExo = initialeExo.toUpper();
    QString opCourante = m_operation;
    opCourante.remove(0,1);
    opCourante.push_front(initialeExo);
//    QHash<QChar, int> hash;
//        hash['+']=1;
//        hash['-']=2;
//        hash['x']=3;
//    switch (hash.value(m_operation)) {
//        case 1 : opCourante = "Addition"; break;
//        case 3 : opCourante = "Multiplication"; break;
//        }

    config.beginGroup(opCourante);
        if (m_score==m_total) {
            if (*m_niveau=="Niveau1") config.setValue("NiveauEnCours"+opCourante, "Niveau2");
            else if (*m_niveau=="Niveau2") config.setValue("NiveauEnCours"+opCourante, "Niveau3");
                 else if (*m_niveau=="Niveau3") config.setValue("NiveauEnCours"+opCourante, "Personnel");
    }


       //*m_niveau = config.value("NiveauEnCours"+opCourante).toString();

    config.endGroup();
        }
}

void exercice::on_leResultat_returnPressed()
{
    if (!m_ui->leResultat->text().isEmpty()) on_btnFeu_clicked();
}

void exercice::on_btnEditeur_clicked()
{
   // this ->hide();
   Editeur* ed = new Editeur(this);
   ed->show();
    //idee eric, fait appel a l objet "global"
   // parent->m_editeur->show();
}
