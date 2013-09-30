#ifndef SAUVEGARDELOG_H
#define SAUVEGARDELOG_H

#include <QObject>
#include <QtNetwork>

class sauvegardeLog : public QObject
{
    Q_OBJECT
public:
    sauvegardeLog(QDate date, QTime heure, QString utilisateur, QString question, QString repUtil, QString repAtt);

private:
    QUrl* m_url;
    QString* m_protocole;
    QString* m_serveur;
    QString* m_cible;
    QString* m_variable;
 //   getenv("LOGIN")
};

#endif // SAUVEGARDELOG_H
