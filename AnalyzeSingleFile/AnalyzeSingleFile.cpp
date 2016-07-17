//////////////////////////////////////////////////////////////////////////////
// AnalyzeSingleFile.h - analyzes a files and provides metrics				//
// Version :	  2.1                                                       //
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Parallel Dependency Analysis								//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////

#include "AnalyzeSingleFile.h"

using namespace Utilities;

////////////////////////////////////////////
/* Will set the value of VERBOSE variable */
void AnalyzeSingleFile::setVerbose(bool paramV) {
	VERBOSE = paramV;
}

////////////////////////////////////////////
/* Will Analyze a file with path "filename". 
Analyze -> Parse, Print AST, Print Scopes */
void AnalyzeSingleFile::analyze(std::string filename)
{
	if (VERBOSE) { std::cout << "\n [VERBOSE] : Validating File : " << filename << "\n"; }
	std::string fileSpec = FileSystem::Path::getFullFileSpec(filename);
	if (!fileExists(fileSpec))
	{
		std::cout << "\n Specified file Not found";
		std::cout << "\n Are you sure file \"" << fileSpec << "\" exists ?";
		std::cout << "\n ";
		system("pause");
		return;
	}

	ConfigParseToConsole configure;
	configure.setVerbose(VERBOSE);

	Parser* pParser = configure.Build();
	try {
		if (pParser) { if (!configure.Attach(fileSpec)) { std::cout << "\n could not open file" << fileSpec << "\n "; } }
		else { std::cout << "\n Parser not built \n "; }

		while (pParser->next())
			pParser->parse();

		if (_ShowResult) { StringHelper::Title("\n File : " + fileSpec, '='); }

		Repository* repo = Repository::getInsance();
		element* rootElem = repo->getTree()->getRoot();									// Retriving Root Element to set its linecount and Complexity.
		rootElem->lineCountEnd = repo->lineCount();										// Set Line count(s)
		rootElem->lineCount = rootElem->lineCountEnd - rootElem->lineCountBegin + 1;
		rootElem->complexity = repo->getTree()->getComplexity(rootElem);				// Set Complexity
		if (_ShowResult) { printTree(repo); }											// Printing AST on the command prompt
		if (_ShowResult) { printFunctions(repo); }										// Printing file scopes on the command prompt
		repo->getTable()->addEntry("EOS", "namespace EOS");								// Add Namespace EOS for Global Namespace
		setTypeTableFields(repo, fileSpec);												// Set some values in TypeTable (Filename and Namespace)
		setTable(repo);																	// Set the typeTable in Metrics to the typetable obtained from Repository
																						// Doing this will prevent keeping entire repo instance in the memory and just hold the partial typetable.
		if (_ShowResult) {
			printTable(repo);															// Printing the TypeTable
			std::cout << "\n";
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n [EXCEPTION] : " << ex.what() << "\n\n";
	}
}

/////////////////////////////////////////////////////////////////
/* Set the _ShowResult parameter. True = Shows Result on Console, 
False = Doesnot Show Result on Console */
void AnalyzeSingleFile::ShowResult(bool paramV) {
	_ShowResult = paramV;
}

///////////////////////////////////////////
/* Will set some fields in the typetable */
void AnalyzeSingleFile::setTypeTableFields(Repository* repo, std::string fileSpec) {
	if (VERBOSE) { std::cout << "\n [VERBOSE] : Set Namespace and Filename for file : " << fileSpec << "\n"; }
	repo->getTable()->setFileName(fileSpec);
	repo->getTable()->setNameSpace();
}

/////////////////////////////////////////////////////
/* Will return the partial type table by reference */
std::vector<Entry>* AnalyzeSingleFile::getTable() {
	return &partialTT;
}

/////////////////////////////////////////////////////////////////////
/* Will set partialTT value as the repository's partial type table */
void AnalyzeSingleFile::setTable(Repository* repo) {
	if (VERBOSE) { std::cout << "\n [VERBOSE] : Set Partial Type Table using Repository's Type Table\n"; }
	partialTT = repo->getTable()->pTypeTable();
}

//////////////////////////
/* Will print TypeTable */
void AnalyzeSingleFile::printTable(Repository* repo) {
	if (VERBOSE) { std::cout << "\n [VERBOSE] : Printing Table from Repo\n"; }
	repo->getTable()->showTable();
}

///////////////////////////////////
/* Will print the AST to console */
void AnalyzeSingleFile::printTree(Repository* repo)
{
	if (VERBOSE) { std::cout << "\n [VERBOSE] : Printing AST to Console\n"; }
	StringHelper::Title("Abstract Syntax Tree", '-');
	std::cout << "\n The tree format is:\n (ScopeType, ScopeName, Begin Line, End Line, Scope Lines, Complexity)\n";
	repo->getTree()->treeWalk(repo->getTree()->getRoot());
	std::cout << "\n\n";
}

////////////////////////////////////////////////////////////////////////
/* Will print the Function Type Elements in the ScopeStack to console */
void AnalyzeSingleFile::printFunctions(Repository* repo)
{
	if (VERBOSE) { std::cout << "\n [VERBOSE] : Printing Funtion List to Console\n"; }
	std::vector<std::string> vec;
	repo->getTree()->scopes(repo->getTree()->getRoot(), vec);

	StringHelper::Title("Functions", '-');
	std::cout << "\n " << std::left << std::setw(30) << "Name";
	std::cout << std::left << std::setw(15) << "Begin" << std::left << std::setw(15) << "End";
	std::cout << std::left << std::setw(15) << "Lines" << std::left << std::setw(15) << "Complexity" << "\n";
	std::cout << " ************************************************************************************\n";
	for (auto vector : vec)
	{
		std::vector<std::string> _scope = StringHelper::split(vector);
		if (_scope[0] != "function")
			continue;

		size_t index;
		bool first = false;
		for (index = 1; index < _scope.size(); index++)
		{
			if (!first)
			{
				std::cout << " " << std::left << std::setw(30) << StringHelper::lrtrim(_scope[index]);
				first = true;
			}
			else
			{
				std::cout << std::left << std::setw(15) << StringHelper::lrtrim(_scope[index]);
			}
		}
		std::cout << "\n";
	}
	std::cout << "\n\n";
}

////////////////////////////////////////////////////////
/* Will check if file the with path 'filename' exists */
bool AnalyzeSingleFile::fileExists(const std::string& filename)
{
	struct stat buf;
	if (stat(filename.c_str(), &buf) != -1)
	{
		return true;
	}
	return false;

}

#ifdef TEST_ANALYZESINGLEFILE
///////////////
/* TEST STUB */
int main()
{
	Timer time;
	time.StartClock();
	AnalyzeSingleFile asf;
	asf.ShowResult(true);
	asf.setVerbose(true);
	asf.analyze("../TestFiles/AnalyzeSingleFile/test.h");
	std::cout << "\n [Execution Time] : " << time.StopClock() << " ms";
	std::cout << "\n ";
	system("pause");
}
#endif 

