/**
  * Classe de base en version zéro pour les exercices AbulEdu permettant de structurer
  * nos logiciels et respecter ainsi un certain nombre de choses dont l'interface publique
  * à laquelle le plugin peut se connecter pour échanger des données (logs pour le moment).
  * La classe AbulEduExerciceV0 hérite de QMainWindow pour que vous puissiez créer une application
  * qui dérive d'AbulEduExerciceV0 le plus simplement possible.
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/Normalisation_des_%C3%A9changes_entre_l%27application_et_le_plugin
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/Utilisation_de_abuleduexercicev0
  * @author 2009-2010 Eric Seigne <eric.seigne@ryxeo.com>
  * @see The GNU Public License (GPL)
  *
  * @warning NE MODIFIEZ SURTOUT PAS CES FICHIERS, SI VOUS LE FAITES NE SOYEZ PAS ÉTONNÉS
  *          QUE L'APPLICATION NE S'INTERFACE PLUS CORRECTEMENT AVEC LE PLUGIN !!!!!
  *
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
#include <QWidget>
#include <QSettings>
#include <QString>
#include <QDate>
#include <QHash>
#include <QEvent>

class AbulEduExerciceV0: public QMainWindow
{
    Q_ENUMS(EvaluationExercice);
public:
    AbulEduExerciceV0(QWidget *parent = 0);
    ~AbulEduExerciceV0();


    /**
      * La liste des evaluations possibles a:ok, d:ko
      * note: pour l'instant ça n'est pas utilisé: convertir un enum en
      * QString (pour expédition sur serveur), je ne sais pas faire !
      */
    enum EvaluationExercice { a, b, c, d, z };

    //----------------- IHM --------------------- //
    /** pour l'instant pas d'IHM dans cette classe, peut-être que ça viendra d'ici peu */

    //----------------- Connecteurs pour le plugin --------------------- //
    /**
      * Recupere la totalite du tableau des logs en cours
      * Normalement vous ne devriez pas avoir à utiliser ces fonctions, elles
      * ne sont la que pour permettre au plugin de se connecter sur l'application
      */
    virtual QHash<int, QHash<QString, QString> > getPluginLogs(QString login="", QString group="");
    /** Le plugin affecte le tableau des logs téléchargés */
    virtual void setPluginLogs(QHash<int, QHash<QString, QString> >);
    /** Permet d'indiquer au plugin ce qu'on veut télécharger comme logs */
    virtual QHash<QString, QString> downloadPluginLogsFilter();

    //----------------- Lanceurs d'evenements --------------------- //
    /**
      * Permet de lancer des évènements a destination du plugin
      * vous pouvez faire appel à ces fonction à n'importe quel moment
      */
    /** Provoque un evenement qui a pour objectif d'envoyer les logs sur le serveur */
    virtual void pushAbulEduLogs();
    /**
      * Déclanche le téléchargement des logs en émettant un évènement
      * @see setAbeDownloadLogsFilter qui permet de gérer les filtres de téléchargement
      */
    virtual void downloadAbulEduLogs();

    //----------------- Les evements --------------------- //
    /**
      * Enregistrement de deux évènements type permettant à l'application de lancer des
      * évènements à n'importe quel moment
      */
    static const QEvent::Type AbulEduLogsPush = static_cast<QEvent::Type>(QEvent::User+100);
    static const QEvent::Type AbulEduLogsDownload  = static_cast<QEvent::Type>(QEvent::User+101);

    //----------------- Manipulation des variables membres --------------------- //
    /**
      * @note Ces fonctions sont à utiliser comme vous le souhaitez. Pour plus de simpicité leur nom est normalisé:
      * @note - setAbe: permet d'affecter des valeurs
      * @note - getAbe: permet de récupérer des valeurs
      */

    /**
      * Stocke une ligne de log dans le tableau local en prevision de l'envoi sur le serveur de logs
      * @param question: la question posée
      * @param answer: la réponse fournie
      * @param score: le score
      * @param nbPrintedQuestions: le nombre de question qui ont déjà été affichées
      * @param evaluation: l'évaluation de la réponse, a: bien, b: bof, c: bofbof d: pas bien, z: pas de réponse
      * @param expected: la réponse qu'on attendait "bonne réponse"
      * @param answerTime: le temps que l'utilisateur a mis pour fournir la réponse
      * @param answerIntermediate: réponse intermédiaire (en cas de construction étape par étape d'une réponse)
      * @param answerProposed: la liste des réponses que le logiciel proposait (sous la forme d'une liste de
      *                 réponses sépparées par des ";" (csv)
      */
    virtual void setAbeLineLog(QString question,   QString answer,
                               int score=-1,        int nbPrintedQuestions=0, QString evaluation="",
                               QString expected="", QString answerTime="",     QString answerIntermediate="",
                               QString answerProposed="");
    /**
      * Permet au développeur de filtrer ce qu'on veut télécharger comme logs
      * @param dateBegin: date de début
      * @param dateEnd: date de fin
      * @param login: l'identifiant de l'utilisateur dont on veut récupérer les logs
      * @param group: le nom du groupe dont on veut récupérer les logs
      * @param evaluation: permet de ne récupérer que les logs dont l'évaluation a une certaine valeur
      * @param skill: nom de la compétence (au sens abuledu) qu'on recherche
      */
    virtual void setAbeDownloadLogsFilter(QDate dateBegin=QDate::currentDate(), QDate dateEnd=QDate::currentDate(),
                                          QString login="", QString group="", QString evaluation="", QString skill="");
    /** Reset le tableau des logs en cours: remet à zéro le tableau des logs */
    virtual void resetAbeLogs();
    /** Donne un nom à l'exercice */
    virtual void setAbeExerciceName(QString s);
    /** Configure le niveau de l'exercice */
    virtual void setAbeLevel(QString s);
    /** Renseigne le nombre total de questions */
    virtual void setAbeNbTotalQuestions(int n);
    /** Renseigne la compétence au sens abuledu */
    virtual void setAbeSkill(QString s);
    /** Choix du niveau de debug (pour l'instant 1 ou 0) */
    virtual void setAbeDebugLevel(int i=0);

    /** Renvoie le nom de l'exercice */
    virtual QString getAbeExerciceName();
    /** Renvoie le niveau de l'exercice */
    virtual QString getAbeLevel();
    /** Renvoie le nombre total de questions */
    virtual int getAbeNbTotalQuestions();
    /** Renvoie la compétence au sens abuledu */
    virtual QString getAbeSkill();

    // a faire ... to become en version v1
    //    virtual void setAbulEduParameters(QSettings s, QString login, QString group);
    //    /** Recupere les paramètres sur le serveur */
    //    virtual QSettings getAbulEduParameters(QString login, QString group);
    //    /** Provoque un evenement pour envoyer les paramètres sur le serveur */
    //    virtual void pushAbulEduParameters();


protected:
    QString m_exerciceName;
    QString m_level;
    int m_nbTotalQuestions;
    QString m_skill;

private:
    QHash<int, QHash<QString, QString> > m_arrayLogs;
    QSettings m_parameters;
    QHash<QString, QString> m_downloadFilter;
    int m_localDebug;
};

#endif
