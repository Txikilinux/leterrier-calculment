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

protected:
    void paintEvent(QPaintEvent* e );

private:
    Ui::interfaceClass *ui;

private slots:
    void on_btnMultiplication_clicked();
    void on_btnAddition_clicked();
};

#endif // INTERFACE_H
