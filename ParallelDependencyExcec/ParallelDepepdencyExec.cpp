#include "ParallelDepepdencyExec.h"

using PrettyHelper = Utilities::StringHelper;

////////////////////////////////////////////////
// Will set the VERBOSE variable values
void ParallelDependencExec::setVerbose(bool paramV) {
	VERBOSE = paramV;
}

///////////////////////////////////////////////////////////
// Will perform the analysis and display result to console
void ParallelDependencExec::performAnalysis(std::string path, std::vector<std::string> pattern) {
	ParallelDependencyAnalysis execRun;
	execRun.setVerbose(VERBOSE);
	execRun.PerformAnalysis(path, pattern);
}

#ifdef TEST_PARALLELDEPEXEC
///////////////////////////////////////////////////////////
// Requirements 3 to 8
void Requirement3to8() {
	PrettyHelper::Title("\n\n Requirement 3", '=');
	std::cout << "\n Please check the Project 3 folder for the packages";
	PrettyHelper::Title("\n\n Requirement 4", '=');
	std::cout << "\n The ThreadPool package can be found in ..\\ThreadPool\\ folder or it can be seen in Visual Studio";
	PrettyHelper::Title("\n\n Requirement 5", '=');
	std::cout << "\n The Tasks package can be found in ..\\Tasks\\ folder or it can be seen in Visual Studio";
	PrettyHelper::Title("\n\n Requirement 6", '=');
	std::cout << "\n The TaypeAnalysis package can be found in ..\\GetTypeTable\\ folder or it can be seen in Visual Studio";
	PrettyHelper::Title("\n\n Requirement 7", '=');
	std::cout << "\n The DependencyAnalysis package can be found in ..\\AnalyzeFile\\ folder or it can be seen in Visual Studio";
	PrettyHelper::Title("\n\n Requirement 8", '=');
	std::cout << "\n The ParallelDependencyAnalysis package can be found in ..\\ParallelDependencyAnalysis\\ folder or it can be seen in Visual Studio";
	std::cout << "\n\n ";
}

///////////////////////////////////////////////////////////
// Requirement 9
void Requirement9(std::string path, std::vector<std::string> pattern, bool verbose) {
	PrettyHelper::Title("\n\n Requirement 9", '=');
	std::cout << "\n ";
	ParallelDependencExec PDE;
	PDE.setVerbose(verbose);
	PDE.performAnalysis(path, pattern);
}

///////////////////////////////////////////////////////////
// TEST STUB ENTRY POINT
int main(int argc, char* argv[]) {
	int nuArgs = argc - 1;
	std::string path;
	std::vector<std::string> pattern;

	switch (nuArgs)
	{
	case 0:
		path = "../Test";
		pattern = { "*.h","*.cpp" };
		break;
	case 1:
		path = argv[1];
		pattern = { "*.h","*.cpp" };
		break;
	default:
		path = argv[1];
		for (int index = 2; index < argc; index++)
			pattern.push_back(argv[index]);
		break;
	}

	Requirement3to8();
	system("pause");

	// Verbose variable for accessing V#ERBOSE mode
	// set it to true to access VERBOSE Mode (must try it)
	bool VERBOSE = false;
	Requirement9(path, pattern, VERBOSE);
	system("pause");
	
	PrettyHelper::Title("\n\n Requirement 10", '=');
	std::cout << "\n This executable is the Automated Test Suite, which is the Requirement 10\n\n ";
	system("pause");
}

#endif