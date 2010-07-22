#include "boutonspolygone.h"
#include "exercice.h"
#include "editeur.h"
#include <QtGui>

boutonsPolygone::boutonsPolygone(QString operation)
        : QGraphicsItem()
{
    m_base = new QPoint(0,0);
    m_taille = new QSize(100,100);
    m_angle = 0;
    m_zoom = 0;
    m_action = new QString(operation);
    this->QGraphicsItem::setCursor(Qt::PointingHandCursor);
}

//redéfinition de la zone sensible : je la mets comme le bouton
QRectF boutonsPolygone::boundingRect() const
    {
    return QRectF(m_base->x(),m_base->y(),m_taille->width(),m_taille->height());
    }

void boutonsPolygone::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::NoBrush);
    QRect rect(m_base->x(), m_base->y(), m_taille->width(), m_taille->height());
    painter->drawRect(rect.adjusted(0, 0, -1, -1));
}

void boutonsPolygone::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
    QGraphicsItem::mousePressEvent(e);
    if (*m_action=="editeur") {
        Editeur* ed = new Editeur;
        ed->show();
        }
    else if (*m_action=="sortie") emit sortie();
        else {
        exercice* ex = new exercice(*m_action);
        ex->show();
        }
    e->accept();

}

void boutonsPolygone::deplace(int x, int y)
{
    m_base->setX(x);
    m_base->setY(y);
}

void boutonsPolygone::retaille(int w, int h)
{
    m_taille->setWidth(w);
    m_taille->setHeight(h);
}

void boutonsPolygone::tourne(int angle)
{
    m_angle = angle;
    this->rotate(angle);
}
