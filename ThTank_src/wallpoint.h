//Class WallPoint is created to export and import the boundaries from the simulation
//field to an xml file where they can be saved
// Details of methods functionality are found in the .cpp file

#ifndef WALLPOINT_H
#define WALLPOINT_H

#include <QtXml>

#include "constants.h"

class WallPoint
{
public:
    WallPoint();
    WallPoint(int,int);
    ~WallPoint();
    int get_row() const;
    int get_col() const;
    void set_row(int);
    void set_col(int);
    bool xml2wallPoint(QDomElement wallPointNode, WallPoint &wallPointIn);
    bool wallPoint2xml(const WallPoint &wallPointIn, QDomElement &wallPointNode);

private:
    int row; //row number in pixel of the wallPoint
    int col; //column number in pixel of the wallPoint
};

#endif // WALLPOINT_H
