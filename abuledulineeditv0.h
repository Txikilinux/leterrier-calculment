#ifndef ABULEDULINEEDITV0_H
#define ABULEDULINEEDITV0_H

#include <QLineEdit>
#include <QDebug>
#include <QKeyEvent>

class AbulEduLineEditV0 : public QLineEdit
{
public:
    AbulEduLineEditV0(QWidget* parent);
protected:
    void keyPressEvent(QKeyEvent *);
};

#endif // ABULEDULINEEDITV0_H
