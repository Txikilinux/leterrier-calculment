#ifndef BOUTONSPOLYGONE_H
#define BOUTONSPOLYGONE_H

#include <QPushButton>

class boutonsPolygone : public QPushButton
{
    Q_OBJECT
public:
    boutonsPolygone(QPointF*,QWidget *parent = 0);

protected:
        void paintEvent(QPaintEvent *);

private:
    QPointF* m_base;

};

#endif // BOUTONSPOLYGONE_H
