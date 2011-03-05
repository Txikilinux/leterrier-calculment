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

private slots:
    void on_pbDocumentation_clicked();
    void on_pbInstaller_clicked();
};

#endif // ABULEDUACCUEIL_H
