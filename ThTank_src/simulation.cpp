// The class Simulation is responsible for the simulation process and has methods for preparing
// the field and performing the TLM algorithm. The class is inhertied from the QObject class in
// order to be able to use the multithreading system of Qt. In order to perform the simulation,
// the class contains among other attributes, one object of the class Scene.

#include "simulation.h"
#include "scene.h"
#include "sources.h"
#include "source.h"
#include <QVector>
#include <QPoint>
#include <QDebug>
#include <math.h>
#include "constants.h"

//constructor
Simulation::Simulation(QObject *parent):
    QObject(parent)
{

}

//destructor
Simulation::~Simulation(){

}

//getters
int Simulation::get_simStep() const {return simStep;}
int Simulation::get_maxStep() const {return maxStep;}
int Simulation::get_speed() const {return simSpeed;}
int Simulation::get_contrast() const {return simContrast;}
bool Simulation::get_simulating() const {return simulating;}
bool Simulation::get_sendData() const {return sendData;}
bool Simulation::get_fieldClosed() const {return fieldClosed;}
Scene Simulation::get_scene() const {return scene;}

//setters
void Simulation::set_simStep(const int &simStepIn){simStep=simStepIn;}
void Simulation::set_maxStep(const int &maxStepIn){maxStep=maxStepIn;}
void Simulation::set_speed(const int &speedIn){simSpeed=speedIn;}
void Simulation::set_contrast(const int &contrastIn){simContrast=contrastIn;}
void Simulation::set_simulating(const bool &simulatingIn){simulating=simulatingIn;}
void Simulation::set_sendData(const bool &sendDataIn){sendData=sendDataIn;}
void Simulation::set_scene(const Scene &sceneIn){scene=sceneIn;}
void Simulation::set_fieldClosed(const bool &fieldClosedIn) {fieldClosed=fieldClosedIn;}
Scene* Simulation::set_scene(){return &scene;}

// Simulation::init()
//
// PURPOSE
//   Simulation is initialized
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
void Simulation::init(){
    init_matrixes();
    fieldClosed=0;
    sendData=true;
}

// Simulation::sources_emision()
//
// PURPOSE
//   The value of the emission is source by source calculated and given to each input matrix
//
// INPUT
//   const float &twoPiValue: value of two times pi
//   const float &soundSpeed: value of the speed of sound
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
void Simulation::sources_emision(){
    for (int idx=0;idx<scene.get_sources().get_nSources();idx++){       //go through all emmitting sources

        int row=scene.get_sources().get_sourceByIdx(idx).get_row();     //get the position of the source
        int col=scene.get_sources().get_sourceByIdx(idx).get_col();

        //get the presure value that the source emits during this simulation step
        float val=scene.get_sources().get_sourceByIdx(idx).generate_newVal(TWO_PI_VALUE,SCALED_SOUND_SPEED,scene.get_delta(),simStep);

        //emitted presure value is added to the already calculated in the last step incident values
        inValsN[row][col]+=val;
        inValsS[row][col]+=val;
        inValsE[row][col]+=val;
        inValsW[row][col]+=val;
    }
}

// Simulation::prepareAndSendMicData()
//
// PURPOSE
//   This function takes the names of the michrophones present in the scene and the value of pressure in their positions and send it as a array with the
//   signal send mic data to the waveform plotting window
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
void Simulation::prepareAndSendMicData(){
    QVector<QString> names(4,"no mic");
    QVector<float> values(4,0);

    for (int idx=0;idx<4;idx++){
        if(idx<scene.get_mics().get_nMics()){
            names[idx]=scene.get_mics().get_micByIdx(idx).get_name();
            values[idx]=presVals[scene.get_mics().get_micByIdx(idx).get_row()][scene.get_mics().get_micByIdx(idx).get_col()];
        }
    }
    emit sendMicData(names,values,getMilisecond());
}

// Simulation::send_update()
//
// PURPOSE
//   This function (SLOT) changes the variable sendData to true
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
void Simulation::send_update(){
    sendData=true;
}

// Simulation::data_actualization()
//
// PURPOSE
//   This function (SLOT) changes the variable dataActualization to the bool value indicated by state
//
// INPUT
//   bool state - value to be given to dataActualization value
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
void Simulation::data_actualization(bool state){
    dataActualization=state;
}

// Simulation::init_matrixes()
//
// PURPOSE
//   Following matrixes are initialized:
//   1-field
//   2-presVals
//   3-outValsN/outValsS/outValsE/outValsW
//   4-inValsN/inValsS/inValsE/inValsW
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
void Simulation::init_matrixes(){
    data = new unsigned char[VERTICAL_PIXELS * HORIZONTAL_PIXELS*4];

   for (int i = 0, j = 0; i < VERTICAL_PIXELS * HORIZONTAL_PIXELS; ++i, j += 4) {
       data[j] = data[j + 2] = data[j + 3] =        // R, G, B
               (unsigned char)(255);
       data[j + 1] = 255;
   }

    // Linear memory allocation
        int* temp0 = new int[VERTICAL_PIXELS * HORIZONTAL_PIXELS]; 
        float* temp1 = new float[VERTICAL_PIXELS * HORIZONTAL_PIXELS];

        float* temp2 = new float[VERTICAL_PIXELS * HORIZONTAL_PIXELS]; // Linear memory allocation

        float* temp3 = new float[VERTICAL_PIXELS * HORIZONTAL_PIXELS];
        float* temp4 = new float[VERTICAL_PIXELS * HORIZONTAL_PIXELS];
        float* temp5 = new float[VERTICAL_PIXELS * HORIZONTAL_PIXELS];
        float* temp6 = new float[VERTICAL_PIXELS * HORIZONTAL_PIXELS];
        float* temp7 = new float[VERTICAL_PIXELS * HORIZONTAL_PIXELS];
        float* temp8 = new float[VERTICAL_PIXELS * HORIZONTAL_PIXELS];
        float* temp9 = new float[VERTICAL_PIXELS * HORIZONTAL_PIXELS];



        // These are the important steps:
        // Allocate the pointers inside the array,
        // which will be used to index the linear memory
        field = new int*[VERTICAL_PIXELS];
        presVals = new float*[VERTICAL_PIXELS];

        outValsN = new float*[VERTICAL_PIXELS];// Allocate the pointers inside the array,
                                               // which will be used to index the linear memory

        outValsS = new float*[VERTICAL_PIXELS];
        outValsE = new float*[VERTICAL_PIXELS];
        outValsW = new float*[VERTICAL_PIXELS];
        inValsN = new float*[VERTICAL_PIXELS];
        inValsS = new float*[VERTICAL_PIXELS];
        inValsE = new float*[VERTICAL_PIXELS];
        inValsW = new float*[VERTICAL_PIXELS];


        // Let the pointers inside the array point to the correct memory addresses

        for (int i = 0; i < VERTICAL_PIXELS; ++i)
        {

            field[i] = (temp0 + i * HORIZONTAL_PIXELS);
            presVals[i] = (temp1 + i * HORIZONTAL_PIXELS);

            outValsN[i] = (temp2 + i * HORIZONTAL_PIXELS); //Pointers inside the array point
                                                           //to the correct memory addresses

            outValsS[i] = (temp3 + i * HORIZONTAL_PIXELS);
            outValsE[i] = (temp4 + i * HORIZONTAL_PIXELS);
            outValsW[i] = (temp5 + i * HORIZONTAL_PIXELS);
            inValsN[i] = (temp6 + i * HORIZONTAL_PIXELS);
            inValsS[i] = (temp7 + i * HORIZONTAL_PIXELS);
            inValsE[i] = (temp8 + i * HORIZONTAL_PIXELS);
            inValsW[i] = (temp9 + i * HORIZONTAL_PIXELS);

        }

        for(int row=0;row<VERTICAL_PIXELS;row++){
            for(int col=0;col<HORIZONTAL_PIXELS;col++){

                field[row][col]=0;
                presVals[row][col]=0.f;

                outValsN[row][col]=0.f; //initialize with 0s

                outValsS[row][col]=0.f;
                outValsE[row][col]=0.f;
                outValsW[row][col]=0.f;
                inValsN[row][col]=0.f;
                inValsS[row][col]=0.f;
                inValsE[row][col]=0.f;
                inValsW[row][col]=0.f;

            }
        }
}

