#ifndef __RUN13TRIGGERS__
#define __RUN13TRIGGERS__

//STL
#include <map>
#include <vector>
#include <iostream>
#include <utility>
#include <iomanip>

class Run13Triggers
{
	public:
		// Creates a map of all triggers
		Run13Triggers();
		~Run13Triggers();
		// Tests if the trigger is a Run13 Muon trigger
		bool IsRun13Trigger( int triggerBit);
		void ShowTriggers();
	private:
		// Stores all trigger information
		std::multimap<std::string, int> triggers;
		typedef std::multimap<std::string, int> TriggerBit;
		
};

#endif
