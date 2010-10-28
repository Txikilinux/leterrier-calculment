#include <QtGui/QApplication>
#include <QDebug>
#include "abuleduexercicev0.h"

AbulEduExerciceV0::AbulEduExerciceV0(QWidget *parent) :
    QMainWindow(parent)
{
    m_exerciceName     = "";
    m_level            = "";
    m_nbTotalQuestions = 0;
    m_skillFr          = "";
    m_arrayLogs.clear();
}

AbulEduExerciceV0::~AbulEduExerciceV0()
{
    delete this;
}

    void AbulEduExerciceV0::setAbulEduLineLog(int score,int nbPrintedQuestions, QString question,QString answer,QString evaluation,
                                              QString expected,QString answerTime,QString answerIntermediate,QString answerProposed)
{
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
    if(m_skillFr != "")
        line.insert("skillFr", m_skillFr);
    if(answerTime != "")
        line.insert("answerTime", answerTime);
    if(answerIntermediate != "")
        line.insert("answerIntermediate", answerIntermediate);
    if(answerProposed != "")
        line.insert("answerProposed", answerProposed);

    m_arrayLogs.insert(m_arrayLogs.count(),line);
}

void AbulEduExerciceV0::resetAbulEduLogs()
{
    m_arrayLogs.clear();
}

QHash<int, QHash<QString, QString> >  AbulEduExerciceV0::getAbulEduLogs()
{
    //qDebug() << m_arrayLogs;
    return m_arrayLogs;
}

void AbulEduExerciceV0::pushAbulEduLogs()
{
    QEvent* logEvent = new QEvent(AbulEduLogsSend);
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

void AbulEduExerciceV0::setAbeSkillFr(QString s)
{
    m_skillFr = s;
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

QString AbulEduExerciceV0::getAbeSkillFr()
{
    return m_skillFr;
}