// Simulation::update_simulation()
//
// PURPOSE
//   Updates the simulation one step
//   1-Sources emit
//   2-actualize the output values
//   3-presure values are calculated (inside a loop)
//   4-new incoming values are calculated using the out values(inside a loop)
//   4-simulation step is increased by one
//   5-matrix data(to be plotted) is filled with the scaled presure values
//   6-Updating finished signal is sent
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
void Simulation::update_simulation(){
  while(1){
   if(simStep>=maxStep) simulating=false;
   if (!simulating){
      for (int row = 0; row<VERTICAL_PIXELS;row++){
        for (int col = 0; col<HORIZONTAL_PIXELS;col++){
           presVals[row][col]=(0.5*(outValsN[row][col]+outValsS[row][col]+outValsE[row][col]+outValsW[row][col]));
        }
      }
   }
   else{
    //actualize input values with new source emitted values
    sources_emision();


    for(int row=0;row<VERTICAL_PIXELS;row++){      //go through all the nodes
       for(int col=0;col<HORIZONTAL_PIXELS;col++){

        //calculation of the scattered (reflected) values
        outValsN[row][col]=0.5*(-inValsN[row][col]+inValsS[row][col]+inValsE[row][col]+inValsW[row][col]);
        outValsS[row][col]=0.5*(-inValsS[row][col]+inValsE[row][col]+inValsW[row][col]+inValsN[row][col]);
        outValsE[row][col]=0.5*(-inValsE[row][col]+inValsW[row][col]+inValsN[row][col]+inValsS[row][col]);
        outValsW[row][col]=0.5*(-inValsW[row][col]+inValsN[row][col]+inValsS[row][col]+inValsE[row][col]);
        //calculate total presure values
        presVals[row][col]=(0.5*(outValsN[row][col]+outValsS[row][col]+outValsE[row][col]+outValsW[row][col]));

      }
    }

    //adapt the reflection coefficient
    float adaptedRef=((1+scene.get_refCoef())-sqrt(2)*(1-scene.get_refCoef())) / ((1+scene.get_refCoef())+sqrt(2)*(1-scene.get_refCoef()));

    //calculate input presure values
    for(int row=0;row<VERTICAL_PIXELS;row++){
       for(int col=0;col<HORIZONTAL_PIXELS;col++){
          //calculate input presure values
          switch(field[row][col]){
            case 0: //free field, no neighbours
                inValsN[row][col]=outValsS[row-1][col];
                inValsS[row][col]=outValsN[row+1][col];
                inValsE[row][col]=outValsW[row][col+1];
                inValsW[row][col]=outValsE[row][col-1];
                break;
            case 1: //walls
                inValsN[row][col]=0;
                inValsS[row][col]=0;
                inValsE[row][col]=0;
                inValsW[row][col]=0;
                break;
            case 2: //north obstacle            (1 0 0 0)
                inValsN[row][col]=adaptedRef*outValsN[row][col];
                inValsS[row][col]=outValsN[row+1][col];
                inValsE[row][col]=outValsW[row][col+1];
                inValsW[row][col]=outValsE[row][col-1];
                break;
            case 3: //south obstacle            (0 1 0 0)
                inValsN[row][col]=outValsS[row-1][col];
                inValsS[row][col]=adaptedRef*outValsS[row][col];
                inValsE[row][col]=outValsW[row][col+1];
                inValsW[row][col]=outValsE[row][col-1];
                break;
            case 4: //east obstacle             (0 0 1 0)
                inValsN[row][col]=outValsS[row-1][col];
                inValsS[row][col]=outValsN[row+1][col];
                inValsE[row][col]=adaptedRef*outValsE[row][col];
                inValsW[row][col]=outValsE[row][col-1];
                break;
            case 5: //west obstacle             (0 0 0 1)
                inValsN[row][col]=outValsS[row-1][col];
                inValsS[row][col]=outValsN[row+1][col];
                inValsE[row][col]=outValsW[row][col+1];
                inValsW[row][col]=adaptedRef*outValsW[row][col];
                break;
            case 17: //north field termination
                inValsN[row][col]=scene.get_boundaryCoef()*outValsN[row][col];
                inValsS[row][col]=outValsN[row+1][col];
                inValsE[row][col]=outValsW[row][col+1];
                inValsW[row][col]=outValsE[row][col-1];
                break;
            case 18: //south field termination
                inValsN[row][col]=outValsS[row-1][col];
                inValsS[row][col]=scene.get_boundaryCoef()*outValsS[row][col];
                inValsE[row][col]=outValsW[row][col+1];
                inValsW[row][col]=outValsE[row][col-1];
                break;
            case 19: //east field termination
                inValsN[row][col]=outValsS[row-1][col];
                inValsS[row][col]=outValsN[row+1][col];
                inValsE[row][col]=scene.get_boundaryCoef()*outValsE[row][col];
                inValsW[row][col]=outValsE[row][col-1];
                break;
            case 20: //west field termination
                inValsN[row][col]=outValsS[row-1][col];
                inValsS[row][col]=outValsN[row+1][col];
                inValsE[row][col]=outValsW[row][col+1];
                inValsW[row][col]=scene.get_boundaryCoef()*outValsW[row][col];
                break;
            default: //corners, n>1 walls, walls+boundaries
                update_uncommon(row,col);
                break;
          }
       }       
    }


    simStep++;
    QThread::msleep((10-simSpeed)*20);
  }

   while(!sendData || dataActualization) QThread::msleep(1);
   sendData=false;
   if (simulating) prepareAndSendMicData();
   emit updating_finished();
  }
}

// Simulation::drawPressdata()
//
// PURPOSE
//   This function takes the values in presVals (the matrix containing the pressure values of the node) and converts them to dBSPL and then
//   gives them one value between 1 and 255 (in order to be plotted in RGB). The values are also scaled depending on the contrast value.
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
void Simulation::drawPressdata(){
  float maxAmplitude=get_scene().get_sources().get_maxAmplitude();
  if (maxAmplitude==0) maxAmplitude=lastMaxAmplitude;
  lastMaxAmplitude=maxAmplitude;
  float dbPress;
  float dbPressScaled;
  int minAmplitude=maxAmplitude*(simContrast/100.f);
  float scale=255.f/(maxAmplitude-minAmplitude);
  int k=0;
  for (int row = 0; row<VERTICAL_PIXELS;row++) {
     for (int col = 0; col<HORIZONTAL_PIXELS;col++) {
      if (field[row][col]==1){
          data[k] =0;
          data[k+1]=0;
          data[k+2] =0;
      }
      else{
        dbPress=pa2db(presVals[row][col]);
        dbPressScaled=(dbPress-minAmplitude)*scale;
        if(abs(presVals[row][col])<=0 || dbPressScaled <= minAmplitude){
          data[k] =255;
          data[k+1]=255;
          data[k+2] =255;
        }
        else if(presVals[row][col]>0){
          if(dbPressScaled>255){
           data[k] =0;
           data[k+1]=255;
           data[k+2]=255;
          }
          else{
           data[k] =255;
           data[k+2] =255-dbPressScaled;
           data[k+1]=data[k+2];
          }
        }
        else if(presVals[row][col]<0){
          if(dbPressScaled>255){
           data[k] =255;
           data[k+1]=0;
           data[k+2]=255;
          }
          else{
           data[k] =255-dbPressScaled;
           data[k+1]=data[k];
           data[k+2] =255;
          }
        }
      }
      k+=4;
    }
  }
}

