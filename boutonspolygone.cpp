#include "boutonspolygone.h"
#include <QtGui>

boutonsPolygone::boutonsPolygone(QPointF* ptBase,QWidget* parent)
        : QPushButton(parent)
{
    m_base = ptBase;
    this->setGeometry(m_base->x(),m_base->y(),201,201);
}

void boutonsPolygone::paintEvent(QPaintEvent * event)
{

     QPainterPath zone;
        QPointF* pt1 = new QPointF(100,0);
        QPointF*pt2 = new QPointF(200,0);
        QPointF*pt3 = new QPointF(200,200);
        QPointF*pt4 = new QPointF(100,200);
        QPointF*pt5 = new QPointF(0,100);
        zone.moveTo(*pt5);
        zone.lineTo(*pt1);
        zone.lineTo(*pt2);
        zone.lineTo(*pt3);
        zone.lineTo(*pt4);
        zone.lineTo(*pt5);

 QPainter painter(this);
 painter.setPen(QPen(QColor(79, 106, 25), 3, Qt::NoPen ,Qt::FlatCap, Qt::MiterJoin));
 painter.drawPath(zone);
   painter.end();
    event->accept();
    this->setCursor(Qt::PointingHandCursor);
}
