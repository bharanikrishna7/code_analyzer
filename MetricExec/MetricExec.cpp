//////////////////////////////////////////////////////////////////////////////
// MetricExec.cpp - analyzes multiple files and provides metrics			//
// Version :	  1.0                                                       //
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Code Parser with AST										//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////

#include"MetricExec.h"

using PrettyHelper = Utilities::StringHelper;

// This function will search for all the files in the location 'path' which
// have certain pattern(s). These file locations will be stored in files vector
// of MetricExec variable calling this function.
void MetricExec::setFiles(std::string path, std::vector<std::string> patterns)
{
	std::string _path = path;

	DataStore ds;
	FileMgr fm(path, ds);
	for (auto pat : patterns)
	{
		fm.addPattern(pat);
	}
	fm.search();
	for (auto file : ds)
	{
		files.push_back(file);
	}
}

// This function will perform analysis for each and every file 
// whose path is present in the vector files, of the MetricExec
// variable calling this function.
void MetricExec::getAnalysis(std::vector<std::string> files)
{
	Metrics m;
	for (auto file : files) {
		m.analyze(file);
	}
}

// Will return the vector files of the MetricsExec variable
std::vector<std::string> MetricExec::returnFiles()
{
	return files;
}


#ifdef TEST_METRICEXEC
void Requirement3to8()
{
	PrettyHelper::Title("\n REQUIREMENT 3:", '#');
	std::cout << "\n These packages can be viewed from file explorer or Viusal Studio.\n\n\n";
	
	PrettyHelper::Title("\n REQUIREMENT 4:", '#');
	std::cout << "\n The \"Parser\" package can be viewed from file explorer or Viusal Studio.\n\n\n";
	
	PrettyHelper::Title("\n REQUIREMENT 5:", '#');
	std::cout << "\n The Action and Rules package is present in \"Parser\" project of this solution.\n Please look at it to verify rules and actions.\n\n\n";
	
	PrettyHelper::Title("\n REQUIREMENT 6:", '#');
	std::cout << "\n The AST package can be viewed from file explorer or Visual Studio.\n\n\n";
	
	PrettyHelper::Title("\n REQUIREMENT 7", '#');
	std::cout << "\n The FileMgr package can be viewed from file explorer or Visual Studio.\n\n\n";
	
	PrettyHelper::Title("\n REQUIREMENT 8", '#');
	std::cout << "\n The Metrics analysis is being done by \"Metrics\" package.\n It can be viewed from file explorer or Visual Studio.\n\n\n";

}

void Requirement9(std::string &path, std::vector<std::string> &rules)
{
	PrettyHelper::Title("\n REQUIREMENT 9", '#');
	MetricExec me;
	me.setFiles(path, rules);
	
	if (me.returnFiles().size() == 0)
	{
		std::cout << "\n No files with given pattern are present in the path provided\n\n";
		return;
	}

	std::cout << " Press any key to begin analysis . . .";
	std::cin.get();

	PrettyHelper::Title("\n Begin Analysis :", '=');
	me.getAnalysis(me.returnFiles());
	std::cout << "\n Analysis Complete\n\n";
}

// Prints path and rules parameter which have been passed to the main function.
void printPathNRules(std::string &path, std::vector<std::string> &rules)
{
	std::cout << "\n PATH : " << path;
	std::cout << "\n RULES :";
	for (size_t index = 0; index < rules.size(); index++)
	{
		std::cout << "\n " << index << " : " << rules[index];
	}
	std::cout << "\n\n";
}

int main(int argc, char* argv[])
{
	int nuArgs = argc - 1;
	std::string path;
	std::vector<std::string> rules;
	
	switch (nuArgs)
	{
	case 0:
		path = "../Test";
		rules = { "*.h","*.cpp" };
		break;
	case 1:
		path = argv[1];
		rules = { "*.h","*.cpp" };
		break;
	default:
		path = argv[1];
		for (int index = 2; index < argc; index++)
			rules.push_back(argv[index]);
		break;
	}

	printPathNRules(path, rules);

	Requirement3to8();
	Requirement9(path, rules);
	return 1;
}
#endif