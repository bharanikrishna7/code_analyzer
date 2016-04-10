//////////////////////////////////////////////////////////////////////////////
// Metrics.h - analyzes a files and provides metrics						//
// Version :	  1.0                                                       //
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Parallel Dependency Analysis								//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////

#include "Metrics.h"

using PrettyHelper = Utilities::StringHelper;

/////////////////////////////////////////////////
// Will set the value of VERBOSE variable
void Metrics::setVerbose(bool paramV) {
	VERBOSE = paramV;
}

/////////////////////////////////////////////////
// Will Analyze a file with path "filename"
// Analyze -> Parse, Print AST, Print Scopes
void Metrics::analyze(std::string filename)
{
	std::string fileSpec = FileSystem::Path::getFullFileSpec(filename);
	if (!fileExists(fileSpec))
	{
		std::cout << "\n Specified file Not found";
		std::cout << "\n Are you sure file \"" << fileSpec << "\" exists ?";
		std::cout << "\n\n";
		return;
	}

	ConfigParseToConsole configure;
	Parser* pParser = configure.Build();
	try {
		if (pParser) { if (!configure.Attach(fileSpec)) { std::cout << "\n could not open file" << fileSpec << "\n"; }}
		else { std::cout << "\n Parser not built \n"; }

		while (pParser->next())
			pParser->parse();
		
		if (VERBOSE) { PrettyHelper::Title("\n File : " + fileSpec, '='); }

		Repository* repo = Repository::getInsance();
		node* rootElem = repo->getTree()->getRoot();									// Retriving Root Element to set its linecount and Complexity.
		rootElem->lineCountEnd = repo->lineCount();										// Set Line count(s)
		rootElem->lineCount = rootElem->lineCountEnd - rootElem->lineCountBegin + 1;
		rootElem->complexity = repo->getTree()->getComplexity(rootElem);				// Set Complexity
		if (VERBOSE) { printTree(repo); }												//Printing AST on the command prompt
		if (VERBOSE) { printFunctions(repo); }											//Printing file scopes on the command prompt
		repo->getTable()->addEntry("EOS", "namespace EOS");								//Add Namespace EOS for Global Namespace
		setTTFields(repo, fileSpec);													//Set some values in TypeTable (Filename and Namespace)
		setTable(repo);																	//Set the typeTable in Metrics to the typetable obtained from Repository
																						//Doing this will prevent keeping entire repo instance in the memory and just hold the partial typetable.
		if (VERBOSE) {					
			printTable(repo);															//Printing the TypeTable
			std::cout << "\n";
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
}

/////////////////////////////////////////////////
// Will set some fields in the typetable
void Metrics::setTTFields(Repository* repo, std::string fileSpec) {
	repo->getTable()->setFileName(fileSpec);
	repo->getTable()->setNameSpace();
}

/////////////////////////////////////////////////
// Will return the partial type table
std::vector<Entry>* Metrics::getTable() {
	return &partialTT;
}

///////////////////////////////////////////////////////////////////
// Will set partialTT value as the repository's partial type table
void Metrics::setTable(Repository* repo) {
	partialTT = repo->getTable()->pTypeTable();
}

/////////////////////////////////////////////////
// Will print TypeTable
void Metrics::printTable(Repository* repo) {
	repo->getTable()->showTable();
}

/////////////////////////////////////////////////
// Will print the AST to console
void Metrics::printTree(Repository* repo)
{
	PrettyHelper::Title("Abstract Syntax Tree", '-');
	std::cout << "\n The tree format is:\n (ScopeType, ScopeName, Begin Line, End Line, Scope Lines, Complexity)\n";
	repo->getTree()->treeWalk(repo->getTree()->getRoot());
	std::cout << "\n\n";
}

//////////////////////////////////////////////////////////////////////
// Will print the Function Type Elements in the ScopeStack to console
void Metrics::printFunctions(Repository* repo)
{
	std::vector<std::string> vec;
	repo->getTree()->scopes(repo->getTree()->getRoot(), vec);
	
	PrettyHelper::Title("Functions", '-');
	std::cout << "\n " << std::left << std::setw(25) << "Name";
	std::cout << std::left << std::setw(15) << "Begin" << std::left << std::setw(15) << "End"; 
	std::cout << std::left << std::setw(15) << "Lines" << std::left << std::setw(15) << "Complexity" << "\n";
	std::cout << " ************************************************************************************\n";
	for (auto vector : vec)
	{
		std::vector<std::string> _scope = PrettyHelper::split(vector);
		if (_scope[0] != "function")
			continue;

		size_t index;
		bool first = false;
		for (index = 1; index < _scope.size(); index++)
		{
			if (!first)
			{
				std::cout << " " << std::left << std::setw(25) << PrettyHelper::lrtrim(_scope[index]);
				first = true;
			}
			else
			{
				std::cout << std::left << std::setw(15) << PrettyHelper::lrtrim(_scope[index]);
			}
		}
		std::cout << "\n";
	}
	std::cout << "\n\n";
}

///////////////////////////////////////////////////////////
// Will check if file the with path 'filename' exists
bool Metrics::fileExists(const std::string& filename)
{
	struct stat buf;
	if (stat(filename.c_str(), &buf) != -1)
	{
		return true;
	}
	return false;

}

#ifdef TEST_METRICS
/////////////////////////////////////////////////
// TEST STUB
int main()
{
	Metrics ma;
	ma.analyze("../Test/TestScope.cpp");
}
#endif 

