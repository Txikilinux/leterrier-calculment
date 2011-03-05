#include "abuleduaccueil.h"
#include "ui_abuleduaccueil.h"

AbulEduAccueil::AbulEduAccueil(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AbulEduAccueil)
{
    ui->setupUi(this);
    setWindowFlags( (windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint);
}

AbulEduAccueil::~AbulEduAccueil()
{
    delete ui;
}

void AbulEduAccueil::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
