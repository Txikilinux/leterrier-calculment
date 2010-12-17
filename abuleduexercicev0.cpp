/**
  * Classe de base en version zéro pour les exercices AbulEdu permettant de structurer
  * nos logiciels et respecter ainsi un certain nombre de choses dont l'interface publique
  * à laquelle le plugin peut se connecter pour échanger des données (logs pour le moment)
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/Normalisation_des_%C3%A9changes_entre_l%27application_et_le_plugin
  * @author 2009-2010 Eric Seigne <eric.seigne@ryxeo.com>
  * @see The GNU Public License (GPL)
  *
  *
  * @warning: NE MODIFIEZ SURTOUT PAS CES FICHIERS, SI VOUS LE FAITES NE SOYEZ PAS ÉTONNÉS
  *           QUE L'APPLICATION NE S'INTERFACE PLUS CORRECTEMENT AVEC LE PLUGIN !!!!!
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

#include <QtGui/QApplication>
#include <QDebug>
#include <QPluginLoader>
#include <QDir>
#include "abuleduexercicev0.h"


/** constructeur ... */
AbulEduExerciceV0::AbulEduExerciceV0(QWidget *parent) :
    QMainWindow(parent)
{  
    //Initialisation des différentes variables
    m_exerciceName     = "";
    m_level            = "";
    m_nbTotalQuestions = 0;
    m_skill            = "";
    m_localDebug       = 1;
    m_arrayLogs.clear();
    m_downloadFilter.clear();

    //Ensuite on charge les plugins qui se trouvent dans le sous dossier plugins de l'application
    QDir pluginsDir(qApp->applicationDirPath());
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#if defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    if(pluginsDir.cd("plugins")) {
        foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
            if(m_localDebug)
                qDebug() << "Chargement du plugin " << fileName;
            QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
            QObject *plugin = pluginLoader.instance();
            if(m_localDebug)
                qDebug() << "Chargement du plugin (2)" << plugin;
            if (plugin && m_localDebug) {
                qDebug() << "Chargement du plugin " << fileName << " réussi";
            }
        }
    }
}

AbulEduExerciceV0::~AbulEduExerciceV0()
{

}

void AbulEduExerciceV0::setAbeLineLog(QString question, QString answer,
                                      int score, int nbPrintedQuestions, QString evaluation,
                                      QString expected, QString answerTime, QString answerIntermediate, QString answerProposed)
{
    //Construction de la structure de données
    QHash <QString, QString> line;
    line.insert("exerciceName", m_exerciceName);
    line.insert("level", m_level);
    line.insert("score", QString::number(score));
    line.insert("nbTotalQuestions", QString::number(m_nbTotalQuestions));
    line.insert("nbPrintedQuestions", QString::number(nbPrintedQuestions));
    line.insert("question", question);
    line.insert("answer", answer);
    line.insert("evaluation", evaluation);
    if(expected != "")
        line.insert("expected", expected);
    if(m_skill != "")
        line.insert("skill", m_skill);
    if(answerTime != "")
        line.insert("answerTime", answerTime);
    if(answerIntermediate != "")
        line.insert("answerIntermediate", answerIntermediate);
    if(answerProposed != "")
        line.insert("answerProposed", answerProposed);

    m_arrayLogs.insert(m_arrayLogs.count(),line);
}

void AbulEduExerciceV0::resetAbeLogs()
{
    m_arrayLogs.clear();
}

QHash<int, QHash<QString, QString> >  AbulEduExerciceV0::getPluginLogs(QString login, QString group)
{
    if(m_localDebug)
        qDebug() << "abuleduexercicesv0.cpp getPluginLogs " << m_arrayLogs;
    return m_arrayLogs;
}

QHash<int, QHash<QString, QString> >  AbulEduExerciceV0::getPluginLogsDownloaded()
{
    if(m_localDebug)
        qDebug() << "abuleduexercicesv0.cpp getPluginLogsDownloaded() ";
    return m_arrayLogsDownloaded;
}

QHash<int, QString> AbulEduExerciceV0::getPluginUsers()
{
    if(m_localDebug)
        qDebug() << "abuleduexercicesv0.cpp getPluginUsers " << m_arrayUsers;
    return m_arrayUsers;
}

QHash<QString, QString> AbulEduExerciceV0::getPluginGroups()
{
    if(m_localDebug)
        qDebug() << "abuleduexercicesv0.cpp getPluginGroups " << m_arrayGroups;
    return m_arrayGroups;
}

QHash<QString, QString> AbulEduExerciceV0::getPluginDomains()
{
    if(m_localDebug)
        qDebug() << "abuleduexercicesv0.cpp getPluginDomains " << m_arrayDomains;
    return m_arrayDomains;
}

QHash<int, QString> AbulEduExerciceV0::getPluginApplications()
{
    if(m_localDebug)
        qDebug() << "abuleduexercicesv0.cpp getPluginApplications " << m_arrayApplications;
    return m_arrayApplications;
}

QHash<int, QString> AbulEduExerciceV0::getPluginSkills()
{
    if(m_localDebug)
        qDebug() << "abuleduexercicesv0.cpp getPluginSkills " << m_arraySkills;
    return m_arraySkills;
}

