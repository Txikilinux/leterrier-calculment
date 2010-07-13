#ifndef EXERCICE_H
#define EXERCICE_H

#include <QtGui/QMainWindow>

namespace Ui {
    class exercice;
}

class exercice : public QMainWindow {
    Q_OBJECT
    Q_DISABLE_COPY(exercice)
public:
    explicit exercice(QString,QWidget *parent = 0);
    virtual ~exercice();
    Ui::exercice *m_ui;

protected:
    virtual void changeEvent(QEvent *e);

private:
    //Ui::exercice *m_ui;
};

#endif // EXERCICE_H
