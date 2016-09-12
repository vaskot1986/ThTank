//class Scene which contains among others exactly one object of each class Sources, Microphones and WallPoints.
//Each of those three classes can contain 0 or more elements of the classes Source, Microphone and WallPoint and
//have their own methods to add or delete elements from themselves. At the same time, the classes Source, Microphone
//and WallPoint have their own methods and attributes specific for them. The class Scene and each of the classes it
//composes have their own methods for being imported or exported as .xml objects.
// Details of methods functionality are found in the .cpp file

#ifndef SCENE_H
#define SCENE_H

#include <QVector>
#include <QPoint>
#include <QtXml>
#include <QString>
#include <QTextStream>
#include <QFile>
#include <QIODevice>

#include "sources.h"
#include "mics.h"
#include "wallpoints.h"
#include "constants.h"

class Scene
{
public:
    Scene();
    ~Scene();

    Sources get_sources() const;
    Mics get_mics() const;
    WallPoints get_wallPoints() const;
    float get_delta() const;
    float get_refCoef() const;
    float get_boundaryCoef() const;

    void set_delta(float);
    void set_boundaryCoef(float);
    void set_refCoef(float);
    Sources* set_sources();
    Mics* set_mics();
    WallPoints* set_wallPoints();
    void set_sources(Sources sourcesIn);
    void set_mics(Mics micsIn);
    void set_wallPoints(WallPoints wallPointsIn);  

    bool saveXMLscene(const QString &name);
    bool loadXMLscene(const QString &name, Scene &sceneIn);
    bool xml2scene(QDomElement sceneNode, Scene &sceneIn);
    bool scene2xml(QDomDocument &document);

    void clear();


private:
   Sources sources; //sources object containing the sources of the scene
   Mics mics; //mics object containing the mics of the scene
   WallPoints wallPoints; //wallpoint object containing the wallPoints of the scene, used only during import/export to .xml
   float delta; //delta value of the scene
   float refCoef; //refflection coefficient of the scene
   float boundaryCoef; //refflection coefficient of the grid termination
};

#endif // SCENE_H
