#include <QtGui/QApplication>
#include "interface.h"
#include "exercice.h"
#include "editeur.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString exo = argv[1];
        exo.remove("--exercice=", Qt::CaseInsensitive);
        exo.toLower();
    QString nivo = argv[2];
        if (!nivo.isNull()) {
            nivo.remove("--niveau=", Qt::CaseInsensitive);
            nivo.prepend("Niveau");
            }
    interface w;
    w.setWindowTitle("Calcul Mental");
    exercice e(exo,0,nivo);
    e.setWindowTitle("exercice");
//    Editeur ed;
//    ed.show();
    if (exo=="") w.show();
    else e.show();
    return a.exec();
}
