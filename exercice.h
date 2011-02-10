#ifndef EXERCICE_H
#define EXERCICE_H

#include <QtGui/QMainWindow>
#include <QGraphicsScene>
#include "baudruche.h"
#include "abuleduexercicev0.h"

namespace Ui {
    class exercice;
}

class exercice : public AbulEduExerciceV0 {
    Q_OBJECT
    Q_DISABLE_COPY(exercice)
public:
    explicit exercice(QString,QWidget *parent=0, int val=0, QString niveau="");
    virtual ~exercice();
    void setImgFond(QPixmap* chemin=0);
    void adapte(QPixmap);

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::exercice *m_ui;
    int m_score;
    int m_total;
    QPoint* m_depart;
    QString m_operation;
    QGraphicsScene* m_scene;
    QPixmap* m_imgFond;
    baudruche* m_baudruche;
    QString* m_trace;
    int m_minG;
    int m_maxG;
    int m_minD;
    int m_maxD;
    int m_temps;
    int m_resultatEnCours;
    int m_cible;
    void chargerParametres();
    double m_ratioTaille;



private slots:
    void on_btnRejouer_clicked();
    void on_btnEditeur_clicked();
    void on_leResultat_returnPressed();
    void on_btnFeu_clicked();
    void on_btnBallon_clicked();
    void on_btnQuitter_clicked();
    void afficheResultat(QString);
    void pousseLogs(QString);
};

#endif // EXERCICE_H