// Simulation::update_uncommon()
//
// PURPOSE
//   Updates the points in the input matrixes which have a not common field value, for example corners (only 4 each step) or points which are neighboured in several sides
//
// INPUT
//   const& int row: row index of the point
//   const& int col: col index of the point
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									06/12	0.9
//
void Simulation::update_uncommon(const int &row, const int &col){
    //adapt the reflection coefficient
    float adaptedRef=((1+scene.get_refCoef())-sqrt(2)*(1-scene.get_refCoef())) / ((1+scene.get_refCoef())+sqrt(2)*(1-scene.get_refCoef()));
    switch(field[row][col]){
    /*
    //2 termination 0 wall (corners)
    */
    case 21: //north+east termination, no wall
        inValsN[row][col]=scene.get_boundaryCoef()*outValsN[row][col];
        inValsS[row][col]=outValsN[row+1][col];
        inValsE[row][col]=scene.get_boundaryCoef()*outValsE[row][col];
        inValsW[row][col]=outValsE[row][col-1];
        break;
    case 22: //north+west termination, no wall
        inValsN[row][col]=scene.get_boundaryCoef()*outValsN[row][col];
        inValsS[row][col]=outValsN[row+1][col];
        inValsE[row][col]=outValsW[row][col+1];
        inValsW[row][col]=scene.get_boundaryCoef()*outValsW[row][col];
        break;
    case 23: //south+east termination, no wall
        inValsN[row][col]=outValsS[row-1][col];
        inValsS[row][col]=scene.get_boundaryCoef()*outValsS[row][col];
        inValsE[row][col]=scene.get_boundaryCoef()*outValsE[row][col];
        inValsW[row][col]=outValsE[row][col-1];
        break;
    case 24: //south+west termination, no wall
        inValsN[row][col]=outValsS[row-1][col];
        inValsS[row][col]=scene.get_boundaryCoef()*outValsS[row][col];
        inValsE[row][col]=outValsW[row][col+1];
        inValsW[row][col]=scene.get_boundaryCoef()*outValsW[row][col];
        break;
    /*
    //0 termination 2 walls
    */
    case 6: //north+east wall       (1 0 1 0)
        inValsN[row][col]=adaptedRef*outValsN[row][col];
        inValsS[row][col]=outValsN[row+1][col];
        inValsE[row][col]=adaptedRef*outValsE[row][col];
        inValsW[row][col]=outValsE[row][col-1];
        break;
    case 7: //south+east wall       (0 1 1 0)
        inValsN[row][col]=outValsS[row-1][col];
        inValsS[row][col]=adaptedRef*outValsS[row][col];
        inValsE[row][col]=adaptedRef*outValsE[row][col];
        inValsW[row][col]=outValsE[row][col-1];
        break;
    case 8: //north+west obstacle       (1 0 0 1)
        inValsN[row][col]=adaptedRef*outValsN[row][col];
        inValsS[row][col]=outValsN[row+1][col];
        inValsE[row][col]=outValsW[row][col+1];
        inValsW[row][col]=adaptedRef*outValsW[row][col];
        break;
    case 9: //south+west wall       (0 1 0 1)
        inValsN[row][col]=outValsS[row-1][col];
        inValsS[row][col]=adaptedRef*outValsS[row][col];
        inValsE[row][col]=outValsW[row][col+1];
        inValsW[row][col]=adaptedRef*outValsW[row][col];
        break;
    case 10: //south+north wall     (1 1 0 0)
        inValsN[row][col]=adaptedRef*outValsN[row][col];
        inValsS[row][col]=adaptedRef*outValsS[row][col];
        inValsE[row][col]=outValsW[row][col+1];
        inValsW[row][col]=outValsE[row][col-1];
        break;
    case 11: //west+east wall       (0 0 1 1)
        inValsN[row][col]=outValsS[row-1][col];
        inValsS[row][col]=outValsN[row+1][col];
        inValsE[row][col]=adaptedRef*outValsE[row][col];
        inValsW[row][col]=adaptedRef*outValsW[row][col];
        break;
    /*
    //0 termination 3 walls
    */
    case 12: //north+east+west wall (1 0 1 1)
        inValsN[row][col]=adaptedRef*outValsN[row][col];
        inValsS[row][col]=outValsN[row+1][col];
        inValsE[row][col]=adaptedRef*outValsE[row][col];
        inValsW[row][col]=adaptedRef*outValsW[row][col];
        break;
    case 13: //south+east+west wall (0 1 1 1)
        inValsN[row][col]=outValsS[row-1][col];
        inValsS[row][col]=adaptedRef*outValsS[row][col];
        inValsE[row][col]=adaptedRef*outValsE[row][col];
        inValsW[row][col]=adaptedRef*outValsW[row][col];
        break;
    case 14: //north+south+west wall(1 0 0 1)
        inValsN[row][col]=adaptedRef*outValsN[row][col];
        inValsS[row][col]=adaptedRef*outValsS[row][col];
        inValsE[row][col]=outValsW[row][col+1];
        inValsW[row][col]=adaptedRef*outValsW[row][col];
        break;
    case 15: //north+south+east wall(1 1 1 0)
        inValsN[row][col]=adaptedRef*outValsN[row][col];
        inValsS[row][col]=adaptedRef*outValsS[row][col];
        inValsE[row][col]=adaptedRef*outValsE[row][col];
        inValsW[row][col]=outValsE[row][col-1];
        break;
    /*
    //0 termination 4 walls
    */
    case 16: //north+west+south+east wall (1 1 1 1)
        inValsN[row][col]=adaptedRef*outValsN[row][col];
        inValsS[row][col]=adaptedRef*outValsS[row][col];
        inValsE[row][col]=adaptedRef*outValsE[row][col];
        inValsW[row][col]=adaptedRef*outValsW[row][col];
        break;

    /*
    //1 termination 1 wall
    */
    case 25: //north termination, east wall
        inValsN[row][col]=scene.get_boundaryCoef()*outValsN[row][col];
        inValsS[row][col]=outValsN[row+1][col];
        inValsE[row][col]=adaptedRef*outValsE[row][col];
        inValsW[row][col]=outValsE[row][col-1];
        break;
    case 26: //north termination, south wall
        inValsN[row][col]=scene.get_boundaryCoef()*outValsN[row][col];
        inValsS[row][col]=adaptedRef*outValsS[row][col];
        inValsE[row][col]=outValsW[row][col+1];
        inValsW[row][col]=outValsE[row][col-1];
        break;
    case 27: //north termination, west wall
        inValsN[row][col]=scene.get_boundaryCoef()*outValsN[row][col];
        inValsS[row][col]=outValsN[row+1][col];
        inValsE[row][col]=outValsW[row][col+1];
        inValsW[row][col]=adaptedRef*outValsW[row][col];
        break;

    case 28: //south termination, west wall
        inValsN[row][col]=outValsS[row-1][col];
        inValsS[row][col]=scene.get_boundaryCoef()*outValsS[row][col];
        inValsE[row][col]=outValsW[row][col+1];
        inValsW[row][col]=adaptedRef*outValsW[row][col];
        break;
    case 29: //south termination, north wall
        inValsN[row][col]=adaptedRef*outValsN[row][col];
        inValsS[row][col]=scene.get_boundaryCoef()*outValsS[row][col];
        inValsE[row][col]=outValsW[row][col+1];
        inValsW[row][col]=outValsE[row][col-1];
        break;
    case 30: //south termination, east wall
        inValsN[row][col]=outValsS[row-1][col];
        inValsS[row][col]=scene.get_boundaryCoef()*outValsS[row][col];
        inValsE[row][col]=adaptedRef*outValsE[row][col];
        inValsW[row][col]=outValsE[row][col-1];
        break;

    case 31: //easttermination, south wall
        inValsN[row][col]=outValsS[row-1][col];
        inValsS[row][col]=adaptedRef*outValsS[row][col];
        inValsE[row][col]=scene.get_boundaryCoef()*outValsE[row][col];
        inValsW[row][col]=outValsE[row][col-1];
        break;
    case 32: //easttermination, west wall
        inValsN[row][col]=outValsS[row-1][col];
        inValsS[row][col]=outValsN[row+1][col];
        inValsE[row][col]=scene.get_boundaryCoef()*outValsE[row][col];
        inValsW[row][col]=adaptedRef*outValsW[row][col];
        break;
    case 33: //easttermination, north wall
        inValsN[row][col]=outValsS[row-1][col];
        inValsS[row][col]=adaptedRef*outValsS[row][col];
        inValsE[row][col]=scene.get_boundaryCoef()*outValsE[row][col];
        inValsW[row][col]=outValsE[row][col-1];
        break;

    case 34: //westtermination, north wall
        inValsN[row][col]=adaptedRef*outValsN[row][col];
        inValsS[row][col]=outValsN[row+1][col];
        inValsE[row][col]=outValsW[row][col+1];
        inValsW[row][col]=scene.get_boundaryCoef()*outValsW[row][col];
        break;
    case 35: //westtermination, east wall
        inValsN[row][col]=outValsS[row-1][col];
        inValsS[row][col]=outValsN[row+1][col];
        inValsE[row][col]=adaptedRef*outValsE[row][col];
        inValsW[row][col]=scene.get_boundaryCoef()*outValsW[row][col];
        break;
    case 36: //westtermination, south wall
        inValsN[row][col]=outValsS[row-1][col];
        inValsS[row][col]=adaptedRef*outValsS[row][col];
        inValsE[row][col]=outValsW[row][col+1];
        inValsW[row][col]=scene.get_boundaryCoef()*outValsW[row][col];
        break;
    /*
    //2termination, 1 wall (corner)
    */
    case 37: //north+easttermination, west wall
        inValsN[row][col]=scene.get_boundaryCoef()*outValsN[row][col];
        inValsS[row][col]=outValsN[row+1][col];
        inValsE[row][col]=scene.get_boundaryCoef()*outValsE[row][col];
        inValsW[row][col]=adaptedRef*outValsW[row][col];
        break;
    case 38: //north+easttermination, south wall
        inValsN[row][col]=scene.get_boundaryCoef()*outValsN[row][col];
        inValsS[row][col]=adaptedRef*outValsS[row][col];
        inValsE[row][col]=scene.get_boundaryCoef()*outValsE[row][col];
        inValsW[row][col]=outValsE[row][col-1];
        break;

    case 39: //north+west termination, east wall
        inValsN[row][col]=scene.get_boundaryCoef()*outValsN[row][col];
        inValsS[row][col]=outValsN[row+1][col];
        inValsE[row][col]=adaptedRef*outValsE[row][col];
        inValsW[row][col]=scene.get_boundaryCoef()*outValsW[row][col];
        break;
    case 40: //north+west termination, south wall
        inValsN[row][col]=scene.get_boundaryCoef()*outValsN[row][col];
        inValsS[row][col]=adaptedRef*outValsS[row][col];
        inValsE[row][col]=outValsW[row][col+1];
        inValsW[row][col]=scene.get_boundaryCoef()*outValsW[row][col];
        break;

    case 41: //south+east termination, north wall
        inValsN[row][col]=adaptedRef*outValsN[row][col];
        inValsS[row][col]=scene.get_boundaryCoef()*outValsS[row][col];
        inValsE[row][col]=scene.get_boundaryCoef()*outValsE[row][col];
        inValsW[row][col]=outValsE[row][col-1];
        break;
    case 42: //south+east termination, west wall
        inValsN[row][col]=outValsS[row-1][col];
        inValsS[row][col]=scene.get_boundaryCoef()*outValsS[row][col];
        inValsE[row][col]=scene.get_boundaryCoef()*outValsE[row][col];
        inValsW[row][col]=adaptedRef*outValsW[row][col];
        break;

    case 43: //south+west termination, north wall
        inValsN[row][col]=adaptedRef*outValsN[row][col];
        inValsS[row][col]=scene.get_boundaryCoef()*outValsS[row][col];
        inValsE[row][col]=outValsW[row][col+1];
        inValsW[row][col]=scene.get_boundaryCoef()*outValsW[row][col];
        break;
    case 44: //south+west termination, east wall
        inValsN[row][col]=outValsS[row-1][col];
        inValsS[row][col]=scene.get_boundaryCoef()*outValsS[row][col];
        inValsE[row][col]=adaptedRef*outValsE[row][col];
        inValsW[row][col]=scene.get_boundaryCoef()*outValsW[row][col];
        break;
    /*
    //1 termination 2 walls
    */
    case 45: //north termination, south+east wall
        inValsN[row][col]=scene.get_boundaryCoef()*outValsN[row][col];
        inValsS[row][col]=adaptedRef*outValsS[row][col];
        inValsE[row][col]=adaptedRef*outValsE[row][col];
        inValsW[row][col]=outValsE[row][col-1];
        break;
    case 46: //north termination, east+west wall
        inValsN[row][col]=scene.get_boundaryCoef()*outValsN[row][col];
        inValsS[row][col]=outValsN[row+1][col];
        inValsE[row][col]=adaptedRef*outValsE[row][col];
        inValsW[row][col]=adaptedRef*outValsW[row][col];
        break;
    case 47: //north termination, south+west wall
        inValsN[row][col]=scene.get_boundaryCoef()*outValsN[row][col];
        inValsS[row][col]=adaptedRef*outValsS[row][col];
        inValsE[row][col]=outValsW[row][col+1];
        inValsW[row][col]=adaptedRef*outValsW[row][col];
        break;

    case 48: //south termination, north+east wall
        inValsN[row][col]=adaptedRef*outValsN[row][col];
        inValsS[row][col]=scene.get_boundaryCoef()*outValsS[row][col];
        inValsE[row][col]=adaptedRef*outValsE[row][col];
        inValsW[row][col]=outValsE[row][col-1];
        break;
    case 49: //south termination, east+west wall
        inValsN[row][col]=outValsS[row-1][col];
        inValsS[row][col]=scene.get_boundaryCoef()*outValsS[row][col];
        inValsE[row][col]=adaptedRef*outValsE[row][col];
        inValsW[row][col]=adaptedRef*outValsW[row][col];
        break;
    case 50: //south termination, north+west wall
        inValsN[row][col]=adaptedRef*outValsN[row][col];
        inValsS[row][col]=scene.get_boundaryCoef()*outValsS[row][col];
        inValsE[row][col]=outValsW[row][col+1];
        inValsW[row][col]=adaptedRef*outValsW[row][col];
        break;

    case 51: //east termination, north+west wall
        inValsN[row][col]=adaptedRef*outValsN[row][col];
        inValsS[row][col]=outValsN[row+1][col];
        inValsE[row][col]=scene.get_boundaryCoef()*outValsE[row][col];
        inValsW[row][col]=adaptedRef*outValsW[row][col];
        break;
    case 52: //east termination, north+south wall
        inValsN[row][col]=adaptedRef*outValsN[row][col];
        inValsS[row][col]=adaptedRef*outValsS[row][col];
        inValsE[row][col]=scene.get_boundaryCoef()*outValsE[row][col];
        inValsW[row][col]=outValsE[row][col-1];
        break;
    case 53: //east termination, south+west wall
        inValsN[row][col]=outValsS[row-1][col];
        inValsS[row][col]=adaptedRef*outValsS[row][col];
        inValsE[row][col]=scene.get_boundaryCoef()*outValsE[row][col];
        inValsW[row][col]=adaptedRef*outValsW[row][col];
        break;

    case 54: //west termination, north+east wall
        inValsN[row][col]=adaptedRef*outValsN[row][col];
        inValsS[row][col]=outValsN[row+1][col];
        inValsE[row][col]=adaptedRef*outValsE[row][col];
        inValsW[row][col]=scene.get_boundaryCoef()*outValsW[row][col];
        break;
    case 55: //west termination, north+south wall
        inValsN[row][col]=adaptedRef*outValsN[row][col];
        inValsS[row][col]=adaptedRef*outValsS[row][col];
        inValsE[row][col]=outValsW[row][col+1];
        inValsW[row][col]=scene.get_boundaryCoef()*outValsW[row][col];
        break;
    case 56: //west termination, south+east wall
        inValsN[row][col]=outValsS[row-1][col];
        inValsS[row][col]=adaptedRef*outValsS[row][col];
        inValsE[row][col]=adaptedRef*outValsE[row][col];
        inValsW[row][col]=scene.get_boundaryCoef()*outValsW[row][col];
        break;
    /*
    //2 boundaries 2 walls (corners)
    */
    case 57://north+east termination, south+west wall
        inValsN[row][col]=scene.get_boundaryCoef()*outValsN[row][col];
        inValsS[row][col]=adaptedRef*outValsS[row][col];
        inValsE[row][col]=scene.get_boundaryCoef()*outValsE[row][col];
        inValsW[row][col]=adaptedRef*outValsW[row][col];
        break;
    case 58://north+west termination, south+east wall
        inValsN[row][col]=scene.get_boundaryCoef()*outValsN[row][col];
        inValsS[row][col]=adaptedRef*outValsS[row][col];
        inValsE[row][col]=adaptedRef*outValsE[row][col];
        inValsW[row][col]=scene.get_boundaryCoef()*outValsW[row][col];
        break;
    case 59://south+east termination, north+west wall
        inValsN[row][col]=adaptedRef*outValsN[row][col];
        inValsS[row][col]=scene.get_boundaryCoef()*outValsS[row][col];
        inValsE[row][col]=scene.get_boundaryCoef()*outValsE[row][col];
        inValsW[row][col]=adaptedRef*outValsW[row][col];
        break;
    case 60://south+west termination, north+east wall
        inValsN[row][col]=adaptedRef*outValsN[row][col];
        inValsS[row][col]=scene.get_boundaryCoef()*outValsS[row][col];
        inValsE[row][col]=adaptedRef*outValsE[row][col];
        inValsW[row][col]=scene.get_boundaryCoef()*outValsW[row][col];
        break;
    /*
    //1 boundaries 3 walls
    */
   case 61://north termination, south+east+west wall
        inValsN[row][col]=scene.get_boundaryCoef()*outValsN[row][col];
        inValsS[row][col]=adaptedRef*outValsS[row][col];
        inValsE[row][col]=adaptedRef*outValsE[row][col];
        inValsW[row][col]=adaptedRef*outValsW[row][col];
        break;
   case 62://south termination, east+north+west wall
        inValsN[row][col]=adaptedRef*outValsN[row][col];
        inValsS[row][col]=scene.get_boundaryCoef()*outValsS[row][col];
        inValsE[row][col]=adaptedRef*outValsE[row][col];
        inValsW[row][col]=adaptedRef*outValsW[row][col];
        break;
   case 63://east termination, south,west,north wall
        inValsN[row][col]=adaptedRef*outValsN[row][col];
        inValsS[row][col]=adaptedRef*outValsS[row][col];
        inValsE[row][col]=scene.get_boundaryCoef()*outValsE[row][col];
        inValsW[row][col]=adaptedRef*outValsW[row][col];
        break;
   case 64://west termination, north,east,south wall
        inValsN[row][col]=adaptedRef*outValsN[row][col];
        inValsS[row][col]=adaptedRef*outValsS[row][col];
        inValsE[row][col]=adaptedRef*outValsE[row][col];
        inValsW[row][col]=scene.get_boundaryCoef()*outValsW[row][col];
        break;
    }
}

