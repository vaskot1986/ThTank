//Class source contains the attributes and methods of the source elements in the scene, the source
//are placed in the scene and emit presure values depending on their configuration and type
// Details of methods functionality are found in the .cpp file

#ifndef SOURCE_H
#define SOURCE_H

#include<QString>
#include <Qstring>
#include <cmath>
#include <QtXml>

#include "utils.h"
#include "constants.h"

using namespace std;

class Source
{
public:
    enum SourceType{
        sinus=0, delta=1, sweep=2, gauss=3
    };
    Source();
    ~Source();
    Source(QString nameIn,int rowIn,int colIn,SourceType typeIn,int phaseIn,float ampIn,int freqIn);
    QString get_name() const;
    int get_row() const;
    int get_col() const;
    SourceType get_type() const;
    int get_type_int() const;
    int get_phase() const;
    float get_amp() const;
    float get_freq() const;

    void set_name(QString);
    void set_row(int);
    void set_col(int);
    void set_type(SourceType);
    void set_type(int);
    void set_phase(int);
    void set_amp(float);
    void set_freq(float);

    float generate_newVal(const float &twoPiValue,const float &soundSpeed,const float &simDelta,const float &simStep) const;
    bool xml2source(QDomElement sourceNode, Source &sourceIn);
    bool source2xml(const Source &sourceIn, QDomElement &sourceNode);

private:
    QString name; //name of the source
    int row; //row in pixels where the source is
    int col; //column in pixels where the source is
    SourceType type; //type of source from enum SourceType
    int phase; //phase of the source
    float amp; //amplitude of the source
    float freq; //frequency of the source
};

#endif // SOURCE_H
