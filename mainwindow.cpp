#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    this->setParent(parent);
    this->setWindowTitle("Symulacje na sieciach");
    this->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    
    simulationWidget->setWindowRole("SimulationWidget");
    simulationWidget->setWindowTitle("Okno symulacji");
    simulationWidget->setWindowFlag(Qt::WindowType::WindowStaysOnTopHint);
    simulationWidget->setWindowFlag(Qt::WindowType::Widget);
    
    timer = new  QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    miniature = new Miniature(0,ui->latticeFrame->x(),ui->latticeFrame->y(),this);
    miniature->show();

    ui->text->setCenterOnScroll(true);
    
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initSimulation()
{
    switch (ui->chooseModel->currentIndex()) {

    case 0: isingSimulation = new IsingSimulation(ui->latticeNumber->value(),ui->lattSize->value(),ui->windowWidth->value(),ui->windowHeight->value(),
                                                  ui->temperature->value(),ui->fpsAmount->value(),chooseMaterial(),ui->checkColor->checkState(),ui->checkSpin->checkState(),ui->checkLattice->checkState(),ui->showField->checkState(),simulationWidget); break;
    case 1: pottsSimulation = new PottsSimulation(ui->latticeNumber->value(),ui->lattSize->value(),ui->windowWidth->value(),ui->windowHeight->value(),
                                                  ui->temperature->value(),ui->fpsAmount->value(),chooseMaterial(),ui->checkColor->checkState(),ui->checkSpin->checkState(),ui->checkLattice->checkState(),ui->showField->checkState(),simulationWidget); break;
    case 2: xySimulation = new XYSimulation(ui->latticeNumber->value(),ui->lattSize->value(),ui->windowWidth->value(),ui->windowHeight->value(),
                                            ui->temperature->value(),ui->fpsAmount->value(),chooseMaterial(), ui->checkColor->checkState(),ui->checkSpin->checkState(),ui->checkLattice->checkState(),ui->showField->checkState(),simulationWidget); break;

    }
}

void MainWindow::setIterationValue()
{

    if(activeSimulation[ui->chooseModel->currentIndex()]){
        switch (ui->chooseModel->currentIndex()) {
        case 0: isingSimulation->maxIteration = maxIteration; break;
        case 1: pottsSimulation->maxIteration = maxIteration; break;
        case 2: xySimulation->maxIteration = maxIteration; break;

        }
    }

}

void MainWindow::setIterationStartPoint()
{

    if(activeSimulation[ui->chooseModel->currentIndex()]){
        switch (ui->chooseModel->currentIndex()) {
        case 0: isingSimulation->startPoint = startIteration; break;
        case 1: pottsSimulation->startPoint = startIteration; break;
        case 2: xySimulation->startPoint = startIteration; break;

        }
    }

}

void MainWindow::on_simulate_clicked()
{
    
    disenableButton();
    activeSimulation[ui->chooseModel->currentIndex()] = true;
    
    simulationWidget->resize(ui->windowWidth->value(),ui->windowHeight->value());
    
    if(simulationWidget->isVisible()) simulationWidget->close();
    initSimulation();
    simulationWidget->show();
    if(ui->iterationStop->checkState()) setIterationValue();
    setIterationStartPoint();
    if(fieldToggler) turnOnField();
    
    stop = true;
    ui->stop->setText("STOP");
    ui->stop->setEnabled(true);
    ui->reset->setEnabled(true);
    timer->start(1000);

}


void MainWindow::on_stop_clicked()
{
    if(stop)
    {
        //------------------------
        ui->stop->setText("WZNÓW");
        stop = false;
        timer->stop();
        ui->simulate->setEnabled(true);
        stopSimulation();
        ui->actionZapisz->setEnabled(true);
        //------------------------
    }
    else
    {
        //------------------------
        ui->stop->setText("STOP");
        stop = true;
        timer->start();
        continueSimulation();
        ui->actionZapisz->setEnabled(false);
        
        //------------------------
    }
    
}
void MainWindow::continueSimulation()
{
    if(activeSimulation[ui->chooseModel->currentIndex()]){
        switch (ui->chooseModel->currentIndex()) {
        case 0: isingSimulation->timer->start(); break;
        case 1: pottsSimulation->timer->start(); break;
        case 2: xySimulation->timer->start(); break;
            
        }
    }
}


