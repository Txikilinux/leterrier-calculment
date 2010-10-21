/**
  * Implémentation de la fonctionnalité de sauvegarde des résultats d'exercice sur un serveur ici pour test
  *
  * @warning c'est du provisoire en attendant la vraie sauvegarde prévue pour la version 1.2
  * @see https://redmine.ryxeo.com/projects/leterrier-calculment/
  * @author 2009-2010 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
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

#include "sauvegardelog.h"

//constructeur pour la méthode GET
sauvegardeLog::sauvegardeLog(QDate date, QTime heure, QString utilisateur,QString question, QString repUtil, QString repAtt)
{
    m_protocole = new QString("http://");
    m_serveur = new QString("192.168.99.59");
    m_cible = new QString("/log.php");
    m_variable=new QString("");
        m_variable->append("?date=");
            m_variable->append(date.toString(Qt::ISODate));
        m_variable->append("&heure=");
            m_variable->append(heure.toString(Qt::ISODate));
        m_variable->append("&login=");
            m_variable->append(utilisateur);
        m_variable->append("&question=");
            m_variable->append(QUrl::toPercentEncoding(question));
        m_variable->append("&reponse=");
            m_variable->append(repUtil);
        m_variable->append("&bonnereponse=");
            m_variable->append(repAtt);
        m_variable->append("&duree=0");
    QString chaineUrl = *m_protocole;
        chaineUrl.append(m_serveur);
        chaineUrl.append(m_cible);
        chaineUrl.append(m_variable);
    m_url = new QUrl(chaineUrl,QUrl::TolerantMode);
    const QNetworkRequest requete(*m_url);
    QNetworkAccessManager *m = new QNetworkAccessManager;
    QNetworkReply *r = m->post(requete, "");
    //on peut peut être se servir de ce r pour détecter des erreurs
}
