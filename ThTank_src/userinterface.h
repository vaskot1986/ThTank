//The class UserInterface is responsible for the main program window. Its arrtibures are the buttons, text fields
//and layouts that the user see. The simulation canvas is a class itself which is inherited of the QLabel class.

#ifndef USERINTERFACE_H
#define USERINTERFACE_H

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
#include <QObject>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QToolTip>
#include <QString>
#include <QVector>
#include <drawlabel.h>

#include"constants.h"

//constructor of the class, sets each element in its position, size, etc...
//basically prepares the layouts of the GUI
class UserInterface{
public:
    //workaround for the buggy comboboxes
    int typeSourceShown;
    int typeWallShown;
    QVector<QString> sourceTypes;
    QVector<QString> wallTypes;

    //diferent panels for the different options
    QGroupBox *sourcesPan;
    QGroupBox *wallsPan;
    QGroupBox *microphonesPan;
    QGroupBox *simulationPan;
    QGroupBox *scenePan;

    //widgets of panel draw
    DrawLabel *drawPanelLab;
    QLabel *infoLabel;

    //widgets of sources panel
    QLabel *nameSourcesLab; //labels
    QLabel *typeSourcesLab;
    QLabel *amplitudeSourcesLab;
    QLabel *frequencySourcesLab;
    QLabel *phaseSourcesLab;
    QLabel *positionSourcesLab;
    QLabel *xSourcesLab;
    QLabel *ySourcesLab;
    QLineEdit *nameSourcesEdi; //text edits
    QLineEdit *amplitudeSourcesEdi;
    QLineEdit *frequencySourcesEdi;
    QLineEdit *phaseSourcesEdi;
    QLineEdit *xSourcesEdi;
    QLineEdit *ySourcesEdi;
    QSlider *amplitudeSourcesSli; //sliders
    QSlider *frequencySourcesSli;
    QSlider *phaseSourcesSli;
    QPushButton *nextSourcesBut; //buttons
    QPushButton *prevSourcesBut;
    QPushButton *newSourcesBut;
    QPushButton *clearAllSourcesBut;
    QLabel *typeSourcesCom; //workaround combo boxes
    QPushButton *nextSourceType;
    QPushButton *prevSourceType;

    //widgets of microphones panel
    QLabel *nameMicrophonesLab; //labels
    QLabel *positionMicrophonesLab;
    QLabel *xMicrophonesLab;
    QLabel *yMicrophonesLab;
    QLineEdit *nameMicrophonesEdi; //text edits
    QLineEdit *xMicrophonesEdi;
    QLineEdit *yMicrophonesEdi;
    QPushButton *prevMicrophonesBut; //buttons
    QPushButton *nextMicrophonesBut;
    QPushButton *newMicrophonesBut;
    QPushButton *clearAllMicrophonesBut;
    QPushButton *plotSimulationBut;

    //widgets of walls panel
    QLabel *reflectionWallsLab;
    QLabel *typeWallsLab;
    QLineEdit *reflectionWallsEdi;
    QSlider *reflectionWallsSli; //sliders
    QPushButton *newWallsBut; //buttons
    QPushButton *delWallsBut;
    QPushButton *clearAllWallsBut;
    QCheckBox *closeFieldBut;
    QLabel *typeWallsCom; //workaround combo boxes
    QPushButton *nextWallType;
    QPushButton *prevWallType;

    //widgets of simulation panel
    QLabel *stepSimulationLab; //labels
    QLabel *maxSimulationLab;
    QLabel *speedSimulationLab;
    QLabel *actualSimulationLab;
    QLabel *contrastSimulationLab;
    QLineEdit *maxSimulationEdi; //text edits
    QLineEdit *speedSimulationEdi;
    QLineEdit *contrastSimulationEdi;
    QSlider *speedSimulationSli; //sliders
    QSlider *contrastSimulationSli;
    QPushButton *startSimulationBut; //buttons
    QPushButton *resetSimulationBut;
    QPushButton *screenShotBut;

    //widgets of scene panel
    QLabel *deltaSceneLab;
    QLineEdit *deltaSceneEdi;
    QLineEdit *sizeSceneEdi;
    QSlider *deltaSceneSli;
    QPushButton *loadSceneBut;
    QPushButton *saveSceneBut;
    QCheckBox *autoResetBut;

