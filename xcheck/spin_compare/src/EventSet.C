#include "EventSet.h"
#include <cmath>
#include <sstream>
#include <string>

EventSet::EventSet () {
  xid   = -1;
  pT    = -999.;
  eta   = -999.;
  dw23  = -999.;
  Wness = -999.;
}


EventSet::~EventSet() {
}

bool EventSet::IsSimilarTo( const EventSet& s, float threshold ) {
  if( this->xid != s.xid ) return false;
  if( (fabs( this->pT - s.pT )/(this->pT)) > threshold ) return false;
  if( (fabs( this->eta - s.eta )/(this->eta)) > threshold ) return false;
  if( (fabs( this->dw23 - s.dw23 )/(this->dw23)) > threshold ) return false;
  if( (fabs( this->Wness - s.Wness )/(this->Wness)) > threshold ) return false;
  return true;
}

std::string EventSet::GetEventString() {
  std::stringstream ss;
  ss << this->xid << " " << this->pT << " " << this->eta << " " << this->dw23 << " " << this->Wness;
  return ss.str();
}
