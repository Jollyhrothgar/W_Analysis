This readme file gives a very brief overview of each source file, and its intended usage.
I will try to keep this file updated, and will update the readme with each subsequent CVS
tag.

Introduction:

These source files encapsulate the complete W-Analysis of Mike Beaumier, written for
analyzing the data sets taking during Run 13, with some limited cross-checking
functionality implemented for the Run 12 data set. Tasks handled by this analysis code
range from generating W likelihoods from a merged and greatly reduced tree of physics
data, generating the aforementioned tree from picoDST trees generated for individual runs,
generating  Probability Density Functions, making physics and intermediate plots, and 
extracting the signal to background ratio for the W->mu signal.

In general, the pDST tree with multiple subevent leaves per event branch is referred to
"newsngmuons" and the goal of the intial part of the analysis is to generate a reduced
tree, with cuts applied, which is small enough to run over quickly, and has merged all
pDSTs into a single "analysis tree". This secondary tree is the target of all analysis
code, and is referred to as the "analysis tree" or "newsngmuons_basic_cut".

The analysis tree is generated with some place-holder data for "Wness" which is actually
filled in CalcualteLikelihoodFunctions.C

SOURCES --> src/

--TREE GENERATION, ENVIRONMENT SETUP, GENERAL PURPOSE--
SingleMuonsConsts.h 
SingleMuons.C       
ReduceProduction.C  
MergeTrees.C        
HelperFunctions.C   
BinarySearchMerge.C
BasicCutTreeVariables.C
Plotter.C           


--LIKELIHOOD ANALYSIS--
SingleMuonsLikelihood.C
GeneratePhysicsDistributions.C
CalculateLikelihoodFunctions.C
dw23VsWness.C

