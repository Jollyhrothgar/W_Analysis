#ifndef __COMPARE_PATTERNS_H__
#define __COMPARE_PATTERNS_H__
// STL
#include <string>
#include <map>

// boost tuple is no longer used, but I'm keeping this here so I don't
// forget how to use it.
#ifndef __CINT__
#include "boost/tuple/tuple.hpp"
#include "boost/tuple/tuple_comparison.hpp"
#endif /* __CINT__ */ 

// Custom
#include "EventSet.h"

class ComparePatterns {
  public:
   ComparePatterns(const std::string& pat1, const std::string& pat2, float threshold, std::string outputDir = ""); 
   /** Inialize with two text file continaing whitepace separated event information, 
   one event per line */
                                                                      
   ~ComparePatterns();
   int Run(); /** Call the private members in the right order, to run the comparison */
  private:
#ifndef __CINT__ 
   typedef std::map< int, std::map< int, EventSet > > run_events;
   run_events file1_events;
   run_events file2_events;
#else
   class run_events;
   class file1_events;
   class file2_events;
#endif /* __CINT__ */
   std::string file1;
   std::string file2;
   int LoadFile(const std::string& f, run_events& file_events);
   int FilterEvents(); /** removes common events, except for events which are not similar enough 
                       Sorts events according to which file contained which events. */
   float threshold; /** Set threshold for fractional % difference that two events can be before being flagged
                    as different */
   std::string outputDir; /** place to put the resulting comparison files */
};

#endif
