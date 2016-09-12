// The class sources contains the vector with the sources on the scene the number of sources
// and methods relative to them
// Details of methods functionality are found in the .cpp file

#ifndef SOURCES_H
#define SOURCES_H

#include<QString>
#include <QVector>
#include <QPoint>
#include <QDebug>
#include <QtXml>
#include "source.h"

using namespace std;

class Sources
{
public:
    Sources();
    ~Sources();
    int get_nSources() const;
    QVector<Source> get_sourcesVector() const;
    Source get_sourceByIdx(int) const;

    void set_nSources(int);
    QVector<Source>* set_sourcesVector();
    Source* set_sourceByIdx(int);

    void add_source(Source);
    void delete_sourceByName(QString);
    void delete_sourceByIdx(int);

    int find_sourceByName(QString) const;
    int find_sourceByPos(int,int) const;

    QString get_newFreeSourceName() const;
    QPoint get_newFreeSourcePos(char opt, int rowCandidate, int colCandidate,int rows,int cols) const;
    float get_maxAmplitude();

    bool xml2sources(QDomElement sourcesVectorNode, Sources &sourcesVectorIn);
    bool sources2xml(const Sources &sourcesVectorIn, QDomDocument &documentIn, QDomElement &sourcesVectorNode);

    void clear();

private:
    int nSources; //number of sources in the vector
    QVector<Source> sourcesVector; //vector containing the sources objects
};

#endif // SOURCES_H
