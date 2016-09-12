//This class corresponds to the canvas of the waveform plot

#ifndef PLOTCANVAS
#define PLOTCANVAS

#include <qwt_plot.h>

class PlotCanvas : public QwtPlot
{
    public:
    PlotCanvas( QWidget *parent=0) : QwtPlot(parent){}
};

#endif // PLOTCANVAS

