//This file contains all the constants used in the program

#ifndef CONSTANTS
#define CONSTANTS

#include <QFont>
#include <math.h>
#include <QMetaType>

//for the gui widgets sizes
const int BUT_WIDTH=80;
const int BUT_HEIGTH=20;
const int BUT_SMALL_WIDTH=20;
const int BUT_SMALL_HEIGTH=20;
const int EDI_SMALL_WIDTH=35;
const int EDI_LARGE_WIDTH=100;
const int EDI_HEIGTH=18;
const int SLID_WIDTH=130;
const int SLID_HEIGTH=22;
const int SLID_WIDTH_SMALL=80;
const int COM_WIDTH=90;
const int COM_HEIGTH=22;
const int LAB_HEIGTH=18;
const int LAB_SMALL_WIDTH=30;
const int LAB_LARGE_WIDTH=75;
const int CHECK_WIDTH=75;
const int CHECK_HEIGTH=22;
const int PANELS_WIDTH=280;

//for the sizes of the letters (in windows they appear bigger!)
#ifdef Q_OS_WIN
 const QFont EDI_FONT("Lucida Grande UI", 8);
 const QFont LAB_FONT("Lucida Grande UI", 8);
 const QFont BUT_FONT("Lucida Grande UI", 8);
 const QFont COM_FONT("Lucida Grande UI", 8);
 const QFont CHECK_FONT("Lucida Grande UI", 8);
#else
 const QFont EDI_FONT("Lucida Grande UI", 10);
 const QFont LAB_FONT("Lucida Grande UI", 11);
 const QFont BUT_FONT("Lucida Grande UI", 11);
 const QFont COM_FONT("Lucida Grande UI", 11);
 const QFont CHECK_FONT("Lucida Grande UI", 11);
#endif

//for the waveform plot buffer
const int SIZE_PLOT=600;

//for the calculations
const float SOUND_SPEED=340.f;
const float PI_VALUE=3.1415926;
const float TWO_PI_VALUE=6.2831853;
const float FREE_FIELD_BOUNDARIES=(1-sqrt(2))/(1+sqrt(2));
const float REF_PRESS=0.00002;
const float SCALED_SOUND_SPEED=SOUND_SPEED*sqrt(2);

//max/min frequency, max/min amplitude, max/min phase, max/min delta,
const float MIN_FREQUENCY=1;
const float MAX_FREQUENCY=20000;
const int MIN_PHASE=0;
const int MAX_PHASE=360;
const int MIN_SPEED=1;
const int MAX_SPEED=10;
const float MIN_AMPLITUDE_P=0.00002; //0dB
const float MAX_AMPLITUDE_P=20.f;
const int MIN_AMPLITUDE_DB=20*log10(MIN_AMPLITUDE_P/REF_PRESS);
const int MAX_AMPLITUDE_DB=20*log10(MAX_AMPLITUDE_P/REF_PRESS);
const float MIN_DELTA=0.001;
const float MAX_DELTA=1.f;
const float MIN_REFLECTION=0.f;
const float MAX_REFLECTION=1.f;
const int MIN_SOURCE_TYPE=0;
const int MAX_SOURCE_TYPE=3;
const int MIN_CONTRAST=10; //equivalent to 20%
const int MAX_CONTRAST=60; //equivalent to 80%

//drawing area sizes
const int VERTICAL_PIXELS=700;
const int HORIZONTAL_PIXELS=700;

//default values
const int DEFAULT_SPEED=MAX_SPEED;
const float DEFAULT_AMPLITUDE_P=2;
const int DEFAULT_AMPLITUDE_DB=20*log10(DEFAULT_AMPLITUDE_P/REF_PRESS);
const int DEFAULT_ROW=VERTICAL_PIXELS/2;
const int DEFAULT_COL=HORIZONTAL_PIXELS/2;
const float DEFAULT_REFLECTION=MAX_REFLECTION;
const float DEFAULT_DELTA=0.01;
const float DEFAULT_FREQUENCY=1000.f;
const float DEFAULT_PHASE=0.f;
const float DEFAULT_BOUNDARY=FREE_FIELD_BOUNDARIES;
const int DEFAULT_MAX_STEP=99999;
const int DEFAULT_CONTRAST=40;

//properties of objects
const int COLOR_SOURCES[3]={0, 0, 255};
const int COLOR_MICS[3]={0, 0, 125};
const int WIDTH_SOURCES=10;
const int HEIGHT_SOURCES=10;
const int WIDTH_MICS=10;
const int HEIGHT_MICS=10;

//typedef
typedef QVector<QString> NameArray;
typedef QVector<float> FloatArray;
//Q_DECLARE_METATYPE(NameArray);

#endif // CONSTANTS

