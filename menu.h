#ifndef MENU_H
#define MENU_H

#include <QMainWindow>
#include "mainwindow.h"

#include <QApplication>

namespace Ui {
class Menu;
}

class Menu : public QMainWindow
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr);
    ~Menu();

private slots:

    void on_newSimulationBtn_clicked();

    void on_exitBtn_clicked();

private:
    Ui::Menu *ui;
};

#endif // MENU_H
