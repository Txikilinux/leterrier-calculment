#include "exercice.h"
#include "ui_exercice.h"

exercice::exercice(QString exo,QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::exercice)
{
    m_ui->setupUi(this);
}

exercice::~exercice()
{
    delete m_ui;
}

void exercice::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
