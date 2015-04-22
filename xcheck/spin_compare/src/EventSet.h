#ifndef __EVENT_SET__
#define __EVENT_SET__

#include <string>

class EventSet {
 public:
   EventSet();
   ~EventSet();
   
   bool IsSimilarTo( const EventSet& s, float threshold); /** Returns true if s is similar to this (user defined) 
                                                   subject to a fractional percent difference specificed 
						   by threshold. Any integer values must be exactly 
						   similar. Example - if threshold is set to 0.01, then 
						   two float values for an event must be within 1% of 
						   eachother. */
   std::string GetEventString(); /** Returns a string of whitespace separated event data */

   // Define Various Event Parameters Here (add or subtract as needed)
   int   xid  ;
   float pT   ;
   float eta  ;
   float dw23 ;
   float Wness;
};

#endif