// Simulation::set_fieldBoundaries()
//
// PURPOSE
//   A vector containing the points of the field which correspond to the boundaries is sent to the function "dValues", the field values are then actualized
//   with the values corresponding to it's characteristics
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
//
void Simulation::set_fieldBoundaries(){
    QVector<QPoint> boundaryPoints= QVector<QPoint>((2*VERTICAL_PIXELS)+(2*HORIZONTAL_PIXELS)-4);

    for(int i=0;i<VERTICAL_PIXELS;i++){
        boundaryPoints[i].setY(i);
        boundaryPoints[i].setX(0);
        boundaryPoints[i+VERTICAL_PIXELS].setY(i);
        boundaryPoints[i+HORIZONTAL_PIXELS].setX(HORIZONTAL_PIXELS-1);
    }
    for(int i=0;i<HORIZONTAL_PIXELS-2;i++){
        boundaryPoints[i+2*VERTICAL_PIXELS].setY(0);
        boundaryPoints[i+2*VERTICAL_PIXELS].setX(i+1);
        boundaryPoints[i+2*VERTICAL_PIXELS+HORIZONTAL_PIXELS-2].setY(VERTICAL_PIXELS-1);
        boundaryPoints[i+2*VERTICAL_PIXELS+HORIZONTAL_PIXELS-2].setX(i+1);
    }

    actualize_fieldValues(boundaryPoints);
    boundaryPoints.clear();
}

