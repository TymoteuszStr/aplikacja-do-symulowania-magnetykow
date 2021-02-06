#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "xysimulation.h"
#include "isingsimulation.h"
#include "pottssimulation.h"
#include "miniature.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    IsingSimulation * isingSimulation;
    PottsSimulation * pottsSimulation;
    XYSimulation * xySimulation;

    Miniature * miniature;

    double fieldVal{0};
    int fieldDir{0};
    bool fieldToggler{false};
    int maxIteration{200000};
    int startIteration{1000};
    QString textEdit = "";

private slots:
    void on_simulate_clicked();
    void on_checkLattice_clicked(bool checked);
    void on_checkSpin_clicked(bool checked);
    void on_checkColor_clicked(bool checked);
    void on_temperature_valueChanged(double arg1);
    void on_fpsAmount_valueChanged(int arg1);
    void on_stop_clicked();
    void on_reset_clicked();
    void on_actionNowa_triggered();
    void on_field_clicked(bool checked);
    void on_chooseField_currentIndexChanged(int index);
    void on_showField_clicked(bool checked);
    void on_shapeOfField_currentIndexChanged(int index);
    void on_latticeNumber_valueChanged(int arg1);
    void on_iterationStop_clicked(bool checked);
    void on_fieldDirection_valueChanged(int arg1);
    void on_fieldValue_valueChanged(double arg1);
    void on_iterationValue_valueChanged(int arg1);
    void update();
    void on_actionWyj_cie_triggered();

    void on_actionZapisz_triggered();

    void on_iterationValue2_valueChanged(int arg1);

private:
    QWidget * simulationWidget = new  QWidget();

    bool activeSimulation[3]{false};
    bool stop{true};
    int iterator{0};
    QTimer * timer;

    void initSimulation();
    void setDigitValue();
    void disenableButton();
    void stopSimulation();
    void closeAndClearSimulationWidget();
    void setIterationValue();
    void setIterationStartPoint();
    void continueSimulation();
    void turnOnField();
    void enableButton();
    void chooseShapeOfField();
    int chooseMaterial();
    void closeEvent(QCloseEvent *event ) override;
    Ui::MainWindow *ui;


};
#endif // MAINWINDOW_H
