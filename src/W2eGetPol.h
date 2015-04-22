#ifndef __W2EGETPOL_H__
#define __W2EGETPOL_H__ 

#include <SpinDBOutput.hh>
#include <SpinDBContent.hh>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <map>
#include <fstream>

class W2eGetPol
{
 public:
  W2eGetPol(void);
  ~W2eGetPol(void){};

  int InitPat(int run);
  
  int GetPattern(int crossing){return pattern[(crossing+xingshift)%120];}
  int GetXing(int crossing){return (crossing+xingshift)%120;}
  double GetScaler(int crossing){return scaler[(crossing+xingshift)%120];}
  double GetPol(int beam){return pol[beam];}
  double GetXingshift(){return xingshift;}

 private:
  void Pattern();
  
  
  ofstream fout;
  int runnumber;
  int xingshift;
  int qalevel;
  SpinDBContent spin_cont;
  SpinDBOutput spin_out;
  int pattern[120];
  double scaler[120];
  double pol[2];
  double polerr[2];
};

#endif // __W2eGetPol_H__