// Simulation::actualize_fieldValues()
//
// PURPOSE
//   The function takes a vector of points (the points of the field which have to be actualized). Each point is checked on the field matrix and actualized (a value is given)
//   ,depending on it's characteristics (boundary, wall, free field....), there are 64 codes. Please check the end of this file to see the codes.
//
//
// INPUT
//   const QVector< QPoint > &pointVector: the vector of points to be actualized in the field matrix.
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
//
void Simulation::actualize_fieldValues(const QVector< QPoint > &pointVector){
    QVector<QPoint>::const_iterator it;
    int neighbours=0;
    int boundaries=0;
    for(it = pointVector.begin(); it != pointVector.end(); it++){
       neighbours=0;
       boundaries=0;
       if (!is_pointWall(it)){
          neighbours=get_PointWallNeighbours(it);
          if(neighbours==0) field[it->y()][it->x()]=0;
          else if(neighbours==5) boundaries=get_PointBoundaries(it);
          else{
              switch(neighbours){
                case 1:
                  if (field[it->y()-1][it->x()]==1) field[it->y()][it->x()]=2;//wall(N)
                  else if (field[it->y()+1][it->x()]==1) field[it->y()][it->x()]=3;//wall(S)
                  else if (field[it->y()][it->x()+1]==1) field[it->y()][it->x()]=4;//wall(E)
                  else field[it->y()][it->x()]=5;//wall(W)
                  break;
                case 2:
                  if (field[it->y()-1][it->x()]==1 &&  field[it->y()][it->x()+1]==1) field[it->y()][it->x()]=6;//wall(N,E)
                  else if (field[it->y()+1][it->x()]==1 &&  field[it->y()][it->x()+1]==1) field[it->y()][it->x()]=7;//wall(S,E)
                  else if (field[it->y()-1][it->x()]==1 &&  field[it->y()][it->x()-1]==1) field[it->y()][it->x()]=8;//wall(N,W)
                  else if (field[it->y()+1][it->x()]==1 &&  field[it->y()][it->x()-1]==1) field[it->y()][it->x()]=9;//wall(S,W)
                  else if (field[it->y()+1][it->x()]==1 &&  field[it->y()-1][it->x()]==1) field[it->y()][it->x()]=10;//wall(S,N)
                  else field[it->y()][it->x()]=11;//wall(W,E)
                  break;
                case 3:
                  if (field[it->y()-1][it->x()]==1 &&  field[it->y()][it->x()+1]==1 && field[it->y()][it->x()-1]==1) field[it->y()][it->x()]=12;//wall(N,W,E)
                  else if (field[it->y()+1][it->x()]==1 &&  field[it->y()][it->x()+1]==1 && field[it->y()][it->x()-1]==1) field[it->y()][it->x()]=13;//wall(S,E,W)
                  else if (field[it->y()-1][it->x()]==1 &&  field[it->y()+1][it->x()]==1 && field[it->y()][it->x()-1]==1) field[it->y()][it->x()]=14;//wall(N,S,W)
                  else field[it->y()][it->x()]=15;//wall(N,S,E)
                  break;
              default:
                  field[it->y()][it->x()]=16;//wall(N,S,E,W)
              }
          }
       }
       if(boundaries!=0){
            switch(boundaries){
              case 1: //one boundary
                if(it->y()==0){ //north
                  if      (field[it->y()+1][it->x()]!=1 &&  field[it->y()][it->x()+1]!=1 && field[it->y()][it->x()-1]!=1) field[it->y()][it->x()]=17;//no wall
                  else if (field[it->y()+1][it->x()]==1 &&  field[it->y()][it->x()+1]!=1 && field[it->y()][it->x()-1]!=1) field[it->y()][it->x()]=26;//wall(S)
                  else if (field[it->y()+1][it->x()]!=1 &&  field[it->y()][it->x()+1]==1 && field[it->y()][it->x()-1]!=1) field[it->y()][it->x()]=25;//wall(E)
                  else if (field[it->y()+1][it->x()]!=1 &&  field[it->y()][it->x()+1]!=1 && field[it->y()][it->x()-1]==1) field[it->y()][it->x()]=27;//wall(W)
                  else if (field[it->y()+1][it->x()]!=1 &&  field[it->y()][it->x()+1]==1 && field[it->y()][it->x()-1]==1) field[it->y()][it->x()]=46;//wall(E,W)
                  else if (field[it->y()+1][it->x()]==1 &&  field[it->y()][it->x()+1]==1 && field[it->y()][it->x()-1]!=1) field[it->y()][it->x()]=45;//wall(S,E)
                  else if (field[it->y()+1][it->x()]==1 &&  field[it->y()][it->x()+1]!=1 && field[it->y()][it->x()-1]==1) field[it->y()][it->x()]=47;//wall(S,W)
                  else field[it->y()][it->x()]=61;//wall(S,E,W)
                }
                else if(it->y()==VERTICAL_PIXELS-1){ //south
                  if      (field[it->y()-1][it->x()]!=1 &&  field[it->y()][it->x()+1]!=1 && field[it->y()][it->x()-1]!=1) field[it->y()][it->x()]=18;//no wall
                  else if (field[it->y()-1][it->x()]==1 &&  field[it->y()][it->x()+1]!=1 && field[it->y()][it->x()-1]!=1) field[it->y()][it->x()]=29;//wall(N)
                  else if (field[it->y()-1][it->x()]!=1 &&  field[it->y()][it->x()+1]==1 && field[it->y()][it->x()-1]!=1) field[it->y()][it->x()]=30;//wall(E)
                  else if (field[it->y()-1][it->x()]!=1 &&  field[it->y()][it->x()+1]!=1 && field[it->y()][it->x()-1]==1) field[it->y()][it->x()]=28;//wall(W)
                  else if (field[it->y()-1][it->x()]!=1 &&  field[it->y()][it->x()+1]==1 && field[it->y()][it->x()-1]==1) field[it->y()][it->x()]=49;//wall(E,W)
                  else if (field[it->y()-1][it->x()]==1 &&  field[it->y()][it->x()+1]==1 && field[it->y()][it->x()-1]!=1) field[it->y()][it->x()]=48;//wall(N,E)
                  else if (field[it->y()-1][it->x()]==1 &&  field[it->y()][it->x()+1]!=1 && field[it->y()][it->x()-1]==1) field[it->y()][it->x()]=50;//wall(N,W)
                  else field[it->y()][it->x()]=62;//wall(N,E,W)
                 }
                else if(it->x()==HORIZONTAL_PIXELS-1){ //east
                  if      (field[it->y()-1][it->x()]!=1 &&  field[it->y()+1][it->x()]!=1 && field[it->y()][it->x()-1]!=1) field[it->y()][it->x()]=19;//no wall
                  else if (field[it->y()-1][it->x()]==1 &&  field[it->y()+1][it->x()]!=1 && field[it->y()][it->x()-1]!=1) field[it->y()][it->x()]=33;//wall(N)
                  else if (field[it->y()-1][it->x()]!=1 &&  field[it->y()+1][it->x()]==1 && field[it->y()][it->x()-1]!=1) field[it->y()][it->x()]=31;//wall(S)
                  else if (field[it->y()-1][it->x()]!=1 &&  field[it->y()+1][it->x()]!=1 && field[it->y()][it->x()-1]==1) field[it->y()][it->x()]=32;//wall(W)
                  else if (field[it->y()-1][it->x()]==1 &&  field[it->y()+1][it->x()]==1 && field[it->y()][it->x()-1]!=1) field[it->y()][it->x()]=52;//wall(N,S)
                  else if (field[it->y()-1][it->x()]==1 &&  field[it->y()+1][it->x()]!=1 && field[it->y()][it->x()-1]==1) field[it->y()][it->x()]=51;//wall(N,W)
                  else if (field[it->y()-1][it->x()]!=1 &&  field[it->y()+1][it->x()]==1 && field[it->y()][it->x()-1]==1) field[it->y()][it->x()]=53;//wall(S,W)
                  else field[it->y()][it->x()]=63;//wall(N,S,W)
                }
                else{ //west
                  if      (field[it->y()-1][it->x()]!=1 &&  field[it->y()+1][it->x()]!=1 && field[it->y()][it->x()+1]!=1) field[it->y()][it->x()]=20;//no wall
                  else if (field[it->y()-1][it->x()]==1 &&  field[it->y()+1][it->x()]!=1 && field[it->y()][it->x()+1]!=1) field[it->y()][it->x()]=34;//wall(N)
                  else if (field[it->y()-1][it->x()]!=1 &&  field[it->y()+1][it->x()]==1 && field[it->y()][it->x()+1]!=1) field[it->y()][it->x()]=36;//wall(S)
                  else if (field[it->y()-1][it->x()]!=1 &&  field[it->y()+1][it->x()]!=1 && field[it->y()][it->x()+1]==1) field[it->y()][it->x()]=35;//wall(E)
                  else if (field[it->y()-1][it->x()]==1 &&  field[it->y()+1][it->x()]==1 && field[it->y()][it->x()+1]!=1) field[it->y()][it->x()]=55;//wall(N,S)
                  else if (field[it->y()-1][it->x()]==1 &&  field[it->y()+1][it->x()]!=1 && field[it->y()][it->x()+1]==1) field[it->y()][it->x()]=54;//wall(N,E)
                  else if (field[it->y()-1][it->x()]!=1 &&  field[it->y()+1][it->x()]==1 && field[it->y()][it->x()+1]==1) field[it->y()][it->x()]=56;//wall(S,E)
                  else field[it->y()][it->x()]=64;//wall(N,S,E)
                }
                break;
              case 2: //two boundaries
                if(it->y()==0 && it->x()==HORIZONTAL_PIXELS-1){ //north east
                  if      (field[it->y()+1][it->x()]!=1 &&  field[it->y()][it->x()-1]!=1) field[it->y()][it->x()]=21;//no wall
                  else if (field[it->y()+1][it->x()]!=1 &&  field[it->y()][it->x()-1]==1) field[it->y()][it->x()]=37;//wall(W)
                  else if (field[it->y()+1][it->x()]==1 &&  field[it->y()][it->x()-1]!=1) field[it->y()][it->x()]=38;//wall(S)
                  else  field[it->y()][it->x()]=57;//wall(W,S)
                }
                else if(it->y()==0 && it->x()==0){ //north west
                  if      (field[it->y()+1][it->x()]!=1 &&  field[it->y()][it->x()+1]!=1) field[it->y()][it->x()]=22;//no wall
                  else if (field[it->y()+1][it->x()]==1 &&  field[it->y()][it->x()+1]!=1) field[it->y()][it->x()]=40;//wall(S)
                  else if (field[it->y()+1][it->x()]!=1 &&  field[it->y()][it->x()+1]==1) field[it->y()][it->x()]=39;//wall(E)
                  else  field[it->y()][it->x()]=58;//wall(S,E)
                }
                else if(it->y()==VERTICAL_PIXELS-1 && it->x()==HORIZONTAL_PIXELS-1){ //south east
                  if      (field[it->y()-1][it->x()]!=1 &&  field[it->y()][it->x()-1]!=1) field[it->y()][it->x()]=23;//no wall
                  else if (field[it->y()-1][it->x()]==1 &&  field[it->y()][it->x()-1]!=1) field[it->y()][it->x()]=41;//wall(N)
                  else if (field[it->y()-1][it->x()]!=1 &&  field[it->y()][it->x()-1]==1) field[it->y()][it->x()]=42;//wall(W)
                  else  field[it->y()][it->x()]=59;//wall(N,W)
                }
                else{ //south west
                  if      (field[it->y()-1][it->x()]!=1 &&  field[it->y()][it->x()+1]!=1) field[it->y()][it->x()]=24;//no wall
                  else if (field[it->y()-1][it->x()]==1 &&  field[it->y()][it->x()+1]!=1) field[it->y()][it->x()]=43;//wall(N)
                  else if (field[it->y()-1][it->x()]!=1 &&  field[it->y()][it->x()+1]==1) field[it->y()][it->x()]=44;//wall(E)
                  else  field[it->y()][it->x()]=60;//wall(N,E)
                }
            }
       }


    }
}

