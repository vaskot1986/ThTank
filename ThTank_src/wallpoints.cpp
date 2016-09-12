// The class wallPoints contains the vector with the wallpoints on the scene the number of wallpoints
// and methods relative to them. This class is only used for export/import the scene to .xml file

#include "wallpoints.h"

//constructor
WallPoints::WallPoints(){
    nWallPoints=0;
    wallPointsVector= QVector<WallPoint>(nWallPoints);
}

//destructors
WallPoints::~WallPoints(){
}
void WallPoints::clear(){
    nWallPoints=0;
    wallPointsVector= QVector<WallPoint>(nWallPoints);
}

//getters
int WallPoints::get_nWallPoints() const{return nWallPoints;}
QVector<WallPoint> WallPoints::get_wallPointsVector() const{return wallPointsVector;}

//setters
void WallPoints::set_nWallPoints(int nWallPointsIn) {nWallPoints=nWallPointsIn;}
QVector<WallPoint>* WallPoints::set_wallPointsVector(){return &wallPointsVector;}


// WallPoint::get_wallPointByIdx()
//
// PURPOSE
//   Returns a WallPoint from the WallPoint vector, used when consulting, the WallPoint can not be modified
//
// INPUT
// int searchedIdx - index of the searched WallPoint
//
// OUTPUT
//   None
//
// RETURN VALUE
//   (WallPoint) WallPoint searched
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
WallPoint WallPoints::get_wallPointByIdx(int searchedIdx) const{
    if (searchedIdx>=nWallPoints) throw std::exception();
    return wallPointsVector[searchedIdx];
}

// WallPoints::add_wallPoint()
//
// PURPOSE
//   //Adds a new wallPoint to the wallPoints vector
//
// INPUT
//  WallPoint newWallPoint - WallPoint to be added
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
void WallPoints::add_wallPoint(WallPoint newWallPoint){
    wallPointsVector.push_back(newWallPoint);
    nWallPoints++;
}

// WallPoints::delete_wallPointByIdx()
//
// PURPOSE
//   //Deletes one element with a certain index (Idx) of the wallPoints vector
//
// INPUT
//   int searchedIdx - Index of the wallpoint to be deleted from vector
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
void WallPoints::delete_wallPointByIdx(int searchedIdx){
    if(searchedIdx>=0 && nWallPoints>0){
     wallPointsVector.erase(wallPointsVector.begin()+searchedIdx);
     nWallPoints--;
    }
}

// WallPoints::wallPoint2QPoint()
//
// PURPOSE
//   //creates a vector with wall points in order to add it to the field matrix from the xml file
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   QVector<QPoint>
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
QVector<QPoint> WallPoints::wallPoint2QPoint(){
   QVector<QPoint> temp;
   for(int i=0;i<nWallPoints;i++){
       temp.push_back(QPoint(wallPointsVector[i].get_col(),wallPointsVector[i].get_row())); //adds each wallpoint vector from the object to a new
       //vector of QPoints
   }
   return temp;
}

// WallPoints::xml2wallPoints()
//
// PURPOSE
//   Takes the field matrix of indexes and adds each wallpoint in it to the field
//
// INPUT
//   int** fieldIn - simulation field int matrix
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
void WallPoints::fillWallPoints(int** fieldIn){
   for(int row=0; row<VERTICAL_PIXELS; row++){
     for(int col=0; col<HORIZONTAL_PIXELS; col++){
        if(fieldIn[row][col]==1) {
           add_wallPoint(WallPoint(row,col)); //if there is a boundary in this points, add it to the wall points vector
        }
     }
   }
}

// WallPoints::xml2wallPoints()
//
// PURPOSE
//   Takes an xml mics element and convert it to a WallPoints element object
//
// INPUT
//   QDomElement wallPointsVectorNode - xml WallPoints element
//
// OUTPUT
//   WallPoints &wallPointsVectorIn - WallPoints element where obtained
//
// RETURN VALUE
//   (bool) true if action was performed correctly
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
bool WallPoints::xml2wallPoints(QDomElement wallPointsVectorNode, WallPoints &wallPointsVectorIn){
    WallPoint wallPointTemp;
    QDomElement wallPointNode = wallPointsVectorNode.firstChildElement();
    if(wallPointsVectorNode.tagName()!="wallPoints") return false;
    while(!wallPointNode.isNull()){
       if(wallPointTemp.xml2wallPoint(wallPointNode,wallPointTemp)){
           wallPointsVectorIn.add_wallPoint(wallPointTemp);
       }
       wallPointNode = wallPointNode.nextSiblingElement();
    }
    return true;
}

// WallPoints::wallPoints2xml()
//
// PURPOSE
//   Takes a WallPoints object and convert it to a xml WallPoints element
//
// INPUT
//   const WallPoints &wallPointsVectorIn - WallPoints object to convert to xml element
//   QDomDocument &documentIn - document variable created by the calling function
//
// OUTPUT
//   QDomElement &wallPointsVectorNode - xml WallPoints element where the WallPoints object will be saved
//
// RETURN VALUE
//   (bool) true if action was performed correctly
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
bool WallPoints::wallPoints2xml(const WallPoints &wallPointsVectorIn, QDomDocument &documentIn,QDomElement &wallPointsVectorNode){
    for(int i= 0; i<wallPointsVectorIn.get_nWallPoints(); i++){
        QDomElement wallPoint = documentIn.createElement("wallPoint");
        wallPointsVectorIn.get_wallPointByIdx(i).wallPoint2xml(wallPointsVectorIn.get_wallPointByIdx(i),wallPoint);
        wallPointsVectorNode.appendChild(wallPoint);
    }
    return true;
}
