//The class shape is responsible for drawing the cursor and other shapes in the field such as sources or microphones.
//The cursor may appear with different shapes depending on the action which is being performed (adding a wall, moving a source...).

#include "shape.h"

//constructors
Shape::Shape(ShapeType shapeTypeIn){
  shapeType=shapeTypeIn;
  shapeWidth=1;
  shapeHeigth=1;
}
Shape::Shape(){shapeType=point;}

Shape::Shape(ShapeType shapeTypeIn,int shapeWidthIn, int shapeHeigthIn){shapeType=shapeTypeIn; shapeWidth=shapeWidthIn; shapeHeigth=shapeHeigthIn;}

//destructors
Shape::~Shape(){
}
void Shape::clearShapePoints(){
    shapePoints.clear();
    shapePoints=QVector<QPoint>(0);
}

//getters
int Shape::get_shapeWidth(){return shapeWidth;}
int Shape::get_shapeHeigth(){return shapeHeigth;}
Shape::ShapeType Shape::get_shapeType(){return shapeType;}
QVector<QPoint> Shape::get_shapePoints(){return shapePoints;}

//setters
void Shape::set_shapeWidth(int shapeWidthIn){shapeWidth=shapeWidthIn;}
void Shape::set_shapeHeigth(int shapeHeigthIn){shapeHeigth=shapeHeigthIn;}
void Shape::set_shapeType(ShapeType shapeTypeIn){shapeType = shapeTypeIn;}
void Shape::set_shapeType(int shapeTypeIn){
    switch (shapeTypeIn){
       case 0: shapeType=Shape::point;
        break;
       case 1: shapeType=Shape::rectFull;
        break;
       case 2: shapeType=Shape::rectEmpty;
        break;
       case 3: shapeType=Shape::circleFull;
        break;
       case 4: shapeType=Shape::circleEmpty;
        break;
       case 5: shapeType=Shape::source;
        break;
       case 6: shapeType=Shape::mic;
        break;
    }
}

