//Class source contains the attributes and methods of the source elements in the scene, the source
//are placed in the scene and emit presure values depending on their configuration and type

#include "source.h"

using namespace std;

//constructor
Source::Source(){}
Source::Source(QString nameIn,int rowIn,int colIn,SourceType typeIn,int phaseIn,float ampIn,int freqIn):
     name(nameIn),row(rowIn),col(colIn),type(typeIn),phase(phaseIn),amp(ampIn),freq(freqIn){}

//destructor
Source::~Source(){
}

//getters
QString Source::get_name() const{ return name; }
int Source::get_row() const{ return row; }
int Source::get_col() const{ return col; }
Source::SourceType Source::get_type() const{ return type; }
int Source::get_type_int() const{
    switch (type){
       case sinus:
        return 0;
        break;
       case delta:
        return 1;
        break;
       case sweep:
        return 2;
        break;
       case gauss:
        return 3;
        break;
       default:
        return 0;
     }
}
int Source::get_phase() const{ return phase; }
float Source::get_amp() const{ return amp; }
float Source::get_freq() const{ return freq; }

//setters
void Source::set_name(QString nameIn){ name= nameIn; }
void Source::set_row(int rowIn){ row= rowIn; }
void Source::set_col(int colIn){ col= colIn; }
void Source::set_type(SourceType typeIn){ type= typeIn; }
void Source::set_phase(int phaseIn){ phase = phaseIn; }
void Source::set_amp(float ampIn){ amp = ampIn; }
void Source::set_freq(float freqIn){ freq= freqIn; }
void Source::set_type(int typeIn){
    switch (typeIn){
       case 0: type=Source::sinus;
        break;
       case 1: type=Source::delta;
        break;
       case 2: type=Source::sweep;
        break;
       case 3: type=Source::gauss;
        break;
       default: type=Source::sinus;
    }
}

// Source::xml2source()
//
// PURPOSE
//   gives the pressure value in a certain simulation step emitted by a source
//
// INPUT
//  const float &twoPiValue - value of 2 times Pi for faster calculation
//  const float &soundSpeed - speed of sound
//  const float &simDelta - delta value of the simulation
//  const float &simStep - current simulation step
//
// OUTPUT
//   None
//
// RETURN VALUE
//   (float) pressure value emmited by the source
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
float Source::generate_newVal(const float &twoPiValue,const float &soundSpeed,const float &simDelta,const float &simStep) const{
     float value=0;
     switch (type){ //depending on the type of source the value will be calculated diferently
     case sinus: //classical sinus source
         value=amp*sin(twoPiValue*freq*simStep*(simDelta/soundSpeed)+phase*(PI_VALUE)/180);
         break;
     case delta: //delta is a source with value equal to amplitude in simulation step 0 and amplitude 0 the rest of steps
         if(simStep==0) value=amp;
         break;
     case sweep:{ //sweep source starts with the frequency of the source and it is increased by 3 hz every simulation step,
         //when reaches a frequency with a wavelength equal to 10 times the delta, it stops the emission (if not stopped, problems with sampling
         // frequency due to the TLM limitations would appear)
         float sweepFreq=simStep+freq+3;
         if(SOUND_SPEED/(sweepFreq)>10*simDelta && sweepFreq<=MAX_FREQUENCY  && sweepFreq>=MIN_FREQUENCY) value=amp*sin(twoPiValue*sweepFreq*simStep*(simDelta/soundSpeed)+phase*(PI_VALUE)/180);
         else value=0;
     }
         break;
     case gauss: //gaussian like source, taken from the NTNU matlab ripple tank simulator
         value=amp*exp(-pow(PI_VALUE,2) * pow((freq*simStep*(simDelta/soundSpeed)-1),2));
         break;
     default:
         value=-1;
         break;
     }
     return value;
}

// Source::xml2source()
//
// PURPOSE
//   Takes a xml source element and convert it to a source object
//
// INPUT
//   QDomElement sourceNode - xml node element corresponding to the source node
//
// OUTPUT
//   Source &sourceIn - source object where the xml node element info will be saved
//
// RETURN VALUE
//   (bool) true if action was performed correctly
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
bool Source::xml2source(QDomElement sourceNode, Source &sourceIn){
    bool succes=false;

    QDomNamedNodeMap attributes=sourceNode.attributes();
    //check if attributes are there, if not correct, return
    if(sourceNode.tagName()!="source") return false;
    if(!attributes.contains("amp") || !attributes.contains("row") || !attributes.contains("col")) return false;
    if(!attributes.contains("type") || !attributes.contains("freq") || !attributes.contains("phase") || !attributes.contains("name")) {qDebug()<<"hm"; return false;}

    //get the values of each attribute, if their are not correct, return
    QString name=attributes.namedItem("name").toAttr().value();
    int row=attributes.namedItem("row").toAttr().value().toInt(&succes); {if(!succes) return false;}
    int col=attributes.namedItem("col").toAttr().value().toInt(&succes); {if(!succes) return false;}
    int freq=attributes.namedItem("freq").toAttr().value().toInt(&succes); {if(!succes) return false;}
    float phase=attributes.namedItem("phase").toAttr().value().toFloat(&succes);
        if(!succes){
         phase=attributes.namedItem("phase").toAttr().value().toInt(&succes);
         if(!succes) return false;
        }
    float amp=attributes.namedItem("amp").toAttr().value().toFloat(&succes);
        if(!succes){
         amp=attributes.namedItem("amp").toAttr().value().toInt(&succes);
         if(!succes) return false;
        }
    int type=attributes.namedItem("type").toAttr().value().toInt(&succes); {if(!succes) return false;}

    //check if values of attributes are in the ranges and name is correct, if not, return
    removeBadChars(name);
    if(name.count()<=0) return false;
    if(row<0 || row>=VERTICAL_PIXELS) return false;
    if(col<0 || col>=HORIZONTAL_PIXELS) return false;
    if(freq<MIN_FREQUENCY || freq>MAX_FREQUENCY) return false;
    if(amp<MIN_AMPLITUDE_P || amp>MAX_AMPLITUDE_P) return false;
    if(phase<MIN_PHASE || phase>MAX_PHASE) return false;
    if(type<MIN_SOURCE_TYPE || type>MAX_SOURCE_TYPE) return false;

    sourceIn=Source(name,row,col,Source::sinus,phase,amp,freq);
    sourceIn.set_type(type);
    return true;
}

// Source::source2xml()
//
// PURPOSE
//   Takes a source object and convert it to a xml source element
//
// INPUT
//   const Source &sourceIn - source object to convert to xml element
//
// OUTPUT
//   QDomElement &sourceNode - xml source element where the source object will be saved
//
// RETURN VALUE
//   (bool) true if action was performed correctly
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
bool Source::source2xml(const Source &sourceIn, QDomElement &sourceNode){
        sourceNode.setAttribute("name",sourceIn.get_name());
        sourceNode.setAttribute("row",QString::number(sourceIn.get_row()));
        sourceNode.setAttribute("col",QString::number(sourceIn.get_col()));
        sourceNode.setAttribute("type",QString::number(sourceIn.get_type()));
        sourceNode.setAttribute("phase",QString::number(sourceIn.get_phase()));
        sourceNode.setAttribute("amp",QString::number(sourceIn.get_amp()));
        sourceNode.setAttribute("freq",QString::number(sourceIn.get_freq()));
        return true;
}
