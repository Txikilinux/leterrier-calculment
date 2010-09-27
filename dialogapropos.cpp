#include "dialogapropos.h"
#include "ui_dialogapropos.h"

DialogAPropos::DialogAPropos(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::DialogAPropos)
{
    m_ui->setupUi(this);
}

DialogAPropos::~DialogAPropos()
{
    delete m_ui;
}

void DialogAPropos::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
