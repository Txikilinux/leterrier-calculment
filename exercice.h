#ifndef EXERCICE_H
#define EXERCICE_H

#include <QtGui/QMainWindow>
#include <QGraphicsScene>
#include <QTextEdit>
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
    int m_temps;


protected:
    Ui::exercice *m_ui;
    virtual void changeEvent(QEvent *e);
    int m_score;
    int m_total;
    int m_resultatEnCours;
    int m_cible;
    QStringList* m_listeEchecs;
    QPointer<baudruche> m_baudruche;
    QPoint* m_depart;
    QString m_operation;
    QPixmap* m_imgFond;
    QGraphicsScene* m_scene;
    QString* m_trace;
    void chargerParametres();
    void animeBaudruche();
    void adapteFenetreConsigne(QString texte);

private:
    int m_minG;
    int m_maxG;
    int m_minD;
    int m_maxD;
    double m_ratioTaille;
    QTextEdit* m_consignes;

protected slots:
    void on_btnAide_clicked();
    void on_btnRejouer_clicked();
    void on_btnEditeur_clicked();
    void on_leResultat_returnPressed();
    virtual void on_btnFeu_clicked();
    virtual void on_btnBallon_clicked();
    void on_btnQuitter_clicked();
    void afficheResultat(QString);
    void pousseLogs(QString);
    virtual void ajouteErreur(QString msg);
    virtual void on_btn2chance_clicked();
};

#endif // EXERCICE_H
