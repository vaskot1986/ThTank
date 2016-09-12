//class ThTank is called by the Main when the program is started. It contains one instance of each Scene, UserInterface,
//PlotWindow and Shape class and it initialises the connections between all signals and slots.

#include "thtank.h"
// ThTank::ThTank()
//
// PURPOSE
//   Constructor:
//   1-Calls initGui()
//   2-Calls initGuiConnections()
//   3-Calls simulation member func init()
//   4-initialize image object for plotting
//   5-calls simulation member func DoSetup()
//   6-calls simulation member func moveToThread()
//   7-starts the simulation thread
//
// INPUT
//  None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
ThTank::ThTank(QWidget *parent): QWidget(parent)
{
    qRegisterMetaType<NameArray>("NameArray");
    qRegisterMetaType<FloatArray>("FloatArray");
    //initialize some variables
    userMode=free;
    sourceSelected=-1;
    micSelected=-1;
    autoReset=false;

    // Initialize GUI
    mGUI.init(this);

    //init connections
    initConnections();

    //initialize Simulation data
    sim.init();

    //load default scene
    Scene temp;
    loadScene(temp);

    //start the simulation thread
    simThread.start(QThread::TimeCriticalPriority);
    sim.moveToThread(&simThread);
}

// ThTank::initGuiConnections()
//
// PURPOSE
//   Initialize all the connections between signals and slots of the GUI
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::initConnections(){
   connect(mGUI.nameSourcesEdi,         SIGNAL(editingFinished()),             this, SLOT(sourceNameEdiChange()));
   connect(mGUI.amplitudeSourcesSli,    SIGNAL(sliderReleased()),              this, SLOT(sourceAmpSliChange()));
   connect(mGUI.amplitudeSourcesEdi,    SIGNAL(editingFinished()),             this, SLOT(sourceAmpEdiChange()));

   connect(mGUI.nextSourceType,         SIGNAL(clicked()),                     this, SLOT(sourceTypeComIncreased()));
   connect(mGUI.prevSourceType,         SIGNAL(clicked()),                     this, SLOT(sourceTypeComDecreased()));

   connect(mGUI.frequencySourcesSli,    SIGNAL(sliderReleased()),              this, SLOT(sourceFreqSliChange()));
   connect(mGUI.frequencySourcesEdi,    SIGNAL(editingFinished()),             this, SLOT(sourceFreqEdiChange()));
   connect(mGUI.phaseSourcesSli,        SIGNAL(sliderReleased()),              this, SLOT(phaseSourcesSliChange()));
   connect(mGUI.phaseSourcesEdi,        SIGNAL(editingFinished()),             this, SLOT(phaseSourcesEdiChange()));
   connect(mGUI.xSourcesEdi,            SIGNAL(editingFinished()),             this, SLOT(sourceXEdiChange()));
   connect(mGUI.ySourcesEdi,            SIGNAL(editingFinished()),             this, SLOT(sourceYEdiChange()));
   connect(mGUI.nameMicrophonesEdi,     SIGNAL(editingFinished()),             this, SLOT(micNameEdiChange()));
   connect(mGUI.xMicrophonesEdi,        SIGNAL(editingFinished()),             this, SLOT(micXEdiChange()));
   connect(mGUI.yMicrophonesEdi,        SIGNAL(editingFinished()),             this, SLOT(micYEdiChange()));
   connect(mGUI.screenShotBut,          SIGNAL(clicked()),                     this, SLOT(screenShotButCilcked()));
   connect(plotter.screenShotBut,       SIGNAL(clicked()),                     this, SLOT(screenShotButCilckedPlotter()));
   connect(mGUI.reflectionWallsSli,     SIGNAL(sliderReleased()),              this, SLOT(reflectionWallsSliChange()));
   connect(mGUI.reflectionWallsEdi,     SIGNAL(editingFinished()),             this, SLOT(reflectionWallsEdiChange()));
   connect(mGUI.deltaSceneSli,          SIGNAL(sliderReleased()),              this, SLOT(deltaSceneSliChange()));
   connect(mGUI.deltaSceneEdi,          SIGNAL(editingFinished()),             this, SLOT(deltaSceneEdiChange()));
   connect(mGUI.sizeSceneEdi,           SIGNAL(editingFinished()),             this, SLOT(sizeSceneEdiChange()));
   connect(mGUI.speedSimulationSli,     SIGNAL(sliderReleased()),              this, SLOT(speedSimulationSliChange()));
   connect(mGUI.speedSimulationEdi,     SIGNAL(editingFinished()),             this, SLOT(speedSimulationEdiChange()));
   connect(mGUI.contrastSimulationSli,  SIGNAL(sliderReleased()),              this, SLOT(contrastSimulationSliChange()));
   connect(mGUI.contrastSimulationEdi,  SIGNAL(editingFinished()),             this, SLOT(contrastSimulationEdiChange()));
   connect(&sim,                        SIGNAL(updating_finished()),           this,SLOT(draw_field()));
   connect(&sim,                        SIGNAL(sendMicData(NameArray,FloatArray,float)),            &plotter,SLOT(updatePlot(NameArray,FloatArray,float)));
   connect(this,                        SIGNAL(dataActualization(bool)),       &sim,SLOT(data_actualization(bool)),Qt::DirectConnection);
   connect(this,                        SIGNAL(drawing_finished()),            &sim,SLOT(send_update()),Qt::DirectConnection);
   connect(mGUI.nextSourcesBut,         SIGNAL(clicked()),                     this,SLOT(next_source()));
   connect(mGUI.prevSourcesBut,         SIGNAL(clicked()),                     this,SLOT(prev_source()));
   connect(mGUI.newSourcesBut,          SIGNAL(clicked()),                     this,SLOT(source_added()));
   connect(mGUI.clearAllSourcesBut,     SIGNAL(clicked()),                     this,SLOT(sourcesCleared()));
   connect(mGUI.nextMicrophonesBut,     SIGNAL(clicked()),                     this,SLOT(next_mic()));
   connect(mGUI.prevMicrophonesBut,     SIGNAL(clicked()),                     this,SLOT(prev_mic()));
   connect(mGUI.newMicrophonesBut,      SIGNAL(clicked()),                     this,SLOT(mic_added()));
   connect(mGUI.clearAllMicrophonesBut, SIGNAL(clicked()),                     this,SLOT(microphonesCleared()));
   connect(mGUI.newWallsBut,            SIGNAL(clicked()),                     this,SLOT(wall_addMode()));
   connect(mGUI.delWallsBut,            SIGNAL(clicked()),                     this,SLOT(wall_deleteMode()));
   connect(mGUI.clearAllWallsBut,       SIGNAL(clicked()),                     this,SLOT(wallsCleared()));

   connect(mGUI.nextWallType,         SIGNAL(clicked()),                     this, SLOT(wallTypeComIncreased()));
   connect(mGUI.prevWallType,         SIGNAL(clicked()),                     this, SLOT(wallTypeComDecreased()));

   connect(mGUI.closeFieldBut,          SIGNAL(clicked(const bool &)),         this,SLOT(closeFieldPressed(const bool &)));
   connect(mGUI.autoResetBut,           SIGNAL(clicked(const bool &)),         this,SLOT(autoResetPressed(const bool &)));
   connect(mGUI.drawPanelLab,           SIGNAL(posChanged(const QPoint &)),    this,SLOT(mouseMoveEventDrawPanel(const QPoint &)));
   connect(this,                        SIGNAL(keyPressed(const int &)),       this,SLOT(keyPressEventInterface(const int &)));
   connect(mGUI.drawPanelLab,           SIGNAL(mouseClicked(const int &)),     this,SLOT(mouseClickedEventDrawPanel(const int &)));
   connect(mGUI.drawPanelLab,           SIGNAL(mouseReleased(const int &)),    this,SLOT(mouseReleasedEventDrawPanel(const int &)));
   connect(mGUI.drawPanelLab,           SIGNAL(enter()),                       this,SLOT(enterEventDrawPanel()));
   connect(mGUI.drawPanelLab,           SIGNAL(leave()),                       this,SLOT(leaveEventDrawPanel()));
   connect(&simThread,                  SIGNAL(started()),                     &sim,SLOT(update_simulation()));
   connect(mGUI.saveSceneBut,           SIGNAL(clicked()),                     this,SLOT(saveSceneClicked()));
   connect(mGUI.loadSceneBut,           SIGNAL(clicked()),                     this,SLOT(loadSceneClicked()));
   connect(mGUI.startSimulationBut,     SIGNAL(clicked()),                     this,SLOT(startSimulationButCilcked()));
   connect(mGUI.resetSimulationBut,     SIGNAL(clicked()),                     this,SLOT(resetSimulation()));
   connect(mGUI.plotSimulationBut,      SIGNAL(clicked()),                     this,SLOT(plotSimulationButClicked()));
   connect(mGUI.maxSimulationEdi,       SIGNAL(editingFinished()),             this,SLOT(maxSimulationEdiChange()));
}

// ThTank::loadScene()
//
// PURPOSE
//   Load a scene
//
// INPUT
//   const Scene &SceneIn - The scene object that is going to be loaded
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::loadScene(const Scene &SceneIn){
    //set simulating bool to false and synchronise
    sim.set_simulating(false);
    QThread::msleep(20);
    emit dataActualization(true);
    QThread::msleep(20);

    //set simulation step to 0, clear the scene and set the new scene
    sim.set_simStep(0);
    sim.set_scene()->clear();
    sim.set_scene(SceneIn);

    //set all matrixes to 0
    setMatrix2null(sim.field);
    setMatrix2null(sim.presVals);
    setMatrix2null(sim.outValsN);
    setMatrix2null(sim.outValsS);
    setMatrix2null(sim.outValsE);
    setMatrix2null(sim.outValsW);
    setMatrix2null(sim.inValsN);
    setMatrix2null(sim.inValsS);
    setMatrix2null(sim.inValsE);
    setMatrix2null(sim.inValsW);

    //set the gui fields, slider, etc... to the values of the scene
    mGUI.reflectionWallsEdi->setText(QString::number(sim.get_scene().get_refCoef()));
    mGUI.deltaSceneEdi->setText(QString::number(sim.get_scene().get_delta()));
    mGUI.nameSourcesEdi->editingFinished();
    mGUI.typeSourceShown=0;
    mGUI.typeSourcesCom->setText(mGUI.sourceTypes.at(0));
    mGUI.amplitudeSourcesEdi->editingFinished();
    mGUI.frequencySourcesEdi->editingFinished();
    mGUI.phaseSourcesEdi->editingFinished();
    mGUI.xSourcesEdi->editingFinished();
    mGUI.ySourcesEdi->editingFinished();
    mGUI.nameMicrophonesEdi->editingFinished();
    mGUI.xMicrophonesEdi->editingFinished();
    mGUI.yMicrophonesEdi->editingFinished();
    mGUI.typeWallShown=0;
    mGUI.typeWallsCom->setText(mGUI.wallTypes.at(0));
    mGUI.reflectionWallsEdi->editingFinished();
    mGUI.speedSimulationEdi->editingFinished();
    mGUI.contrastSimulationEdi->editingFinished();
    mGUI.deltaSceneEdi->editingFinished();
    mGUI.maxSimulationEdi->editingFinished();

    //set the boundaries and actualize with new wall points
    sim.set_fieldBoundaries();
    sim.add_walls2field(sim.get_scene().get_wallPoints().wallPoint2QPoint());
    mGUI.startSimulationBut->setText("start");
    plotter.close();
    emit dataActualization(false);
}