    void init(QWidget *parent){

        //workaround combo boxes, set the values of all different sources and walls in the declared QString vectors
        sourceTypes.append("sin");
        sourceTypes.append("delta");
        sourceTypes.append("sweep");
        sourceTypes.append("gauss");

        wallTypes.append("point");
        wallTypes.append("rect Full");
        wallTypes.append("rect Empty");
        wallTypes.append("circle Full");
        wallTypes.append("circle Empty");

        typeSourceShown=0;
        typeWallShown=0;

        parent->setAttribute(Qt::WA_Hover, true);
        //Style of the window and size
        parent->setWindowTitle("THTank");
        parent->setFont(QFont("Helvetica",8));
        parent->setFixedSize(1024,768);


        //groups of items declaration and main layout
        sourcesPan = new QGroupBox("Sources");
        sourcesPan->setFixedWidth(PANELS_WIDTH);
        wallsPan = new QGroupBox("Boundaries");
        wallsPan->setFixedWidth(PANELS_WIDTH);
        microphonesPan = new QGroupBox("Microphones");
        microphonesPan->setFixedWidth(PANELS_WIDTH);
        simulationPan = new QGroupBox("Simulation");
        simulationPan->setFixedWidth(PANELS_WIDTH);
        scenePan = new QGroupBox("Scene");
        scenePan->setFixedWidth(PANELS_WIDTH);
        drawPanelLab= new DrawLabel();
        drawPanelLab->setMinimumSize(700,700);
        infoLabel=new QLabel("");

        QGridLayout *mainLayout = new QGridLayout;

        mainLayout->addWidget(drawPanelLab,0,0,15,2);
        mainLayout->addWidget(infoLabel,37,0,2,10);
        mainLayout->addWidget(sourcesPan,0,11,12,3);
        mainLayout->addWidget(wallsPan,12,11,9,3);
        mainLayout->addWidget(microphonesPan,21,11,6,3);
        mainLayout->addWidget(simulationPan,27,11,6,3);
        mainLayout->addWidget(scenePan,34,11,5,3);
        parent->setLayout(mainLayout);


        //sources button group declaration and layout
        nameSourcesLab= new QLabel("name",sourcesPan);
        nameSourcesLab->setFont(LAB_FONT);
        nameSourcesLab->setFixedSize(LAB_LARGE_WIDTH,LAB_HEIGTH);
        nameSourcesEdi = new QLineEdit(sourcesPan);
        nameSourcesEdi->setFont(EDI_FONT);
        nameSourcesEdi->setFixedSize(EDI_LARGE_WIDTH,EDI_HEIGTH);
        typeSourcesLab= new QLabel("type",sourcesPan);
        typeSourcesLab->setFont(LAB_FONT);
        typeSourcesLab->setFixedSize(LAB_LARGE_WIDTH,LAB_HEIGTH);
        typeSourcesCom = new QLabel(sourcesPan);
        typeSourcesCom->setFont(COM_FONT);
        typeSourcesCom->setFixedSize(COM_WIDTH,COM_HEIGTH);
        amplitudeSourcesLab= new QLabel("amplitude(dB)",sourcesPan);
        amplitudeSourcesLab->setFont(LAB_FONT);
        amplitudeSourcesLab->setFixedSize(LAB_LARGE_WIDTH,LAB_HEIGTH);
        amplitudeSourcesSli = new QSlider(Qt::Horizontal,sourcesPan);
        amplitudeSourcesSli->setFixedSize(SLID_WIDTH,SLID_HEIGTH);
        amplitudeSourcesEdi = new QLineEdit(sourcesPan);
        amplitudeSourcesEdi->setFont(EDI_FONT);
        amplitudeSourcesEdi->setFixedSize(EDI_SMALL_WIDTH,EDI_HEIGTH);
        frequencySourcesLab= new QLabel("frequency(Hz)",sourcesPan);
        frequencySourcesLab->setFont(LAB_FONT);
        frequencySourcesLab->setFixedSize(LAB_LARGE_WIDTH,LAB_HEIGTH);
        frequencySourcesSli = new QSlider(Qt::Horizontal,sourcesPan);
        frequencySourcesSli->setFixedSize(SLID_WIDTH,SLID_HEIGTH);
        frequencySourcesEdi = new QLineEdit(sourcesPan);
        frequencySourcesEdi->setFont(EDI_FONT);
        frequencySourcesEdi->setFixedSize(EDI_SMALL_WIDTH,EDI_HEIGTH);
        phaseSourcesLab= new QLabel("phase(deg)",sourcesPan);
        phaseSourcesLab->setFont(LAB_FONT);
        phaseSourcesLab->setFixedSize(LAB_LARGE_WIDTH,LAB_HEIGTH);
        phaseSourcesSli = new QSlider(Qt::Horizontal,sourcesPan);
        phaseSourcesSli->setFixedSize(SLID_WIDTH,SLID_HEIGTH);
        phaseSourcesEdi = new QLineEdit(sourcesPan);
        phaseSourcesEdi->setFont(EDI_FONT);
        phaseSourcesEdi->setFixedSize(EDI_SMALL_WIDTH,EDI_HEIGTH);
        positionSourcesLab= new QLabel("position(m)",sourcesPan);
        positionSourcesLab->setFont(LAB_FONT);
        positionSourcesLab->setFixedSize(LAB_LARGE_WIDTH,LAB_HEIGTH);
        xSourcesLab= new QLabel("x",sourcesPan);
        xSourcesLab->setFont(LAB_FONT);
        ySourcesLab= new QLabel("y",sourcesPan);
        ySourcesLab->setFont(LAB_FONT);
        xSourcesEdi = new QLineEdit(sourcesPan);
        xSourcesEdi->setFont(EDI_FONT);
        xSourcesEdi->setFixedSize(EDI_SMALL_WIDTH,EDI_HEIGTH);
        ySourcesEdi = new QLineEdit(sourcesPan);
        ySourcesEdi->setFont(EDI_FONT);
        ySourcesEdi->setFixedSize(EDI_SMALL_WIDTH,EDI_HEIGTH);

        nextSourcesBut = new QPushButton(">>",sourcesPan);
        nextSourcesBut->setFont(BUT_FONT);
        nextSourcesBut->setFixedSize(BUT_SMALL_WIDTH,BUT_SMALL_HEIGTH);
        prevSourcesBut = new QPushButton("<<",sourcesPan);
        prevSourcesBut->setFont(BUT_FONT);
        prevSourcesBut->setFixedSize(BUT_SMALL_WIDTH,BUT_SMALL_HEIGTH);

        nextSourceType = new QPushButton(">",sourcesPan);
        nextSourceType->setFont(BUT_FONT);
        nextSourceType->setFixedSize(BUT_SMALL_WIDTH,BUT_SMALL_HEIGTH);
        prevSourceType = new QPushButton("<",sourcesPan);
        prevSourceType->setFont(BUT_FONT);
        prevSourceType->setFixedSize(BUT_SMALL_WIDTH,BUT_SMALL_HEIGTH);

        newSourcesBut = new QPushButton("new",sourcesPan);
        newSourcesBut->setFont(BUT_FONT);
        newSourcesBut->setFixedSize(BUT_WIDTH,BUT_HEIGTH);
        clearAllSourcesBut= new QPushButton("clear all",sourcesPan);
        clearAllSourcesBut->setFont(BUT_FONT);
        clearAllSourcesBut->setFixedSize(BUT_WIDTH,BUT_HEIGTH);

        QVBoxLayout *sourcesLayout = new QVBoxLayout;
        QHBoxLayout *line1s = new QHBoxLayout;
        QHBoxLayout *line2s = new QHBoxLayout;
        QHBoxLayout *line3s = new QHBoxLayout;
        QHBoxLayout *line4s = new QHBoxLayout;
        QHBoxLayout *line5s = new QHBoxLayout;
        QHBoxLayout *line6s = new QHBoxLayout;
        QHBoxLayout *line7s = new QHBoxLayout;

        line1s->addWidget(nameSourcesLab);
        line1s->addWidget(nameSourcesEdi);
        line1s->addSpacing(10);
        line1s->addWidget(prevSourcesBut);
        line1s->addWidget(nextSourcesBut);
        line1s->setAlignment(nameSourcesLab,Qt::AlignLeft);
        line1s->setAlignment(nameSourcesEdi,Qt::AlignLeft);

        line2s->addWidget(typeSourcesLab);
        line2s->addWidget(typeSourcesCom);
        line2s->addWidget(prevSourceType);
        line2s->addWidget(nextSourceType);
        line2s->setAlignment(typeSourcesLab,Qt::AlignLeft);
        line2s->setAlignment(typeSourcesCom,Qt::AlignRight);

        line3s->addWidget(amplitudeSourcesLab);
        line3s->addWidget(amplitudeSourcesSli);
        line3s->addWidget(amplitudeSourcesEdi);
        line3s->setAlignment(amplitudeSourcesLab,Qt::AlignLeft);
        line3s->setAlignment(amplitudeSourcesSli,Qt::AlignCenter);
        line3s->setAlignment(amplitudeSourcesEdi,Qt::AlignRight);

        line4s->addWidget(frequencySourcesLab);
        line4s->addWidget(frequencySourcesSli);
        line4s->addWidget(frequencySourcesEdi);
        line4s->setAlignment(frequencySourcesLab,Qt::AlignLeft);
        line4s->setAlignment(frequencySourcesSli,Qt::AlignCenter);
        line4s->setAlignment(frequencySourcesEdi,Qt::AlignRight);

        line5s->addWidget(phaseSourcesLab);
        line5s->addWidget(phaseSourcesSli);
        line5s->addWidget(phaseSourcesEdi);
        line5s->setAlignment(phaseSourcesLab,Qt::AlignLeft);
        line5s->setAlignment(phaseSourcesSli,Qt::AlignCenter);
        line5s->setAlignment(phaseSourcesEdi,Qt::AlignRight);

        line6s->addWidget(positionSourcesLab);
        line6s->addSpacing(90);
        line6s->addWidget(xSourcesLab);
        line6s->addWidget(xSourcesEdi);
        line6s->addWidget(ySourcesLab);
        line6s->addWidget(ySourcesEdi);
        line6s->setAlignment(positionSourcesLab,Qt::AlignLeft);
        line6s->setAlignment(xSourcesLab,Qt::AlignRight);
        line6s->setAlignment(xSourcesEdi,Qt::AlignCenter);
        line6s->setAlignment(ySourcesLab,Qt::AlignRight);
        line6s->setAlignment(ySourcesEdi,Qt::AlignRight);

        line7s->addWidget(newSourcesBut);
        line7s->addSpacing(90);
        line7s->addWidget(clearAllSourcesBut);
        line7s->setAlignment(newSourcesBut,Qt::AlignLeft);

        sourcesLayout->addLayout(line1s);
        sourcesLayout->addLayout(line2s);
        sourcesLayout->addLayout(line3s);
        sourcesLayout->addLayout(line4s);
        sourcesLayout->addLayout(line5s);
        sourcesLayout->addLayout(line6s);
        sourcesLayout->addLayout(line7s);
        sourcesPan->setLayout(sourcesLayout);

        //walls button group declaration and layout
        reflectionWallsLab = new QLabel("reflection",wallsPan);
        reflectionWallsLab->setFixedSize(LAB_LARGE_WIDTH,LAB_HEIGTH);
        reflectionWallsLab->setFont(LAB_FONT);
        reflectionWallsSli = new QSlider(Qt::Horizontal,wallsPan);
        reflectionWallsSli->setFixedSize(SLID_WIDTH,SLID_HEIGTH);
        reflectionWallsEdi = new QLineEdit(wallsPan);
        reflectionWallsEdi->setFont(EDI_FONT);
        reflectionWallsEdi->setFixedSize(EDI_SMALL_WIDTH,EDI_HEIGTH);
        typeWallsLab = new QLabel("type",wallsPan);
        typeWallsLab->setFixedSize(LAB_LARGE_WIDTH,LAB_HEIGTH);
        typeWallsLab->setFont(LAB_FONT);
        typeWallsCom = new QLabel(wallsPan);
        typeWallsCom->setFont(COM_FONT);
        typeWallsCom->setFixedSize(COM_WIDTH,COM_HEIGTH);
        newWallsBut = new QPushButton("new",wallsPan);
        newWallsBut->setFont(BUT_FONT);
        newWallsBut->setFixedSize(BUT_WIDTH,BUT_HEIGTH);
        delWallsBut = new QPushButton("eraser",wallsPan);
        delWallsBut->setFont(BUT_FONT);
        delWallsBut->setFixedSize(BUT_WIDTH,BUT_HEIGTH);
        clearAllWallsBut= new QPushButton("clear all",wallsPan);
        clearAllWallsBut->setFont(BUT_FONT);
        clearAllWallsBut->setFixedSize(COM_WIDTH,BUT_HEIGTH);
        closeFieldBut= new QCheckBox("close field",wallsPan);
        closeFieldBut->setFont(CHECK_FONT);
        closeFieldBut->setFixedSize(CHECK_WIDTH,CHECK_HEIGTH);

        nextWallType = new QPushButton(">",wallsPan);
        nextWallType->setFont(BUT_FONT);
        nextWallType->setFixedSize(BUT_SMALL_WIDTH,BUT_SMALL_HEIGTH);
        prevWallType = new QPushButton("<",wallsPan);
        prevWallType->setFont(BUT_FONT);
        prevWallType->setFixedSize(BUT_SMALL_WIDTH,BUT_SMALL_HEIGTH);


        QVBoxLayout *wallsLayout = new QVBoxLayout;
        QHBoxLayout *line1w = new QHBoxLayout;
        QHBoxLayout *line2w = new QHBoxLayout;
        QHBoxLayout *line3w = new QHBoxLayout;

        line1w->addWidget(typeWallsLab);
        line1w->addWidget(typeWallsCom);
        line1w->addWidget(prevWallType);
        line1w->addWidget(nextWallType);
        line1w->addWidget(closeFieldBut);
        line1w->setAlignment(typeWallsLab,Qt::AlignLeft);
        line1w->setAlignment(typeWallsCom,Qt::AlignCenter);
        line1w->setAlignment(closeFieldBut,Qt::AlignRight);

        line2w->addWidget(reflectionWallsLab);
        line2w->addWidget(reflectionWallsSli);
        line2w->addWidget(reflectionWallsEdi);
        line2w->setAlignment(reflectionWallsLab,Qt::AlignLeft);
        line2w->setAlignment(reflectionWallsSli,Qt::AlignCenter);
        line2w->setAlignment(reflectionWallsEdi,Qt::AlignRight);

        line3w->addWidget(newWallsBut);
        line3w->addWidget(delWallsBut);
        line3w->addWidget(clearAllWallsBut);
        line3w->setAlignment(newWallsBut,Qt::AlignLeft);
        line3w->setAlignment(delWallsBut,Qt::AlignLeft);

        wallsLayout->addLayout(line1w);
        wallsLayout->addLayout(line2w);
        wallsLayout->addLayout(line3w);
        wallsPan->setLayout(wallsLayout);


        //microphones button group declaration and layout
        nameMicrophonesLab = new QLabel("name",microphonesPan);
        nameMicrophonesLab->setFont(LAB_FONT);
        nameMicrophonesLab->setFixedSize(LAB_LARGE_WIDTH,LAB_HEIGTH);
        nameMicrophonesEdi = new QLineEdit(microphonesPan);
        nameMicrophonesEdi->setFont(EDI_FONT);
        nameMicrophonesEdi->setFixedSize(EDI_LARGE_WIDTH,EDI_HEIGTH);
        positionMicrophonesLab = new QLabel("position(m)",microphonesPan);
        positionMicrophonesLab->setFont(LAB_FONT);
        positionMicrophonesLab->setFixedSize(LAB_LARGE_WIDTH,LAB_HEIGTH);
        xMicrophonesLab = new QLabel("x",microphonesPan);
        xMicrophonesLab->setFont(LAB_FONT);
        xMicrophonesEdi = new QLineEdit(microphonesPan);
        xMicrophonesEdi->setFont(EDI_FONT);
        xMicrophonesEdi->setFixedSize(EDI_SMALL_WIDTH,EDI_HEIGTH);
        yMicrophonesLab = new QLabel("y",microphonesPan);
        yMicrophonesLab->setFont(LAB_FONT);
        yMicrophonesEdi = new QLineEdit(microphonesPan);
        yMicrophonesEdi->setFont(EDI_FONT);
        yMicrophonesEdi->setFixedSize(EDI_SMALL_WIDTH,EDI_HEIGTH);
        nextMicrophonesBut = new QPushButton(">>",microphonesPan);
        nextMicrophonesBut->setFont(BUT_FONT);
        nextMicrophonesBut->setFixedSize(BUT_SMALL_WIDTH,BUT_SMALL_HEIGTH);
        prevMicrophonesBut = new QPushButton("<<",microphonesPan);
        prevMicrophonesBut->setFont(BUT_FONT);
        prevMicrophonesBut->setFixedSize(BUT_SMALL_WIDTH,BUT_SMALL_HEIGTH);
        newMicrophonesBut = new QPushButton("new",microphonesPan);
        newMicrophonesBut->setFont(BUT_FONT);
        newMicrophonesBut->setFixedSize(BUT_WIDTH,BUT_HEIGTH);
        clearAllMicrophonesBut= new QPushButton("clear all",microphonesPan);
        clearAllMicrophonesBut->setFont(BUT_FONT);
        clearAllMicrophonesBut->setFixedSize(BUT_WIDTH,BUT_HEIGTH);
        plotSimulationBut = new QPushButton("plot signals",microphonesPan);
        plotSimulationBut->setFont(BUT_FONT);
        plotSimulationBut->setFixedSize(BUT_WIDTH,BUT_HEIGTH);


        QVBoxLayout *microphonesLayout = new QVBoxLayout;
        QHBoxLayout *line1m = new QHBoxLayout;
        QHBoxLayout *line2m = new QHBoxLayout;
        QHBoxLayout *line3m = new QHBoxLayout;

        line1m->addWidget(nameMicrophonesLab);
        line1m->addWidget(nameMicrophonesEdi);
        line1m->addSpacing(10);
        line1m->addWidget(prevMicrophonesBut);
        line1m->addWidget(nextMicrophonesBut);
        line1m->setAlignment(nameMicrophonesLab,Qt::AlignLeft);
        line1m->setAlignment(nameMicrophonesEdi,Qt::AlignLeft);

        line2m->addWidget(positionMicrophonesLab);
        line2m->addSpacing(90);
        line2m->addWidget(xMicrophonesLab);
        line2m->addWidget(xMicrophonesEdi);
        line2m->addWidget(yMicrophonesLab);
        line2m->addWidget(yMicrophonesEdi);
        line2m->setAlignment(positionMicrophonesLab,Qt::AlignLeft);
        line2m->setAlignment(xMicrophonesLab,Qt::AlignRight);
        line2m->setAlignment(xMicrophonesEdi,Qt::AlignCenter);
        line2m->setAlignment(yMicrophonesLab,Qt::AlignRight);
        line2m->setAlignment(yMicrophonesEdi,Qt::AlignRight);

        line3m->addWidget(newMicrophonesBut);
        line3m->addWidget(clearAllMicrophonesBut);
        line3m->addWidget(plotSimulationBut);
        line3m->setAlignment(newMicrophonesBut,Qt::AlignLeft);
        line3m->setAlignment(clearAllMicrophonesBut,Qt::AlignCenter);
        line3m->setAlignment(plotSimulationBut,Qt::AlignRight);

        microphonesLayout->addLayout(line1m);
        microphonesLayout->addLayout(line2m);
        microphonesLayout->addLayout(line3m);
        microphonesPan->setLayout(microphonesLayout);

        //simulation button group declaration and layout
        stepSimulationLab = new QLabel("step",simulationPan);
        stepSimulationLab->setFont(LAB_FONT);
        actualSimulationLab = new QLabel("0",simulationPan);
        actualSimulationLab->setFont(LAB_FONT);
        maxSimulationLab = new QLabel("max",simulationPan);
        maxSimulationLab->setFont(LAB_FONT);
        maxSimulationEdi = new QLineEdit(simulationPan);
        maxSimulationEdi->setFont(EDI_FONT);
        maxSimulationEdi->setFixedSize(EDI_SMALL_WIDTH,EDI_HEIGTH);
        speedSimulationLab = new QLabel("speed",simulationPan);
        speedSimulationLab->setFont(LAB_FONT);
        speedSimulationLab->setFixedSize(LAB_LARGE_WIDTH,LAB_HEIGTH);
        speedSimulationSli = new QSlider(Qt::Horizontal,simulationPan);
        speedSimulationSli->setFixedSize(SLID_WIDTH,SLID_HEIGTH);
        speedSimulationEdi = new QLineEdit(simulationPan);
        speedSimulationEdi->setFont(EDI_FONT);
        speedSimulationEdi->setFixedSize(EDI_SMALL_WIDTH,EDI_HEIGTH);
        contrastSimulationLab = new QLabel("contrast",simulationPan);
        contrastSimulationLab->setFont(LAB_FONT);
        contrastSimulationLab->setFixedSize(LAB_LARGE_WIDTH,LAB_HEIGTH);
        contrastSimulationSli = new QSlider(Qt::Horizontal,simulationPan);
        contrastSimulationSli->setFixedSize(SLID_WIDTH,SLID_HEIGTH);
        contrastSimulationEdi = new QLineEdit(simulationPan);
        contrastSimulationEdi->setFont(EDI_FONT);
        contrastSimulationEdi->setFixedSize(EDI_SMALL_WIDTH,EDI_HEIGTH);
        startSimulationBut = new QPushButton("start",simulationPan);
        startSimulationBut->setFont(BUT_FONT);
        startSimulationBut->setFixedSize(BUT_WIDTH,BUT_HEIGTH);
        resetSimulationBut = new QPushButton("reset",simulationPan);
        resetSimulationBut->setFont(BUT_FONT);
        resetSimulationBut->setFixedSize(BUT_WIDTH,BUT_HEIGTH);
        screenShotBut = new QPushButton("print",simulationPan);
        screenShotBut->setFont(BUT_FONT);
        screenShotBut->setFixedSize(BUT_WIDTH,BUT_HEIGTH);

        QVBoxLayout *simulationLayout = new QVBoxLayout;
        QHBoxLayout *line1si = new QHBoxLayout;
        QHBoxLayout *line2si = new QHBoxLayout;
        QHBoxLayout *line3si = new QHBoxLayout;
        QHBoxLayout *line4si = new QHBoxLayout;

        line1si->addWidget(stepSimulationLab);
        line1si->addWidget(actualSimulationLab);
        line1si->addSpacing(50);
        line1si->addWidget(maxSimulationLab);
        line1si->addWidget(maxSimulationEdi);
        line1si->setAlignment(stepSimulationLab,Qt::AlignLeft);
        line1si->setAlignment(maxSimulationEdi,Qt::AlignRight);

        line2si->addWidget(speedSimulationLab);
        line2si->addWidget(speedSimulationSli);
        line2si->addWidget(speedSimulationEdi);
        line2si->setAlignment(speedSimulationLab,Qt::AlignLeft);
        line2si->setAlignment(speedSimulationSli,Qt::AlignCenter);
        line2si->setAlignment(speedSimulationEdi,Qt::AlignRight);

        line3si->addWidget(contrastSimulationLab);
        line3si->addWidget(contrastSimulationSli);
        line3si->addWidget(contrastSimulationEdi);
        line3si->setAlignment(contrastSimulationLab,Qt::AlignLeft);
        line3si->setAlignment(contrastSimulationSli,Qt::AlignCenter);
        line3si->setAlignment(contrastSimulationEdi,Qt::AlignRight);


        line4si->addWidget(startSimulationBut);
        line4si->addWidget(resetSimulationBut);
        line4si->addWidget(screenShotBut);
        line4si->setAlignment(startSimulationBut,Qt::AlignLeft);
        line4si->setAlignment(resetSimulationBut,Qt::AlignCenter);
        line4si->setAlignment(screenShotBut,Qt::AlignRight);


        simulationLayout->addLayout(line1si);
        simulationLayout->addLayout(line2si);
        simulationLayout->addLayout(line3si);
        simulationLayout->addLayout(line4si);
        simulationPan->setLayout(simulationLayout);

        //scene button group declaration and layout
        deltaSceneLab = new QLabel("delta/size(m)",scenePan);
        deltaSceneLab->setFont(LAB_FONT);
        deltaSceneLab->setFixedSize(LAB_LARGE_WIDTH,LAB_HEIGTH);
        deltaSceneSli = new QSlider(Qt::Horizontal,scenePan);
        deltaSceneSli->setFixedSize(SLID_WIDTH_SMALL,SLID_HEIGTH);
        deltaSceneEdi = new QLineEdit(scenePan);
        deltaSceneEdi->setFont(EDI_FONT);
        deltaSceneEdi->setFixedSize(EDI_SMALL_WIDTH,EDI_HEIGTH);
        sizeSceneEdi = new QLineEdit(scenePan);
        sizeSceneEdi->setFont(EDI_FONT);
        sizeSceneEdi->setFixedSize(EDI_SMALL_WIDTH,EDI_HEIGTH);
        loadSceneBut = new QPushButton("load",scenePan);
        loadSceneBut->setFont(BUT_FONT);
        loadSceneBut->setFixedSize(BUT_WIDTH,BUT_HEIGTH);
        saveSceneBut = new QPushButton("save",scenePan);
        saveSceneBut->setFont(BUT_FONT);
        saveSceneBut->setFixedSize(BUT_WIDTH,BUT_HEIGTH);
        autoResetBut= new QCheckBox("auto reset",scenePan);
        autoResetBut->setFont(CHECK_FONT);
        autoResetBut->setFixedSize(CHECK_WIDTH,CHECK_HEIGTH);


        QVBoxLayout *sceneLayout = new QVBoxLayout;
        QHBoxLayout *line1sc = new QHBoxLayout;
        QHBoxLayout *line2sc = new QHBoxLayout;

        line1sc->addWidget(deltaSceneLab);
        line1sc->addWidget(deltaSceneSli);
        line1sc->addWidget(deltaSceneEdi);
        line1sc->addWidget(sizeSceneEdi);
        line1sc->setAlignment(deltaSceneSli,Qt::AlignLeft);
        line1sc->setAlignment(deltaSceneEdi,Qt::AlignRight);
        line1sc->setAlignment(sizeSceneEdi,Qt::AlignRight);

        line2sc->addWidget(loadSceneBut);
        line2sc->addWidget(saveSceneBut);
        line2sc->addWidget(autoResetBut);
        line2sc->setAlignment(loadSceneBut,Qt::AlignLeft);
        line2sc->setAlignment(saveSceneBut,Qt::AlignCenter);
        line2sc->setAlignment(autoResetBut,Qt::AlignRight);


        sceneLayout->addLayout(line1sc);
        sceneLayout->addLayout(line2sc);
        scenePan->setLayout(sceneLayout);

        //set the values of the sliders
        deltaSceneSli->setMinimum(0);
        deltaSceneSli->setMaximum(1000);
        speedSimulationSli->setMinimum(0);
        speedSimulationSli->setMaximum(1000);
        reflectionWallsSli->setMinimum(0);
        reflectionWallsSli->setMaximum(1000);
        amplitudeSourcesSli->setMinimum(0);
        amplitudeSourcesSli->setMaximum(1000);
        frequencySourcesSli->setMinimum(0);
        frequencySourcesSli->setMaximum(1000);
        phaseSourcesSli->setMinimum(0);
        phaseSourcesSli->setMaximum(1000);

    }
    void lose_focus(){
        amplitudeSourcesEdi->clearFocus();
        contrastSimulationEdi->clearFocus();
        deltaSceneEdi->clearFocus();
        frequencySourcesEdi->clearFocus();
        maxSimulationEdi->clearFocus();
        reflectionWallsEdi->clearFocus();
        nameMicrophonesEdi->clearFocus();
        nameSourcesEdi->clearFocus();
        phaseSourcesEdi->clearFocus();
        sizeSceneEdi->clearFocus();
        speedSimulationEdi->clearFocus();
        xSourcesEdi->clearFocus();
        ySourcesEdi->clearFocus();
        xMicrophonesEdi->clearFocus();
        yMicrophonesEdi->clearFocus();
    }

};

#endif // USERINTERFACE_H
