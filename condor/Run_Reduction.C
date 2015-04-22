void Run_Reduction(std::string inputDirectory, std::string outputDirectory, std::string fileName, std::string library)
{
	gSystem->Load(library.c_str());
	ReduceProduction(inputDirectory, outputDirectory, fileName);
}
