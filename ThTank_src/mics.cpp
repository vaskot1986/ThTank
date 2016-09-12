// The class mics contains the vector with the mics on the scene the number of mics
// and methods relative to them

#include "mics.h"

using namespace std;

//constructor
Mics::Mics(){
    nMics=0;
    micsVector= QVector<Mic>(nMics);
}

//destructors
Mics::~Mics(){
}
void Mics::clear(){
    nMics=0;
    micsVector= QVector<Mic>(nMics);
}


//getters
int Mics::get_nMics() const{return nMics;}
QVector<Mic> Mics::get_micsVector() const{return micsVector;}

//setters
void Mics::set_nMics(int nMicsIn) {nMics=nMicsIn;}
QVector<Mic>* Mics::set_micsVector(){return &micsVector;}


// Mics::find_micByName()
//
// PURPOSE
//   Looks if a name is used by a mic and returns the index of the mic with that name or -1 if name is free
//
// INPUT
// QString searchedName - name searched
//
// OUTPUT
//   None
//
// RETURN VALUE
//   (int) index of the mic with that name or -1 if no mic found
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
int Mics::find_micByName(QString searchedName) const{
    int idx=-1;
    int count=0;
    bool found=false;
    while(count<nMics && found==false){
      if(micsVector[count].get_name()==searchedName){
          idx=count;
          found=true;
      }
      count++;
     }
    return idx;
}

// Mics::find_micByPos()
//
// PURPOSE
//   Looks if a position is occupied by a mic and returns the index of the mic in the position or -1 if position is free
//
// INPUT
// int row - index of the row
// int col - index of the column
//
// OUTPUT
//   None
//
// RETURN VALUE
//   (int) index of the mic on that position or -1 if no mic found
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
int Mics::find_micByPos(int row,int col) const{
    int idx=-1;
    int count=0;
    bool found=false;
    while(count<nMics && found==false){
      if(micsVector[count].get_row()==row && micsVector[count].get_col()==col){
          idx=count;
          found=true;
      }
      count++;
     }
    return idx;
}

// Mics::get_micByIdx()
//
// PURPOSE
//   Returns a mic from the mics vector, used when consulting, the mic can not be modified
//
// INPUT
// int searchedIdx - index of the searched mic
//
// OUTPUT
//   None
//
// RETURN VALUE
//   (Mic) Mic searched
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
Mic Mics::get_micByIdx(int searchedIdx) const{
    if (searchedIdx>=nMics) throw std::exception();
    return micsVector[searchedIdx];
}

// Mics::set_micByIdx()
//
// PURPOSE
//   //Returns the adress of a certain mic with idx searchedIdx in order to modify it
//
// INPUT
//  int searchedIdx - index of the searched mic element
//
// OUTPUT
//   None
//
// RETURN VALUE
//   (Mic*) pointer to the adress of the mic element to modify
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
Mic* Mics::set_micByIdx(int searchedIdx){
    if (searchedIdx>=nMics) return NULL;
    return &micsVector[searchedIdx];
}

// Simulation::get_newFreeMicName()
//
// PURPOSE
//   //Returns an appropiate default name for a new mic
//
// INPUT
// None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   (QString) Name generated
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
QString Mics::get_newFreeMicName() const{
    int idx=nMics;
    QString candidate="Mic"+QString::number(idx); //gives the source the name mic+ idx corresponding to the number of mics +1
    int existName=find_micByName(candidate);

    while(existName!=-1){ //if name is taken or repeated increment the index
        idx++;
        candidate="Mic"+QString::number(idx);
        existName=find_micByName(candidate);
    }
    return candidate;
}

