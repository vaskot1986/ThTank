//The class UserInterface is responsible for the main program window. It's arrtibures are the buttons
//, text fields and layouts that the user see. The simulation canvas is a class itself which is inherited
//of the QLabel class.
// Details of methods functionality are found in the .cpp file


#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QWidget>
#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_plot_layout.h>
#include <qgridlayout.h>
#include <qwt_plot_grid.h>
#include <QLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QRadioButton>
#include <qwt_plot_curve.h>
#include <QThread>
#include <QVector>
#include <QtCore/qmath.h>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <qwt_scale_engine.h>
#include <constants.h>
#include <plotcanvas.h>


class PlotWindow: public QWidget
{
    Q_OBJECT
private:
public:
    QGridLayout *mainLay;
    QVBoxLayout *micButsLayout;
    QVBoxLayout *optionsButsLayout;
    QVBoxLayout *userLayout;
    QGroupBox *userPan;
    QGroupBox *micsPan;
    QGroupBox *optionsPan;
    PlotCanvas *plotCanvas;
    QLayout *mainWIndow;
    QCheckBox *mic1But;
    QCheckBox *mic2But;
    QCheckBox *mic3But;
    QCheckBox *mic4But;
    QPushButton *screenShotBut;
    QCheckBox *autoRangeBut;
    QLabel *yRangeTxt;
    QLineEdit *yRangeEdi;
    QLabel *yRangeUnitsTxt;

    QwtPlotCurve *curve1; //curve object for plotting the first microphone values
    QwtPlotCurve *curve2; //curve object for plotting the second microphone values
    QwtPlotCurve *curve3; //curve object for plotting the third microphone values
    QwtPlotCurve *curve4; //curve object for plotting the fourth microphone values
    QVector<QPointF> plotValuesMic1; //vector containing the values to be plotted for the first waveform
    QVector<QPointF> plotValuesMic2; //vector containing the values to be plotted for the second waveform
    QVector<QPointF> plotValuesMic3; //vector containing the values to be plotted for the third waveform
    QVector<QPointF> plotValuesMic4; //vector containing the values to be plotted for the fourth waveform
    bool actualizing; //indicates that the plotting canvas is under modification
    bool plotVisible; //indicates if the window is been activated
    bool working; //indicates that the plotting canvas is under modification
    bool autoRange; //indicates if the range is automatic or has been set by the user
    double userRange; //contains the range given by the user

    PlotWindow(QWidget *parent = 0);
    void resetPlot();
    void showEvent( QShowEvent* event );
    void hideEvent( QHideEvent* event );

public slots:
    void updatePlot(const NameArray &names,const FloatArray &values,const float &step);
    void plotMic1(const bool &active);
    void plotMic2(const bool &active);
    void plotMic3(const bool &active);
    void plotMic4(const bool &active);
    void autoRangeClicked(const bool &active);
    void userRangeChanged();
};

#endif // PLOTWINDOW_H