QHash<int, QString> AbulEduExerciceV0::getPluginExercices()
{
    if(m_localDebug)
        qDebug() << "abuleduexercicesv0.cpp getPluginExercices " << m_arrayExercices;
    return m_arrayExercices;
}

void AbulEduExerciceV0::pushAbulEduLogs()
{
    QEvent* logEvent = new QEvent(AbulEduLogsPush);
    QApplication::postEvent(this, logEvent);
}


void AbulEduExerciceV0::setAbeExerciceName(QString s)
{
    m_exerciceName = s;
}

void AbulEduExerciceV0::setAbeLevel(QString s)
{
    m_level = s;
}

void AbulEduExerciceV0::setAbeNbTotalQuestions(int n)
{
    m_nbTotalQuestions = n;
}

void AbulEduExerciceV0::setAbeSkill(QString s)
{
    m_skill = s;
}

QString AbulEduExerciceV0::getAbeExerciceName()
{
    return m_exerciceName;
}

QString AbulEduExerciceV0::getAbeLevel()
{
    return m_level;
}

int AbulEduExerciceV0::getAbeNbTotalQuestions()
{
    return m_nbTotalQuestions;
}

QString AbulEduExerciceV0::getAbeSkill()
{
    return m_skill;
}

QHash<QString, QString> AbulEduExerciceV0::downloadPluginLogsFilter()
{
    return m_downloadFilter;
}

void AbulEduExerciceV0::setPluginLogs(QHash<int, QHash<QString, QString> > h)
{
    qDebug() << "abuleduexercicev0.cpp : setPluginLogs (1)" << h;
    m_arrayLogsDownloaded.clear();
    m_arrayLogsDownloaded = h;
    qDebug() << "abuleduexercicev0.cpp : setPluginLogs (2)";
}

void AbulEduExerciceV0::setPluginUsers(QHash<int, QString> h)
{
    m_arrayUsers.clear();
    m_arrayUsers = h;
}

void AbulEduExerciceV0::setPluginGroups(QHash<QString, QString> h)
{
    m_arrayGroups.clear();
    m_arrayGroups = h;
}

void AbulEduExerciceV0::setPluginDomains(QHash<QString, QString> h)
{
    m_arrayDomains.clear();
    m_arrayDomains = h;
}

void AbulEduExerciceV0::setPluginApplications(QHash<int, QString> h)
{
    m_arrayApplications.clear();
    m_arrayApplications = h;
}

void AbulEduExerciceV0::setPluginSkills(QHash<int, QString> h)
{
    m_arraySkills.clear();
    m_arraySkills = h;
}

void AbulEduExerciceV0::setPluginExercices(QHash<int, QString> h)
{
    m_arrayExercices.clear();
    m_arrayExercices = h;
}

void AbulEduExerciceV0::downloadAbulEduLogs()
{
    QEvent* downloadEvent = new QEvent(AbulEduLogsDownload);
    QApplication::postEvent(this, downloadEvent);
}

void AbulEduExerciceV0::downloadAbulEduUsers()
{
    QEvent* downloadEvent = new QEvent(AbulEduUsersDownload);
    QApplication::postEvent(this, downloadEvent);
}

void AbulEduExerciceV0::downloadAbulEduGroups()
{
    QEvent* downloadEvent = new QEvent(AbulEduGroupsDownload);
    QApplication::postEvent(this, downloadEvent);
}

void AbulEduExerciceV0::downloadAbulEduDomains()
{
    QEvent* downloadEvent = new QEvent(AbulEduDomainsDownload);
    QApplication::postEvent(this, downloadEvent);
}

void AbulEduExerciceV0::downloadAbulEduApplications()
{
    QEvent* downloadEvent = new QEvent(AbulEduApplicationsDownload);
    QApplication::postEvent(this, downloadEvent);
}

void AbulEduExerciceV0::downloadAbulEduSkills()
{
    QEvent* downloadEvent = new QEvent(AbulEduSkillsDownload);
    QApplication::postEvent(this, downloadEvent);
}

void AbulEduExerciceV0::downloadAbulEduExercices()
{
    QEvent* downloadEvent = new QEvent(AbulEduExercicesDownload);
    QApplication::postEvent(this, downloadEvent);
}


void AbulEduExerciceV0::setAbeDownloadLogsFilter(QDate dateBegin, QDate dateEnd, QString login, QString group, QString evaluation, QString skill)
{
    m_downloadFilter.insert("login",login);
    m_downloadFilter.insert("group",group);
    m_downloadFilter.insert("dateBegin",dateBegin.toString("yyyy-MM-dd"));
    m_downloadFilter.insert("dateEnd",dateEnd.toString("yyyy-MM-dd"));
    m_downloadFilter.insert("evaluation",evaluation);
    m_downloadFilter.insert("skill",skill);
}

void AbulEduExerciceV0::setAbeDebugLevel(int i)
{
    m_localDebug = i;
}

void AbulEduExerciceV0::sendSignalDownloaded(QString s)
{
    emit signalDownloaded(s);
}
