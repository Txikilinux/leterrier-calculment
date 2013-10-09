#ifndef INTERFACE_H
#define INTERFACE_H

#include <QMainWindow>
#include "editeur.h"
#include "exercice.h"
#include "abuleduapplicationv1.h"

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
    int m_hauteurMax;

protected:
    void paintEvent(QPaintEvent* e );

private:
    Ui::interfaceClass *ui;
    QTranslator qtTranslator;
    QTranslator myappTranslator;
    QString m_locale;
    QSignalMapper* m_signalMapper;
    void creeMenuLangue();

private slots:


private slots:
    void on_actionSur_des_multiplications_triggered();
    void on_actionSur_des_soustractions_triggered();
    void on_actionSur_des_additions_triggered();
    void on_action_un_nombre_al_atoire_triggered();
    void on_action_10_triggered();
    void on_action_20_triggered();
    void on_action_100_triggered();
    void on_actionSoustractions_triggered();
    void on_actionDe_5_triggered();
    void on_actionDe_10_triggered();
    void on_actionDe_15_triggered();
    void on_actionDe_20_triggered();
    void on_actionDe_25_triggered();
    void on_actionDe_50_triggered();
    void on_btnInitialise_clicked();
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
    void changelangue(QString langue);
    void on_actionMaison_des_nombres_triggered();
    void on_actionVerrouillage_nombres_changed();

    void on_action_Journal_de_mes_activit_s_triggered();
    void on_action_Changer_d_utilisateur_triggered();

};

#endif // INTERFACE_H
