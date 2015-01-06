/** Lanceur sélectif pour les exercices du logiciel Calcul mental
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/LeTerrierExercice
  * @author 2011-2012 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2013 Icham Sirat <icham.sirat@ryxeo.com>
  * @see The GNU Public License (GPL)
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 2 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful, but
  * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  * for more details.
  *
  * You should have received a copy of the GNU General Public License along
  * with this program; if not, write to the Free Software Foundation, Inc.,
  * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
  */

#ifndef ABULEDULANCEURV1_H
#define ABULEDULANCEURV1_H

#include "exercice.h"
#include "exercicemaisonnombres.h"
#include <QWidget>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>
#include <QDesktopWidget>

namespace Ui {
    class AbuleduLanceurV1;
}

class AbuleduLanceurV1 : public QWidget
{
    Q_OBJECT

public:
    explicit AbuleduLanceurV1(AbulEduIdentitesV1* identite, QWidget *parent = 0);
    ~AbuleduLanceurV1();

private:
    Ui::AbuleduLanceurV1 *ui;
    QString m_nomFichierConfExercices;
    QString m_nomExercice;
    QString m_intituleExercice;
    QStringList m_listeExercices;
    QStringList m_listeNiveaux;
    void adapte();

private slots:
    void fillCbNombre(QString);
    void fillCbExercice();
    void fillCbNiveau(QString);
//    void parcoursRecursif(QString);
    void associeNomIntitule(QString);
    void on_btnAnnuler_clicked();
    void on_btnLancer_clicked();

    void on_chbBilan_clicked();

signals:
    void cbExerciceFini(QString exercice);
};

#endif // ABULEDULANCEURV1_H
