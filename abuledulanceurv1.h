#ifndef ABULEDULANCEURV1_H
#define ABULEDULANCEURV1_H

#include "exercice.h"
#include <QWidget>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>

namespace Ui {
    class AbuleduLanceurV1;
}

class AbuleduLanceurV1 : public QWidget
{
    Q_OBJECT

public:
    explicit AbuleduLanceurV1(QWidget *parent = 0);
    ~AbuleduLanceurV1();

private:
    Ui::AbuleduLanceurV1 *ui;
    QString m_nomFichierConfExercices;
    QString m_nomExercice;
    QString m_intituleExercice;
    QStringList m_listeExercices;
    QStringList m_listeNiveaux;

private slots:
    void fillCbNombre(QString);
    void fillCbExercice();
    void fillCbNiveau(QString);
//    void parcoursRecursif(QString);
    void associeNomIntitule(QString);
    void on_btnAnnuler_clicked();
    void on_btnLancer_clicked();

signals:
    void cbExerciceFini(QString exercice);
};

#endif // ABULEDULANCEURV1_H
