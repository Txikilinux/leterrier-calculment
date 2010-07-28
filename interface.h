#ifndef INTERFACE_H
#define INTERFACE_H

#include <QtGui/QMainWindow>
#include "editeur.h"
#include "exercice.h"

namespace Ui
{
    class interfaceClass;
}

class interface : public QMainWindow
{
    Q_OBJECT

public:
    interface(QWidget *parent = 0);
    ~interface();
    Editeur* m_editeur;
    exercice* m_exercice;

protected:
    void paintEvent(QPaintEvent* e );

private:
    Ui::interfaceClass *ui;

private slots:


private slots:
    void on_actionAfficher_l_diteur_triggered();
    void on_actionX_9_triggered();
    void on_actionX_8_triggered();
    void on_actionX_7_triggered();
    void on_actionX_6_triggered();
    void on_actionX_5_triggered();
    void on_actionX_4_triggered();
    void on_actionX_3_triggered();
    void on_actionX_2_triggered();
    void on_actionp9_triggered();
    void on_actionp8_triggered();
    void on_actionp7_triggered();
    void on_actionp6_triggered();
    void on_actionp5_triggered();
    void on_actionp4_triggered();
    void on_actionp3_triggered();
    void on_actionp2_triggered();
    void on_actionMultiplications_triggered();
    void on_actionAdditions_triggered();
    void on_actionQuitter_triggered();
};

#endif // INTERFACE_H
