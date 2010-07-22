#ifndef BOUTONSPOLYGONE_H
#define BOUTONSPOLYGONE_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPushButton>

class boutonsPolygone : public QPushButton, public QGraphicsItem
{
    Q_OBJECT
public:
//    boutonsPolygone(QPointF*,QWidget *parent = 0);
    boutonsPolygone(QString);
//    boutonsPolygone(int, QWidget *parent = 0);
    void deplace(int,int);
    void retaille(int,int);
    void tourne(int);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*);


private:
    QPoint* m_base;
    int m_angle;
    int m_zoom;
    QSize* m_taille;
    QString* m_action;

};

#endif // BOUTONSPOLYGONE_H
