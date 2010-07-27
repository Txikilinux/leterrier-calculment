#include "exercice.h"
#include "ui_exercice.h"
#include "sauvegardelog.h"
#include "editeur.h"
#include <QDate>
#include <QTime>

#include <QGraphicsItemAnimation>
const int NBTOTAL = 5;
const int NBCHIFFRE = 2;
const int INTMING = 0;
const int INTMAXG = 9;
const int INTMIND = 0;
const int INTMAXD = 9;

exercice::exercice(QString exo,QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::exercice)
{
    m_ui->setupUi(this);
    //l'ouverture de la fenêtre exercice doit empêcher qu'on accède à la fenêtre interface
    this ->setWindowModality( Qt::ApplicationModal ) ;
    //le drapeau DeleteOnClose fait que l'objet créé sera détruit lors de sa fermeture
    //this->setAttribute(Qt::WA_DeleteOnClose);
    setPalette(QPalette(QColor(250, 250, 200)));
    this->setWindowTitle(tr("Mon ballon !!"));
    QPixmap* imgFond = new QPixmap("./images/philippe.jpg");
    QBrush* fond = new QBrush(*imgFond);
    m_ui->vue->setBackgroundBrush(*fond);
    m_scene = new QGraphicsScene();
    m_ui->vue->setScene(m_scene);
    m_scene->setSceneRect(0, 100, 700, 700);
    this->m_score=0;
    this->m_total=0;
    m_ui->lblPoints->setText("0");
    m_ui->lblTotal->setText("0");
    m_ui->lblArg->setText(exo);
    if (exo=="addition" || exo=="") m_operation='+';
    else if (exo=="soustraction") m_operation='-';
         else if (exo=="multiplication") m_operation='x';

    Editeur ed;
   // m_niveau = new QString(ed.getNiveauEnCours()); //mais en fait en faisant comme ça c'est toujours le niveau 1 !!!
    m_niveau = new QString("Niveau3");
    QSettings config("./maConfig.ini", QSettings::IniFormat);
    QString opCourante="Addition";
    QHash<QChar, int> hash;
        hash['+']=1;
        hash['-']=2;
        hash['x']=3;

    switch (hash.value(m_operation)) {
        case 1 : opCourante = "Addition"; break;
        case 3 : opCourante = "Multiplication"; break;
        }
    qDebug() <<"L'opération en cours est une "<<opCourante;

    config.beginGroup(opCourante);
        *m_niveau = config.value("NiveauEnCours"+opCourante).toString();
        config.beginGroup(*m_niveau);
            m_maxG = config.value(tr("MaxGauche")).toInt();
            m_minG = config.value(tr("MinGauche")).toInt();
            m_maxD = config.value(tr("MaxDroite")).toInt();
            m_minD = config.value(tr("MinDroite")).toInt();
            qDebug() << "MaxGauche : " << m_maxG << "MinGauche : " << m_minG << "MaxDroite : " << m_maxD << "MinDroite : " << m_minD<< "Mon niveau : "<<*m_niveau;
        config.endGroup();
    config.endGroup();
    m_ui->btnBallon->setFocus();

    m_ui->btnFeu->setDisabled(true);
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

void exercice::on_btnQuitter_clicked()
{
    this->close();
}

void exercice::on_btnBallon_clicked()
{
    //instanciation d'une baudruche et connexion aux autres objets
    QPoint* depart = new QPoint(300,400);
    m_baudruche = new baudruche(m_minG,m_maxG,m_minD,m_maxD,m_operation,*depart);
        connect(m_baudruche, SIGNAL(valueChanged(int)),m_ui->lcdNumber, SLOT(display(int)));
        if (m_total<NBTOTAL - 1) {
            connect(m_baudruche, SIGNAL(destroyed(bool)), m_ui->btnBallon, SLOT(setEnabled(bool)));
            connect(m_baudruche, SIGNAL(destroyed()), m_ui->btnBallon, SLOT(setFocus()));
        }
        connect(m_baudruche, SIGNAL(destroyed(bool)), m_ui->btnFeu, SLOT(setDisabled(bool)));
        connect(m_baudruche, SIGNAL(destroyed()), m_ui->leResultat, SLOT(clear()));
        connect(m_baudruche->m_timer, SIGNAL(finished()),m_baudruche, SLOT(detruireTps()));
        connect(m_baudruche, SIGNAL(tempsFini(QString)), m_ui->lblMsg, SLOT(setText(QString)));
        m_baudruche->emetRes();
        m_scene->addItem(m_baudruche);

    //affichage du nombre de ballons déjà instanciés
    m_total = m_ui->lblTotal->text().toInt();
    m_total++;
    QString monTotal = "";
    monTotal = monTotal.setNum(m_total);
    m_ui->lblTotal->setText(monTotal);
    m_ui->lblMsg->setText("");

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
        }
    else {
        m_ui->lblMsg->setText(tr("PERDU"));
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

    if (m_total==NBTOTAL) {
        //debug eric
        qDebug() << "m_total:" << m_total << " et NBTOTAL:" << NBTOTAL << "et score :: " << m_score;

        QPoint* depart = new QPoint(200,300);
        m_baudruche = new baudruche(m_score,*depart);
        m_ui->vue->setScene(m_scene);
        QString tabBallons[] = {"./images/ballonBleu.png","./images/ballonJaune.png","./images/ballonRouge.png","./images/ballonVert.png","./images/ballonOrange.png"};
        for (int i=0;i<5;i++) {
            QGraphicsPixmapItem* image = new QGraphicsPixmapItem();
            QPixmap* img = new QPixmap(tabBallons[i]);
            image->setPixmap(*img);
            m_scene->addItem(image);
            image->setPos(depart->x()+35*(i+1),depart->y()+10*(i+1));
            image->setZValue(NBTOTAL-1-i);
            }

    m_scene->addItem(m_baudruche);
        m_baudruche->setZValue(NBTOTAL);

    QString scoreEnString;
        scoreEnString.setNum(m_score);
    QString totalEnString;
        totalEnString.setNum(m_total);
    //écriture du SCORE et du NBTOTAL dans le journal des logs
    sauvegardeLog* envoieScore = new sauvegardeLog(QDate::currentDate(), QTime::currentTime(), utilisateur, "score", totalEnString, scoreEnString);

    //mise à jour ou pas du niveau
    QSettings config("./maConfig.ini", QSettings::IniFormat);
    QString opCourante="Addition";
    QHash<QChar, int> hash;
        hash['+']=1;
        hash['-']=2;
        hash['x']=3;
    switch (hash.value(m_operation)) {
        case 1 : opCourante = "Addition"; break;
        case 3 : opCourante = "Multiplication"; break;
        }

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
