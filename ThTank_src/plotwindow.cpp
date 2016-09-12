//The class UserInterface is responsible for the main program window. It's arrtibures are the buttons
//, text fields and layouts that the user see. The simulation canvas is a class itself which is inherited
//of the QLabel class.

#include "plotwindow.h"

//constructor, prepares the elements of the window, it's layouts, etc...., and makes its own connections
//used only in for this widget
PlotWindow::PlotWindow(QWidget *parent)
    : QWidget(parent)
{
    actualizing=false;

    setFixedSize(1000,400);

    userPan= new QGroupBox("User");

    micsPan = new QGroupBox("Microphones",userPan);
    mic1But = new QCheckBox("mic1",micsPan);
    mic2But = new QCheckBox("mic2",micsPan);
    mic3But = new QCheckBox("mic3",micsPan);
    mic4But = new QCheckBox("mic4",micsPan);

    plotCanvas=new PlotCanvas();

    optionsPan = new QGroupBox("Options",userPan);
    screenShotBut= new QPushButton("Print waveplot",optionsPan);
    autoRangeBut= new QCheckBox("Auto range",optionsPan);
    yRangeTxt = new QLabel("Range y axis",optionsPan);
    yRangeEdi= new QLineEdit("0",optionsPan);
    yRangeUnitsTxt = new QLabel("Pa",optionsPan);

    QHBoxLayout *lineRange = new QHBoxLayout;

    lineRange->addWidget(yRangeTxt);
    lineRange->addWidget(yRangeEdi);
    lineRange->addWidget(yRangeUnitsTxt);

    optionsButsLayout = new QVBoxLayout;
    optionsButsLayout->addLayout(lineRange);
    optionsButsLayout->addWidget(autoRangeBut);
    optionsButsLayout->addWidget(screenShotBut);

    micButsLayout = new QVBoxLayout;
    micButsLayout->addWidget(mic1But);
    micButsLayout->addWidget(mic2But);
    micButsLayout->addWidget(mic3But);
    micButsLayout->addWidget(mic4But);

    userLayout = new QVBoxLayout;
    userLayout->addWidget(micsPan);
    userLayout->addWidget(optionsPan);

    mic1But->setStyleSheet("QCheckBox { color: blue }");
    mic2But->setStyleSheet("QCheckBox { color: red }");
    mic3But->setStyleSheet("QCheckBox { color: green }");
    mic4But->setStyleSheet("QCheckBox { color: yellow }");

    micsPan->setLayout(micButsLayout);
    optionsPan->setLayout(optionsButsLayout);

    userPan->setLayout(userLayout);

    mainLay = new QGridLayout;
    mainLay->addWidget(plotCanvas,0,1,0,8);
    mainLay->addWidget(userPan,0,9,0,2);

    this->setLayout(mainLay);

    plotCanvas->setTitle("Received Microphon Signals");
    plotCanvas->setCanvasBackground(QColor(Qt::white));

    //plotCanvas->setAttribute(QwtPlotItem::AutoScale, false);
    plotCanvas->axisScaleEngine(QwtPlot::xBottom)->setAttribute(QwtScaleEngine::Floating,true);
    plotCanvas->axisScaleEngine(QwtPlot::yLeft)->setAttribute(QwtScaleEngine::Floating,true);
    plotCanvas->setAutoReplot(false);

    // axis names
    plotCanvas->setAxisTitle(QwtPlot::xBottom,"time (ms)");
    plotCanvas->setAxisTitle(QwtPlot::yLeft,"Pressure (Pa)");

    curve1 = new QwtPlotCurve();
    curve2 = new QwtPlotCurve();
    curve3 = new QwtPlotCurve();
    curve4 = new QwtPlotCurve();

    curve1->setPen(* new QPen(Qt::blue));
    curve2->setPen(* new QPen(Qt::red));
    curve3->setPen(* new QPen(Qt::green));
    curve4->setPen(* new QPen(Qt::yellow));

    //connect slots to button functions
    connect(this->mic1But,SIGNAL(clicked(bool)),this,SLOT(plotMic1(bool)));
    connect(this->mic2But,SIGNAL(clicked(bool)),this,SLOT(plotMic2(bool)));
    connect(this->mic3But,SIGNAL(clicked(bool)),this,SLOT(plotMic3(bool)));
    connect(this->mic4But,SIGNAL(clicked(bool)),this,SLOT(plotMic4(bool)));
    connect(this->autoRangeBut,SIGNAL(clicked(bool)),this,SLOT(autoRangeClicked(bool)));
    connect(this->yRangeEdi,SIGNAL(editingFinished()),this,SLOT(userRangeChanged()));


    //set user range to 1 and autoscale active
    userRange=1;
    autoRangeBut->setCheckState(Qt::Checked);
    yRangeEdi->setText(QString::number(userRange));

}

