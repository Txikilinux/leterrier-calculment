#ifndef ABULEDUACCUEIL_H
#define ABULEDUACCUEIL_H

#include <QMainWindow>

namespace Ui {
    class AbulEduAccueil;
}

class AbulEduAccueil : public QMainWindow {
    Q_OBJECT
public:
    AbulEduAccueil(QWidget *parent = 0);
    ~AbulEduAccueil();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::AbulEduAccueil *ui;
};

#endif // ABULEDUACCUEIL_H
