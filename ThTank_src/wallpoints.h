// The class wallPoints contains the vector with the wallpoints on the scene the number of wallpoints
// and methods relative to them. This class is only used for export/import the scene to .xml file
// Details of methods functionality are found in the .cpp file

#ifndef WALLPOINTS_H
#define WALLPOINTS_H

#include <QVector>
#include <QtXml>
#include <QPoint>

#include "wallpoint.h"

class WallPoints
{
public:
    WallPoints();
    ~WallPoints();

    int get_nWallPoints() const;
    QVector<WallPoint> get_wallPointsVector() const;
    WallPoint get_wallPointByIdx(int searchedIdx) const;

    void set_nWallPoints(int);
    QVector<WallPoint>* set_wallPointsVector();

    void add_wallPoint(WallPoint newWallPoint);
    void delete_wallPointByIdx(int searchedIdx);

    QVector<QPoint> wallPoint2QPoint();
    void fillWallPoints(int** fieldIn);

    void clear();
    bool xml2wallPoints(QDomElement wallPointsVectorNode, WallPoints &wallPointsVectorIn);
    bool wallPoints2xml(const WallPoints &wallPointsVectorIn, QDomDocument &documentIn,QDomElement &wallPointsVectorNode);

private:
    int nWallPoints; //number of wallPoints
    QVector<WallPoint> wallPointsVector; //vector containing the wallpoint objects
};

#endif // WALLPOINTS_H