// ThTank::draw_field()
//
// PURPOSE
//   Plot the values of the data matrix.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::draw_field(){
    sim.drawPressdata();
    draw_objects(prepare_sources(),COLOR_SOURCES);
    draw_objects(prepare_mics(),COLOR_MICS);
    draw_objects(prepare_cursor(cursorPos),COLOR_SOURCES);
    mGUI.actualSimulationLab->setText(QString::number(sim.get_simStep()).append(" (").append(QString::number(sim.getMilisecond())).append("ms)"));
    mGUI.drawPanelLab->setPixmap(QPixmap::fromImage(QImage(sim.data,VERTICAL_PIXELS,HORIZONTAL_PIXELS,QImage::Format_RGB32)));
    emit drawing_finished();
}

// ThTank::draw_objects()
//
// PURPOSE
//   A vector of points is "inserted" in the data matrix which is the matrix that will be plotted
//
// INPUT
//   const QVector<int> &points= vector of points that will be drawn in the image
//   const int color[]=vector of three ints containing the value of every RGB color
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build
void ThTank::draw_objects(const QVector<int> &points,const int color[]){
    //QString info;
    for(int i=0; i<points.size();i+=3){
        sim.data[points[i]]=color[0];
        sim.data[points[i+1]]=color[1];
        sim.data[points[i+2]]=color[2];
        //info.append(QString::number(points[i])).append(" ");
    }
    //qDebug()<<info;
}

// ThTank::prepare_Cursor()
//
// PURPOSE
//   Prepares a vector with the pixels index corresponding to the drawing of the cursor
//
// INPUT
//   const QPoint &center= position of the cursor
//   const int &size= size of the delete cursor
//
// OUTPUT
//   None
//
// RETURN VALUE
//   QVector<int>= vector with the pixels to be drawn
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
QVector<int> ThTank::prepare_cursor(const QPoint &center){
    QVector<int> points;
    if (userMode==free || userMode==sourceEdit || userMode==micEdit || mGUI.drawPanelLab->hasFocus()==false) return points;

    if(userMode==wallDelete || userMode==wallAdd){
        shape.set_shapePoints(center,VERTICAL_PIXELS,HORIZONTAL_PIXELS);
        return xy2absolut(shape.get_shapePoints());
    }
    return points;
}

// ThTank::mouseMoveEventDrawPanel()
//
// PURPOSE
//   SLOT this function is called when the cursor finds itself inside the simulation field, depending on what is happening
//   the cursor is drawn in a way or another
//
// INPUT
//   const QPoint &pos - position of the cursor
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::mouseMoveEventDrawPanel(const QPoint &pos){
    print_toolTip(); //the cursor is printed depending on the action is taking place (place source, boundary, move source....)
    cursorPos=pos;
        if(userMode==sourceEdit){
            if(dragSource){ //this happens when the user is dragging a source
            sim.set_scene()->set_sources()->set_sourceByIdx(sourceSelected)->set_col(cursorPos.x());
            sim.set_scene()->set_sources()->set_sourceByIdx(sourceSelected)->set_row(cursorPos.y());
            }
        }
        else if(userMode==micEdit){
            if(dragMic){ //this happens when the user is dragging a microphone
            sim.set_scene()->set_mics()->set_micByIdx(micSelected)->set_col(cursorPos.x());
            sim.set_scene()->set_mics()->set_micByIdx(micSelected)->set_row(cursorPos.y());
            }
        }
}

// ThTank::xy2absolut()
//
// PURPOSE
//  Takes a vector of points corresponding to a part of the field and turns it in to data points in linear positions for RGB
//
// INPUT
//   const QVector<QPoint> &pointVector - vector of points corresponding to the points in the field
//
// OUTPUT
//   None
//
// RETURN VALUE
//  QVector<int>
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
QVector<int> ThTank::xy2absolut(const QVector<QPoint> &pointVector){
    QVector<int> absVector=QVector<int>(pointVector.size()*3); //create a vector 3 times bigger than the one given (for RGB)
    QVector<QPoint>::const_iterator it; //iterator to go through the vector
    int count=0;
    int absValue;
    for(it = pointVector.begin(); it != pointVector.end(); it++){ //until vector has still values..
       absValue=(it->x()+it->y()*HORIZONTAL_PIXELS)*4; //get the absolute value (x4 because RGB  and transparence channels)
       absVector[count]=absValue;  //R into absolute value + 0
       absVector[count+1]=absValue+1; //G into absolute value + 1
       absVector[count+2]=absValue+2; //B into absolute value + 2
       count+=3; //plus 3 and get a new absolute value
    }
    return absVector;
}

// ThTank::setMatrix2null()
//
// PURPOSE
//  Sets a matrix of floats to 0
//
// INPUT
//   None
//
// OUTPUT
//   float** matrix - matrix of ints to be set to 0
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::setMatrix2null(float** matrix){
   for(int row=0;row<VERTICAL_PIXELS;row++){
       for(int col=0;col<HORIZONTAL_PIXELS;col++){
           matrix[row][col]=0.f;
       }
   }
}

// ThTank::setMatrix2null()
//
// PURPOSE
//  Sets a matrix of ints to 0
//
// INPUT
//   None
//
// OUTPUT
//   int** matrix - matrix of ints to be set to 0
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::setMatrix2null(int** matrix){
   for(int row=0;row<VERTICAL_PIXELS;row++){
       for(int col=0;col<HORIZONTAL_PIXELS;col++){
           matrix[row][col]=0;
       }
   }
}

// ThTank::prepare_sources()
//
// PURPOSE
//   Prepares a vector with the pixels index corresponding to the drawing of the sources
//
// INPUT
//   const int &size= size of the sources markers
//
// OUTPUT
//   None
//
// RETURN VALUE
//   QVector<int>= vector with the pixels to be drawn
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
QVector<int> ThTank::prepare_sources(){
    QVector<QPoint> points;
    Shape sourceShape=Shape(Shape::source,HEIGHT_SOURCES,WIDTH_SOURCES);
    Shape selectedSourceShape=Shape(Shape::source,HEIGHT_SOURCES*3,WIDTH_SOURCES*3);
    for(int i=0;i<sim.get_scene().get_sources().get_nSources();i++){
        sourceShape.set_shapePoints(QPoint(sim.get_scene().get_sources().get_sourceByIdx(i).get_col(),sim.get_scene().get_sources().get_sourceByIdx(i).get_row()),VERTICAL_PIXELS,HORIZONTAL_PIXELS);
        points.append(sourceShape.get_shapePoints());
    }
    if(userMode==sourceEdit){
        selectedSourceShape.set_shapePoints(QPoint(sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_col(),sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_row()),VERTICAL_PIXELS,HORIZONTAL_PIXELS);
        points.append(selectedSourceShape.get_shapePoints());
    }
    return xy2absolut(points);
}

// ThTank::prepare_mics()
//
// PURPOSE
//   Prepares a vector with the pixels index corresponding to the drawing of the microphones
//
// INPUT
//   const int &size= size of the mics markers
//
// OUTPUT
//   None
//
// RETURN VALUE
//   QVector<int>= vector with the pixels to be drawn
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
QVector<int> ThTank::prepare_mics(){
    QVector<QPoint> points;
    Shape micShape=Shape(Shape::mic,HEIGHT_MICS,WIDTH_MICS);
    Shape selectedMicShape=Shape(Shape::source,HEIGHT_MICS*3,WIDTH_MICS*3);
    for(int i=0;i<sim.get_scene().get_mics().get_nMics();i++){
        micShape.set_shapePoints(QPoint(sim.get_scene().get_mics().get_micByIdx(i).get_col(),sim.get_scene().get_mics().get_micByIdx(i).get_row()),VERTICAL_PIXELS,HORIZONTAL_PIXELS);
        points.append(micShape.get_shapePoints());
    }
    if(userMode==micEdit){
        selectedMicShape.set_shapePoints(QPoint(sim.get_scene().get_mics().get_micByIdx(micSelected).get_col(),sim.get_scene().get_mics().get_micByIdx(micSelected).get_row()),VERTICAL_PIXELS,HORIZONTAL_PIXELS);
        points.append(selectedMicShape.get_shapePoints());
    }
    return xy2absolut(points);
}

// ThTank::userModeChanged()
//
// PURPOSE
//   SLOT IS called when the usermode is changed for example, source edition is finished, it takes the corresponding actions in the GUI
//   and in the data according to new mode
//
// INPUT
//   UserMode newMode - the new userMode from enum UserMode
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::userModeChanged(UserMode newMode){
    userMode=newMode;
    switch(newMode){
      case sourceEdit:{
        mGUI.wallsPan->setAutoFillBackground(false);
        mGUI.microphonesPan->setAutoFillBackground(false);
        mGUI.sourcesPan->setAutoFillBackground(true);
        mGUI.newWallsBut->setStyleSheet("");
        mGUI.delWallsBut->setStyleSheet("");
        mGUI.sourcesPan->setPalette(QPalette(Qt::lightGray));
        break;
      }
      case wallAdd:{
        mGUI.sourcesPan->setAutoFillBackground(false);
        mGUI.microphonesPan->setAutoFillBackground(false);
        mGUI.wallsPan->setAutoFillBackground(true);
        mGUI.delWallsBut->setStyleSheet("");
        mGUI.newWallsBut->setStyleSheet("background-color: red");
        mGUI.wallsPan->setPalette(QPalette(Qt::lightGray));
        break;
      }
      case wallDelete:{
        mGUI.sourcesPan->setAutoFillBackground(false);
        mGUI.microphonesPan->setAutoFillBackground(false);
        mGUI.wallsPan->setAutoFillBackground(true);
        mGUI.newWallsBut->setStyleSheet("");
        mGUI.delWallsBut->setStyleSheet("background-color: red");
        mGUI.wallsPan->setPalette(QPalette(Qt::lightGray));
       break;
      }
      case micEdit:{
        mGUI.wallsPan->setAutoFillBackground(false);
        mGUI.sourcesPan->setAutoFillBackground(false);
        mGUI.microphonesPan->setAutoFillBackground(true);
        mGUI.newWallsBut->setStyleSheet("");
        mGUI.delWallsBut->setStyleSheet("");
        mGUI.microphonesPan->setPalette(QPalette(Qt::lightGray));
       break;
      }
      case free:{
        mGUI.wallsPan->setAutoFillBackground(false);
        mGUI.sourcesPan->setAutoFillBackground(false);
        mGUI.microphonesPan->setAutoFillBackground(false);
        mGUI.newWallsBut->setStyleSheet("");
        mGUI.delWallsBut->setStyleSheet("");
       break;
      }
    }
}

// ThTank::sourceTypeComIncreased()
//
// PURPOSE
//   SLOT called when source type ">" button is pressed, the GUI and if a source was being edited, show in the label the new type of source
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::sourceTypeComIncreased(){
    if(mGUI.typeSourceShown<mGUI.sourceTypes.size()-1){
        qDebug()<<mGUI.sourceTypes.size();
      mGUI.typeSourceShown++;
      sourceTypeComChange(mGUI.typeSourceShown);
    }
}

// ThTank::sourceTypeComIncreased()
//
// PURPOSE
//   SLOT called when source type "<" button is pressed, the GUI and if a source was being edited, show in the label the new type of source
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::sourceTypeComDecreased(){
    if(mGUI.typeSourceShown>0){
      mGUI.typeSourceShown--;
      sourceTypeComChange(mGUI.typeSourceShown);
    }
}

// ThTank::sourceTypeComIncreased()
//
// PURPOSE
//   SLOT called when boundary type ">" button is pressed the GUI show in the label the new type of boundary shape
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::wallTypeComIncreased(){
    if(mGUI.typeWallShown<mGUI.wallTypes.size()-1){
      mGUI.typeWallShown++;
      wallTypeComChange(mGUI.typeWallShown);
    }
}

