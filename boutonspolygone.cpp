#include "boutonspolygone.h"
#include <QtGui>

//boutonsPolygone::boutonsPolygone(QPoint* ptBase,QWidget* parent)
boutonsPolygone::boutonsPolygone(QWidget* parent)
        : QPushButton(parent)
{
    //m_base = ptBase;
    m_base = new QPoint(0,0);
    m_taille = new QSize(201,201);
    this->setGeometry(m_base->x(),m_base->y(),m_taille->width(),m_taille->height());
    m_angle = 0;
    m_zoom = 0;
}

//boutonsPolygone::boutonsPolygone(int angle,QWidget* parent)
//        : QPushButton(parent)
//{
//    //m_base = ptBase;
//    m_base = new QPoint(0,0);
//    m_taille = new QSize(201,201);
//    this->setGeometry(m_base->x(),m_base->y(),m_taille->width(),m_taille->height());
//    m_angle = angle;
//    m_zoom = 0;
//}

void boutonsPolygone::paintEvent(QPaintEvent * event)
{

     QPainterPath zone;
//        QPoint* pt1 = new QPointF(0 + m_taille->width()*sin(m_angle),0 + m_taille->width()*cos(m_angle));
//        QPoint*pt2 = new QPointF(m_taille->width() + m_taille->width()*sin(m_angle)-1,0 + m_taille->width()*cos(m_angle));
//        QPoint*pt3 = new QPointF(m_taille->width() + m_taille->width()*sin(m_angle)-1,m_taille->height() + m_taille->width()*cos(m_angle)-1);
//        QPoint*pt4 = new QPointF(0 + m_taille->width()*sin(m_angle),m_taille->height() + m_taille->width()*cos(m_angle)-1);
        QPoint* pt1 = new QPoint(0,0);
        QPoint*pt2 = new QPoint(m_taille->width()-1,0);
        QPoint*pt3 = new QPoint(m_taille->width()-1,m_taille->height()-1);
        QPoint*pt4 = new QPoint(0,m_taille->height()-1);
        zone.lineTo(*pt2);
        zone.lineTo(*pt3);
        zone.lineTo(*pt4);
        zone.lineTo(*pt1);

     QPainter painter(this);
     //painter.setPen(QPen(QColor(79, 106, 25), 3, Qt::NoPen ,Qt::FlatCap, Qt::MiterJoin));
     //Si besoin de visualiser les contours
     painter.setPen(QPen(QColor(79, 106, 25), 3, Qt::DashLine ,Qt::FlatCap, Qt::MiterJoin));
     painter.rotate(m_angle);
     painter.drawPath(zone);

     painter.end();
     event->accept();
     this->setCursor(Qt::PointingHandCursor);
}

void boutonsPolygone::deplace(int x, int y)
{
    m_base->setX(x);
    m_base->setY(y);
    this->setGeometry(x,y, m_taille->width(), m_taille->height());
}

void boutonsPolygone::retaille(int w, int h)
{
    m_taille->setWidth(w);
    m_taille->setHeight(h);
    this->setGeometry(m_base->x(), m_base->y(), w, h);
}

void boutonsPolygone::tourne(int angle)
{
    m_angle = angle;
    this->repaint();
}
