//class utils implement some functions used by different classes in the program

#include "constants.h"
#include "utils.h"

// removeBadChars()
//
// PURPOSE
//  Takes a string as input and removes characters wich are not A-Z a-z 0-9
//
// INPUT
// QString &inputStr - string to be corrected
//
// OUTPUT
//  QString &inputStr - string to be corrected
//
// RETURN VALUE
//   None
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
void removeBadChars(QString &inputStr){
  for (int i=0;i<48;i++) inputStr.remove(char(i));
  for (int i=58;i<65;i++) inputStr.remove(char(i));
  for (int i=91;i<96;i++) inputStr.remove(char(i));
  for (int i=123;i<256;i++) inputStr.remove(char(i));
}

// log10mod()
//
// PURPOSE
//  first function to approximate log10(), checks the 10th power of the number and calls the function conv() which gives the finer approximation
//
// INPUT
// const int &value - value indicating how many times is bigger pressure than pressure ref
//
// OUTPUT
//  None
//
// RETURN VALUE
//   (float)- value in db
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
float log10mod(const int &value){
return (value >= 1000000)? 6 :
    (value >= 100000)? conv(5,1000000,value) :
    (value >= 10000)? conv(4,100000,value) :
    (value >= 1000)? conv(3,10000,value) :
    (value >= 100)? conv(2,1000,value) :
    (value >= 10)? conv(1,100,value) :
    (value >= 1)? conv(0,10,value) :
    (value >= 0)? conv(0,1,value) : 0;
}

// log10mod()
//
// PURPOSE
//  second function to approximate log10(), once it knows the power, checks the decimal numbers with the remaining number
//
// INPUT
// const int &power - power of tenth of the number
// const float &bigVal - next value of power of tenth
// const int &val - value indicating how many times is bigger pressure than pressure ref
//
// OUTPUT
//  None
//
// RETURN VALUE
//   (float)- value in db
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
float conv(const int &power, const float &bigVal, const int &val){
    int res=10*(val/bigVal);
    switch(res){
     case 9: return power+0.954;
     case 8: return power+0.903;
     case 7: return power+0.845;
     case 6: return power+0.778;
     case 5: return power+0.699;
     case 4: return power+0.602;
     case 3: return power+0.477;
     case 2: return power+0.301;
     default: return power;
    }
}

// pa2db()
//
// PURPOSE
//  Convert pascals to dB SPL
//
// INPUT
// const float &pa - value in Pascal pressure units
//
// OUTPUT
//  None
//
// RETURN VALUE
//   Int- value in dB SPL
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
int pa2db(const float &pa){
  int val=fabs(pa)/REF_PRESS;
  #ifdef Q_OS_WIN32 //if system is win 32 use approximation functions. Log10() results to be too slow performant
    return 20*log10mod(val);
  #else //if system is not win 32 use normal log10() functions
    return 20*log10(val);
  #endif
}

// db2pa()
//
// PURPOSE
//  Convert dB SPL to pascals
//
// INPUT
// const int &db - vvalue in dB SPL
//
// OUTPUT
//  None
//
// RETURN VALUE
//   (float)- value in Pascal pressure units
//
//
//	AUTHOR		CHANGES										DATE	VERSION
//	A. Vazquez	First build									12/15	0.9
float db2pa(const int &db){
  return REF_PRESS*pow(10,db/20.f);
}