// ThTank::sourceTypeComIncreased()
//
// PURPOSE
//   SLOT called when boundary type "<" button is pressed the GUI show in the label the new type of boundary shape
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::wallTypeComDecreased(){
    if(mGUI.typeWallShown>0){
      mGUI.typeWallShown--;
      wallTypeComChange(mGUI.typeWallShown);
    }
}

// ThTank::startSimulationButCilcked()
//
// PURPOSE
//   SLOT Activate the simulation once the button is clicked
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::startSimulationButCilcked(){
   QString option=mGUI.startSimulationBut->text();
   if (option=="start"){
       if(!simThread.isRunning()) simThread.start();
       sim.set_simulating(true);
       mGUI.startSimulationBut->setText("pause");

   }
   else{
       sim.set_simulating(false);
       mGUI.startSimulationBut->setText("start");
   }
}

// ThTank::screenShotButCilcked()
//
// PURPOSE
//   SLOT Save a picture when the print button is clicked
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::screenShotButCilcked(){
    userModeChanged(free);
    sim.set_simulating(false);
    mGUI.startSimulationBut->setText("start");
    QString filename=QFileDialog::getSaveFileName(this,"Please select the folder and name of the file to save",QDir::currentPath(),"All files *.*",0,QFileDialog::DontUseNativeDialog);
    if (filename.length()==0){
        return;
    }
    QPixmap pix=this->mGUI.drawPanelLab->grab();
        if(!pix.save(filename.append(".png"),"PNG")){
        QMessageBox::information(this, "Error", "Error saving the file...");
        return;
    }
    else
       QMessageBox::information(this, "Succes!", QString("File saved as ").append(filename));
}

// ThTank::screenShotButCilckedPlotter()
//
// PURPOSE
//   SLOT Save a picture when the print button is clicked on the waveform plot
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::screenShotButCilckedPlotter(){
    userModeChanged(free);
    sim.set_simulating(false);
    mGUI.startSimulationBut->setText("start");
    QString filename=QFileDialog::getSaveFileName(this,"Please select the folder and name of the file to save",QDir::currentPath(),"All files *.*",0,QFileDialog::DontUseNativeDialog);
    if (filename.length()==0){
        return;
    }
    QPixmap pix=this->plotter.plotCanvas->grab();
        if(!pix.save(filename.append(".png"),"PNG")){
        QMessageBox::information(this, "Error", "Error saving the file...");
        return;
    }
    else
       QMessageBox::information(this, "Succes!", QString("File saved as ").append(filename));
}

// ThTank::resetSimulation()
//
// PURPOSE
//   SLOT Resets the simulation
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::resetSimulation(){
    userModeChanged(free);
    sim.set_simulating(false);
    QThread::msleep(20);
    emit dataActualization(true);
    QThread::msleep(20);

    sim.set_simStep(0);

    //set all matrixes to 0
    setMatrix2null(sim.presVals);
    setMatrix2null(sim.outValsN);
    setMatrix2null(sim.outValsS);
    setMatrix2null(sim.outValsE);
    setMatrix2null(sim.outValsW);
    setMatrix2null(sim.inValsN);
    setMatrix2null(sim.inValsS);
    setMatrix2null(sim.inValsE);
    setMatrix2null(sim.inValsW);


    //set the boundaries and actualize with new wall points
    mGUI.startSimulationBut->setText("start");
    plotter.close();
    emit dataActualization(false);
}

// ThTank::plotSimulationButClicked()
//
// PURPOSE
//   SLOT Opens the waveform plot window if button was pressed
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::plotSimulationButClicked(){
    plotter.show();
}

// ThTank::loadSceneClicked()
//
// PURPOSE
//   SLOT Activated when load scene button is clicked, tries to load a scene saved in xml file
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::loadSceneClicked(){
   Scene temp;
   //open dialog window
   QString filename=QFileDialog::getOpenFileName(this,"Please select the file you want to open",QDir::currentPath(),"All files *.xml",0,QFileDialog::DontUseNativeDialog);
   if (filename.length()==0){
       return;
   }
   //call function to load the scene in xml
   if(!temp.loadXMLscene(filename,temp)){
       QMessageBox::information(this, "Error", "Error loading the file...");
       return;
   }
   else{
    QMessageBox::information(this, "Succes!", QString("File ").append(filename).append(" was loaded!"));
    //if reading of xml and conversion to scene object was succesful, load the scene
    loadScene(temp);
    QDir directory = QFileInfo(filename).absoluteDir();
    QDir::setCurrent(directory.absolutePath());
   }
}

// ThTank::saveSceneClicked()
//
// PURPOSE
//   SLOT Activated when save scene button is clicked, tries to save a scene object in a xml file
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::saveSceneClicked(){
   //open dialog window for the user to give location and name of the scene file in xml which is going to be created
   QString filename=QFileDialog::getSaveFileName(this,"Please select the folder and name of the file to save",QDir::currentPath(),"All files *.*",0,QFileDialog::DontUseNativeDialog);
   if (filename.length()==0){
       return;
   }
   filename.append(".xml");
   //fill the wallpoints object which is only used by loading and saving scene
   sim.set_scene()->set_wallPoints()->fillWallPoints(sim.field);
   //convert the scene object to xml element, if error prone message
   if(!sim.get_scene().saveXMLscene(filename)){
       QMessageBox::information(this, "Error", "Error saving the file...");
       return;
   }
   else{
      QDir directory = QFileInfo(filename).absoluteDir();
      QDir::setCurrent(directory.absolutePath());
      QMessageBox::information(this, "Succes!", QString("File saved as ").append(filename));
   }
}

// ThTank::print_toolTip()
//
// PURPOSE
//   called by cursor when it enters the field, it prints the text under the fiel with the coordinates of the cursor, info aabout a shape... depending
//   on the userMode (modifying a source, adding a source, a wall.....)
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::print_toolTip(){
   if(!mGUI.drawPanelLab->isActiveWindow()) return;
   QString info="";
   int N,S,E,W;
   float delta=sim.get_scene().get_delta();
   switch (userMode){
     case free: //if no edition is taken place show coordinates of cursor
       info.append("Cursor Coordinates(x/y)-->").append(QString::number(cursorPos.x()*delta)).append("/").append(QString::number(cursorPos.y()*delta));
       break;
     case wallDelete:
     case wallAdd: //if a boundary is being added or deletes, show the coordinates of the shape and it's size
       if(shape.get_shapeType()==Shape::point || shape.get_shapeType()==Shape::rectEmpty || shape.get_shapeType()==Shape::rectFull){
         N=cursorPos.y()-shape.get_shapeHeigth();
         S=cursorPos.y()+shape.get_shapeHeigth();
         E=cursorPos.x()+shape.get_shapeWidth();
         W=cursorPos.x()-shape.get_shapeWidth();
         if(N<0) N=0;
         if(S>=VERTICAL_PIXELS) S=VERTICAL_PIXELS-1;
         if(E>=HORIZONTAL_PIXELS) E=HORIZONTAL_PIXELS-1;
         if(W<0) W=0;
         info.append("Width/Heigth----------->");
         //info.append(" NW: (").append(number(N*delta)).append("/").append(number(W*delta)).append(")");
         //info.append(" NE: (").append(number(N*delta)).append("/").append(number(E*delta)).append(")");
         //info.append(" SW: (").append(number(S*delta)).append("/").append(number(W*delta)).append(")");
         //info.append(" SE: (").append(number(S*delta)).append("/").append(number(E*delta)).append(")");
         info.append(QString::number((S-N)*delta)).append("/").append(QString::number((E-W)*delta)).append("m");
         info.append("<br>Upper-Left corner(x/y)---> ").append(QString::number(W*delta)).append("/").append(QString::number(N*delta)).append("m");
       }
       else if(shape.get_shapeType()==Shape::circleFull || shape.get_shapeType()==Shape::circleEmpty){
         info.append("Diameter----->").append(QString::number(shape.get_shapeHeigth()*2*delta)).append("m");
         info.append("<br>Center(x/y)--->").append(QString::number(cursorPos.x()*delta)).append("/").append(QString::number(cursorPos.y()*delta)).append("m");

       }
       break;
      case sourceEdit: //if a source is being edited, show the name of the source and it's coordinates
       info.append("Source selected-->").append(sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_name());
       info.append("<br>Cursor Coordinates(x/y)-->").append(QString::number(cursorPos.x()*delta)).append("/").append(QString::number(cursorPos.y()*delta));
       break;
      case micEdit: //if a mic is being edited, show the name of the mic and it's coordinates
       info.append("Mic selected-->").append(sim.get_scene().get_mics().get_micByIdx(micSelected).get_name());
       info.append("<br>Cursor Coordinates(x/y)-->").append(QString::number(cursorPos.x()*delta)).append("/").append(QString::number(cursorPos.y()*delta));
       break;
   }
   mGUI.infoLabel->setText(info);
}

// ThTank::mouseClickedEventDrawPanel()
//
// PURPOSE
//   SLOT activated when the mouse is clicked inside the simulation field (right or left click)
//
// INPUT
//   const int &button - button (right or left button)
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::mouseClickedEventDrawPanel(const int &button){
   if (button==Qt::RightButton){ //right button clicked
       switch (userMode){
        case sourceEdit:
        case micEdit:
        case free:{ //if free, edit source or edit mic and right button is clicked, a new source is created in the place where cursor finds itself
           mGUI.xSourcesEdi->setText(QString::number(cursorPos.x()*sim.get_scene().get_delta()));
           mGUI.ySourcesEdi->setText(QString::number(cursorPos.y()*sim.get_scene().get_delta()));
           emit mGUI.newSourcesBut->clicked();
        }
         break;
        case wallDelete:
        case wallAdd:
         break;
       }
   }
   else{ //left button clicked
    bool state=sim.get_simulating();
    switch (userMode){
    case wallAdd: //if in boundary edition mode, the boundary shape is pasted on the field
     mutex.lock();
     sim.set_simulating(false);
     mutex.unlock();
     QThread::msleep(50);
     sim.add_walls2field(shape.get_shapePoints());
     if(state) sim.set_simulating(true);
     break;
    case wallDelete: //if in boundary edition mode, the boundary being covered by the shape is eliminated from the field
     mutex.lock();
     sim.set_simulating(false);
     mutex.unlock();
     QThread::msleep(50);
     sim.delete_walls2field(shape.get_shapePoints());
     if(state) sim.set_simulating(true);
     break;
    case sourceEdit:
    case micEdit:
    case free:{ //if free mode, and user clicked near a source or mic go to edition mode of that source or mic ( click
        // must be 10 pixels from source/mic center to enter in edition mode of that source mic/source)
     int idx=-1;
     int distance=0;
     int minDistance=10;
     for(int i=0;i<sim.get_scene().get_sources().get_nSources();i++){
        int distV=sim.get_scene().get_sources().get_sourceByIdx(i).get_row()-cursorPos.y();
        int distH=sim.get_scene().get_sources().get_sourceByIdx(i).get_col()-cursorPos.x();
        distance=round( sqrt( pow(distV,2.0)+pow(distH,2.0) ) );
        if (distance<=minDistance){
            minDistance=distance;
            idx=i;
        }
     }
     if(idx!=-1){
         userModeChanged(sourceEdit);
         dragSource=1;
         sourceSelected=idx;
         mGUI.nameSourcesEdi->setText(sim.get_scene().get_sources().get_sourceByIdx(idx).get_name());
         mGUI.xSourcesEdi->setText(QString::number(sim.get_scene().get_sources().get_sourceByIdx(idx).get_col()*sim.get_scene().get_delta()));
         mGUI.ySourcesEdi->setText(QString::number(sim.get_scene().get_sources().get_sourceByIdx(idx).get_row()*sim.get_scene().get_delta()));
         mGUI.phaseSourcesEdi->setText(QString::number(sim.get_scene().get_sources().get_sourceByIdx(idx).get_phase()));
         mGUI.frequencySourcesEdi->setText(QString::number(sim.get_scene().get_sources().get_sourceByIdx(idx).get_freq()));
         mGUI.amplitudeSourcesEdi->setText(QString::number(pa2db(sim.get_scene().get_sources().get_sourceByIdx(idx).get_amp())));
         mGUI.typeSourceShown=sim.get_scene().get_sources().get_sourceByIdx(idx).get_type();
         mGUI.typeSourcesCom->setText(mGUI.sourceTypes.at(mGUI.typeSourceShown));
         emit mGUI.amplitudeSourcesEdi->editingFinished();
         emit mGUI.phaseSourcesEdi->editingFinished();
         emit mGUI.frequencySourcesEdi->editingFinished();
     }
     else{
         minDistance=10;
         for(int i=0;i<sim.get_scene().get_mics().get_nMics();i++){
            int distV=sim.get_scene().get_mics().get_micByIdx(i).get_row()-cursorPos.y();
            int distH=sim.get_scene().get_mics().get_micByIdx(i).get_col()-cursorPos.x();
            distance=round( sqrt( pow(distV,2.0)+pow(distH,2.0) ) );
            if (distance<=minDistance){
                minDistance=distance;
                idx=i;
            }
         }
         if(idx!=-1){
             userModeChanged(micEdit);
             dragMic=1;
             micSelected=idx;
             mGUI.nameMicrophonesEdi->setText(sim.get_scene().get_mics().get_micByIdx(idx).get_name());
             mGUI.xMicrophonesEdi->setText(QString::number(sim.get_scene().get_mics().get_micByIdx(idx).get_col()*sim.get_scene().get_delta()));
             mGUI.yMicrophonesEdi->setText(QString::number(sim.get_scene().get_mics().get_micByIdx(idx).get_row()*sim.get_scene().get_delta()));
         }
     }
    }
     break;
    }
   }
   print_toolTip();
}

