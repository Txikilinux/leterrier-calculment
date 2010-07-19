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
    //m->PostOperation;
}
