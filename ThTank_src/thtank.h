//class ThTank is called by the Main when the program is started. It contains one instance of each Scene, UserInterface,
//PlotWindow and Shape class and it initialises the connections between all signals and slots.
// Details of methods functionality are found in the .cpp file

#ifndef THTANK_H
#define THTANK_H

#include <QApplication>
#include <QCheckBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLineEdit>
#include <QLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QString>
#include <QVBoxLayout>
#include <QPainter>
#include <QImage>
#include <QTimer>
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QSlider>
#include <QComboBox>
#include <QMessageBox>
#include <QDebug>
#include <QStringList>
#include <QThread>
#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QMutex>
#include <QPointF>
#include <QFileDialog>
#include <QChar>
#include <QPalette>
#include <qwt_compass.h>

#include <QtXml>

#include <cmath>
#include <fstream>
#include <iostream>

#include <utils.h>
#include "constants.h"
#include "simulation.h"
#include "shape.h"
#include "userinterface.h"
#include "drawlabel.h"
#include "plotwindow.h"


// Main class
class ThTank: public QWidget
{
    Q_OBJECT
public:
    enum UserMode{
        free=0, sourceEdit=1, micEdit=2, wallDelete=3, wallAdd=4
    };

    ThTank(QWidget *parent=0);

    void loadScene(const Scene &sceneIn);
    void initConnections();
    QVector<int> prepare_sources();
    QVector<int> prepare_mics();
    QVector<int> prepare_cursor(const QPoint &);
    void draw_objects(const QVector<int> &,const int color[]);
    QVector<int> xy2absolut(const QVector<QPoint> &);
    void print_toolTip();
    void setMatrix2null(float** matrix);
    void setMatrix2null(int** matrix);
    void userModeChanged(UserMode newMode);

private:    
    UserInterface mGUI; //the user interface object
    PlotWindow plotter; //the waveform plotting window object
    Simulation sim; //the simulation object which calculates the tlm
    Shape shape; //object containing the shape of the cursor
    QThread simThread; //thread object where the simulation object is sent
    QImage img; //image containing the simulation values
    UserMode userMode; //actual mode of the user interface (free, addsource, addmic...)
    QPoint cursorPos; //position of the cursor in the field
    QMutex mutex; //mutex variable for sinc
    bool autoReset; //indicates if autoreset is activated
    int sourceSelected; //indicates in case of source edition mode, which source is being modified
    int micSelected; //indicates in case of mic edition mode, which microphone is being modified
    bool dragSource; //idicates if a source is being dragged for cursor drawing purposes
    bool dragMic; //idicates if a mic is being dragged for cursor drawing purposes
    string fileDir;


signals:
    void keyPressed(const int &);
    void drawing_finished();
    void dataActualization(bool);
    void resetPlot();

private slots:
    void wall_addMode();
    void wall_deleteMode();
    void source_added();
    void mic_added();
    void next_source();
    void prev_source();
    void next_mic();
    void prev_mic();
    void sourceAmpSliChange();
    void sourceAmpEdiChange();
    void sourceTypeComChange(int);
    void sourceFreqSliChange();
    void sourceFreqEdiChange();
    void sourceNameEdiChange();
    void sourceXEdiChange();
    void sourceYEdiChange();
    void micNameEdiChange();
    void micXEdiChange();
    void micYEdiChange();
    void reflectionWallsSliChange();
    void reflectionWallsEdiChange();
    void wallTypeComChange(int);
    void closeFieldPressed(const bool &clicked);
    void deltaSceneSliChange();
    void deltaSceneEdiChange();
    void phaseSourcesSliChange();
    void phaseSourcesEdiChange();
    void speedSimulationSliChange();
    void speedSimulationEdiChange();
    void contrastSimulationSliChange();
    void contrastSimulationEdiChange();
    void draw_field();
    void mouseMoveEventDrawPanel(const QPoint &);
    void keyPressEventInterface(const int &);
    void mouseClickedEventDrawPanel(const int &button);
    void mouseReleasedEventDrawPanel(const int &button);
    void enterEventDrawPanel();
    void leaveEventDrawPanel();
    void sizeSceneEdiChange();
    void saveSceneClicked();
    void loadSceneClicked();
    void autoResetPressed(const bool &clicked);
    void startSimulationButCilcked();
    void resetSimulation();
    void plotSimulationButClicked();
    void maxSimulationEdiChange();
    void sourcesCleared();
    void wallsCleared();
    void microphonesCleared();
    void screenShotButCilcked();
    void screenShotButCilckedPlotter();
    void sourceTypeComIncreased();
    void sourceTypeComDecreased();
    void wallTypeComIncreased();
    void wallTypeComDecreased();

protected:
    virtual void keyPressEvent(QKeyEvent *event){
       QWidget::keyPressEvent(event);
       emit keyPressed(event->key());
    }
};



#endif // THTANK_H