// ThTank::mouseReleasedEventDrawPanel()
//
// PURPOSE
//   SLOT activated when the mouse is released inside the simulation field, only used when a mic or source was being dragged, it sets its position
//
// INPUT
//   const int &button - button (right or left button)
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::mouseReleasedEventDrawPanel(const int &button){
   if(button==Qt::LeftButton){
    if(userMode==sourceEdit){
     dragSource=0;
     mGUI.xSourcesEdi->setText(QString::number(sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_col()*sim.get_scene().get_delta()));
     mGUI.ySourcesEdi->setText(QString::number(sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_row()*sim.get_scene().get_delta()));
     emit mGUI.xSourcesEdi->editingFinished();
     emit mGUI.ySourcesEdi->editingFinished();
    }
    if(userMode==micEdit){
     dragMic=0;
     mGUI.xMicrophonesEdi->setText(QString::number(sim.get_scene().get_mics().get_micByIdx(micSelected).get_col()*sim.get_scene().get_delta()));
     mGUI.yMicrophonesEdi->setText(QString::number(sim.get_scene().get_mics().get_micByIdx(micSelected).get_row()*sim.get_scene().get_delta()));
     emit mGUI.xMicrophonesEdi->editingFinished();
     emit mGUI.yMicrophonesEdi->editingFinished();
    }
  }
}

// ThTank::enterEventDrawPanel()
//
// PURPOSE
//   SLOT activated when the cursor enters the simulation field
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::enterEventDrawPanel(){
    print_toolTip();
    mGUI.drawPanelLab->setFocus(Qt::OtherFocusReason);
}

// ThTank::leaveEventDrawPanel()
//
// PURPOSE
//   SLOT activated when the cursor leaves the simulation field
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::leaveEventDrawPanel(){
    mGUI.infoLabel->setText("");
    this->setFocus(Qt::OtherFocusReason);
}

// ThTank::keyPressEventInterface()
//
// PURPOSE
//   SLOT called when a key of the keyboard is pressed
//
// INPUT
//   const int &key - key of the keyboard that was pressed
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::keyPressEventInterface(const int &key){
    switch (key){
    case Qt::Key_Down:
    case Qt::Key_Up:
    case Qt::Key_Left:
    case Qt::Key_Right:
        if (userMode==wallDelete || userMode==wallAdd){ //if wallDelete or WallAdd mode, makes the figure shape change
          switch(shape.get_shapeType()){
           case Shape::point:
           case Shape::circleFull:
           case Shape::circleEmpty: //diameter of circle, or length/width together of point is incremented (up,right key) or decremented (left,down)
             if(key==Qt::Key_Down || key==Qt::Key_Left) shape.shapeDecrement('a');
             else if(key==Qt::Key_Up || key==Qt::Key_Right) shape.shapeIncrement('a');
             break;
           case Shape::rectEmpty:
           case Shape::rectFull: //width of rectangle is incremented (right key), decremented (left key) or height is incrmenented (up key) or decremented (down key)
             if(key==Qt::Key_Down) shape.shapeDecrement('v');
             else if(key==Qt::Key_Left) shape.shapeDecrement('h');
             else if(key==Qt::Key_Up) shape.shapeIncrement('v');
             else if(key==Qt::Key_Right) shape.shapeIncrement('h');
             break;
           default:
              break;
          }
        }
        else if(userMode==sourceEdit){ // if source edit mode, gets the next (up, right keys)  or previous (down, left) source and actualize de GUI fields
            if(sourceSelected>0 && key==Qt::Key_Down) sourceSelected--;
            else if(sourceSelected>0 && key==Qt::Key_Left) sourceSelected--;
            else if(sourceSelected<sim.get_scene().get_sources().get_nSources()-1 && key==Qt::Key_Up) sourceSelected++;
            else if(sourceSelected<sim.get_scene().get_sources().get_nSources()-1 && key==Qt::Key_Right) sourceSelected++;
                mGUI.nameSourcesEdi->setText(sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_name());
                mGUI.xSourcesEdi->setText(QString::number(sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_col()*sim.get_scene().get_delta()));
                mGUI.ySourcesEdi->setText(QString::number(sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_row()*sim.get_scene().get_delta()));
                mGUI.phaseSourcesEdi->setText(QString::number(sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_phase()));
                mGUI.frequencySourcesEdi->setText(QString::number(sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_freq()));
                mGUI.amplitudeSourcesEdi->setText(QString::number(pa2db(sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_amp())));
                mGUI.typeSourceShown=sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_type();
                mGUI.typeSourcesCom->setText(mGUI.sourceTypes.at(mGUI.typeSourceShown));
                emit mGUI.amplitudeSourcesEdi->editingFinished();
                emit mGUI.phaseSourcesEdi->editingFinished();
                emit mGUI.frequencySourcesEdi->editingFinished();

        }
        else if(userMode==micEdit){ // if mic edit mode, gets the next (up, right keys) or previous (down, left) mic and actualize de GUI fields
            if(micSelected>0 && key==Qt::Key_Down) micSelected--;
            else if(micSelected>0 && key==Qt::Key_Left) micSelected--;
            else if(micSelected<sim.get_scene().get_mics().get_nMics()-1 && key==Qt::Key_Up) micSelected++;
            else if(micSelected<sim.get_scene().get_mics().get_nMics()-1 && key==Qt::Key_Right) micSelected++;
                mGUI.nameMicrophonesEdi->setText(sim.get_scene().get_mics().get_micByIdx(micSelected).get_name());
                mGUI.xMicrophonesEdi->setText(QString::number(sim.get_scene().get_mics().get_micByIdx(micSelected).get_col()*sim.get_scene().get_delta()));
                mGUI.yMicrophonesEdi->setText(QString::number(sim.get_scene().get_mics().get_micByIdx(micSelected).get_row()*sim.get_scene().get_delta()));

        }
        break;


    case Qt::Key_W: //if in edit source or mic mode move the source/mic one point up
        if(userMode==sourceEdit){
            if(sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_row()>0){
              int pixelRow=sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_row()-1;
              int pixelCol=sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_col();
              int found=sim.get_scene().get_sources().find_sourceByPos(pixelRow,pixelCol);
              if(found!=-1 && found!=sourceSelected){
                  pixelRow=sim.get_scene().get_sources().get_newFreeSourcePos('N',pixelRow,pixelCol,VERTICAL_PIXELS,HORIZONTAL_PIXELS).y();
              }
              sim.set_scene()->set_sources()->set_sourceByIdx(sourceSelected)->set_row(pixelRow);
              mGUI.ySourcesEdi->setText(QString::number(sim.get_scene().get_delta()*pixelRow));
              mGUI.xSourcesEdi->setText(QString::number(sim.get_scene().get_delta()*pixelCol));
            }
        }
        else if(userMode==micEdit){
            if(sim.get_scene().get_mics().get_micByIdx(micSelected).get_row()>0){
              int pixelRow=sim.get_scene().get_mics().get_micByIdx(micSelected).get_row()-1;
              int pixelCol=sim.get_scene().get_mics().get_micByIdx(micSelected).get_col();
              int found=sim.get_scene().get_mics().find_micByPos(pixelRow,pixelCol);
              if(found!=-1 && found!=micSelected){
                  pixelRow=sim.get_scene().get_mics().get_newFreeMicPos('N',pixelRow,pixelCol,VERTICAL_PIXELS,HORIZONTAL_PIXELS).y();
              }
              sim.set_scene()->set_mics()->set_micByIdx(micSelected)->set_row(pixelRow);
              mGUI.yMicrophonesEdi->setText(QString::number(sim.get_scene().get_delta()*pixelRow));
              mGUI.xMicrophonesEdi->setText(QString::number(sim.get_scene().get_delta()*pixelCol));
            }
        }
        break;
    case Qt::Key_S: //if in edit source or mic mode move the source/mic one point down
        if(userMode==sourceEdit){
            if(sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_row()<VERTICAL_PIXELS-1){
              int pixelRow=sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_row()+1;
              int pixelCol=sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_col();
              int found=sim.get_scene().get_sources().find_sourceByPos(pixelRow,pixelCol);
              if(found!=-1 && found!=sourceSelected){
                  pixelRow=sim.get_scene().get_sources().get_newFreeSourcePos('S',pixelRow,pixelCol,VERTICAL_PIXELS,HORIZONTAL_PIXELS).y();
              }
              sim.set_scene()->set_sources()->set_sourceByIdx(sourceSelected)->set_row(pixelRow);
              mGUI.ySourcesEdi->setText(QString::number(sim.get_scene().get_delta()*pixelRow));
              mGUI.xSourcesEdi->setText(QString::number(sim.get_scene().get_delta()*pixelCol));
            }
        }
        else if(userMode==micEdit){
            if(sim.get_scene().get_mics().get_micByIdx(micSelected).get_row()<VERTICAL_PIXELS-1){
              int pixelRow=sim.get_scene().get_mics().get_micByIdx(micSelected).get_row()+1;
              int pixelCol=sim.get_scene().get_mics().get_micByIdx(micSelected).get_col();
              int found=sim.get_scene().get_mics().find_micByPos(pixelRow,pixelCol);
              if(found!=-1 && found!=micSelected){
                  pixelRow=sim.get_scene().get_mics().get_newFreeMicPos('S',pixelRow,pixelCol,VERTICAL_PIXELS,HORIZONTAL_PIXELS).y();
              }
              sim.set_scene()->set_mics()->set_micByIdx(micSelected)->set_row(pixelRow);
              mGUI.yMicrophonesEdi->setText(QString::number(sim.get_scene().get_delta()*pixelRow));
              mGUI.xMicrophonesEdi->setText(QString::number(sim.get_scene().get_delta()*pixelCol));
            }
        }
        break;
    case Qt::Key_D: //if in edit source or mic mode move the source/mic one point right
        if(userMode==sourceEdit){
            if(sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_col()<HORIZONTAL_PIXELS-1){
              int pixelRow=sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_row();
              int pixelCol=sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_col()+1;
              int found=sim.get_scene().get_sources().find_sourceByPos(pixelRow,pixelCol);
              if(found!=-1 && found!=sourceSelected){
                  pixelCol=sim.get_scene().get_sources().get_newFreeSourcePos('E',pixelRow,pixelCol,VERTICAL_PIXELS,HORIZONTAL_PIXELS).x();
              }
              sim.set_scene()->set_sources()->set_sourceByIdx(sourceSelected)->set_col(pixelCol);
              mGUI.ySourcesEdi->setText(QString::number(sim.get_scene().get_delta()*pixelRow));
              mGUI.xSourcesEdi->setText(QString::number(sim.get_scene().get_delta()*pixelCol));
            }
        }
        else if(userMode==micEdit){
            if(sim.get_scene().get_mics().get_micByIdx(micSelected).get_col()<HORIZONTAL_PIXELS-1){
              int pixelRow=sim.get_scene().get_mics().get_micByIdx(micSelected).get_row();
              int pixelCol=sim.get_scene().get_mics().get_micByIdx(micSelected).get_col()+1;
              int found=sim.get_scene().get_mics().find_micByPos(pixelRow,pixelCol);
              if(found!=-1 && found!=micSelected){
                  pixelCol=sim.get_scene().get_mics().get_newFreeMicPos('E',pixelRow,pixelCol,VERTICAL_PIXELS,HORIZONTAL_PIXELS).x();
              }
              sim.set_scene()->set_mics()->set_micByIdx(micSelected)->set_col(pixelCol);
              mGUI.yMicrophonesEdi->setText(QString::number(sim.get_scene().get_delta()*pixelRow));
              mGUI.xMicrophonesEdi->setText(QString::number(sim.get_scene().get_delta()*pixelCol));
            }
        }
        break;
    case Qt::Key_A: //if in edit source or mic mode move the source/mic one point left
        if(userMode==sourceEdit){
            if(sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_col()>0){
              int pixelRow=sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_row();
              int pixelCol=sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_col()-1;
              int found=sim.get_scene().get_sources().find_sourceByPos(pixelRow,pixelCol);
              if(found!=-1 && found!=sourceSelected){
                  pixelCol=sim.get_scene().get_sources().get_newFreeSourcePos('W',pixelRow,pixelCol,VERTICAL_PIXELS,HORIZONTAL_PIXELS).x();
              }
              sim.set_scene()->set_sources()->set_sourceByIdx(sourceSelected)->set_col(pixelCol);
              mGUI.ySourcesEdi->setText(QString::number(sim.get_scene().get_delta()*pixelRow));
              mGUI.xSourcesEdi->setText(QString::number(sim.get_scene().get_delta()*pixelCol));
            }
        }
        else if(userMode==micEdit){
            if(sim.get_scene().get_mics().get_micByIdx(micSelected).get_col()>0){
              int pixelRow=sim.get_scene().get_mics().get_micByIdx(micSelected).get_row();
              int pixelCol=sim.get_scene().get_mics().get_micByIdx(micSelected).get_col()-1;
              int found=sim.get_scene().get_mics().find_micByPos(pixelRow,pixelCol);
              if(found!=-1 && found!=micSelected){
                  pixelCol=sim.get_scene().get_mics().get_newFreeMicPos('W',pixelRow,pixelCol,VERTICAL_PIXELS,HORIZONTAL_PIXELS).x();
              }
              sim.set_scene()->set_mics()->set_micByIdx(micSelected)->set_col(pixelCol);
              mGUI.yMicrophonesEdi->setText(QString::number(sim.get_scene().get_delta()*pixelRow));
              mGUI.xMicrophonesEdi->setText(QString::number(sim.get_scene().get_delta()*pixelCol));
            }
        }
        break;
    case Qt::Key_Return: //if in edit source or mic mode actualize the changed field
        if (userMode==sourceEdit || userMode==micEdit){
          UserMode temp=userMode;
          mGUI.lose_focus();
          userModeChanged(temp);
        }
        break;
    case Qt::Key_Escape: //go to free user mode
        userModeChanged(free);
        break;

    case Qt::Key_Backspace: //if in edit source or mic mode delete the selected source/mic
    case Qt::Key_Delete:
        if (userMode==sourceEdit){
          userModeChanged(free);
          sim.set_scene()->set_sources()->delete_sourceByIdx(sourceSelected);
          //reset simulation if autoreset is active
          if (autoReset) resetSimulation();
        }
        if (userMode==micEdit){
          userModeChanged(free);
          sim.set_scene()->set_mics()->delete_micByIdx(micSelected);
          //reset simulation if autoreset is active
          if (autoReset) resetSimulation();
        }
        break;

    case Qt::Key_P: //makes a screenshot
        emit mGUI.screenShotBut->clicked();
        break;
       }
    print_toolTip();

}

