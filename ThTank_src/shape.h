//The class shape is responsible for drawing the cursor and other shapes in the field such as sources or microphones.
//The cursor may appear with different shapes depending on the action which is being performed (adding a wall, moving a source...).
// Details of methods functionality are found in the .cpp file

#ifndef SHAPE
#define SHAPE

#include <QVector>
#include <QPoint>
#include <QToolTip>
#include <math.h>

class Shape{
public:
    enum ShapeType{
        point=0, rectFull=1, rectEmpty=2, circleFull=3, circleEmpty=4, source=5, mic=6
    };

    Shape();
    Shape(ShapeType);
    Shape(ShapeType shapeTypeIn,int shapeWidthIn, int shapeHeigthIn);
    ~Shape();

    int get_shapeWidth();
    int get_shapeHeigth();
    ShapeType get_shapeType();
    QVector<QPoint> get_shapePoints();

    void set_shapeWidth(int);
    void set_shapeHeigth(int);
    void set_shapeType(ShapeType);
    void set_shapeType(int);
    void set_shapePoints(const QPoint &,const int &,const int &);

    void clearShapePoints();
    void shapeDecrement(char);
    void shapeIncrement(char);

private:
    ShapeType shapeType; //type of the shape from enum ShapeType
    int shapeWidth; //width of the shape
    int shapeHeigth; //height of the shape
    QVector<QPoint> shapePoints; //contains the points in the field that the shapes occupies
};

#endif // SHAPE