SingleMuonsConsts.h
	This header file contains variables used for configuring the analysis environment, as
	well as indexing variables for accessing specic analysis variables. Indexing is
	implemented as follows:
		* A Master "Size" Index (contains the number of variables to be used in the set
		* Variable lookup indicies
	
	The mapping is between an "index" variable, and a corresponding variable which lives
	inside an indexed container - as used in SingleMuonsLikelihood, and the macros. This
	is used in lieu of a std::map, because of root's poor support of the data structure in
	interactive environments, reducing directory clutter for automatically generated
	dictionaries. I wish I knew of a better way to do this...

	Example:
		NUMBER_OF_PHYSICS_DISTS = 2
		DIST_1_INDEX = 0
		DIST_2_INDEX
		TH1F* PhysDists[NUMBER_OF_PHYSICS_DISTS]
		PhysDist[DIST_1_INDEX] // more obvious than calling "PhysDist[0]"

SingleMuons.C
	This class is a wrapper designed to handle reading the tree "newsngmuons", as
	extracted from the pDST generated in production 2103 for the muon analysis. The class
	is designed to filter out a single event from the "newsngmuons" tree into a single
	C++ state - i.e., at any given time, the class is access one event from "newsngmuons"
	and all sub-event branches and variable lists.

	Specifically, for each event, this class stores all associated muon track data as
	vectors.

	For explicit usage of the class, see ReduceProduction.C, and
	MergeTrees.C.

	The class' member data and functions are split into a public interface, and private
	methods. For a complete description of each member data and member function, see
	SingleMuons.h

BasiReduceProduction.C
	This is a compiled which is designed to be run over condor - such that one pDST can be 
	reduced to one analysis tree - and these trees can later be merged interactively.

	Ultimately, this function should be called inside of ReduceProduction to avoid errors
	from synchronizing the two functions. It could be done, by making a temporary
	directory for single tree output, then calling MergeTrees.C to make a single merged
	tree, and deleting the temporary directory with individual trees.

	Arguments:
		Arg1 (string): the directory containing unmerged analysis trees
		Arg2 (string): the directory to store the single merged analysis tree
		Arg3 (string): the filename of the single output tree. 


MergeTrees.C
	This macro takes a list of unmerged analysis trees "newsngmuons_basic_cut" style
	trees, and merges them all into a single file. 
	Arguments: 
		Arg1 (string): filename with the list of trees to merge (\n separated files)
		Arg2 (string): filename for merged tree
	
	Uses directory structure defined in SingleMuonsConsts.h

HelperFunctions.C
	Numerous functions used for index-lookup (typical useage within a loop), root-object
	name generation, limited file reading functions. See HelperFunctions.h for a complete
	list of defined functionality.

	This set of functions is analysis-critical - certain aspects of the analysis, such as
	selecting a set of P.D.F.s is defined in this file.

BinarySearchMerge.C
	This function is designed to search a list of full production, unmerged
	pDSTS for missing events. It implements the std::map binary search
	function to search each full production file for each Evt_Number in the
	RPC1 production.

BasicCutTreeVariables.C
	This struct is designed to streamline tree definition. It can be modified,
	and then every other function which uses the analysis tree will be updated
	with the latest tree structure automatically. Trees will have to be
	reproduced, naturally.

Plotter.C
	Class. written in about an hour and half to provide plots for Francesca, designed to
	plot all PDF kinematic varialbles against one another, exactly once. (No duplicate
	plots - ie - "DG0 vs DDG0" is plotted only once, and is not plotted again as in 
	"DDG0 vs DG0")

SingleMuonsLikelihood.C
	This is a class designed for two usage scenarios. The first is used as a wrapper for
	generating and filling distributions of the kinematic variables, to be used as seeds
	for the Probability Density Functions (P.D.F.s) used in the Likelihood analysis, and
	then writing these distributions out to a root file to be used later.

	This usage scenario is encapsulated in the compiled function
	"GeneratePhysicsDistributions.C"

	The second usage scenario is for loading P.D.F. seed distributions from the root 
	file generated in the first usage scenario, generated P.D.F.s from these
	distributions, and calculated a likelihood based on input analysis variables. This
	usage scenario is taked advantage and encapsulated in the compiled function
	"CalculateLikelihoodFunctions.C"

	As with SingleMuons.C, this class is separated into a public interface, and a private
	set of member functions and member data.

	For a complete description of member functions and data, see SingleMuonsLikelihood.h,
	and for usage examples, see GeneratePhysicsDistributions.C (for usage scenario 1) and
	CalculateLikelihoodFunctions.C (for usage scenario 2).

GeneratePhysicsDistributions.C
	This is a compiled function, which uses the class SingleMuonsLikelihood.C, and the
	trees generated by MergeTrees.C, ReduceProduction.C (all of
	the three preceeding functions are used to generate the *same* analysis tree,
	"newsngmuons_basic_cut", depending on the set of pDSTs used), to generate
	distributions of the analysis variables.

		Arguments:
			Arg1 (string): A root file containing an analysis tree generated from Run data
			Arg2 (string): A root file containing an analysis tree from simulations
			Arg3 (string): A tag for the root file generated by the function which
				contains all of the P.D.F. seed distributions managed by
				SingleMuonsLikelihood.C

CalculateLikelihoodFunctions.C
	This is a compiled function, which uses the class SingleMuonsLikelihood.C to load
	physics distributions generated by GeneratePhysicsDistributions.C and calculate a
	Likelihood for each even in the analysis tree.
	
		Arguments:
			Arg2 (string): PDF distributions stored in the root file specified
			Arg2 (string): root file conatining the analysis tree
			Arg3 (string): root file written, which contains the updated Wness variable
				for each event.

dw23VsWness.C
	This is a class which is used for extrapolation of the dw23 PDF from data for hadronic
	background, and subsequent calculation of the signal to background ratio of W->mu
	events. The class draws from three identically structured analysis trees containing
	data dominated by hadronic background events, W-simulation, or muon background
	simulations.

	The class also supports intermediate reading/writing of data used for extrapolation of
	the dw23 shape for hadronic background for faster tuning of the fitting functions
	contained as members.

	This class perfoms several sets of fits, the first of which, is used for extrapolating
	the shape of dw23 for hadronic background, and simulataneously fits and projects the shape
	of the dw23 distribution on various Wness slices in data.

	The class also supports variable binning in Wness.

	For detailed description of usage, see the macro "Run_dw23WnessFitter.C", and for a
	complete description of class methods and member variables, see dw23VsWnes.h.

MACROS --> macros/

CompareWness.C
dw23Plots.C
StackWness.C
ReduceSimulation.C
Run_Generation.C
Run_Plotter.C
Run_Calculation.C
Run_dw23WnessFitter.C
CompareVariableConditions.C

CompareWness.C
	A simple macro which plots the normalized simulation Wness against the normalized data
	Wness. Analysis trees are pointed to with variables, not macro arguments.

dw23Plots.C
	An old macro which is not used in analysis, but as a cross-check for plots generated
	by dw23VsWness.C

StackWness.C
	Reads in each Wness stored in the analysis trees for each muon background source, and
	stacks them. Rudimentary weighting is performed.

ReduceSimulation.C
	Interactively creates the simulation analysis tree for W->mu events. Uses
	BasicCutTree.C.

Run_Generation.C
	Calls GeneratePhysicsDistributions.C for every analysis tree used - simulation, data,
	and various muon backgrounds.

Run_Plotter.C
	Calls an instance of Plotter.C to generate plots.

Run_Calcualtion.C
	Generates finalized analysis trees for all muon sources.

Run_dw23WnessFitter.C
	Calls an instance of dw23VsWness.C in either 'read and fit' mode or in 'loop and fit'
	mode, depending on an integer argument to the macro (0 or 1). More options can be
	added as dw23VsWness evolves to encompass the Extended Unbinned Maximum Likelihood
	fit.

CompareVariableCondiditions.C
	Plots each kinematic variable from a root file containing PDF distributions, called
	with HelperFunctions.C, and plots a single variale on a single canvas, comparing that
	variables distributions for N mu+, N mu-, S mu+, S mu-. 
