//////////////////////////////////////////////////////////////////////////////
// DependencyyAnalysis.h -	Analyze files to check dependency				//
// Version :	  1.0														//
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Parallel Dependency Analysis								//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////

#include "DependencyAnalysis.h"
#include <algorithm>

using namespace Scanner;
using PrettyHelper = Utilities::StringHelper;

//////////////////////////////////////////////////////////////////////////////////
// Will return the position of the element which has filename "_key"
// in the results vector. If no such vector is found then it will
// return the sze of the vector results.
size_t DependencyAnalysis::findVal(std::string _key) {
	for (size_t index = 0; index < results.size(); index++) {
		if (results[index]._filename == _key)
			return index;
	}
	return results.size();
}

//////////////////////////////////////////////////////////////////////////////////
// Return the address pointer for results.
std::vector<resultType>* DependencyAnalysis::returnResults() {
	return &results;
}

//////////////////////////////////////////////////////////////////////////////////
// Will be similar to stringbuilder append (will be used to append to results)
void DependencyAnalysis::AppendResult(std::string _string1, std::string _string2) {
	// If an entry for same filename already exists
	if (findVal(_string1) < results.size()) {
		size_t pos = findVal(_string1);
		if (std::find(results[pos]._tokens.begin(), results[pos]._tokens.end(), _string2) == results[pos]._tokens.end()) {
			// No instance of same token found
			results[findVal(_string1)]._tokens.push_back(_string2);
		}
		return;
	}

	// If an entry for same filename doesnot exist
	resultType _entry;
	_entry._filename = _string1;
	_entry._tokens.push_back(_string2);
	results.push_back(_entry);
}

//////////////////////////////////////////////////////////////////////////////////
// Will process token "tok"
void DependencyAnalysis::processTok(std::string& tok) {
	if (TypeTable.find(tok) == TypeTable.end()) {
		return;
	}

	// Found the token in MAP {TypeTable}
	auto val = TypeTable.at(tok);
	AppendResult(val._filename, val._name);
	if (val._type == "function (member)")
		AppendResult(val._filename.substr(0, val._filename.size() - 3) + "h", val._name);
}

///////////////////////////////////////////////////////
// Will display the results on console
void DependencyAnalysis::ShowResult() {
	// Return the function if the size is 0
	if (results.size() == 0) {
		std::cout << "\n The current File doesnot depend on any other files \n";
		return;
	}
	std::string _filename;

	PrettyHelper::Title("List of Dependant files and Tokens", '-');
	//Print the vectors
	for (auto entry : results) {
		_filename = entry._filename;
		std::cout << "\n\n " << std::left << std::setw(75) << "FileName => " + _filename;
		std::cout << "\n *******************************************************************************";	
		std::cout << "\n Tokens : \n ";
		bool first = true;
		for (auto _token : entry._tokens) {
			if (first) {
				std::cout << _token;
				first = false;
			}
			else {
				std::cout << " , " << _token;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////
// Will process the file and search for all the dependencies
void DependencyAnalysis::processFile(std::string filename) {
	Toker toker;
	std::fstream in(filename);
	// Check if the filename is valid
	if (!in.good())
	{
		std::cout << "\n  can't open file " << filename << "\n\n";
		return;
	}

	toker.attach(&in);
	do
	{
		std::string tok = toker.getTok();
		processTok(tok);										// Check for match in Final Type Table and perform corresponding action
	} while (in.good());
}


//////////////////////////////////////////////////////////////////////////////////
// Copy an unordered MAP to TypeTable [the entries will be copied by reference]
void DependencyAnalysis::copyMAP(std::unordered_map <std::string, Entry>* fTypeTable) {
	// Sequentially store all the entries in the typetable in parameter to our TypeTable
	for (auto entry : *fTypeTable) {
		std::string key = entry.first;
		Entry _entry = entry.second;
		TypeTable.insert({ key, _entry });
	}
}

#ifdef TEST_DEPANALYSIS

//////////////////////////////////////////////////////////////////////////////////
// Add entries to the type table (only for testing)
// ** optional function, since we should not expose a function to users
// ** which directly manipulate data that should otherwise not be accessible.
// ** Will be used only for package testing.
void DependencyAnalysis::addToTypeTable(std::string _name, std::string _type, std::string _filename, std::string _namespace) {
	Entry entry;
	entry._name = _name;
	entry._type = _type;
	entry._filename = _filename;
	entry._namespace = _namespace;
	std::cout << "\n " << entry.show();
	TypeTable.insert({ _name, entry });
}

///////////////////////////////////
// TEST STUB
void main() {
	DependencyAnalysis AF;

	// Insert elements into typetable

	AF.addToTypeTable("Color", "enum", "FileNM_1", "global");
	AF.addToTypeTable("trial", "namespace", "File__2", "global");
	AF.addToTypeTable("main", "function", "File__3", "trial");
	AF.addToTypeTable("person", "class", "FileNM_1", "global");
	std::cout << "\n Inserting into table is complete\n\n";
	AF.processFile("../Test/testScope.cpp");
	AF.ShowResult();
	std::cout << "\n\n "; 
	system("pause");
}

#endif //TEST_DependencyAnalysis