// Simulation::drawPressdata()
//
// PURPOSE
//   This function checks the simulation step and converts it's value, in function of delta, in real time returning the milisecond
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   (float) the value of the step in real time (miliseconds)
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
float Simulation::getMilisecond(){
    return ((simStep/SCALED_SOUND_SPEED)*scene.get_delta())*1000;
}

// Simulation::is_pointWall()
//
// PURPOSE
//   The function indicates if a point of the field is a wall or not
//
//
// INPUT
//   const QPoint *point: the point to check if it is a wall or not
//
// OUTPUT
//   None
//
// RETURN VALUE
//   bool - if the point is a wall 1 is returned, if not a wall 0 is returned
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
//
bool Simulation::is_pointWall(const QPoint *point){
    return (field[point->y()][point->x()]==1);
}

// Simulation::get_PointBoundaries()
//
// PURPOSE
//   The function returns the number of boundaries a point corresponds to
//
//
// INPUT
//   const QPoint *point: the point to check if it is a termination or not
//
// OUTPUT
//   None
//
// RETURN VALUE
//   int - number of boundaries that the point corresponds to
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
//
int Simulation::get_PointBoundaries(const QPoint *point){
    int boundaries=0;
    if (point->y()==0) boundaries++;
    if (point->y()==VERTICAL_PIXELS-1) boundaries++;
    if (point->x()==0) boundaries++;
    if (point->x()==HORIZONTAL_PIXELS-1) boundaries++;
    return boundaries;
}