void MainWindow::on_reset_clicked()
{


    activeSimulation[ui->chooseModel->currentIndex()] = false;

    delete this;
    delete simulationWidget;

    MainWindow * sim = new MainWindow();
    sim->show();
}
void MainWindow::on_checkLattice_clicked(bool checked)
{
    
    if(activeSimulation[ui->chooseModel->currentIndex()]){
        
        switch (ui->chooseModel->currentIndex()) {
        
        case 0: isingSimulation->showLattice = checked; break;
        case 1: pottsSimulation->showLattice = checked; break;
        case 2: xySimulation->showLattice = checked; break;
            
        }
        if(checked) simulationWidget->show();
        
    }
    
}

void MainWindow::on_checkSpin_clicked(bool checked)
{
    
    if(activeSimulation[ui->chooseModel->currentIndex()]){
        
        switch (ui->chooseModel->currentIndex()) {
        
        case 0: isingSimulation->showArrows = checked; break;
        case 1: pottsSimulation->showArrows = checked; break;
        case 2: xySimulation->showArrows = checked; break;
            
        }
        
        if(checked) simulationWidget->show();
    }
}

void MainWindow::on_checkColor_clicked(bool checked)
{
    if(activeSimulation[ui->chooseModel->currentIndex()]){
        switch (ui->chooseModel->currentIndex()) {
        case 0: isingSimulation->showOnLattice = checked; break;
        case 1: pottsSimulation->showOnLattice = checked; break;
        case 2: xySimulation->showOnLattice = checked; break;
            
        }
        
        if(checked) simulationWidget->show();
    }
    
}

void MainWindow::on_temperature_valueChanged(double arg1)
{

    if(activeSimulation[ui->chooseModel->currentIndex()]){
        switch (ui->chooseModel->currentIndex()) {
        case 0: isingSimulation->T = arg1; break;
        case 1: pottsSimulation->T = arg1; break;
        case 2: xySimulation->T = arg1; break;
            
        }
    }
}

void MainWindow::on_fpsAmount_valueChanged(int arg1)
{

    if(activeSimulation[ui->chooseModel->currentIndex()]){
        switch (ui->chooseModel->currentIndex()) {
        case 0: isingSimulation-> timer->setInterval(1000/arg1);break;
        case 1: pottsSimulation->timer->setInterval(1000/arg1); break;
        case 2: xySimulation->timer->setInterval(1000/arg1); break;
            
        }
    }
}

void MainWindow::enableButton()
{
    ui->latticeNumber->setEnabled(true);
    ui->windowWidth->setEnabled(true);
    ui->windowHeight->setEnabled(true);
    ui->lattSize->setEnabled(true);
    ui->chooseMaterial->setEnabled(true);
    ui->chooseModel->setEnabled(true);
    ui->actionZapisz->setEnabled(true);
}

int MainWindow::chooseMaterial()
{
    if(ui->chooseMaterial->currentIndex()) return -1;
    else return 1;
}

void MainWindow::update()
{
    switch (ui->chooseModel->currentIndex()) {
    case 0:
        if(textEdit != isingSimulation->TextInfo) {

            textEdit = isingSimulation->TextInfo;
            ui->text->setPlainText(textEdit);
        }
        if(!isingSimulation->isRunning) timer->stop();

        break;

    case 1:
        if(textEdit != pottsSimulation->TextInfo) {

            textEdit = pottsSimulation->TextInfo;
            ui->text->setPlainText(textEdit);
        }
        if(!pottsSimulation->isRunning) timer->stop();

        break;

    case 2:
        if(textEdit != xySimulation->TextInfo) {

            textEdit = xySimulation->TextInfo;
            ui->text->setPlainText(textEdit);
        }
        if(!xySimulation->isRunning) timer->stop();

        break;

    }

}
void MainWindow::disenableButton()
{
    ui->latticeNumber->setEnabled(false);
    ui->windowWidth->setEnabled(false);
    ui->windowHeight->setEnabled(false);
    ui->lattSize->setEnabled(false);
    ui->chooseMaterial->setEnabled(false);
    ui->chooseModel->setEnabled(false);    
}

