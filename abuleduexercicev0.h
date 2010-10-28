/**
  * Classe de base pour les exercices AbulEdu permettant de structurer nos logiciels
  * et respecter ainsi un certain nombre de choses
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/Normalisation_des_%C3%A9changes_entre_l%27application_et_le_plugin
  * @author 2009-2010 Eric Seigne <eric.seigne@ryxeo.com>
  * @see The GNU Public License (GPL)
  */
/*
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
#ifndef ABULEDUEXERCICEV0_H
#define ABULEDUEXERCICEV0_H

#include <QMainWindow>
#include <QString>
#include <QHash>
#include <QEvent>

class AbulEduExerciceV0: public QMainWindow
{
    Q_ENUMS(EvaluationExercice);
public:
    AbulEduExerciceV0(QWidget *parent = 0);
    ~AbulEduExerciceV0();

    /** La liste des evaluations possibles a:ok, d:ko*/
    enum EvaluationExercice { a, b, c, d, z };

    /** Stocke une ligne de log dans le tableau local en prevision de l'envoi sur le serveur de logs */
    //EvaluationExercice evaluation
    virtual void setAbulEduLineLog(int score,int nbPrintedQuestions, QString question,QString answer,QString evaluation,
                                   QString expected=0,QString answerTime=0,QString answerIntermediate=0,QString answerProposed=0);

    /** Recupere la totalite du tableau des logs en cours */
    virtual QHash<int, QHash<QString, QString> > getAbulEduLogs();

    /** Reset le tableau des logs en cours */
    virtual void resetAbulEduLogs();

    /** Provoque un evenement pour envoyer les logs sur le serveur */
    virtual void pushAbulEduLogs();

    static const QEvent::Type AbulEduLogsSend = static_cast<QEvent::Type>(QEvent::User+100);
    static const QEvent::Type AbulEduLogsGet  = static_cast<QEvent::Type>(QEvent::User+101);

    /** Donne un nom à l'exercice */
    virtual void setAbeExerciceName(QString s);

    /** Donne un nom à l'exercice */
    virtual void setAbeLevel(QString s);
    /** Renseigne le nombre total de questions */
    virtual void setAbeNbTotalQuestions(int n);
    /** Renseigne la compétence au sens Éducation Nationale (FR) */
    virtual void setAbeSkillFr(QString s);
    /** Renvoie le nom à l'exercice */
    virtual QString getAbeExerciceName();
    /** Renvoie le niveau de l'exercice */
    virtual QString getAbeLevel();
    /** Renvoie le nombre total de questions */
    virtual int getAbeNbTotalQuestions();
    /** Renvoie la compétence au sens Éducation Nationale (FR) */
    virtual QString getAbeSkillFr();

protected:
    QString m_exerciceName;
    QString m_level;
    int m_nbTotalQuestions;
    QString m_skillFr;

private:
    QHash<int, QHash<QString, QString> > m_arrayLogs;
};

#endif