// Simulation::get_PointWallNeighbours()
//
// PURPOSE
//   The function returns the number wall neighbours a point has
//
//
// INPUT
//   const QPoint *point: the point to check if it has wall neighbours or not
//
// OUTPUT
//   None
//
// RETURN VALUE
//   int - 0 = no neighbours / 1,2,3,4 =1,2,3,4 neighbours / 5 = point is a boundary
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
//
int Simulation::get_PointWallNeighbours(const QPoint *point){
     if (get_PointBoundaries(point)!=0) return 5;
     int wallNeighbours=0;
     if (field[point->y()-1][point->x()]==1) wallNeighbours++;
     if (field[point->y()+1][point->x()]==1) wallNeighbours++;
     if (field[point->y()][point->x()+1]==1) wallNeighbours++;
     if (field[point->y()][point->x()-1]==1) wallNeighbours++;
     return wallNeighbours;
}

// Simulation::set_Points2Value()
//
// PURPOSE
//   Changes the value of the given points of the matrix field to a certain value.
//
//
// INPUT
//   const QVector< QPoint > &pointVector: vector of selected points
//   const int &value: new value of the points
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
//
void Simulation::set_Points2Value(const QVector< QPoint > &pointVector,const int &value){
    QVector<QPoint>::const_iterator it;
    for(it = pointVector.begin(); it != pointVector.end(); it++){
       field[it->y()][it->x()]=value;
    }
}

// Simulation::get_regionBoundary()
//
// PURPOSE
//   Takes a vector of points corresponding to a region an returns the termination points of that region
//
//
// INPUT
//   const QVector<QPoint> &pointVector: vector of the points of the region
//   QVector<QPoint> &boundaryPoints: empty vector of the region termination points to be filled
//
// OUTPUT
//   None
//
// RETURN VALUE
//   QVector<QPoint>: vector of the region termination points
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
//
QVector<QPoint> Simulation::get_regionBoundary(const QVector<QPoint> &pointVector){
    QPoint temp;
    QVector<QPoint> boundaryPoints;
    QVector<QPoint>::const_iterator it;
    for(it = pointVector.begin(); it != pointVector.end(); it++){
       if(it->y()==0 || it->y()==VERTICAL_PIXELS-1 || it->y()==0 || it->x()==HORIZONTAL_PIXELS-1) {
           temp.setY(it->y());
           temp.setX(it->x());
           boundaryPoints.push_back(temp);
       }
       else if(field[it->y()-1][it->x()]!=100 || field[it->y()+1][it->x()]!=100 || field[it->y()][it->x()+1]!=100 || field[it->y()][it->x()-1]!=100){
           temp.setY(it->y());
           temp.setX(it->x());
           boundaryPoints.push_back(temp);
       }
    }
    return boundaryPoints;
}

