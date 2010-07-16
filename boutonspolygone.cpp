#include "boutonspolygone.h"
#include <QtGui>

boutonsPolygone::boutonsPolygone(QWidget* parent)
        : QPushButton(parent)
{

    this->setGeometry(0,0,201,201);
}

void boutonsPolygone::paintEvent(QPaintEvent * event)
{
    // Drawing button shape
//    QPainter p(this);
//    QPointF position(79.0,0.0);
//    QPainterPath pPath(position);
//
//    QRectF rect(QPointF(104, 89), QPointF(196, 185));
//
//    pPath.lineTo(123,98);
//    pPath.arcTo(rect, 31*16, 2.5*16);
//    pPath.lineTo(0, 156);
//    pPath.lineTo(13, 58);
//    pPath.closeSubpath();
//
//    QLinearGradient upLinearGrad(position+QPointF(-66, 60), position+QPointF(30, 112));
//    QLinearGradient downLinearGrad(position+QPointF(-66, 60), position+QPointF(30, 112));
//
//    QColor topColor(154, 0, 0, 255);
//    QColor bottomColor(231, 39, 0, 255);
//    downLinearGrad.setColorAt(0, topColor);
//    downLinearGrad.setColorAt(1, bottomColor);
//    upLinearGrad.setColorAt(1, topColor);
//    upLinearGrad.setColorAt(0, bottomColor);
//
//    p.setRenderHint(QPainter::Antialiasing, true);
//    p.setPen(QPen(QBrush(Qt::black), (isDown()) ? 2 : 1));
//    p.setBrush(QBrush((isDown()) ? downLinearGrad : upLinearGrad));
//    p.drawPath(pPath);
     QPainterPath zone;
        QPointF*pt1 = new QPointF(100,0);
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
 painter.setPen(QPen(QColor(79, 106, 25), 1, Qt::SolidLine,Qt::FlatCap, Qt::MiterJoin));
 painter.drawPath(zone);
   painter.end();
    event->accept();
}
