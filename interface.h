#ifndef INTERFACE_H
#define INTERFACE_H

#include <QtGui/QMainWindow>
#include "editeur.h"

namespace Ui
{
    class interfaceClass;
}

class interface : public QMainWindow
{
    Q_OBJECT

public:
    interface(QWidget *parent = 0);
    ~interface();
    Editeur* m_editeur;

protected:
    void paintEvent(QPaintEvent* e );

private:
    Ui::interfaceClass *ui;


private slots:

};

#endif // INTERFACE_H