void MainWindow::closeAndClearSimulationWidget()
{
    simulationWidget->clearMask();
    simulationWidget->close();
}

void MainWindow::stopSimulation()
{
    switch (ui->chooseModel->currentIndex()) {
    case 0: isingSimulation->timer->stop(); break;
    case 1: pottsSimulation->timer->stop(); break;
    case 2: xySimulation->timer->stop(); break;     
    }
}



void MainWindow::on_actionNowa_triggered()
{
    
    MainWindow * newSim = new MainWindow();
    newSim->show();
}


void MainWindow::on_field_clicked(bool checked)
{
    fieldToggler=checked;
    if(checked)
    {
        turnOnField();
        
    }
    else if(activeSimulation[ui->chooseModel->currentIndex()]) {
        switch (ui->chooseModel->currentIndex()) {
        case 0: isingSimulation->turnOffField(); break;
        case 1: pottsSimulation->turnOffField(); break;
        case 2: xySimulation->turnOffField(); break;    }
    }


    ui->chooseField->setEnabled(checked);
    ui->fieldValue->setEnabled(checked);
    ui->fieldDirection->setEnabled(checked);
    ui->showField->setEnabled(checked);

}

void MainWindow::turnOnField()
{
    if(activeSimulation[ui->chooseModel->currentIndex()] && fieldToggler){
        
        switch (ui->chooseModel->currentIndex()) {
        case 0:
            isingSimulation->uniformFieldBool = !(ui->chooseField->currentIndex());
            isingSimulation->whichField=ui->shapeOfField->currentIndex();
            isingSimulation->turnOnField(fieldVal,static_cast<double>(fieldDir)*M_PI/180.0);
            break;

        case 1:
            pottsSimulation->uniformFieldBool = !(ui->chooseField->currentIndex());
            pottsSimulation->whichField=ui->shapeOfField->currentIndex();
            pottsSimulation->turnOnField(fieldVal,static_cast<double>(fieldDir)*M_PI/180.0);
            break;
        case 2:
            xySimulation->uniformFieldBool = !(ui->chooseField->currentIndex());
            xySimulation->whichField=ui->shapeOfField->currentIndex();
            xySimulation->turnOnField(fieldVal,static_cast<double>(fieldDir)*M_PI/180.0);
            break;
        }
    }
}


void MainWindow::on_chooseField_currentIndexChanged(int index)
{
    
    if(index == 0 ) ui->shapeOfField->setEnabled(false);
    else ui->shapeOfField->setEnabled(true);
    
    turnOnField();
    
}

void MainWindow::on_showField_clicked(bool checked)
{
    if(activeSimulation[ui->chooseModel->currentIndex()]){
        
        switch (ui->chooseModel->currentIndex()) {
        case 0: isingSimulation->showField=checked; break;
        case 1: pottsSimulation->showField=checked; break;
        case 2: xySimulation->showField=checked; break;    }
    }
}

void MainWindow::on_shapeOfField_currentIndexChanged(int index)
{
    Q_UNUSED (index)
    turnOnField();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    this->closeAndClearSimulationWidget();
}

void MainWindow::on_latticeNumber_valueChanged(int arg1)
{
    miniature = new Miniature(arg1, ui->latticeFrame->x(),ui->latticeFrame->y(),this);
    miniature->show();
}
void MainWindow::on_iterationStop_clicked(bool checked)
{
    ui->iterationValue->setEnabled(checked);
}

void MainWindow::on_fieldDirection_valueChanged(int arg1)
{
    fieldDir=arg1;
    turnOnField();

}

void MainWindow::on_fieldValue_valueChanged(double arg1)
{
    fieldVal=arg1;
    turnOnField();
}

void MainWindow::on_iterationValue_valueChanged(int arg1)
{
    maxIteration = arg1;
}


void MainWindow::on_actionWyj_cie_triggered()
{
    this->closeAndClearSimulationWidget();
    this->close();

}

void MainWindow::on_actionZapisz_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,"Save a file","./save/");
    isingSimulation->saveOutput(fileName);
}

void MainWindow::on_iterationValue2_valueChanged(int arg1)
{
    startIteration = arg1;
}
