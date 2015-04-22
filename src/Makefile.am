AUTOMAKE_OPTIONS = foreign

AM_CXXFLAGS = -Wall -Werror -std=c++0x 

#INCLUDES = -I$(includedir) -I$(OFFLINE_MAIN)/include -I$(ROOTSYS)/include -I/opt/phenix/include 

BUILT_SOURCES = \
  testexternals.C

AM_CPPFLAGS = \
  -I$(includedir) \
  -I$(OFFLINE_MAIN)/include \
  -I/opt/phenix/include \
  -I$(ROOTSYS)/include

lib_LTLIBRARIES = \
  libSingleMuonsWAnalysis.la

noinst_HEADERS = \
  SingleMuonsConsts.h 

# this can be replaced with a shell command which invokes and captures root-config --libs
libSingleMuonsWAnalysis_la_LDFLAGS = \
  -L$(ROOTSYS)/lib `root-config --evelibs`  

AM_LDFLAGS = -L$(libdir) -L$(OFFLINE_MAIN)/lib `root-config --evelibs` -lRooFitCore -lMinuit -lFoam

libSingleMuonsWAnalysis_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -lphool \
  -L/opt/phenix/lib \
  -luspin \
  -lodbc \
  -lodbc++

libSingleMuonsWAnalysis_la_SOURCES = \
  SingleMuons.C \
  ReduceProductionSingleMuons.C \
  SingleMuonsLikelihood.C \
  dw23VsWness.C \
  HelperFunctions.C \
  ReduceProduction.C \
  CalculateLikelihoodFunctions.C \
  GeneratePhysicsDistributions.C \
  BasicCutTreeVariables.C \
  MergeTrees.C \
  FilterEvents.C \
  Plotter.C \
  Run13Triggers.C \
  SBRextract.C \
  ReduceProductionChain.C \
  W2eGetPol.C \
  AsymmetryCalculator.C \
  DuplicateSearch.C \
  TranslateTree.C \
  EventComparison.C \
  HadronicBackgroundSbrPdf.C \
  MuonBackgroundSbrPdf.C \
  WSignalSbrPdf.C \
  DictOutput.cxx

noinst_PROGRAMS = \
  testexternals

testexternals_LDADD = \
  libSingleMuonsWAnalysis.la

testexternals.C:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

DictOutput.cxx: SingleMuons.h \
  ReduceProductionSingleMuons.h \
  SingleMuonsLikelihood.h \
  dw23VsWness.h \
  HelperFunctions.h \
  ReduceProduction.h \
  CalculateLikelihoodFunctions.h \
  GeneratePhysicsDistributions.h \
  MergeTrees.h \
  Plotter.h \
  BasicCutTreeVariables.h \
  FilterEvents.h \
  Run13Triggers.h \
  SBRextract.h \
  ReduceProductionChain.h \
  W2eGetPol.h AsymmetryCalculator.h \
  DuplicateSearch.h \
  TranslateTree.h \
  EventComparison.h \
  HadronicBackgroundSbrPdf.h \
  MuonBackgroundSbrPdf.h \
  WSignalSbrPdf.h \
  SingleMuonsLinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

clean-local:
	rm -f *Dict.*