// Mics::get_newFreeMicPos()
//
// PURPOSE
//   Checks if a position where a mic is going to be added is occupied by another mic, if it is, looks for the next free position in the direction
//   indicated by char opt (N,S,E,W)
//
// INPUT
// char opt - direction of search (N,S,E,W)
// int rowCandidate - row where the source was going to be added
// int colCandidate - col where the source was going to be added
// int rows - total rows of the field
// int cols - total columns of the field
//
// OUTPUT
//   None
//
// RETURN VALUE
//   (QPoint) Found free point where the mic can be finally added
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
QPoint Mics::get_newFreeMicPos(char opt, int rowCandidate, int colCandidate,int rows,int cols) const{
    int existPos=find_micByPos(rowCandidate,colCandidate);
    while(existPos!=-1){ //if the position is occupied go to the next position depending on the direction
        switch(opt){
          case 'N': {
           rowCandidate--;
          }
          break;
          case 'S':{
           rowCandidate++;
          }
          break;
          case 'E':{
           colCandidate++;
          }
          break;
          case 'W':{
           colCandidate--;
          }
          break;
       }
      if(rowCandidate<0) opt='S'; //if more north no possible find a free pos going south
      else if(rowCandidate>=rows) opt='N'; //if more south no possible find a pos going north
      else if(colCandidate<0) opt='E'; //if more west no possible find a pos going east
      else if(colCandidate>=cols) opt='W'; //if more east no possible find a pos going west
      else {existPos=find_micByPos(rowCandidate,colCandidate);}//if we are not in limits, continue the normal search
    }
    return QPoint(colCandidate,rowCandidate);
}

// Mics::add_mic()
//
// PURPOSE
//   //Adds a new mic to the mics vector
//
// INPUT
//  Mic newMic - Mic to be added
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
void Mics::add_mic(Mic newMic){
    micsVector.push_back(newMic);
    nMics++;
}

// Mics::delete_micByName()
//
// PURPOSE
//   //Deletes one element with a certain name of the mics vector
//
// INPUT
//   QString searchedName - Name of the mic to be deleted from vector
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
void Mics::delete_micByName(QString searchedName){
    int idx=find_micByName(searchedName);
    if (idx!=-1) delete_micByIdx(idx);
}

// Mics::delete_micByIdx()
//
// PURPOSE
//   //Deletes one element with a certain index (Idx) of the mics vector
//
// INPUT
//   int searchedIdx - Index of the mic to be deleted from vector
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
void Mics::delete_micByIdx(int searchedIdx){
    if(searchedIdx>=0 && nMics>0){
     micsVector.erase(micsVector.begin()+searchedIdx);
     nMics--;
    }
}

// Mics::xml2mics()
//
// PURPOSE
//   Takes an xml mics element and convert it to a mics element object
//
// INPUT
//   QDomElement micsVectorNode - xml mics element
//
// OUTPUT
//   Mics &micsVectorIn - mics element where obtained
//
// RETURN VALUE
//   (bool) true if action was performed correctly
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
bool Mics::xml2mics(QDomElement micsVectorNode, Mics &micsVectorIn){
    Mic micTemp;
    QDomElement micNode = micsVectorNode.firstChildElement();
    if(micsVectorNode.tagName()!="mics") return false;
    while(!micNode.isNull()){
        if(micTemp.xml2mic(micNode,micTemp)){
           if(micsVectorIn.find_micByName(micTemp.get_name())==-1 && micsVectorIn.find_micByPos(micTemp.get_row(),micTemp.get_col())==-1 ){
             micsVectorIn.add_mic(micTemp);
           }
           micNode = micNode.nextSiblingElement();
        }
    }
    return true;
}

// Mics::mics2xml()
//
// PURPOSE
//   Takes a micss object and convert it to a xml mics element
//
// INPUT
//   const Mics &micsVectorIn - mics object to convert to xml element
//   QDomDocument &documentIn - document variable created by the calling function
//
// OUTPUT
//   QDomElement &micsVectorNode - xml mics element where the mics object will be saved
//
// RETURN VALUE
//   (bool) true if action was performed correctly
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
bool Mics::mics2xml(const Mics &micsVectorIn, QDomDocument &documentIn,QDomElement &micsVectorNode){
    for(int i= 0; i<micsVectorIn.get_nMics(); i++){
        QDomElement mic = documentIn.createElement("mic");
        micsVectorIn.get_micByIdx(i).mic2xml(micsVectorIn.get_micByIdx(i),mic);
        micsVectorNode.appendChild(mic);
    }
    return true;
}
