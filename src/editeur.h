/** Editeur de paramètres d'exercices pour le logiciel Calcul Mental
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/LeTerrierExercice
  * @author 2010-2014 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
  * @author 2013 Eric Seigne <eric.seigne@ryxeo.com>
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

#ifndef EDITEUR_H
#define EDITEUR_H

#include <QWidget>
#include <QSettings>

namespace Ui {
    class Editeur;
}

class Editeur : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(Editeur)
public:
    explicit Editeur(QWidget *parent = 0);
    virtual ~Editeur();
    int getNiveauEnCours();
    void initialiser();

protected:
    virtual void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::Editeur *m_ui;
    bool m_localDebug;
    int m_niveauEnCours;
    QString *m_operationEnCours;
    QString *m_nomOperationEnCours;
    QSettings* m_settings;
    int m_minG;
    int m_maxG;
    int m_minD;
    int m_maxD;
    void initialiserOperation(QString);
    void initialiserApproche(QString operation);
    void initialiserApprocheM(QString operation);
    void initialiserComplement(QString operation);
    void initialiserDivision();
    QString associeNomIntitule(QString intitule);
    ///
    /// \brief Fonction qui installe les eventFilter sur les objects à surveiller
    ///
    void installEventFilters();

private slots:
    void sauverNiveau(QString);
    void chargerNiveau(QString);
    void changerNiveau(QString);
    void sauverOperation(QString);
    void chargerOperation(QString);
    void changerOperation(QString);
    void ajusterValeurs(int);

    ///
    /// \brief Enregistre les modifications dans l'éditeur et affiche la page d'accueil
    ///
    void on_btnEditeurOK_clicked();

    ///
    /// \brief Annule les modifications dans l'éditeur et affiche la page d'accueil
    ///
    void on_btnEditeurAnnuler_clicked();

signals:
    void signalEditeurExited();
};

#endif // EDITEUR_H