// ThTank::next_source()
//
// PURPOSE
//   SLOT called when next source button ">>" is called, it gets the next source while edit source mode, or the first source if in free mode to edit it
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::next_source(){
    if(userMode==free){ //if not in sourceEdit mode get the first source available and go to edit mode
      if(sim.get_scene().get_sources().get_nSources()>0){
        sourceSelected=1;
        userModeChanged(sourceEdit);
        emit keyPressEventInterface(Qt::Key_Down);
      }
    }
    else if(userMode==sourceEdit){ //if in sourceEdit call key right which is the same as calling next source
        emit keyPressEventInterface(Qt::Key_Right);
    }
    else userModeChanged(free); //if no sources on the scene just go to free mode
}

// ThTank::prev_source()
//
// PURPOSE
//   SLOT called when previous source button "<<" is called, it gets the previous source while edit source mode, or the first source if in free mode to edit it
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::prev_source(){
    if(userMode==free){
      if(sim.get_scene().get_sources().get_nSources()>0){ //if not in sourceEdit mode get the first source available and go to edit mode
        sourceSelected=1;
        userModeChanged(sourceEdit);
        emit keyPressEventInterface(Qt::Key_Down);
      }
    }
    else if(userMode==sourceEdit){ //if in sourceEdit call key left which is the same as calling previous source
        emit keyPressEventInterface(Qt::Key_Left);
    }
    else userModeChanged(free); //if no sources on the scene just go to free mode
}

// ThTank::next_mic()
//
// PURPOSE
//   SLOT called when next mic button ">>" is called, it gets the next mic while edit mic mode, or the first mic if in free mode to edit it
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::next_mic(){

    if(userMode==free){
      if(sim.get_scene().get_mics().get_nMics()>0){ //if not in micEdit mode get the first mic available and go to edit mode
        micSelected=1;
        userModeChanged(micEdit);
        emit keyPressEventInterface(Qt::Key_Down);
      }
    }
    else if(userMode==micEdit){ //if in micEdit call key left which is the same as calling next mic
        emit keyPressEventInterface(Qt::Key_Right);
    }
    else userModeChanged(free); //if no mics on the scene just go to free mode
}

// ThTank::prev_mic()
//
// PURPOSE
//   SLOT called when previous mic button "<<" is called, it gets the previous mic while edit mic mode, or the first mic if in free mode to edit it
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::prev_mic(){
    if(userMode==free){
      if(sim.get_scene().get_mics().get_nMics()>0){ //if not in micEdit mode get the first mic available and go to edit mode
        micSelected=1;
        userModeChanged(micEdit);
        emit keyPressEventInterface(Qt::Key_Down);
      }
    }
    else if(userMode==micEdit){ //if in micEdit call key left which is the same as calling previous mic
        emit keyPressEventInterface(Qt::Key_Left);
    }
    else userModeChanged(free); //if no mics on the scene just go to free mode
}

// ThTank::sourceNameEdiChange()
//
// PURPOSE
//   Actualize the value of the source name after a change in the source name edit field ocurred
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::sourceNameEdiChange(){
   QString valueStrIn=mGUI.nameSourcesEdi->text();

   //check if value is correct eliminating all characters which are not letters or numbers
   removeBadChars(valueStrIn);

   //check that name is not repeated (if repeated get a new one) and change the source name
   int found=sim.get_scene().get_sources().find_sourceByName(valueStrIn);
   if(userMode==sourceEdit){
       if(found!=-1 && found!=sourceSelected) valueStrIn=sim.get_scene().get_sources().get_newFreeSourceName();
       sim.set_scene()->set_sources()->set_sourceByIdx(sourceSelected)->set_name(valueStrIn);
   }
   else{
       if(found!=-1 || valueStrIn.length()<=0) valueStrIn=sim.get_scene().get_sources().get_newFreeSourceName();
   }

   //set the value in case it was corrected
   mGUI.nameSourcesEdi->setText(valueStrIn);

}

// ThTank::sourceXEdiChange()
//
// PURPOSE
//   Actualize the value of the source row number after a change in the source x edit field ocurred
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::sourceXEdiChange(){
   QString valueStrIn=mGUI.xSourcesEdi->text();

   //check if value can be converted
   bool correct=false;
   float value=valueStrIn.toFloat(&correct);
   if(!correct){
       int temp=valueStrIn.toInt(&correct);
       if (correct) value=temp;
       else value=DEFAULT_COL*sim.get_scene().get_delta();
   }

   //convert the value to pixels
   int pixelPos=round(value/sim.get_scene().get_delta());

   //check that value is not out of the max pixels, in case it is, set it to min or max directly
   if (pixelPos<0) pixelPos=0;
   if (pixelPos>=HORIZONTAL_PIXELS) pixelPos=HORIZONTAL_PIXELS-1;

   //check that name is not repeated (if repeated get a new one) and change the source name
   int found=sim.get_scene().get_sources().find_sourceByPos(round(mGUI.ySourcesEdi->text().toFloat()/sim.get_scene().get_delta()),pixelPos);
   if(userMode==sourceEdit){
       if(found!=-1 && found!=sourceSelected) pixelPos=sim.get_scene().get_sources().get_newFreeSourcePos('E',sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_row(),pixelPos,VERTICAL_PIXELS,HORIZONTAL_PIXELS).x();
           sim.set_scene()->set_sources()->set_sourceByIdx(sourceSelected)->set_col(pixelPos);
   }
   else{
       if(found!=-1) pixelPos=sim.get_scene().get_sources().get_newFreeSourcePos('E',round(mGUI.ySourcesEdi->text().toFloat()/sim.get_scene().get_delta()),pixelPos,VERTICAL_PIXELS,HORIZONTAL_PIXELS).x();
   }

   //set the value in case it was corrected
   mGUI.xSourcesEdi->setText(QString::number(pixelPos*sim.get_scene().get_delta()));

}

// ThTank::sourceYEdiChange()
//
// PURPOSE
//   Actualize the value of the source row number after a change in the source x edit field ocurred
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::sourceYEdiChange(){
   QString valueStrIn=mGUI.ySourcesEdi->text();

   //check if value can be converted
   bool correct=false;
   float value=valueStrIn.toFloat(&correct);
   if(!correct){
       int temp=valueStrIn.toInt(&correct);
       if (correct) value=temp;
       else value=DEFAULT_ROW*sim.get_scene().get_delta();
   }

   //convert the value to pixels
   int pixelPos=round(value/sim.get_scene().get_delta());

   //check that value is not out of the max pixels, in case it is, set it to min or max directly
   if (pixelPos<0) pixelPos=0;
   if (pixelPos>=VERTICAL_PIXELS) pixelPos=VERTICAL_PIXELS-1;

   //check that name is not repeated (if repeated get a new one) and change the source name
   int found=sim.get_scene().get_sources().find_sourceByPos(pixelPos,round(mGUI.xSourcesEdi->text().toFloat()/sim.get_scene().get_delta()));
   if(userMode==sourceEdit){
       if(found!=-1 && found!=sourceSelected) pixelPos=sim.get_scene().get_sources().get_newFreeSourcePos('S',pixelPos,sim.get_scene().get_sources().get_sourceByIdx(sourceSelected).get_col(),VERTICAL_PIXELS,HORIZONTAL_PIXELS).y();
           sim.set_scene()->set_sources()->set_sourceByIdx(sourceSelected)->set_row(pixelPos);
   }
   else{
       if(found!=-1) pixelPos=sim.get_scene().get_sources().get_newFreeSourcePos('S',pixelPos,round(mGUI.xSourcesEdi->text().toFloat()/sim.get_scene().get_delta()),VERTICAL_PIXELS,HORIZONTAL_PIXELS).y();
   }

   //set the value in case it was corrected
   mGUI.ySourcesEdi->setText(QString::number(pixelPos*sim.get_scene().get_delta()));
}

