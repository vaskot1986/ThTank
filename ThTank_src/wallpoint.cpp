//Class WallPoint is created to export and import the boundaries from the simulation
//field to an xml file where they can be saved

#include "wallpoint.h"

//constructor
WallPoint::WallPoint(){}
WallPoint::WallPoint(int rowIn,int colIn):row(rowIn),col(colIn){}

//destructor
WallPoint::~WallPoint(){
}

//getters
int WallPoint::get_row() const{ return row; }
int WallPoint::get_col() const{ return col; }

//setters
void WallPoint::set_row(int rowIn){ row=rowIn; }
void WallPoint::set_col(int colIn){ col=colIn; }

// WallPoint::xml2wallPoint()
//
// PURPOSE
//   Takes a xml wallpoint element and convert it to a wallPoint object
//
// INPUT
//   QDomElement wallPointNode - xml node element corresponding to the wallPoint node
//
// OUTPUT
//   WallPoint &wallPointIn - WallPoint object where the xml node element info will be saved
//
// RETURN VALUE
//   (bool) true if action was performed correctly
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
bool WallPoint::xml2wallPoint(QDomElement wallPointNode, WallPoint &wallPointIn){
    bool succes=false;

    QDomNamedNodeMap attributes=wallPointNode.attributes();
    //check if attributes are there, if not correct, return
    if(wallPointNode.tagName()!="wallPoint") return false;
    if(!attributes.contains("row") || !attributes.contains("col")) return false;

    //get the values of each attribute, if their are not correct, return
    int row=attributes.namedItem("row").toAttr().value().toInt(&succes); {if(!succes) return false;}
    int col=attributes.namedItem("col").toAttr().value().toInt(&succes); {if(!succes) return false;}

    //check if values of attributes are in the ranges, if not, return
    if(row<0 || row>=VERTICAL_PIXELS) return false;
    if(col<0 || col>=HORIZONTAL_PIXELS) return false;

    wallPointIn=WallPoint(row,col);
    return true;
}

// WallPoint::wallPoint2xml()
//
// PURPOSE
//   Takes a WallPoint object and convert it to a xml WallPoint element
//
// INPUT
//   const WallPoint &wallPointIn - WallPoint object to convert to xml element
//
// OUTPUT
//   QDomElement &wallPointNode - xml wallPoint element where the wallPoint object will be saved
//
// RETURN VALUE
//   (bool) true if action was performed correctly
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
bool WallPoint::wallPoint2xml(const WallPoint &wallPointIn, QDomElement &wallPointNode){
    wallPointNode.setAttribute("row",QString::number(wallPointIn.get_row()));
    wallPointNode.setAttribute("col",QString::number(wallPointIn.get_col()));
    return true;
}
