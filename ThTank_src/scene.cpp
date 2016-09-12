//class Scene which contains among others exactly one object of each class Sources, Microphones and WallPoints.
//Each of those three classes can contain 0 or more elements of the classes Source, Microphone and WallPoint and
//have their own methods to add or delete elements from themselves. At the same time, the classes Source, Microphone
//and WallPoint have their own methods and attributes specific for them. The class Scene and each of the classes it
//composes have their own methods for being imported or exported as .xml objects.

#include "scene.h"

//constructor for default scene
Scene::Scene(){
    refCoef=DEFAULT_REFLECTION;
    delta=DEFAULT_DELTA;
    boundaryCoef=DEFAULT_BOUNDARY;
}

//destructors
Scene::~Scene(){
}
void Scene::clear(){
    sources.clear();
    wallPoints.clear();
    mics.clear();
}

//getters
Sources Scene::get_sources() const{return sources;}
Mics Scene::get_mics() const{return mics;}
WallPoints Scene::get_wallPoints() const{return wallPoints;}
float Scene::get_delta() const{return delta;}
float Scene::get_refCoef() const{return refCoef;}
float Scene::get_boundaryCoef() const{return boundaryCoef;}

//setters
Sources* Scene::set_sources(){return &sources;}
Mics* Scene::set_mics(){return &mics;}
WallPoints* Scene::set_wallPoints(){return &wallPoints;}
void Scene::set_sources(Sources sourcesIn){sources=sourcesIn;}
void Scene::set_mics(Mics micsIn){mics=micsIn;}
void Scene::set_wallPoints(WallPoints wallPointsIn){wallPoints=wallPointsIn;}
void Scene::set_delta(float deltaIn){delta=deltaIn;}
void Scene::set_refCoef(float refCoefIn){refCoef=refCoefIn;}
void Scene::set_boundaryCoef(float boundaryCoefIn){boundaryCoef=boundaryCoefIn;}

// Simulation::scene2xml()
//
// PURPOSE
//   Converts the scene itself in an xml element
//
// INPUT
//   NONE
//
// OUTPUT
//   QDomDocument &document - document variable where the scene in saved
//
// RETURN VALUE
//   (bool) true if action was performed correctly
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
bool Scene::scene2xml(QDomDocument &document){

    QDomElement root = document.createElement("scene");

    root.setAttribute("delta",QString::number(delta));
    root.setAttribute("refCoef",QString::number(refCoef));
    root.setAttribute("boundaryCoef",QString::number(boundaryCoef));

    QDomElement sourcesElem=document.createElement("sources");
    QDomElement micsElem=document.createElement("mics");
    QDomElement wallsElem = document.createElement("wallPoints");

    if(!get_wallPoints().wallPoints2xml(get_wallPoints(),document,wallsElem)) return false;
    if(!get_mics().mics2xml(get_mics(),document,micsElem)) return false;
    if(!get_sources().sources2xml(get_sources(),document,sourcesElem)) return false;

    root.appendChild(sourcesElem);
    root.appendChild(micsElem);
    root.appendChild(wallsElem);

    document.appendChild(root);
    return true;

}

// Scene::saveXMLscene()
//
// PURPOSE
//   Takes a scene object, converrts it to xml element with the function (scene2xml) and saves it in an -xml file with a given name including path
//
// INPUT
//   const QString &name - name of the file to be saved (including path)
//
// OUTPUT
// None
//
// RETURN VALUE
//   (bool) true if action was performed correctly
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
bool Scene::saveXMLscene(const QString &name){
       QDomDocument document;

       scene2xml(document);

       QFile file(name);

       if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
           qDebug()<<"error";
           return false;
       }
       else{
           QTextStream stream(&file);
           stream<<document.toString();
           file.close();
           qDebug()<<"saved!";
           return true;
       }

}

// Scene::loadXMLscene()
//
// PURPOSE
//   Takes an xml scene element and converts it to a scene object
//
// INPUT
//   QDomElement sceneNode - xml scene element
//
// OUTPUT
//   Scene &sceneIn - scene object where the scene will be saved
//
// RETURN VALUE
//   (bool) true if action was performed correctly
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
bool Scene::xml2scene(QDomElement sceneNode, Scene &sceneIn){
    bool correct;

    if(sceneNode.tagName()!="scene") return false;

    QDomNamedNodeMap attributes = sceneNode.attributes();
    QDomNodeList items = sceneNode.childNodes();

    if(!attributes.contains("boundaryCoef") || !attributes.contains("refCoef") || !attributes.contains("delta")) return false;

    //check that scene attributes have acceptable values, if not set the default values
    sceneIn.boundaryCoef = attributes.namedItem("boundaryCoef").toAttr().value().toFloat(&correct);
    if(!correct){
        sceneIn.boundaryCoef = attributes.namedItem("boundaryCoef").toAttr().value().toInt(&correct);
        if(!correct) return false;
    }
    sceneIn.refCoef = attributes.namedItem("refCoef").toAttr().value().toFloat(&correct);
    if(!correct){
        sceneIn.refCoef = attributes.namedItem("refCoef").toAttr().value().toInt(&correct);
        if(!correct) return false;
    }
    sceneIn.delta = attributes.namedItem("delta").toAttr().value().toFloat(&correct);
    if(!correct){
        sceneIn.delta = attributes.namedItem("delta").toAttr().value().toInt(&correct);
        if(!correct) return false;
    }

    if (items.count()!=3) return false;
    if(!this->sources.xml2sources(items.at(0).toElement(),sceneIn.sources)) return false;
    if(!this->mics.xml2mics(items.at(1).toElement(),sceneIn.mics)) return false;
    if(!this->wallPoints.xml2wallPoints(items.at(2).toElement(),sceneIn.wallPoints)) return false;

    return true;
}

// Scene::loadXMLscene()
//
// PURPOSE
//   Takes an xml document, reads it and calls the function to convert it in scene object (xml2scene)
//
// INPUT
//   const QString &name - name of the xml file including path
//
// OUTPUT
//   Scene &sceneIn - scene object where the scene will be saved
//
// RETURN VALUE
//   (bool) true if action was performed correctly
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
bool Scene::loadXMLscene(const QString &name, Scene &sceneIn){
       QDomDocument document;

       QFile file(name);

       if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
           qDebug()<<"Failed to open file";
           return false;
       }
       else{
           if(!document.setContent(&file)){
           qDebug()<<"Failed to load document";
           return false;
           }
       }

       QDomElement sceneNode=document.firstChild().toElement();

       file.close();

       if(!this->xml2scene(sceneNode,sceneIn)) return false;

       return true;
}
