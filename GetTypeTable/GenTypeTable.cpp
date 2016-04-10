//////////////////////////////////////////////////////////////////////////////
// GenTypeTable.h - Generates Final Type Table [MAP]						//
// Version :	  1.0														//
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Parallel Dependency Analysis								//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////

#include "GenTypeTable.h"

using PrettyHelper = Utilities::StringHelper;
using WorkResult = std::string;

////////////////////////////////////////////
// Will set the value of VERBOSE variable
void GenTypeTable::setVerbose(bool paramV) {
	VERBOSE = paramV;
}

/////////////////////////////////////////
// Will copy vector path into files
void GenTypeTable::setFiles(std::vector<std::string> _files)
{
	files.swap(_files);
}

////////////////////////////////////////////////////////////////////////////
// Will perform analysis for each and every file whose path is present in
// the vector files, of the GenTypeTable variable calling this function.
void GenTypeTable::getAnalysis(std::vector<std::string> files)
{
	Metrics m;
	for (auto file : files) {
		m.setVerbose(VERBOSE);
		m.analyze(file);
		// The table retreived above must be appended to the unordered_map
		storepTypeTables(*m.getTable());
	}
}

////////////////////////////////////////////////////////////////////////
// Store the partial type tables from metrics analysis in pTypeTables.
void GenTypeTable::storepTypeTables(std::vector<Entry> pTT) {
	pTypeTables.push_back(pTT);
}

////////////////////////////////////////////////////////////////////////////////
// Will make the final Type Table using the partial type tables.
/* This is done instead of integrating it with analysis since this would
* prevent the analysis to be done parallelly (if in future we would want
* to make it that way) as all of them would require access to the fTypeTable
* which will prohibit it from being true parallel implementation.
*
* TO-DO:
* ------
* Make it parallel.
*/
void GenTypeTable::makeDepTable() {
	std::vector<Entry> *cur;
	Entry *row;
	for (size_t index = 0; index < pTypeTables.size(); index++) {
		cur = &pTypeTables.at(index);
		for (size_t _index = 0; _index < cur->size(); _index++) {
			row = &cur->at(_index);
			std::string key = row->_name;
			fTypeTable.insert({ key, *row });
		}
	}

	//Remove global namespace from the Final Type Table
	if (fTypeTable.at("Global")._type == "namespace")
		fTypeTable.erase("Global");
}

//////////////////////////////////////////////////////
// Prints the Final(Complete) Type Table to console.
void GenTypeTable::showFinTable() {
	std::vector<Entry> entries;
	// Get List of all the entries in the MAP to vector since we can print vectors easily
	for (auto val : fTypeTable) {
		entries.push_back(val.second);
	}
	
	//Create a nice readable title
	PrettyHelper::Title("\n Final Type Table", '#');
	std::cout << "\n " << std::left << std::setw(25) << "Name";
	std::cout << std::left << std::setw(20) << "Type" << std::left << std::setw(20) << "Namespace";
	std::cout << std::left << std::setw(15) << "FileName" << std::left << "\n";
	std::cout << " *************************************************************************************************************************\n";
	//Print the vectors
	for (auto entry : entries) {
		std::cout << " " << std::left << std::setw(25) << entry._name;
		std::cout << std::left << std::setw(20) << entry._type << std::left << std::setw(20) << entry._namespace;
		std::cout << std::left << std::setw(15) << entry._filename << std::left << "\n";
	}
	
}

///////////////////////////////////////////////////
// Will return the pointer to the final TypeTable
std::unordered_map <std::string, Entry>* GenTypeTable::returnFinTable() {
	return &fTypeTable;
}

/////////////////////////////////////////////////////////////
// Will return the vector files of the MetricsExec variable.
std::vector<std::string> GenTypeTable::returnFiles()
{
	return files;
}

#ifdef TEST_GENTT
///////////////////////////
// TEST STUB
void main()
{
	PrettyHelper::Title("\n TESTING TYPETABLE CREATION", '#');
	GenTypeTable me;
	me.setVerbose(true);
	// Create a vector of files which will be used as parameter to setfiles.
	std::vector<std::string> _files;
	_files.push_back("C:\\Users\\bhara\\Documents\\Project2\\Test\\test_1.cpp");
	_files.push_back("C:\\Users\\bhara\\Documents\\Project2\\Test\\Test_2.cpp");
	_files.push_back("C:\\Users\\bhara\\Documents\\Project2\\Test\\testScope.cpp");
	_files.push_back("C:\\Users\\bhara\\Documents\\Project2\\Test\\SubDir\\SubDirTest.cpp");

	me.setFiles(_files);

	if (me.returnFiles().size() == 0)
	{
		std::cout << "\n No files with given pattern are present in the path provided\n\n";
		return;
	}

	std::cout << "\n\n Press any key to begin analysis . . .";
	std::cin.get();

	PrettyHelper::Title("\n Begin Analysis :", '=');
	me.getAnalysis(me.returnFiles());
	me.makeDepTable();
	std::cout << "\n\n |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||";
	std::cout << "\n |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n";
	me.showFinTable();
	std::cout << "\n Analysis Complete\n\n ";
	
	system("pause");
}

#endif // TEST_GENTT