// Shape::set_shapePoints()
//
// PURPOSE
//   takes a shape and in function of it's type, position and size, fills the vector inside the shape object with the points that the shape occupies,
//   in order to be printed on the field
//
// INPUT
//   const QPoint &center - center position of the shape
//   const int &sceneHeigth -heigth of the scene (to avoid go out of margins)
//   const int &sceneWidth -width of the scene (to avoid going out of margins)
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
void Shape::set_shapePoints(const QPoint &center,const int &sceneHeigth,const int &sceneWidth){
    shapePoints.clear();
    shapePoints=QVector<QPoint>(0);
    switch (shapeType){
    case rectFull:
    case point: //if shape is a point(square or filled rectangle), jsust return the points of a square in funcion of width, heigth and center, if out of range points, discard them
      for(int row=center.y()-shapeHeigth;row<center.y()+shapeHeigth;row++){
         for(int col=center.x()-shapeWidth;col<center.x()+shapeWidth;col++){
            if(row>=0 && row<sceneHeigth && col>=0 && col<sceneWidth) shapePoints.push_back(QPoint(col,row));
         }
      }
      break;
    case rectEmpty: //if shape is a rectangle empty, jsust return the points of a rectangle borders in funcion of size and center, if out of range points, discard them
      for(int row=center.y()-shapeHeigth;row<center.y()+shapeHeigth;row++){
          if(row>=0 && row<sceneHeigth && center.x()-shapeWidth>=0 && center.x()-shapeWidth<sceneWidth) shapePoints.push_back(QPoint(center.x()-shapeWidth,row));
          if(row>=0 && row<sceneHeigth && center.x()+shapeWidth>=0 && center.x()+shapeWidth<sceneWidth) shapePoints.push_back(QPoint(center.x()+shapeWidth,row));
      }
      for(int col=center.x()-shapeWidth;col<center.x()+shapeWidth;col++){
          if(center.y()-shapeHeigth>=0 && center.y()-shapeHeigth<sceneHeigth && col>=0 && col<sceneWidth) shapePoints.push_back(QPoint(col,center.y()-shapeHeigth));
          if(center.y()+shapeHeigth>=0 && center.y()+shapeHeigth<sceneHeigth && col>=0 && col<sceneWidth) shapePoints.push_back(QPoint(col,center.y()+shapeHeigth));
      }
      break;
    case circleFull: //if shape is a full circle jsust return the points of a circle in funcion of diameter and center, if out of range points, discard them
        for(int row=center.y()-shapeHeigth;row<center.y()+shapeHeigth;row++){
          for(int col=center.x()-shapeWidth;col<center.x()+shapeWidth;col++){
            if ( (pow(center.x()-col,2) + pow(center.y()-row,2)) <= pow(shapeHeigth,2)-4 )
            {
               if(row>=0 && row<sceneHeigth && col>=0 && col<sceneWidth) shapePoints.push_back(QPoint(col,row));
            }
          }
        }
      break;
    case circleEmpty:{ //if shape is an empty circle it gets a little tricky and a function is used, if out of range points, discard them
        int x = shapeHeigth;
        int y = 0;
        int decisionOver2 = 1 - x;   // Decision criterion divided by 2 evaluated at x=r, y=0
        while( y <= x ){
          if(y + center.y()>=0 && y + center.y()<sceneHeigth && x + center.x()>=0 && x + center.x()<sceneWidth)
            shapePoints.push_back(QPoint( x + center.x(),  y + center.y())); // Octant 1
          if(x + center.y()>=0 && x + center.y()<sceneHeigth && y + center.x()>=0 && y + center.x()<sceneWidth)
            shapePoints.push_back(QPoint( y + center.x(),  x + center.y())); // Octant 2
          if(y + center.y()>=0 && y + center.y()<sceneHeigth && -x + center.x()>=0 && -x + center.x()<sceneWidth)
            shapePoints.push_back(QPoint(-x + center.x(),  y + center.y())); // Octant 4
          if(x + center.y()>=0 && x + center.y()<sceneHeigth && -y + center.x()>=0 && -y + center.x()<sceneWidth)
            shapePoints.push_back(QPoint(-y + center.x(),  x + center.y())); // Octant 3
          if(-y + center.y()>=0 && -y + center.y()<sceneHeigth && -x + center.x()>=0 && -x + center.x()<sceneWidth)
            shapePoints.push_back(QPoint(-x + center.x(), -y + center.y())); // Octant 5
          if(-x + center.y()>=0 && -x + center.y()<sceneHeigth && -y + center.x()>=0 && -y + center.x()<sceneWidth)
            shapePoints.push_back(QPoint(-y + center.x(), -x + center.y())); // Octant 6
          if(-y + center.y()>=0 && -y + center.y()<sceneHeigth && x + center.x()>=0 && x + center.x()<sceneWidth)
            shapePoints.push_back(QPoint( x + center.x(), -y + center.y())); // Octant 7
          if(-x + center.y()>=0 && -x + center.y()<sceneHeigth && y + center.x()>=0 && y + center.x()<sceneWidth)
            shapePoints.push_back(QPoint( y + center.x(), -x + center.y())); // Octant 8
          y++;
          if (decisionOver2<=0) decisionOver2 += 2 * y + 1;   // Change in decision criterion for y -> y+1

          else{
            x--;
            decisionOver2 += 2 * (y - x) + 1;   // Change for y -> y+1, x -> x-1
          }
        }
      }
      break;
    case source: //if shape is a source, return a cross like shape from the given center
        for(int row=center.y()-shapeHeigth;row<center.y()+shapeHeigth;row++){
           if(row>=0 && row<sceneHeigth && center.x()>=center.x() && center.x()<sceneWidth) shapePoints.push_back(QPoint(center.x(),row));
        }
        for(int col=center.x()-shapeWidth;col<center.x()+shapeWidth;col++){
           if(center.y()>=0 && center.y()<sceneHeigth && col>=0 && col<sceneWidth) shapePoints.push_back(QPoint(col,center.y()));
        }
        break;
    case mic: //if shape is a mic, return a cross like shape from the given center
        for(int row=center.y()-shapeHeigth;row<center.y()+shapeHeigth;row++){
           if(row>=0 && row<sceneHeigth && center.x()>=center.x() && center.x()<sceneWidth) shapePoints.push_back(QPoint(center.x(),row));
        }
        for(int col=center.x()-shapeWidth;col<center.x()+shapeWidth;col++){
           if(center.y()>=0 && center.y()<sceneHeigth && col>=0 && col<sceneWidth) shapePoints.push_back(QPoint(col,center.y()));
        }
        break;
    }
}

// Shape::shapeIncrement()
//
// PURPOSE
//   Makes the shape one point bigger (verticalls or horizontally) depending on opt
//
// INPUT
//   char opt - direction of increment (h-horizontal or v-vertical)
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
void Shape::shapeIncrement(char opt){
    switch(opt){
    case 'h':
      shapeWidth++;
      break;
    case 'v':
      shapeHeigth++;
      break;
    default:
      shapeWidth++;
      shapeHeigth++;
      break;
    }
}

// Shape::shapeDecrement()
//
// PURPOSE
//   Makes the shape one point smaller (verticalls or horizontally) depending on opt
//
// INPUT
//   char opt - direction of decrement (h-horizontal or v-vertical)
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
void Shape::shapeDecrement(char opt){
    switch (opt){
    case 'h':
      if(shapeWidth>1) shapeWidth--;
      break;
    case 'v':
      if(shapeHeigth>1) shapeHeigth--;
      break;
    default:
      if(shapeWidth>1 && shapeHeigth>1){
         shapeWidth--;
         shapeHeigth--;
      }
      break;
    }
}
