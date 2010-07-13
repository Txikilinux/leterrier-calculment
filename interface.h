#ifndef INTERFACE_H
#define INTERFACE_H

#include <QtGui/QMainWindow>

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

private:
    Ui::interfaceClass *ui;
};

#endif // INTERFACE_H
