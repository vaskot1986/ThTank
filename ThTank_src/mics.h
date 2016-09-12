// The class mics contains the vector with the mics on the scene the number of mics
// and methods relative to them
// Details of methods functionality are found in the .cpp file

#ifndef MICS_H
#define MICS_H

#include<QString>
#include<QPoint>
#include <QVector>
#include <QtXml>
#include "mic.h"

using namespace std;

class Mics
{
public:
    Mics();
    ~Mics();
    int get_nMics() const;
    QVector<Mic> get_micsVector() const;
    Mic get_micByIdx(int) const;

    void set_nMics(int nMicsIn);
    QVector<Mic>* set_micsVector();
    Mic* set_micByIdx(int);

    void add_mic(Mic);
    void delete_micByName(QString);
    void delete_micByIdx(int);

    int find_micByName(QString) const;
    int find_micByPos(int,int) const;

    QString get_newFreeMicName() const;
    QPoint get_newFreeMicPos(char,int,int,int,int) const;


    void clear();
    bool xml2mics(QDomElement micsVectorNode, Mics &micsVectorIn);
    bool mics2xml(const Mics &micsVectorIn, QDomDocument &documentIn,QDomElement &micsVectorNode);


private:
    int nMics; //number of microphones in the vector
    QVector<Mic> micsVector; //vector containing the microphone objects
};

#endif // MICS_H