// Simulation::get_regionNeighbours()
//
// PURPOSE
//   Takes a vector of points corresponding to a region an returns the neighbouring points of that region
//
//
// INPUT
//   const QVector<QPoint> &pointVector: vector of the points of the region
//   QVector<QPoint> &boundaryPoints: empty vector of the region neighbouring points to be filled
//
// OUTPUT
//   None
//
// RETURN VALUE
//   QVector<QPoint>: vector of the region neighbouring points
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
QVector<QPoint> Simulation::get_regionNeighbours(const QVector<QPoint> &pointVector){
    QPoint temp;
    QVector<QPoint> neighbourPoints;
    QVector<QPoint>::const_iterator it;
    for(it = pointVector.begin(); it != pointVector.end(); it++){
        //qDebug()<<QString::number(it->y()).append("/").append(QString::number(it->x()));
        if(it->y()>0){
           if(field[it->y()-1][it->x()]!=100){
             temp.setY(it->y()-1);
             temp.setX(it->x());
             neighbourPoints.push_back(temp);
             //qDebug()<<QString::number(temp.y()).append("/").append(QString::number(temp.x()));
           }
        }
        if(it->y()<VERTICAL_PIXELS-1){
           if (field[it->y()+1][it->x()]!=100){
               temp.setY(it->y()+1);
               temp.setX(it->x());
               neighbourPoints.push_back(temp);
               //qDebug()<<QString::number(temp.y()).append("/").append(QString::number(temp.x()));
           }
        }
        if(it->x()>0){
           if (field[it->y()][it->x()-1]!=100){
               temp.setY(it->y());
               temp.setX(it->x()-1);
               neighbourPoints.push_back(temp);
               //qDebug()<<QString::number(temp.y()).append("/").append(QString::number(temp.x()));
           }
        }
        if(it->x()<HORIZONTAL_PIXELS-1){
           if (field[it->y()][it->x()+1]!=100){
               temp.setY(it->y());
               temp.setX(it->x()+1);
               neighbourPoints.push_back(temp);
               //qDebug()<<QString::number(temp.y()).append("/").append(QString::number(temp.x()));
           }
       }
    }
    return neighbourPoints;
}

// Simulation::add_walls2field()
//
// PURPOSE
//   Add wall points to the field
//
//
// INPUT
//   const QVector< QPoint > &pointVector: vector of wall points to be added
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
void Simulation::add_walls2field(const QVector< QPoint > &pointVector){
    QPoint temp;
    QVector<QPoint> correctedPointVector;
    QVector<QPoint>::const_iterator it;
    //fill new vector with the points which are not already a wall
    for(it = pointVector.begin(); it != pointVector.end(); it++){
       if(field[it->y()][it->x()]!=1){
          temp.setY(it->y());
          temp.setX(it->x());
          correctedPointVector.push_back(temp);
       }
    }

    set_Points2Value(correctedPointVector,100);
    QVector<QPoint> neighbourPoints=get_regionNeighbours(correctedPointVector);
    set_Points2Value(correctedPointVector,1);
    actualize_fieldValues(neighbourPoints);
}

// Simulation::delete_walls2field()
//
// PURPOSE
//   Delete wall points from the field
//
//
// INPUT
//   const QVector< QPoint > &pointVector: vector of wall points to be deleted
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
void Simulation::delete_walls2field(const QVector< QPoint > &pointVector){
    QPoint temp;
    QVector<QPoint> correctedPointVector;
    QVector<QPoint>::const_iterator it;
    //fill new vector with the points which are not already a wall
    for(it = pointVector.begin(); it != pointVector.end(); it++){
       if(field[it->y()][it->x()]!=0){
           temp.setY(it->y());
           temp.setX(it->x());
           correctedPointVector.push_back(temp);
       }
    }
    //
    set_Points2Value(correctedPointVector,100);
    QVector<QPoint> boundaryPoints=get_regionBoundary(correctedPointVector);
    QVector<QPoint> neighbourPoints=get_regionNeighbours(correctedPointVector);
    set_Points2Value(correctedPointVector,0);
    actualize_fieldValues(boundaryPoints);
    actualize_fieldValues(neighbourPoints);
}



/*
 Field matrix codes:

 0- free field(0 wn/0 bn)*             21-0 wn     / 2 bn (N,E)           42-1 wn (W)     / 2 bn (S,E)       63-3 wn (S,W,N) / 1 bn (E)
 1- wall point           *             22-0 wn     / 2 bn (N,W)           43-1 wn (N)     / 2 bn (S,W)       64-3 wn (N,E,S) / 1 bn (W)
 2- 1 wn (N)      / 0 bn *             23-0 wn     / 2 bn (S,E)           44-1 wn (E)     / 2 bn (S,W)
 3- 1 wn (S)      / 0 bn *             24-0 wn     / 2 bn (S,W)           45-2 wn (S,E)   / 1 bn (N)
 4- 1 wn (E)      / 0 bn *             25-1 wn (N) / 1 bn (E)             46-2 wn (E,W)   / 1 bn (N)
 5- 1 wn (W)      / 0 bn *             26-1 wn (N) / 1 bn (S)             47-2 wn (S,W)   / 1 bn (N)
 6- 2 wn (N,E)    / 0 bn               27-1 wn (N) / 1 bn (W)             48-2 wn (N,E)   / 1 bn (S)
 7- 2 wn (S,E)    / 0 bn               28-1 wn (S) / 1 bn (W)             49-2 wn (E,W)   / 1 bn (S)
 8- 2 wn (N,W)    / 0 bn               29-1 wn (S) / 1 bn (N)             50-2 wn (N,W)   / 1 bn (S)
 9- 2 wn (S,W)    / 0 bn               30-1 wn (S) / 1 bn (E)             51-2 wn (N,W)   / 1 bn (E)
 10-2 wn (S,N)    / 0 bn               31-1 wn (E) / 1 bn (S)             52-2 wn (N,S)   / 1 bn (E)
 11-2 wn (E,W)    / 0 bn               32-1 wn (E) / 1 bn (W)             53-2 wn (S,W)   / 1 bn (E)
 12-3 wn (N,E,W)  / 0 bn               33-1 wn (E) / 1 bn (N)             54-2 wn (N,E)   / 1 bn (W)
 13-3 wn (S,E,W)  / 0 bn               34-1 wn (W) / 1 bn (N)             55-2 wn (N,S)   / 1 bn (W)
 14-3 wn (N,S,W)  / 0 bn               35-1 wn (W) / 1 bn (E)             56-2 wn (S,E)   / 1 bn (W)
 15-3 wn (N,S,E)  / 0 bn               36-1 wn (W) / 1 bn (S)             57-2 wn (S,W)   / 2 bn (N,E)
 16-4 wn (N,S,E,W)/ 0 bn               37-1 wn (W) / 2 bn (N,E)           58-2 wn (S,E)   / 2 bn (N,W)
 17-0 wn          / 1 bn (N)*          38-1 wn (S) / 2 bn (N,E)           59-2 wn (N,W)   / 2 bn (S,E)
 18-0 wn          / 1 bn (S)*          39-1 wn (E) / 2 bn (N,W)           60-2 wn (N,E)   / 2 bn (S,W)
 19-0 wn          / 1 bn (E)*          40-1 wn (S) / 2 bn (N,W)           61-3 wn (S,E,W) / 1 bn (N)
 20-0 wn          / 1 bn (W)*          41-1 wn (N) / 2 bn (S,E)           62-3 wn (E,N,W) / 1 bn (S)

 -Format: <CODE>-<n wall neighbours> wn <direction wn> / <n boundaries> bn <direction bn>
 -Directions: N-North/S-South/E-East/W-West
 -Field points with (*) are the most frequent and directly calculated, others are calculated with the function "update_uncommon"

 */
