//this class corresponds to the simulation field canvas

#ifndef DRAWLABEL_H
#define DRAWLABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QEvent>
#include <QPoint>

class DrawLabel: public QLabel
{
   Q_OBJECT

signals:
   void posChanged(const QPoint &);
   void keyPressed(const int &);
   void enter();
   void leave();
   void mouseClicked(const int &);
   void mouseReleased(const int &);

public:
   //constructor
   DrawLabel():QLabel(){
       this->setAttribute(Qt::WA_Hover, true);
       this->setMouseTracking(true);
   }

protected:
   virtual void mouseMoveEvent(QMouseEvent *event){
      QLabel::mouseMoveEvent(event);
      emit posChanged(event->pos());
   }

   void mousePressEvent(QMouseEvent *event){
       QLabel::mousePressEvent(event);
       emit mouseClicked(event->button());
   }

   void mouseReleaseEvent(QMouseEvent *event){
       QLabel::mouseReleaseEvent(event);
       emit mouseReleased(event->button());
   }

   void enterEvent(QEvent * event){
       QLabel::enterEvent(event);
       emit enter();
   }
   void leaveEvent(QEvent * event){
       QLabel::leaveEvent(event);
       emit leave();
   }
   virtual void keyPressEvent(QKeyEvent *event){
      QLabel::keyPressEvent(event);
      emit keyPressed(event->key());
   }
};


#endif // DRAWLABEL_H
