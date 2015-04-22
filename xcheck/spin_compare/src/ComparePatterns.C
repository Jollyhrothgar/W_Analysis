#include "ComparePatterns.h"
#include <iostream>
#include <string>
#include <utility>
#include <map>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iomanip>

#ifndef __CINT__
#include <boost/regex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tokenizer.hpp>
#endif /* __CINT__ */ 

ComparePatterns::ComparePatterns(const std::string& pat1, const std::string& pat2, float threshold, std::string outputDir) {
  std::cout << "Finding differences between " << pat1 << " and " << pat2 << std::endl;
  std::cout << "Sending resuling comparison files to " << outputDir << std::endl;
  std::cout << " Threshold set to " << threshold << std::endl;
  file1 = pat1;
  file2 = pat2;
  this->threshold = threshold;
  this->outputDir = outputDir;
}

ComparePatterns::~ComparePatterns() {
  std::cout << "Done with pattern check." << std::endl;
}

int ComparePatterns::Run() {
  LoadFile(file1,file1_events);
  LoadFile(file2,file2_events);
  FilterEvents();
  return 0;
}

int ComparePatterns::FilterEvents() {
  // Cases
  // 1. File1 and File2 share an event
  // 1a. The event is similar enough, within the threshold
  // 1b. The event is not similar enough, within the threshold.
  // 2. File2 and File2 do not share an event
  // 2a. File 1 has the event, File2 does not
  // 2b. File 2 has the event, File1 does not
  // Look Up Each Event in File_1 in File_1

  // This is preferable, but CINT cannot abide boost regex. Need to figure out a way to 
  // compile to an executable or something to just avoid CINT altogether.
  //boost::cmatch res; // regex matched stuff array
  //boost::regex rx("(/.*/)(.*)");
  //boost::regex_match(file1.c_str(), res, rx);
  //std::cout << res[1] << ", " << res[2] << std::endl;

  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sep("/");
  tokenizer tokens1(file1, sep);
  
  std::stringstream bare_file;
  for( tokenizer::iterator tok_iter = tokens1.begin();
      tok_iter != tokens1.end();
      ++tok_iter ) {
    bare_file.str("");
    bare_file << *tok_iter;
  }

  std::string bare_file1 = bare_file.str();
  
  tokenizer tokens2(file2,sep);
  for( tokenizer::iterator tok_iter = tokens2.begin();
      tok_iter != tokens2.end();
      ++tok_iter ) {
    bare_file.str("");
    bare_file << *tok_iter;
  }
  std::string bare_file2 = bare_file.str();
 
  int f1len = bare_file1.length();
  int f2len = bare_file2.length();

  int margin = 0;
  if( f1len > f2len ) {
    margin = f1len+2;
  } else {
    margin = f2len+2;
  }

  std::string File1OnlyFilename = outputDir + "/EventsOnlyIn_" + bare_file1;
  std::string File2OnlyFilename = outputDir + "/EventsOnlyIn_" + bare_file2;
  std::string SummaryFilename = outputDir + "/Summary.txt";
  std::string CommonEventsFilename = outputDir + "/CommonEvents.txt";
  std::string CommonDifferentEventsFilename = outputDir + "/CommonEventDifferentValue.txt";
  std::ofstream File1Only(File1OnlyFilename.c_str());
  std::ofstream File2Only(File2OnlyFilename.c_str());
  std::ofstream CommonEvents(CommonEventsFilename.c_str());
  std::ofstream CommonDifferentEvents(CommonDifferentEventsFilename.c_str());
  std::ofstream SummaryFile(SummaryFilename.c_str());

  int OnlyInFile1 = 0;
  int OnlyInFile2 = 0;
  int CommonSame  = 0;
  int CommonDiff  = 0;
  for(auto i = file1_events.begin(); i != file1_events.end(); ++i) {
    int Run_Number = i->first;
    std::map<int, EventSet> events = i->second;
    for(auto j = events.begin(); j != events.end(); ++j) {
      int Evt_Number = j->first;
      if( file2_events.find(Run_Number) != file2_events.end() ) {
        // Run Number Match Between File1 and File2
        if( file2_events[Run_Number].find(Evt_Number) != file2_events[Run_Number].end() ) {
          // Run_Number and Evt_Number match between file 1 and 2.
          if( file1_events[Run_Number][Evt_Number].IsSimilarTo(file2_events[Run_Number][Evt_Number], threshold) ) {
            // Case 1a: Erase common entry from both files.
            CommonEvents << Run_Number << " " << Evt_Number << " " << file1_events[Run_Number][Evt_Number].GetEventString() << std::endl;
            file1_events[Run_Number].erase(Evt_Number);
            file2_events[Run_Number].erase(Evt_Number);
            CommonSame++;
          } else {
	          // Case 1b: Run_Number and Evt_Number matched, but the event data are too different.
            // std::cout << " A common run and event number was found, but the values were too different " << Run_Number << ", " << Evt_Number << std::endl;
            CommonDifferentEvents << std::setw(margin) << bare_file1 << ": "  << Run_Number << " " << Evt_Number << " " << file1_events[Run_Number][Evt_Number].GetEventString() << std::endl;
            CommonDifferentEvents << std::setw(margin) << bare_file2 << ": "  << Run_Number << " " << Evt_Number << " " << file2_events[Run_Number][Evt_Number].GetEventString() << std::endl;
            CommonDiff++;
          }
        } else {
          // Could not find Run_Number:Evt_Number from file1 in file2
          // std::cout << file2 << " is missing event " << Evt_Number << " from run: " << Run_Number << std::endl;
          File1Only << Run_Number << " " << Evt_Number << " " << file1_events[Run_Number][Evt_Number].GetEventString() << std::endl;
          OnlyInFile1++;
        }
      } else {
        // Run_Number in File1 was not Found in File2
        //std::cout << file2 << " does not have any events from Run: " << Run_Number << std::endl;
        File1Only << Run_Number << " " << Evt_Number << " " << file1_events[Run_Number][Evt_Number].GetEventString() << std::endl;
        OnlyInFile1++;
      }
      // Done with this event, we can delete it.
      file1_events[Run_Number].erase(Evt_Number);
    }
    // Done with this run, we can delete it.
    file1_events.erase(Run_Number);
  }

  // Any Remaining Events in file2_events were events which were not in file1.
  for(auto i = file2_events.begin(); i != file2_events.end(); ++i) {
    int Run_Number = i->first;
    std::map<int, EventSet> events = i->second;
    for(auto j = events.begin(); j != events.end(); ++j) {
      int Evt_Number = j->first;
      // std::cout << file1 << " is missing Run: " << Run_Number << ", Evt: " << Evt_Number << std::endl;
      File2Only << Run_Number << " " << Evt_Number << " " << file1_events[Run_Number][Evt_Number].GetEventString() << std::endl;
      OnlyInFile2++;
    }
  }
  
  std::cout << "Summary of Differencs" << std::endl
   << "There were " << CommonSame << " common events." << std::endl
   << "There were " << CommonDiff << " events which had common event+run numbers, but had different values otherwise" << std::endl
   << bare_file1 << " had " << OnlyInFile1 << " events not contained in " << bare_file2 << std::endl
   << bare_file2 << " had " << OnlyInFile2 << " events not contained in " << bare_file1 << std::endl;

  SummaryFile << "Summary of Differencs" << std::endl
   << "There were " << CommonSame << " common events." << std::endl
   << "There were " << CommonDiff << " events which had common event+run numbers, but had different values otherwise" << std::endl
   << bare_file1 << " had " << OnlyInFile1 << " events not contained in " << bare_file2 << std::endl
   << bare_file2 << " had " << OnlyInFile2 << " events not contained in " << bare_file1 << std::endl;
  return 0;
}

int ComparePatterns::LoadFile(const std::string& f, run_events& events_map ) {
  std::ifstream inFile(f.c_str());
  if(!inFile) return -1;
  std::string line = "";
  while(getline(inFile,line)) { 
    // Fields in File
    int Run_Number = -1;
    int Evt_Number = -1;
    EventSet s;

    std::stringstream ss;
    ss.str(line);
    ss >> Run_Number >> Evt_Number >> s.xid >> s.pT >> s.eta >> s.dw23 >> s.Wness;
    events_map[Run_Number][Evt_Number] = s;
  }

  std::cout << f << " loaded, with " << events_map.size() << " runs and ";
  int total_events = 0;
  for(auto i = events_map.begin(); i != events_map.end(); ++i) {
    total_events+=i->second.size();
  }
  std::cout << total_events << " events." << std::endl;
  return 0;
}
