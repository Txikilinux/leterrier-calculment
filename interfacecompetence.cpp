#include "interfacecompetence.h"
#include "ui_interfacecompetence.h"
#include "boutonspolygone.h"
#include "interface.h"

#include <QDebug>

InterfaceCompetence::InterfaceCompetence(QString competence,QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::InterfaceCompetence)
{
    m_ui->setupUi(this);
    QString initialeCompetence = competence;
        initialeCompetence.truncate(1);
    competence.remove(0,1);
    int choix = initialeCompetence.toInt();
   qDebug() << "choix vaut " << choix << " et compétence : "<<competence;
    switch (choix) {
        case 1 :
            m_decor = new QPixmap("./images/1gdRoue.png");
            adapte(*m_decor);
                    QPoint origine(m_decor->width()/2 - 35,m_decor->height()/2 - 20);
                    int R=m_decor->width()/2.7;
                    int nb=8;
                    float deformation=0.9;
                    for (int i=0;i<nb;i++) {
                        boutonsPolygone* btn = new boutonsPolygone("tableM",i+2);
                        btn->deplace(origine.x()+R*cos((2*3.14/nb)*i), origine.y()+deformation*R*sin((2*3.14/nb)*i));
                        btn->retaille(50,42);
                        btn->QGraphicsItem::setToolTip("table x"+QString::number(i+2));
                        QPixmap im("./images/nacellePourrie.png");
                        btn->setImage(im);
                        btn->setMTransformable(2);
                        m_scene->addItem(btn);
                    }
        }
}

void InterfaceCompetence::adapte(QPixmap cheminImage)
{
    //QPixmap* imgFond = new QPixmap(cheminImage);
            QBrush* fond = new QBrush(cheminImage);
            m_ui->grvFond->setBackgroundBrush(*fond);
            m_scene = new QGraphicsScene(this);
            m_ui->grvFond->setScene(m_scene);
            m_scene->setSceneRect(0, 0, m_decor->width(), m_decor->height());
            this->resize(m_decor->width()+26,m_decor->height()+60);
}

InterfaceCompetence::~InterfaceCompetence()
{
    delete m_ui;
}

void InterfaceCompetence::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void InterfaceCompetence::on_btnFermer_clicked()
{
    this->close();
}
