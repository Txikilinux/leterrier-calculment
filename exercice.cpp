#include "exercice.h"
#include "ui_exercice.h"

#include <QGraphicsItemAnimation>
const int NBTOTAL = 1;
const int NBCHIFFRE = 2;

exercice::exercice(QString exo,QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::exercice)
{
    m_ui->setupUi(this);
    setPalette(QPalette(QColor(250, 250, 200)));
    this->setWindowTitle("Mon ballon !!");
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
    QApplication::quit();
}

void exercice::on_btnBallon_clicked()
{
    //instanciation d'une baudruche et connexion aux autres objets
    QPoint* depart = new QPoint(300,400);
    m_baudruche = new baudruche(m_operation,*depart);
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
        m_ui->lblMsg->setText("GAGNE");
        m_trace.insert(2, demande);
        }
    else {
        m_ui->lblMsg->setText("PERDU");
        m_trace.insert(1, demande);
        }
    QString monScore = "";
    monScore = monScore.setNum(m_score);
    m_ui->lblPoints->setText(monScore);

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

//            QList<QString> erreurs = m_trace.value("1");
//            for (int i = 0; i < erreurs.size(); ++i)
//                qDebug() << erreurs.at(i) << endl;
        }
}

void exercice::on_leResultat_returnPressed()
{
    if (!m_ui->leResultat->text().isEmpty()) on_btnFeu_clicked();
}
