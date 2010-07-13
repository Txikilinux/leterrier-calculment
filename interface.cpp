#include "interface.h"
#include "ui_interface.h"
#include "exercice.h"
#include "ui_exercice.h"

interface::interface(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::interfaceClass)
{
    ui->setupUi(this);
}

interface::~interface()
{
    delete ui;
}
