#ifndef BOUTONSPOLYGONE_H
#define BOUTONSPOLYGONE_H

#include <QPushButton>

class boutonsPolygone : public QPushButton
{
    Q_OBJECT
public:
    boutonsPolygone(QWidget *parent = 0);

protected:
        void paintEvent(QPaintEvent *);

};

#endif // BOUTONSPOLYGONE_H
