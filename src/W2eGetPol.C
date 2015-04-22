// ODBC
#include <odbc++/connection.h>
#include <odbc++/setup.h>
#include <odbc++/types.h>
#include <odbc++/errorhandler.h>
#include <odbc++/drivermanager.h>
#include <odbc++/resultset.h>
#include <odbc++/resultsetmetadata.h>
#include <odbc++/preparedstatement.h>
#include <odbc++/databasemetadata.h>

// custom
#include "W2eGetPol.h"
#include <sql.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>

using namespace odbc;

W2eGetPol::W2eGetPol(){
}

int W2eGetPol::InitPat(int run){
  runnumber=run;

  spin_out.StoreDBContent(run,run);

  if(spin_out.CheckRunRowStore(run)!=1){return 5;}
  spin_out.GetDBContentStore(spin_cont,run);

  xingshift=spin_cont.GetCrossingShift();

  qalevel = spin_cont.GetQALevel(); 

  Pattern(); 
  return 0;
}

void W2eGetPol::Pattern(){
  double bpol,bpolerr,ypol,ypolerr;
  spin_cont.GetPolarizationBlue(0,bpol,bpolerr);
  spin_cont.GetPolarizationYellow(0,ypol,ypolerr);

  pol[0]=bpol;
  polerr[0]=bpolerr;
  pol[1]=ypol;
  polerr[1]=ypolerr;

  for(int i=0; i<120; i++) {
    int pb=spin_cont.GetSpinPatternBlue(i);  // this function need the correct xid
    int py=spin_cont.GetSpinPatternYellow(i);//

    if(pb== 1 && py== 1) {
      pattern[i]=0;
    } else if( pb== -1 && py== 1) { 
      pattern[i]=1;
    } else if( pb==  1 && py==-1) { 
      pattern[i]=2;
    } else if( pb== -1 && py==-1) { 
      pattern[i]=3;
    } else {                     
      pattern[i]=4;
    }
    scaler[i]=spin_cont.GetScalerBbcNoCut((i)%120);
  }
}
