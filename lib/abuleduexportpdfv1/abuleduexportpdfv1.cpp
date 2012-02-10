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

#include "abuleduexportpdfv1.h"

AbulEduExportPDFV1::AbulEduExportPDFV1(QObject *parent) :
    QObject(parent)
{
}

//QHash<int, QHash<QString, QString> > m_arrayLogs;
void AbulEduExportPDFV1::abeExportPDFSetLogs(QHash<int, QHash<QString, QString> > arrayLogs)
{
    m_arrayLogs = arrayLogs;
}

void AbulEduExportPDFV1::abeExportPDFSetLogin(QString login)
{
    m_login = login;
}

void AbulEduExportPDFV1::abeExportPDFSetSoftware(QString software)
{
    m_software = software;
}


void AbulEduExportPDFV1::abeExportPDFFile()
{
    QString html;
    QString styleSheet;
    QDateTime ladate = QDateTime::currentDateTime();


    QTemporaryFile file("fichier-bilan-");
//    qDebug() << "Nom du fichier temp " << file.fileName();
    if (file.open()) {
        file.close();
        QString fichierTemp(file.fileName() + ".pdf");
        QFileInfo fi(fichierTemp);
        if(! fi.exists()) {
            QTextEdit document;

            //Parcours de la QHash pour sortir le tableau des logs
            html.append(trUtf8("<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'><html><head><title>%1</title> <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />").arg("Export des logs"));

            //style css
            styleSheet.append("<style type=\"text/css\">\
body{ font-size: 8pt; font-family: Verdana, Geneva, sans-serif; } \
h1 { font-size: 10pt; }\
table { border: 1px solid black; }\
</style>");

            document.setStyleSheet(styleSheet);
            html.append(styleSheet);
            html.append(trUtf8("</head><body>"));

            //Le titre
            html.append(trUtf8("<center><h5>Utilisateur: %1 - %2 - %3</h5></center><br /><br />").arg(m_login).arg(m_software).arg(ladate.toString("dd/mm/yyyy - hh:mm:ss")));

            //Faire une boucle de parcours
            QHashIterator<int, QHash <QString,QString> > liste(m_arrayLogs);

            html.append(trUtf8("<table width=\"100%\" border=\"1\"><tr><th>Question</th><th>Réponse</th><th>Réponse attendue</th><th>Évaluation</th></tr>"));

            int numligne = 0;
            while(liste.hasNext()) {
                html.append("<tr>");
                QHash <QString, QString> ligne;
                ligne = liste.next().value();
                numligne++;
                if(ligne.count()>0) {
                    html.append(trUtf8("<td>%1</td>").arg(ligne.value("question")));
                    html.append(trUtf8("<td>%1</td>").arg(ligne.value("answer")));
                    html.append(trUtf8("<td>%1</td>").arg(ligne.value("expected")));
                    html.append(trUtf8("<td>%1</td>").arg(ligne.value("evaluation")));
                }
                html.append("</tr>");
            }
            html.append("</table>");

            html.append("</body></html>");

            document.setHtml(html);
            QPrinter printerPdf(QPrinter::HighResolution);
            printerPdf.setPageMargins(20,10,20,20,QPrinter::Millimeter);
            printerPdf.setOutputFileName(fichierTemp);
            document.print(&printerPdf);
            QDesktopServices::openUrl(fichierTemp);
        }
    }
}