// ThTank::sourceTypeComChange()
//
// PURPOSE
//   SLOT Actualize the value of the source type after a change in the source type ocurred (buttons in sources panel "<" ">" pressed)
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::sourceTypeComChange(int idx){
    mGUI.typeSourcesCom->setText(mGUI.sourceTypes.at(idx));
    if(userMode==sourceEdit){
        sim.set_scene()->set_sources()->set_sourceByIdx(sourceSelected)->set_type(idx);
    }
}

// ThTank::wallTypeComChange()
//
// PURPOSE
//   SLOT Actualize the value of the wall shape after a change in the wall shape ocurred (buttons in boundaries panel "<" ">" pressed)
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::wallTypeComChange(int idx){
    mGUI.typeWallsCom->setText(mGUI.wallTypes.at(idx));
    if(userMode==wallAdd){
       shape.clearShapePoints();
       shape.set_shapeType(idx);
       shape.set_shapeHeigth(2);
       shape.set_shapeWidth(2);
    }
}

// ThTank::closeFieldPressed()
//
// PURPOSE
//   SLOT activated when the close field radio button is clicked or unclicked
//
// INPUT
//   const bool &clicked - true if radio button is clicked, false if unclicked
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::closeFieldPressed(const bool &clicked){
      if(clicked){
          sim.set_scene()->set_boundaryCoef(sim.get_scene().get_refCoef());
          sim.set_fieldClosed(true);
      }
      else{
          sim.set_scene()->set_boundaryCoef(FREE_FIELD_BOUNDARIES);
          sim.set_fieldClosed(false);
      }
}

// ThTank::autoResetPressed()
//
// PURPOSE
//   SLOT activated when the auto reset radio button is clicked or unclicked, the autoreset function will be then active or not
//
// INPUT
//   const bool &clicked - true if radio button is clicked, false if unclicked
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::autoResetPressed(const bool &clicked){
      if(clicked){
          autoReset=true;
      }
      else{
          autoReset=false;
      }
}

// ThTank::sourceAmpSliChange()
//
// PURPOSE
//   Actualize the value of the source amplitude after a change in the slider. Also the text edit field is actualized.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::sourceAmpSliChange(){
   int valueIn=mGUI.amplitudeSourcesSli->value();
   //check value correctness
   if ((valueIn)<mGUI.amplitudeSourcesSli->minimum() || (valueIn)>mGUI.amplitudeSourcesSli->maximum()) return;

   //convert the value to MIN_AMPLITUDE-MAX_AMPLITUDE
   int value=((MAX_AMPLITUDE_DB-MIN_AMPLITUDE_DB)*valueIn/(mGUI.amplitudeSourcesSli->maximum()-mGUI.amplitudeSourcesSli->minimum()))+MIN_AMPLITUDE_DB;

   //print the value converted to QString in the edit text field
   mGUI.amplitudeSourcesEdi->setText(QString::number(value));

   //if edit source mode is active, change the source amplitude
   if(userMode==sourceEdit){
       float val= db2pa(value);
       sim.set_scene()->set_sources()->set_sourceByIdx(sourceSelected)->set_amp(val);
   }
}

// ThTank::sourceAmpEdiChange()
//
// PURPOSE
//   Actualize the value of the source amplitude after a change in the text edit field. Also the slider value is actualized.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::sourceAmpEdiChange(){
   //get the string in edit field
    QString valueStrIn=mGUI.amplitudeSourcesEdi->text();

   //check if value can be converted, if not, set it to min value
   bool correct=false;
   int valueIn=valueStrIn.toInt(&correct);
   if(!correct){
       float temp=valueStrIn.toFloat(&correct);
       if (correct) valueIn=temp;
       else valueIn=DEFAULT_AMPLITUDE_DB;
   }

   //check that value is not out of the max amplitude, in case it is, set it to min or max directly
   if (valueIn<MIN_AMPLITUDE_DB) valueIn=MIN_AMPLITUDE_DB;
   if (valueIn>=MAX_AMPLITUDE_DB) valueIn=MAX_AMPLITUDE_DB;

   //convert the value to min_slider-max_slider and round
   float valueInF=((float)(valueIn-MIN_AMPLITUDE_DB)/(MAX_AMPLITUDE_DB-MIN_AMPLITUDE_DB))*mGUI.amplitudeSourcesSli->maximum()-mGUI.amplitudeSourcesSli->minimum();

   //round
   int valueInI = (int) ((valueInF > 0.0) ? (valueInF + 0.5) : (valueInF - 0.5));

   //set the slider to the converted value
   mGUI.amplitudeSourcesSli->setValue(valueInI);

   //if edit source mode is active, change the source amplitude
   if(userMode==sourceEdit){
       sim.set_scene()->set_sources()->set_sourceByIdx(sourceSelected)->set_amp(db2pa(valueIn));
   }

   //set the value in case it was corrected
   mGUI.amplitudeSourcesEdi->setText(QString::number(valueIn));

}


// ThTank::sourceFreqSliChange()
//
// PURPOSE
//   Actualize the value of the source frequency after a change in the slider. Also the text edit field is actualized.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::sourceFreqSliChange(){
   int valueIn=mGUI.frequencySourcesSli->value();
   //check value correctness
   if ((valueIn)<mGUI.frequencySourcesSli->minimum() || (valueIn)>mGUI.frequencySourcesSli->maximum()) return;

   //get min, max and scaling factor for logartihmic work
   float minValue=MIN_FREQUENCY;
   float maxValue=MAX_FREQUENCY;

   int minSlider=mGUI.frequencySourcesSli->minimum();
   int maxSlider=mGUI.frequencySourcesSli->maximum();

   float minValueL=log10(minValue);
   float maxValueL=log10(maxValue);

   float scale=(maxValueL-minValueL) / (maxSlider-minSlider);

   float valueInLF=pow(10,(minValueL+scale*(valueIn-minSlider)));

   //take out decimals
   float valueInLI = roundf(valueInLF);

   //print the value converted to QString in the edit text field
   mGUI.frequencySourcesEdi->setText(QString::number(valueInLI));

   //if edit source mode is active, change the source frequency
   if(userMode==sourceEdit){
       sim.set_scene()->set_sources()->set_sourceByIdx(sourceSelected)->set_freq(valueInLI);
   }
}

// ThTank::sourceFreqEdiChange()
//
// PURPOSE
//   Actualize the value of the source frequency after a change in the text edit field. Also the slider value is actualized.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::sourceFreqEdiChange(){
    //get min, max and scaling factor for logartihmic work
    int minSlider=mGUI.frequencySourcesSli->minimum();
    int maxSlider=mGUI.frequencySourcesSli->maximum();

    float minValueL=log10(MIN_FREQUENCY);
    float maxValueL=log10(MAX_FREQUENCY);

    float scale=(maxValueL-minValueL) / (maxSlider-minSlider);

   //get the string in edit field
    QString valueStrIn=mGUI.frequencySourcesEdi->text();

   //check if value can be converted if not set it to the half of the slider
   bool correct=false;
   float valueInF=valueStrIn.toFloat(&correct);


   if(!correct){
    int temp=valueStrIn.toInt(&correct);
    if (correct) valueInF=temp;
    else valueInF=DEFAULT_FREQUENCY;
   }

   //check that value is not out of the max frequency, in case it is, set it to min or max directly
   if (valueInF<MIN_FREQUENCY) valueInF=MIN_FREQUENCY;
   if (valueInF>=MAX_FREQUENCY) valueInF=MAX_FREQUENCY;

   //convert the value to inverse logarithmic
   float valueInLF=(log10(valueInF)-minValueL)/scale+minSlider;

   //round the value for the slider
   //round
   int valueInLI = (int) ((valueInLF > 0.0) ? (valueInLF + 0.5) : (valueInLF - 0.5));

   //set the slider to the converted value
   mGUI.frequencySourcesSli->setValue(valueInLI);

   //if edit source mode is active, change the source frequency
   if(userMode==sourceEdit){
     sim.set_scene()->set_sources()->set_sourceByIdx(sourceSelected)->set_freq(valueInF);
   }

   //set the value in case it was corrected
   mGUI.frequencySourcesEdi->setText(QString::number(valueInF));
}

// ThTank::micNameEdiChange()
//
// PURPOSE
//   Actualize the value of the mic name after a change in the mic name edit field ocurred
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::micNameEdiChange(){
   QString valueStrIn=mGUI.nameMicrophonesEdi->text();

   //check if value is correct eliminating all characters which are not letters or numbers
   for (int i=0;i<48;i++) valueStrIn.remove(char(i));
   for (int i=58;i<65;i++) valueStrIn.remove(char(i));
   for (int i=91;i<96;i++) valueStrIn.remove(char(i));
   for (int i=123;i<256;i++) valueStrIn.remove(char(i));

   //check that name is not repeated (if repeated get a new one) and change the source name
   int found=sim.get_scene().get_mics().find_micByName(valueStrIn);
   if(userMode==micEdit){
       if(found!=-1 && found!=micSelected) valueStrIn=sim.get_scene().get_mics().get_newFreeMicName();
       sim.set_scene()->set_mics()->set_micByIdx(micSelected)->set_name(valueStrIn);
   }
   else{
       if(found!=-1 || valueStrIn.length()<=0) valueStrIn=sim.get_scene().get_mics().get_newFreeMicName();
   }

   //set the value in case it was corrected
   mGUI.nameMicrophonesEdi->setText(valueStrIn);

}

// ThTank::micXEdiChange()
//
// PURPOSE
//   Actualize the value of the mic column number after a change in the mic x edit field ocurred
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::micXEdiChange(){
   QString valueStrIn=mGUI.xMicrophonesEdi->text();

   //check if value can be converted
   bool correct=false;
   float value=valueStrIn.toFloat(&correct);
   if(!correct){
       int temp=valueStrIn.toInt(&correct);
       if (correct) value=temp;
       else value=DEFAULT_COL*sim.get_scene().get_delta();
   }

   //convert the value to pixels
   int pixelPos=round(value/sim.get_scene().get_delta());

   //check that value is not out of the max pixels, in case it is, set it to min or max directly
   if (pixelPos<0) pixelPos=0;
   if (pixelPos>=HORIZONTAL_PIXELS) pixelPos=HORIZONTAL_PIXELS-1;

   //check that name is not repeated (if repeated get a new one) and change the source name
   int found=sim.get_scene().get_mics().find_micByPos(round(mGUI.yMicrophonesEdi->text().toFloat()/sim.get_scene().get_delta()),pixelPos);
   if(userMode==micEdit){
       if(found!=-1 && found!=micSelected) pixelPos=sim.get_scene().get_mics().get_newFreeMicPos('E',sim.get_scene().get_mics().get_micByIdx(micSelected).get_row(),pixelPos,VERTICAL_PIXELS,HORIZONTAL_PIXELS).x();
           sim.set_scene()->set_mics()->set_micByIdx(micSelected)->set_col(pixelPos);
   }
   else{
       if(found!=-1) pixelPos=sim.get_scene().get_mics().get_newFreeMicPos('E',round(mGUI.yMicrophonesEdi->text().toFloat()/sim.get_scene().get_delta()),pixelPos,VERTICAL_PIXELS,HORIZONTAL_PIXELS).x();
   }

   //set the value in case it was corrected
   mGUI.xMicrophonesEdi->setText(QString::number(pixelPos*sim.get_scene().get_delta()));

}

