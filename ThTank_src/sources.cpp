// The class sources contains the vector with the sources on the scene the number of sources
// and methods relative to them

#include "sources.h"

//constructor
Sources::Sources(){
    nSources=0;
    sourcesVector= QVector<Source>(nSources);
}

//destructors
Sources::~Sources(){
}
void Sources::clear(){
    nSources=0;
    sourcesVector= QVector<Source>(nSources);
}

//getters
int Sources::get_nSources() const{return nSources;}
QVector<Source> Sources::get_sourcesVector() const{return sourcesVector;}

//setters
void Sources::set_nSources(int nSourcesIn){nSources=nSourcesIn;}
QVector<Source>* Sources::set_sourcesVector(){return &sourcesVector;}

// Sources::find_sourceByName()
//
// PURPOSE
//   Looks if a name is used by a source and returns the index of the source with that name or -1 if name is free
//
// INPUT
// QString searchedName - name searched
//
// OUTPUT
//   None
//
// RETURN VALUE
//   (int) index of the source with that name or -1 if no source found
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
int Sources::find_sourceByName(QString searchedName) const{
    int idx=-1;
    int count=0;
    bool found=false;
    while(count<nSources && found==false){
      if(sourcesVector[count].get_name()==searchedName){
          idx=count;
          found=true;
      }
      count++;
     }
    return idx;
}

// Sources::find_sourceByPos()
//
// PURPOSE
//   Looks if a position is occupied by a source and returns the index of the source in the position or -1 if position is free
//
// INPUT
// int row - index of the row
// int col - index of the column
//
// OUTPUT
//   None
//
// RETURN VALUE
//   (int) index of the source on that position or -1 if no source found
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
int Sources::find_sourceByPos(int row,int col) const{
    int idx=-1;
    int count=0;
    bool found=false;
    while(count<nSources && found==false){
      if(sourcesVector[count].get_row()==row && sourcesVector[count].get_col()==col){
          idx=count;
          found=true;
      }
      count++;
     }
    return idx;
}

// Simulation::get_sourceByIdx()
//
// PURPOSE
//   Returns a source from the sources vector, used when consulting, the source can not be modified
//
// INPUT
// int searchedIdx - index of the searched source
//
// OUTPUT
//   None
//
// RETURN VALUE
//   (Source) Source searched
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
Source Sources::get_sourceByIdx(int searchedIdx) const{
    if (searchedIdx>=nSources) throw std::exception();
    return sourcesVector[searchedIdx];
}

// Sources::get_newFreeSourcePos()
//
// PURPOSE
//   Checks if a position where a source is going to be added is occupied by another source, if it is, looks for the next free position in the direction
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
//   (QPoint) Found free point where the source can be finally added
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
QPoint Sources::get_newFreeSourcePos(char opt, int rowCandidate, int colCandidate,int rows,int cols) const{
    int existPos=find_sourceByPos(rowCandidate,colCandidate);
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
      else {existPos=find_sourceByPos(rowCandidate,colCandidate);}//if we are not in limits, continue the normal search
    }
    return QPoint(colCandidate,rowCandidate);
}

// Sources::set_sourceByIdx()
//
// PURPOSE
//   //Returns the adress of a certain mic with idx searchedIdx in order to modify it
//
// INPUT
//  int searchedIdx - index of the searched source element
//
// OUTPUT
//   None
//
// RETURN VALUE
//   (Mic*) pointer to the adress of the source element to modify
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
Source* Sources::set_sourceByIdx(int searchedIdx){
    if (searchedIdx>=nSources) return NULL;
    return &sourcesVector[searchedIdx];
}



// Sources::get_newFreeSourceName()
//
// PURPOSE
//   //Returns an appropiate default name for a new source
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
QString Sources::get_newFreeSourceName() const{
    int idx=nSources;
    QString candidate="Source"+QString::number(idx); //gives the source the name source+ idx corresponding to the number of sources +1
    int existName=find_sourceByName(candidate);

    while(existName!=-1){  //if name is taken or repeated increment the index
        idx++;
        candidate="Source"+QString::number(idx);
        existName=find_sourceByName(candidate);
    }
    return candidate;
}