// PlotWindow::updatePlot()
//
// PURPOSE
//   This function is called each time a simulation step is done, the field pressure values captured by the microphones
//   are received and prepended in the buffer
//
// INPUT
// const NameArray &names - Names of the microphones
// const FloatArray &values - Pressure values captured by the michrophones
// const float &step - simulation step
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
void PlotWindow::updatePlot(const NameArray &names, const FloatArray &values, const float &step){ 
   //if the window is not visible or actualization is being done, quit the function, else go inside
   if(plotVisible && !actualizing){
    //set the text fields with the name of each microphone
    mic1But->setText(names.at(0));
    mic2But->setText(names.at(1));
    mic3But->setText(names.at(2));
    mic4But->setText(names.at(3));
     if(plotValuesMic1.size()>=SIZE_PLOT){ //if the buffer is full...
       //eliminate last value for each mic
       plotValuesMic1.pop_back();
       plotValuesMic2.pop_back();
       plotValuesMic3.pop_back();
       plotValuesMic4.pop_back();
       //prepend new value
       plotValuesMic1.prepend(QPointF(step,values.at(0)));
       plotValuesMic2.prepend(QPointF(step,values.at(1)));
       plotValuesMic3.prepend(QPointF(step,values.at(2)));
       plotValuesMic4.prepend(QPointF(step,values.at(3)));
     }
     else{ //if buffer is not full..
     //prepend the value
     plotValuesMic1.prepend(QPointF(step,values.at(0)));
     plotValuesMic2.prepend(QPointF(step,values.at(1)));
     plotValuesMic3.prepend(QPointF(step,values.at(2)));
     plotValuesMic4.prepend(QPointF(step,values.at(3)));
     }

    //give the buffer to the curve element (for plotting)
    curve1->setSamples(plotValuesMic1);
    curve2->setSamples(plotValuesMic2);
    curve3->setSamples(plotValuesMic3);
    curve4->setSamples(plotValuesMic4);
    //call replot function to plot
    plotCanvas->replot();
  }
}

// PlotWindow::resetPlot()
//
// PURPOSE
//   This function resets the buffers, it gives them size 0 so they can be filled again
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
void PlotWindow::resetPlot(){
   actualizing=true;
   plotValuesMic1.resize(0);
   plotValuesMic2.resize(0);
   plotValuesMic3.resize(0);
   plotValuesMic4.resize(0);
   actualizing=false;
}

// PlotWindow::plotMic1()
//
// PURPOSE
//   SLOT if plotMic1 radio button is clicked, the mic1 values will be plotted, if unclicked they will not
//
// INPUT
//   const bool &active - indicates clicked or unclicked
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
void PlotWindow::plotMic1(const bool &active){
    if(active) curve1->attach(plotCanvas);
    else curve1->detach();
}

// PlotWindow::plotMic2()
//
// PURPOSE
//   SLOT if plotMic2 radio button is clicked, the mic2 values will be plotted, if unclicked they will not
//
// INPUT
//   const bool &active - indicates clicked or unclicked
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
void PlotWindow::plotMic2(const bool &active){
    if(active) curve2->attach(plotCanvas);
    else curve2->detach();
}

// PlotWindow::plotMic3()
//
// PURPOSE
//   SLOT if plotMic3 radio button is clicked, the mic3 values will be plotted, if unclicked they will not
//
// INPUT
//   const bool &active - indicates clicked or unclicked
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
void PlotWindow::plotMic3(const bool &active){
    if(active) curve3->attach(plotCanvas);
    else curve3->detach();
}

// PlotWindow::plotMic4()
//
// PURPOSE
//   SLOT if plotMic4 radio button is clicked, the mic4 values will be plotted, if unclicked they will not
//
// INPUT
//   const bool &active - indicates clicked or unclicked
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
void PlotWindow::plotMic4(const bool &active){
    if(active) curve4->attach(plotCanvas);
    else curve4->detach();
}

// PlotWindow::autoRangeClicked()
//
// PURPOSE
//   SLOT if auto range radio button is clicked, the range of the y axis will be set automatically, if unclicked, will take the one
//   given by user stored in userRange attribute
//
// INPUT
//   const bool &active - indicates clicked or unclicked
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
void PlotWindow::autoRangeClicked(const bool &active){
    if(active){
        plotCanvas->setAxisAutoScale(0,true);
        autoRange=true;
    }
    else{
        plotCanvas->setAxisScale(0,(-1*userRange),userRange,0);
        autoRange=false;
        autoRangeBut->setCheckState(Qt::Unchecked);
    }
}

// PlotWindow::userRangeChanged()
//
// PURPOSE
//   SLOT if user change the value of the range, the new value will be taken in case user range is active, if not
//   the range will remain the same until user change to userRange
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
void PlotWindow::userRangeChanged(){
    QString valueStrIn=yRangeEdi->text();

    //check if value can be converted
    bool correct=false;
    userRange=valueStrIn.toDouble(&correct);
    if(!correct){
        int temp=valueStrIn.toInt(&correct);
        if (correct) userRange=temp;
        else userRange=1;
    }
    autoRangeBut->clicked(false);
    yRangeEdi->setText(QString::number(userRange));
}

// PlotWindow::showEvent()
//
// PURPOSE
//   SLOT Called when the window appears after user click "plot" in main window
//
// INPUT
//   QShowEvent* event - event occurred
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
void PlotWindow::showEvent( QShowEvent* event ) {
    QWidget::showEvent( event );
    this->setVisible(true);
    resetPlot();
    plotVisible=true;
}

// PlotWindow::hideEvent()
//
// PURPOSE
//   SLOT Called when the window is closed
//
// INPUT
//   QHideEvent* event - event ocurred
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
void PlotWindow::hideEvent( QHideEvent* event ) {
    QWidget::hideEvent( event );
    this->setVisible(false);
    resetPlot();
    plotVisible=false;
}