// ThTank::micYEdiChange()
//
// PURPOSE
//   Actualize the value of the mic row number after a change in the mic y edit field ocurred
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::micYEdiChange(){
   QString valueStrIn=mGUI.yMicrophonesEdi->text();

   //check if value can be converted
   bool correct=false;
   float value=valueStrIn.toFloat(&correct);
   if(!correct){
       int temp=valueStrIn.toInt(&correct);
       if (correct) value=temp;
       else value=DEFAULT_ROW*sim.get_scene().get_delta();
   }

   //convert the value to pixels
   int pixelPos=round(value/sim.get_scene().get_delta());

   //check that value is not out of the max pixels, in case it is, set it to min or max directly
   if (pixelPos<0) pixelPos=0;
   if (pixelPos>=VERTICAL_PIXELS) pixelPos=VERTICAL_PIXELS-1;

   //check that name is not repeated (if repeated get a new one) and change the source name
   int found=sim.get_scene().get_mics().find_micByPos(pixelPos,round(mGUI.xMicrophonesEdi->text().toFloat()/sim.get_scene().get_delta()));
   if(userMode==micEdit){
       if(found!=-1 && found!=micSelected) pixelPos=sim.get_scene().get_mics().get_newFreeMicPos('S',pixelPos,sim.get_scene().get_mics().get_micByIdx(micSelected).get_col(),VERTICAL_PIXELS,HORIZONTAL_PIXELS).y();
           sim.set_scene()->set_mics()->set_micByIdx(micSelected)->set_row(pixelPos);
   }
   else{
       if(found!=-1) pixelPos=sim.get_scene().get_mics().get_newFreeMicPos('S',pixelPos,round(mGUI.xMicrophonesEdi->text().toFloat()/sim.get_scene().get_delta()),VERTICAL_PIXELS,HORIZONTAL_PIXELS).y();
   }

   //set the value in case it was corrected
   mGUI.yMicrophonesEdi->setText(QString::number(pixelPos*sim.get_scene().get_delta()));
}

// ThTank::reflectionWallsEdiChange()
//
// PURPOSE
//   Actualize the value of the walls reflection coefficient after a change in the text edit field. Also the slider value is actualized.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::reflectionWallsSliChange(){

   int valueIn=mGUI.reflectionWallsSli->value();
   //check value correctness
   if ((valueIn)<mGUI.reflectionWallsSli->minimum() || (valueIn)>mGUI.reflectionWallsSli->maximum()) return;

   //convert the value to MIN_REFLECTION-MAX_REFLECTION
   float value=(MAX_REFLECTION-MIN_REFLECTION)*valueIn/(mGUI.reflectionWallsSli->maximum()-mGUI.reflectionWallsSli->minimum())+MIN_REFLECTION;

   //print the value converted to QString in the edit text field
   mGUI.reflectionWallsEdi->setText(QString::number(value));

   //actualize the reflection value of the walls
   sim.set_scene()->set_refCoef(value);
   //if boundaries closed also
   if (sim.get_fieldClosed()) sim.set_scene()->set_boundaryCoef(value);

   //reset simulation if autoreset is active
   if (autoReset) resetSimulation();

}

// ThTank::reflectionWallsEdiChange()
//
// PURPOSE
//   Actualize the value of the walls reflection coefficient after a change in the text edit field. Also the slider value is actualized.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::reflectionWallsEdiChange(){
    float valueInFConv;
   //get the string in edit field
    QString valueStrIn=mGUI.reflectionWallsEdi->text();

   //check if value can be converted, if not, set it to min
   bool correct=false;
   float valueInF=valueStrIn.toFloat(&correct);
   if(!correct){
       int temp=valueStrIn.toInt(&correct);
       if (correct) valueInF=temp;
       else valueInF=DEFAULT_REFLECTION;
   }

   //check that value is not out of the max pixels, in case it is, set it to min or max directly
   if (valueInF<MIN_REFLECTION) valueInF=MIN_REFLECTION;
   if (valueInF>=MAX_REFLECTION) valueInF=MAX_REFLECTION;

   //convert the value to min_slider-max_slider and round

   valueInFConv=((valueInF-MIN_REFLECTION)/(MAX_REFLECTION-MIN_REFLECTION))*(mGUI.reflectionWallsSli->maximum()-mGUI.reflectionWallsSli->minimum());
   int valueInI = (int) ((valueInFConv > 0.0) ? (valueInFConv + 0.5) : (valueInFConv - 0.5));

   //set the slider to the converted value
   mGUI.reflectionWallsSli->setValue(valueInI);

   //actualize the value of the reflection of the walls
   sim.set_scene()->set_refCoef(valueInF);

   //if boundaries closed also
   if (sim.get_fieldClosed()) sim.set_scene()->set_boundaryCoef(valueInF);

   //set the value in case it was corrected
   mGUI.reflectionWallsEdi->setText(QString::number(valueInF));

   //reset simulation if autoreset is active
   if (autoReset) resetSimulation();

}

// ThTank::deltaSceneSliChange()
//
// PURPOSE
//   Actualize the value of the simulation delta after a change in the slider. Also the text edit field is actualized.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::deltaSceneSliChange(){
   int valueIn=mGUI.deltaSceneSli->value();
   //check value correctness
   if ((valueIn)<mGUI.deltaSceneSli->minimum() || (valueIn)>mGUI.deltaSceneSli->maximum()) return;

   //get min, max and scaling factor for logartihmic work
   float minValue=MIN_DELTA;
   float maxValue=MAX_DELTA;

   int minSlider=mGUI.deltaSceneSli->minimum();
   int maxSlider=mGUI.deltaSceneSli->maximum();

   float minValueL=log10(minValue);
   float maxValueL=log10(maxValue);

   float scale=(maxValueL-minValueL) / (maxSlider-minSlider);

   float valueInLF=pow(10,(minValueL+scale*(valueIn-minSlider)));

   //round to the 3rd decimal
   float valueInLI = roundf(valueInLF * 1000) / 1000.0;

   //print the value converted to QString in the edit text field
   mGUI.deltaSceneEdi->setText(QString::number(valueInLI));

   //set the scene size edit field to the new value
   mGUI.sizeSceneEdi->setText(QString::number(valueInLI*HORIZONTAL_PIXELS));

   //actualize the delta value of the scene
   sim.set_scene()->set_delta(valueInLI);

   //reset the plot
   plotter.close();

   //reset simulation if autoreset is active
   if (autoReset) resetSimulation();

}

// ThTank::deltaSceneEdiChange()
//
// PURPOSE
//   Actualize the value of the simulation delta in the text edit field. Also the slider is actualized
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::deltaSceneEdiChange(){
   //get min, max and scaling factor for logartihmic work
   int minSlider=mGUI.deltaSceneSli->minimum();
   int maxSlider=mGUI.deltaSceneSli->maximum();

   float minValueL=log10(MIN_DELTA);
   float maxValueL=log10(MAX_DELTA);

   float scale=(maxValueL-minValueL) / (maxSlider-minSlider);

   //get the string in edit field
   QString valueStrIn=mGUI.deltaSceneEdi->text();

   //check if value can be converted
   bool correct=false;
   float valueInF=valueStrIn.toFloat(&correct);
   if(!correct){
       int temp=valueStrIn.toInt(&correct);
       if (correct) valueInF=temp;
       else valueInF=DEFAULT_DELTA;
   }

   //check that value is not out of the max pixels, in case it is, set it to min or max directly
   if (valueInF<MIN_DELTA) valueInF=MIN_DELTA;
   if (valueInF>=MAX_DELTA) valueInF=MAX_DELTA;

   //convert the value to inverse logarithmic
   float valueInLF=(log10(valueInF)-minValueL)/scale+minSlider;

   //round
   int valueInLI = (int) ((valueInLF > 0.0) ? (valueInLF + 0.5) : (valueInLF - 0.5));

   //actualize the delta value of the scene
   sim.set_scene()->set_delta(valueInF);

   //set the scene size edit field to the new value
   mGUI.sizeSceneEdi->setText(QString::number(valueInF*HORIZONTAL_PIXELS));

   //set the slider to the converted value
   mGUI.deltaSceneSli->setValue(valueInLI);

   //set the value in case it was corrected
   mGUI.deltaSceneEdi->setText(QString::number(valueInF));

   //reset the plot
   plotter.close();

   if (autoReset) resetSimulation();

}

// ThTank::sizeSceneEdiChange()
//
// PURPOSE
//   Actualize the value of the simulation size in the text edit field. Also the slider of the delta and the delta field are actualized
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::sizeSceneEdiChange(){
    //get the string in edit field
    QString valueStrIn=mGUI.sizeSceneEdi->text();

    //check if value can be converted
    bool correct=false;
    float valueInF=valueStrIn.toFloat(&correct);
    if(!correct){
        int temp=valueStrIn.toInt(&correct);
        if (correct) valueInF=temp;
        else valueInF=DEFAULT_DELTA*HORIZONTAL_PIXELS;
    }

    //check that value is not out of the max pixels, in case it is, set it to min or max directly
    if (valueInF/HORIZONTAL_PIXELS<MIN_DELTA) valueInF=MIN_DELTA*HORIZONTAL_PIXELS;
    if (valueInF/HORIZONTAL_PIXELS>=MAX_DELTA) valueInF=MAX_DELTA*HORIZONTAL_PIXELS;

    //set delta edit, and call delta edit function and that will do the work also for the slider and the setting
    QString valueStr;
    valueStr.setNum(valueInF/HORIZONTAL_PIXELS);
    mGUI.deltaSceneEdi->setText(valueStr);

    //set the value in case it was corrected
    mGUI.sizeSceneEdi->setText(QString::number(valueInF));

    emit mGUI.deltaSceneEdi->editingFinished();
}

// ThTank::phaseSourcesSliChange()
//
// PURPOSE
//   Actualize the value of the source phase change in the slider. Also the edit text field is actualized
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::phaseSourcesSliChange(){
   int valueIn=mGUI.phaseSourcesSli->value();
   //check value correctness
   if ((valueIn)<mGUI.phaseSourcesSli->minimum() || (valueIn)>mGUI.phaseSourcesSli->maximum()) return;

   //scale value to obtain a value between MIN_PHASE and MAX_PHASE
   float valueInF=(MAX_PHASE-MIN_PHASE)*valueIn/(mGUI.phaseSourcesSli->maximum()-mGUI.phaseSourcesSli->minimum())+MIN_PHASE;

   //round
   int valueInLI = (int) ((valueInF > 0.0) ? (valueInF + 0.5) : (valueInF - 0.5));

   //print the value converted to QString in the edit text field
   mGUI.phaseSourcesEdi->setText(QString::number(valueInLI));

   //if edit source mode is active, change the source phase
   if(userMode==sourceEdit){
       sim.set_scene()->set_sources()->set_sourceByIdx(sourceSelected)->set_phase(valueInLI);
   }
}

