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
    boutonsPolygone(QString,int);
    void deplace(int,int);
    void retaille(int,int);
    void tourne(int);
    void setImage(QPixmap);//2
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
    int m_val;
    QPixmap* m_image;
};

#endif // BOUTONSPOLYGONE_H
