//class utils implement some functions used by different classes in the program
// Details of methods functionality are found in the .cpp file

#ifndef UTILS_H
#define UTILS_H

#include <QDebug>
#include <QString>

void removeBadChars(QString &inputStr);
float log10Mod(const int &val);
int pa2db(const float &pa);
float db2pa(const int &db);
float conv(const int &pot, const float &bigVal, const int &val);
#endif // UTILS_H