// ThTank::phaseSourcesEdiChange()
//
// PURPOSE
//   Actualize the value of the source phase change in the text edit field. Also the slider is actualized
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::phaseSourcesEdiChange(){
   //get the string in edit field
   QString valueStrIn=mGUI.phaseSourcesEdi->text();

   //check if value can be converted
   bool correct=false;
   int valueIn=valueStrIn.toInt(&correct);
   if(!correct){
       float temp=valueStrIn.toFloat(&correct);
       if (correct) valueIn=temp;
       else valueIn=DEFAULT_PHASE;
   }

   //check that value is not out of the max pixels, in case it is, set it to min or max directly
   if (valueIn<MIN_PHASE) valueIn=MIN_PHASE;
   if (valueIn>=MAX_PHASE) valueIn=MAX_PHASE;

    //scale value to obtain a value between MIN_PHASE and MAX_PHASE to give to the slider
    float valueInF=((float)(valueIn-MIN_PHASE)/(MAX_PHASE-MIN_PHASE))*(mGUI.phaseSourcesSli->maximum()-mGUI.phaseSourcesSli->minimum());
    //convert the value to slider range

    //round
    int valueInI = (int) ((valueInF > 0.0) ? (valueInF + 0.5) : (valueInF - 0.5));

    //set the slider to the converted value
    mGUI.phaseSourcesSli->setValue(valueInI);

    //if edit source mode is active, change the source phase
    if(userMode==sourceEdit){
        sim.set_scene()->set_sources()->set_sourceByIdx(sourceSelected)->set_phase(valueIn);
    }

    //set the value in case it was corrected
    mGUI.phaseSourcesEdi->setText(QString::number(valueIn));


}

// ThTank::speedSceneSliChange()
//
// PURPOSE
//   SLOT Actualize the value of the simulation speed after a change in the slider. Also the edit text field is actualized
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::speedSimulationSliChange(){
   int valueIn=mGUI.speedSimulationSli->value();
   //check value correctness
   if ((valueIn)<mGUI.speedSimulationSli->minimum() || (valueIn)>mGUI.speedSimulationSli->maximum()) return;

   //scale value to obtain a value between MIN_SPEED and MAX_SPEED
   float valueInF=((MAX_SPEED-MIN_SPEED)*valueIn/(mGUI.speedSimulationSli->maximum()-mGUI.speedSimulationSli->minimum()))+MIN_SPEED;

   //round
   int valueInLI = (int) ((valueInF > 0.0) ? (valueInF + 0.5) : (valueInF - 0.5));

   //print the value converted to QString in the edit text field
   mGUI.speedSimulationEdi->setText(QString::number(valueInLI));

   //actualize the speed value of the scene
   sim.set_speed(valueInLI);
}

// ThTank::speedSceneEdiChange()
//
// PURPOSE
//   SLOT Actualize the value of the simulation speed after a change in the edit text field. Also the slider is actualized
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::speedSimulationEdiChange(){
   //get the string in edit field
   QString valueStrIn=mGUI.speedSimulationEdi->text();

   //check if value can be converted, if not, set it to min
   bool correct=false;
   int valueIn=valueStrIn.toInt(&correct);
   if(!correct){
       float temp=valueStrIn.toFloat(&correct);
       if (correct) valueIn=temp;
       else valueIn=DEFAULT_SPEED;
   }

    //check if converted value is in the range
   //check that value is not out of the max pixels, in case it is, set it to min or max directly
   if (valueIn<MIN_SPEED) valueIn=MIN_SPEED;
   if (valueIn>=MAX_SPEED) valueIn=MAX_SPEED;

    //scale value to obtain a value between MIN_PHASE and MAX_PHASE to give to the slider
    float valueInF=((float)(valueIn-MIN_SPEED)/(MAX_SPEED-MIN_SPEED))*(mGUI.speedSimulationSli->maximum()-mGUI.speedSimulationSli->minimum());
    //convert the value to slider range

    //round
    int valueInI = (int) ((valueInF > 0.0) ? (valueInF + 0.5) : (valueInF - 0.5));

    //set the slider to the converted value
    mGUI.speedSimulationSli->setValue(valueInI);

    //actualize the speed value of the scene
    sim.set_speed(valueIn);

    //set the value in case it was corrected
    mGUI.speedSimulationEdi->setText(QString::number(valueIn));

}

// ThTank::contrastSimulationSliChange()
//
// PURPOSE
//   SLOT Actualize the value of the contrast after a change in the slider. Also the edit text field is actualized
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::contrastSimulationSliChange(){
    int valueIn=mGUI.contrastSimulationSli->value();
    //check value correctness
    if ((valueIn)<mGUI.contrastSimulationSli->minimum() || (valueIn)>mGUI.contrastSimulationSli->maximum()) return;

    //scale value to obtain a value between MIN_CONTRAST and MAX_CONTRAST
    float valueInF=((MAX_CONTRAST-MIN_CONTRAST)*valueIn/(mGUI.contrastSimulationSli->maximum()-mGUI.contrastSimulationSli->minimum()))+MIN_CONTRAST;

    //round
    int valueInLI = (int) ((valueInF > 0.0) ? (valueInF + 0.5) : (valueInF - 0.5));

    //print the value converted to QString in the edit text field
    mGUI.contrastSimulationEdi->setText(QString::number(valueInLI));

    //actualize the speed value of the scene
    sim.set_contrast(valueInLI);
}

// ThTank::contrastSimulationEdiChange()
//
// PURPOSE
//   SLOT Actualize the value of the contrast after a change in the edit text field. Also the slider is actualized
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::contrastSimulationEdiChange(){
   //get the string in edit field
   QString valueStrIn=mGUI.contrastSimulationEdi->text();

   //check if value can be converted, if not, set it to min
   bool correct=false;
   int valueIn=valueStrIn.toInt(&correct);
   if(!correct){
       float temp=valueStrIn.toFloat(&correct);
       if (correct) valueIn=temp;
       else valueIn=DEFAULT_CONTRAST;
   }

    //check if converted value is in the range
   //check that value is not out of the max pixels, in case it is, set it to min or max directly
   if (valueIn<MIN_CONTRAST) valueIn=MIN_CONTRAST;
   if (valueIn>=MAX_CONTRAST) valueIn=MAX_CONTRAST;

    //scale value to obtain a value between MIN_CONTRAST and MAX_CONTRAST to give to the slider
    float valueInF=((float)(valueIn-MIN_CONTRAST)/(MAX_CONTRAST-MIN_CONTRAST))*(mGUI.contrastSimulationSli->maximum()-mGUI.contrastSimulationSli->minimum());
    //convert the value to slider range

    //round
    int valueInI = (int) ((valueInF > 0.0) ? (valueInF + 0.5) : (valueInF - 0.5));

    //set the slider to the converted value
    mGUI.contrastSimulationSli->setValue(valueInI);

    //actualize the speed value of the scene
    sim.set_contrast(valueIn);

    //set the value in case it was corrected
    mGUI.contrastSimulationEdi->setText(QString::number(valueIn));
}

// ThTank::maxSimulationEdiChange()
//
// PURPOSE
//   SLOT Called when the user edits the max simulation step, the maximal step of the simulation possible is set to this value
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::maxSimulationEdiChange(){
    //get the string in edit field
    QString valueStrIn=mGUI.maxSimulationEdi->text();

    //check if value can be converted, if not, set it to min
    bool correct=false;
    int valueIn=valueStrIn.toInt(&correct);
    if(!correct){
        float temp=valueStrIn.toFloat(&correct);
        if (correct) valueIn=temp;
        else valueIn=DEFAULT_MAX_STEP;
    }

    //actualize the speed value of the scene
    sim.set_maxStep(valueIn);

    //set the value in case it was corrected
    mGUI.maxSimulationEdi->setText(QString::number(valueIn));
}

void ThTank::mic_added(){
    //set mode to free
    userModeChanged(free);
    //actualize all the fields to make them correct in case they werent
    mGUI.nameMicrophonesEdi->editingFinished();
    mGUI.xMicrophonesEdi->editingFinished();
    mGUI.yMicrophonesEdi->editingFinished();

    QString name=mGUI.nameMicrophonesEdi->text();
    int row=mGUI.yMicrophonesEdi->text().toFloat()/sim.get_scene().get_delta();
    int col=mGUI.xMicrophonesEdi->text().toFloat()/sim.get_scene().get_delta();

    Mic toAdd=Mic(name,row,col);

    sim.set_scene()->set_mics()->add_mic(toAdd);
}

// ThTank::source_added()
//
// PURPOSE
//   Add a source to the simulation
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::source_added(){
    //set mode to free
    userModeChanged(free);
    //actualize all the fields to make them correct in case they werent
    mGUI.nameSourcesEdi->editingFinished();
    mGUI.xSourcesEdi->editingFinished();
    mGUI.ySourcesEdi->editingFinished();
    mGUI.amplitudeSourcesEdi->editingFinished();
    mGUI.frequencySourcesEdi->editingFinished();
    mGUI.phaseSourcesEdi->editingFinished();

    QString name=mGUI.nameSourcesEdi->text();
    float amplitude=db2pa(mGUI.amplitudeSourcesEdi->text().toInt());
    float phase=mGUI.phaseSourcesEdi->text().toFloat();
    int frequency=mGUI.frequencySourcesEdi->text().toInt();
    Source::SourceType type=Source::SourceType(mGUI.typeSourceShown);
    int row=mGUI.ySourcesEdi->text().toFloat()/sim.get_scene().get_delta();
    int col=mGUI.xSourcesEdi->text().toFloat()/sim.get_scene().get_delta();

    Source toAdd=Source(name,row,col,type,phase,amplitude,frequency);

    sim.set_scene()->set_sources()->add_source(toAdd);

    userModeChanged(free);

    //reset simulation if autoreset is active
    if (autoReset) resetSimulation();

}

// ThTank::wall_addMode()
//
// PURPOSE
//   SLOT Called when the user clicked to add boundaries and userMode is changed to wallAdd mode
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::wall_addMode(){
   userModeChanged(wallAdd);
   shape.clearShapePoints();
   shape.set_shapeHeigth(2);
   shape.set_shapeWidth(2);
   shape.set_shapeType(mGUI.typeWallShown);
}

// ThTank::wall_addMode()
//
// PURPOSE
//   SLOT Called when the user clicked to delete boundaries and userMode is changed to wallDelete mode
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::wall_deleteMode(){
    userModeChanged(wallDelete);
    shape.clearShapePoints();
    shape.set_shapeHeigth(2);
    shape.set_shapeWidth(2);
    shape.set_shapeType(Shape::point);
}

// ThTank::sourcesCleared()
//
// PURPOSE
//   SLOT Called when the user clicked to delete all sources, all sources are cleared
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::sourcesCleared(){
    userModeChanged(free);
    bool simulating=sim.get_simulating();
    sim.set_simulating(false);
    QThread::msleep(20);
    emit dataActualization(true);
    QThread::msleep(20);

    //clear the sources
    sim.set_scene()->set_sources()->clear();

    sim.set_simulating(simulating);
    emit dataActualization(false);
}

// ThTank::wallsCleared()
//
// PURPOSE
//   SLOT Called when the user clicked to delete all boundaries, all boundaries are cleared
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::wallsCleared(){
    userModeChanged(free);
    bool simulating=sim.get_simulating();
    sim.set_simulating(false);
    QThread::msleep(20);
    emit dataActualization(true);
    QThread::msleep(20);

    //set field matrix to 0
    setMatrix2null(sim.field);

    //set field boundaries again
    sim.set_fieldBoundaries();
    sim.set_simulating(simulating);
    emit dataActualization(false);
}

// ThTank::wallsCleared()
//
// PURPOSE
//   SLOT Called when the user clicked to delete all microphones, all microphones are cleared
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void ThTank::microphonesCleared(){
    userModeChanged(free);
    bool simulating=sim.get_simulating();
    sim.set_simulating(false);
    QThread::msleep(20);
    emit dataActualization(true);
    QThread::msleep(20);

    //clear the sources
    sim.set_scene()->set_mics()->clear();

    sim.set_simulating(simulating);
    emit dataActualization(false);
}
