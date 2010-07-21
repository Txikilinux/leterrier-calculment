#ifndef BOUTONSPOLYGONE_H
#define BOUTONSPOLYGONE_H

#include <QPushButton>

class boutonsPolygone : public QPushButton
{
    Q_OBJECT
public:
//    boutonsPolygone(QPointF*,QWidget *parent = 0);
    boutonsPolygone(QWidget *parent = 0);
//    boutonsPolygone(int, QWidget *parent = 0);
    void deplace(int,int);
    void retaille(int,int);
    void tourne(int);

protected:
    void paintEvent(QPaintEvent *);

private:
    QPoint* m_base;
    int m_angle;
    int m_zoom;
    QSize* m_taille;

};

#endif // BOUTONSPOLYGONE_H
