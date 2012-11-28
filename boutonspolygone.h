#ifndef BOUTONSPOLYGONE_H
#define BOUTONSPOLYGONE_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPushButton>

class boutonsPolygone : public QPushButton, public QGraphicsItem
{
    Q_OBJECT
          //Q_INTERFACES(QGraphicsItem)
public:
    boutonsPolygone(QString,int=0);//
    void deplace(int,int);
    void bouge(int,int);
    //void eclate();
    //void tombe(int,int,int,QGraphicsScene);
    void retaille(int,int);
    void tourne(int);
    void setImage(QPixmap);
    void setTexte(QString);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
    QPoint getMBase();
    QSize getMTaille();
    void setMTransformable(int);

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
    QString* m_texte;
    int m_val;
    QPixmap* m_image;
    int m_transformable;

private slots:
    void slotMontreLanceur();

};

#endif // BOUTONSPOLYGONE_H
