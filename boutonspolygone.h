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
    boutonsPolygone(QString);
    void deplace(int,int);
    void retaille(int,int);
    void tourne(int);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*);

signals:
    void sortie();

private:
    QPoint* m_base;
    int m_angle;
    int m_zoom;
    QSize* m_taille;
    QString* m_action;

};

#endif // BOUTONSPOLYGONE_H
