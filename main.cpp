#include <QtGui/QApplication>
#include "interface.h"
#include "exercice.h"
#include "editeur.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString exo;
    if (argc>1) {
        exo= argv[1];
        exo.remove("--exercice=", Qt::CaseInsensitive);
        exo.toLower();}
    QString nivo;
    if (argc>2){
        nivo= argv[2];
        nivo.remove("--niveau=", Qt::CaseInsensitive);
        nivo.prepend("Niveau");
        }
    //à améliorer : dans un cas l'interface est créée pour rien, dans l'autre c'est l'exercice... de toute façon c'est de la mémoire utilisée pour rien !!
    interface w;
    w.setWindowTitle("Calcul Mental");
    exercice e(exo,0,nivo);
    e.setWindowTitle("exercice");
    if (argc==1) w.show();
    else e.show();
    return a.exec();
}
