AUTOMAKE_OPTIONS = foreign

AM_CXXFLAGS = -Wall -Werror -std=c++0x 

BUILT_SOURCES = \
  testexternals.C

AM_CPPFLAGS = \
  -I$(includedir) \
  -I$(OFFLINE_MAIN)/include \
  -I$(ROOTSYS)/include

lib_LTLIBRARIES = \
  libHelicityCheck.la

# this can be replaced with a shell command which invokes and captures root-config --libs
libHelicityCheck_la_LDFLAGS = \
  -L$(ROOTSYS)/lib \
  `root-config --evelibs` \
  -lRooFitCore \
  -lMinuit \
  -lFoam \
  -lboost_regex

AM_LDFLAGS = \
  -L$(OFFLINE_MAIN)/lib \
  -L$(ROOTSYS)/lib \
  -L$(libdir) 

libHelicityCheck_la_SOURCES = \
  ComparePatterns.C \
  EventSet.C \
  DictOutput.cxx

noinst_PROGRAMS = \
  testexternals

testexternals_LDADD = \
  libHelicityCheck.la

testexternals.C:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

DictOutput.cxx: \
	ComparePatterns.h \
	EventSet.h \
	LinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

clean-local:
	rm -f *Dict.*
