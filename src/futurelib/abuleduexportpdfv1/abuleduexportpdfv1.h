/**
  * Classe 
  * @author 2011 Eric Seigne <eric.seigne@ryxeo.com>
  * @see The GNU Public License (GNU/GPL) v3
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

#ifndef ABULEDUEXPORTPDFV1_H
#define ABULEDUEXPORTPDFV1_H

#include <QObject>
#include <QHash>
#include <QTemporaryFile>
#include <QFileInfo>
#include <QTextEdit>
#include <QPrinter>
#include <QDesktopServices>
#include <QUrl>
#include <QHashIterator>
#include <QDateTime>
#include <QDebug>
#include <QDir>

class AbulEduExportPDFV1 : public QObject
{
    Q_OBJECT
public:
    explicit AbulEduExportPDFV1(QObject *parent = 0);
    void abeExportPDFSetLogs(QHash<int, QHash<QString, QString> > arrayLogs); /** Permet d'affecter les logs */
    void abeExportPDFFile(); /** Exporte le fichier PDF dans un fichier temporaire et l'ouvre avec le lecteur PDF par defaut */
    void abeExportPDFSetLogin(QString login);
    void abeExportPDFSetSoftware(QString software);

signals:

public slots:

private:
    QHash<int, QHash<QString, QString> > m_arrayLogs;
    QString m_login;
    QString m_software;

};

#endif // ABULEDUEXPORTPDFV1_H
