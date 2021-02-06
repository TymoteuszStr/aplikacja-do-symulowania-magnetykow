#include "menu.h"
#include "ui_menu.h"
#include <QDesktopWidget>

#include <QDir>

Menu::Menu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);
    this->setWindowTitle("Menu");
   this->move(QApplication::desktop()->screenGeometry().width()/2-this->width()/2,
            QApplication::desktop()->screenGeometry().height()/2-this->height()/2);

    QDir newDir,newDir1;
    newDir = QDir::currentPath();
    newDir.mkdir("save");
    newDir1 = QDir::currentPath()+"/save";
    newDir1.mkdir("IsingSimulation");
    newDir1.mkdir("PottsSimulation");
    newDir1.mkdir("XYSimulation");

}

Menu::~Menu()
{
    delete ui;
}


void Menu::on_newSimulationBtn_clicked()
{
         MainWindow * sim = new MainWindow();
         sim->show();
}

void Menu::on_exitBtn_clicked()
{

    QApplication::quit();
}
