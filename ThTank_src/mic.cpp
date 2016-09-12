//Class mic contains the attributes and methods of the microphone elements in the scene, the microphones
//are placed in the scene in the positions where the waveform is going to be observed.
// Details of methods functionality are found in the .cpp file

#include "mic.h"

//constructor
Mic::Mic(){}
Mic::Mic(QString nameIn,int rowIn,int colIn):row(rowIn),col(colIn),name(nameIn){}

//destructor
Mic::~Mic(){
}

//getters
QString Mic::get_name() const{ return name; }
int Mic::get_row() const{ return row; }
int Mic::get_col() const{ return col; }

//setters
void Mic::set_name(QString nameIn){ name=nameIn; }
void Mic::set_row(int rowIn){ row=rowIn; }
void Mic::set_col(int colIn){ col=colIn; }


// Mic::xml2mic()
//
// PURPOSE
//   Takes a xml mic element and convert it to a mic object
//
// INPUT
//   QDomElement micNode - xml node element corresponding to the node
//
// OUTPUT
//   Mic &micIn - mic object where the xml node element info will be saved
//
// RETURN VALUE
//   (bool) true if action was performed correctly
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
bool Mic::xml2mic(QDomElement micNode, Mic &micIn){
    bool succes=false;

    QDomNamedNodeMap attributes=micNode.attributes();
    //check if attributes are there, if not correct, return
    if(micNode.tagName()!="mic") return false;
    if(!attributes.contains("name") || !attributes.contains("row") || !attributes.contains("col")) return false;

    //get the values of each attribute, if their are not correct, return
    QString name=attributes.namedItem("name").toAttr().value();
    int row=attributes.namedItem("row").toAttr().value().toInt(&succes); {if(!succes) return false;}
    int col=attributes.namedItem("col").toAttr().value().toInt(&succes); {if(!succes) return false;}

    //check if values of attributes are in the ranges and name is correct, if not, return
    removeBadChars(name);
    if(name.count()<=0) return false;
    if(row<0 || row>=VERTICAL_PIXELS) return false;
    if(col<0 || col>=HORIZONTAL_PIXELS) return false;

    micIn=Mic(name,row,col);
    return true;
}

// Mic::mic2xml()
//
// PURPOSE
//   Takes a mic object and convert it to a xml mic element
//
// INPUT
//   const Mic &micIn - mic object to convert to xml element
//
// OUTPUT
//   QDomElement &micNode - xml mic element where the mic object will be saved
//
// RETURN VALUE
//   (bool) true if action was performed correctly
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
bool Mic::mic2xml(const Mic &micIn, QDomElement &micNode){
        micNode.setAttribute("name",micIn.get_name());
        micNode.setAttribute("row",QString::number(micIn.get_row()));
        micNode.setAttribute("col",QString::number(micIn.get_col()));
        return true;
}
