//Class mic contains the attributes and methods of the microphone elements in the scene, the microphones
//are placed in the scene in the positions where the waveform is going to be observed.
// Details of methods functionality are found in the .cpp file

#ifndef MIC_H
#define MIC_H

#include<QString>
#include <QtXml>

#include "constants.h"
#include "utils.h"

using namespace std;


class Mic
{
public:
    Mic();
    ~Mic();

    Mic(QString nameIn,int rowIn,int colIn);

    QString get_name() const;
    int get_row() const;
    int get_col() const;

    void set_name(QString);
    void set_row(int);
    void set_col(int);

    bool xml2mic(QDomElement micNode, Mic &micIn);
    bool mic2xml(const Mic &MicIn, QDomElement &micNode);

private:
    int row; //row in pixel of the microphone
    int col; //column in pixel of the microphone
    QString name; //name of the microphone
};

#endif // MIC_H
