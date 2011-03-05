#include "abuleduaccueil.h"
#include "ui_abuleduaccueil.h"
//#include <QProcess>
//#include <QDebug>
#include <QDesktopServices>
#include <QUrl>

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

void AbulEduAccueil::on_pbInstaller_clicked()
{
    QDesktopServices::openUrl(QUrl("file:///" + QCoreApplication::applicationDirPath() + "/Logiciel/Windows/leterrier-calcul-mental-1.0.7-setup.exe"));
}

void AbulEduAccueil::on_pbDocumentation_clicked()
{
    QDesktopServices::openUrl(QUrl("file:///" + QCoreApplication::applicationDirPath() + "/Manuel"));
}