// Sources::add_source()
//
// PURPOSE
//   //Adds a new source to the sources vector
//
// INPUT
//  Source newSource - Source to be added
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
void Sources::add_source(Source newSource){
    sourcesVector.push_back(newSource);
    nSources++;
}

// Sources::delete_sourceByName()
//
// PURPOSE
//   //Deletes one element with a certain name of the sources vector
//
// INPUT
//   QString searchedName) - Name of the source to be deleted from vector
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
void Sources::delete_sourceByName(QString searchedName){
    int idx=find_sourceByName(searchedName);
    if (idx!=-1) delete_sourceByIdx(idx);
}

// Sources::delete_sourceByIdx()
//
// PURPOSE
//   //Deletes one element with a certain index (Idx) of the sources vector
//
// INPUT
//   int searchedIdx - Index of the source to be deleted from vector
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
void Sources::delete_sourceByIdx(int searchedIdx){
    if(searchedIdx>=0 && nSources>0){
     sourcesVector.erase(sourcesVector.begin()+searchedIdx);
     nSources--;
    }
}

// Sources::get_maxAmplitude()
//
// PURPOSE
//   //Returns the amplitude corresponding to the source having the maximal amplitude value
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   (float) amplitude value of the source having the maximal amplitude
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
float Sources::get_maxAmplitude(){
    float maxAmplitude=REF_PRESS;
    for(int i=0;i<nSources;i++){
        if(this->get_sourceByIdx(i).get_amp()>maxAmplitude) maxAmplitude=this->get_sourceByIdx(i).get_amp();
    }
    return 20*log10(maxAmplitude/REF_PRESS);
}

// Sources::xml2sources()
//
// PURPOSE
//   Takes an xml sources element and convert it to a sources element object
//
// INPUT
//   QDomElement sourcesVectorNode - xml sources element
//
// OUTPUT
//   Sources &sourcesVectorIn - sources element where obtained
//
// RETURN VALUE
//   (bool) true if action was performed correctly
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
bool Sources::xml2sources(QDomElement sourcesVectorNode, Sources &sourcesVectorIn){
    Source sourceTemp;
    QDomElement sourceNode = sourcesVectorNode.firstChildElement();
    if(sourcesVectorNode.tagName()!="sources") return false;
    while(!sourceNode.isNull()){
        if(sourceTemp.xml2source(sourceNode,sourceTemp)){
           if(sourcesVectorIn.find_sourceByName(sourceTemp.get_name())==-1 && sourcesVectorIn.find_sourceByPos(sourceTemp.get_row(),sourceTemp.get_col())==-1 ){
             sourcesVectorIn.add_source(sourceTemp);
           }
        }
        sourceNode = sourceNode.nextSiblingElement();
    }
    return true;
}

// Sources::sources2xml()
//
// PURPOSE
//   Takes a sources object and convert it to a xml sources element
//
// INPUT
//   const Sources &sourcesVectorIn - sources object to convert to xml element
//   QDomDocument &documentIn - document variable created by the calling function
//
// OUTPUT
//   QDomElement &sourcesVectorNode - xml sources element where the sources object will be saved
//
// RETURN VALUE
//   (bool) true if action was performed correctly
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
bool Sources::sources2xml(const Sources &sourcesVectorIn, QDomDocument &documentIn,QDomElement &sourcesVectorNode){
    for(int i= 0; i<sourcesVectorIn.get_nSources(); i++){
        QDomElement source = documentIn.createElement("source");
        sourcesVectorIn.get_sourceByIdx(i).source2xml(sourcesVectorIn.get_sourceByIdx(i),source);
        sourcesVectorNode.appendChild(source);
    }
    return true;
}
