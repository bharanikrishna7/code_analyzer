//////////////////////////////////////////////////////////////////////////////
// TypeTable.h - Provides a structure for Partial Type Table				//
// Version :	  1.0														//
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Parallel Dependency Analysis								//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////

#include "TypeTable.h"

using namespace Utilities;

//////////////////////////////////////////////////////////////////////////////////////
// Pushback values into the partial type table {to be used when namespace is known}
void TTable::addEntry(std::string name, std::string type, std::string NS) {
	Entry newEntry;
	newEntry._name = name;
	newEntry._type = type;
	newEntry._namespace = NS;
	_TypeTable.push_back(newEntry);
}

/////////////////////////////////////////////////
// Pushback values into the partial type table
void TTable::addEntry(std::string name, std::string type) {
	Entry newEntry;
	newEntry._name = name;
	newEntry._type = type;
	newEntry._namespace = "";
	_TypeTable.push_back(newEntry);
}

/////////////////////////////////////////////////
// Return the partial type table
std::vector<Entry> TTable::pTypeTable() {
	return _TypeTable;
}

/////////////////////////////////////////////////
// Print the partial type table to console
void TTable::showTable() {
	StringHelper::Title("Partial Type Table", '-');
	std::cout << "\n " << std::left << std::setw(25) << "Name";
	std::cout << std::left << std::setw(20) << "Type" << std::left << std::setw(20) << "Namespace";
	std::cout << std::left << std::setw(15) << "FileName" << std::left << "\n";
	std::cout << " ********************************************************************************************************************\n";

	for (auto entry : _TypeTable) {
		std::cout << " " << std::left << std::setw(25) << entry._name;
		std::cout << std::left << std::setw(20) << entry._type << std::left << std::setw(20) << entry._namespace;
		std::cout << std::left << std::setw(15) << entry._filename << std::left << "\n";
	}
}

//////////////////////////////////////////////////////////
// Set the filename to all the values in the typetable
void TTable::setFileName(std::string fileNM) {
	Entry* curr;
	for (size_t index = 0; index < _TypeTable.size(); index++) {
		curr = &_TypeTable[index];
		curr->_filename = fileNM;
	}
}

/////////////////////////////////////////////////////////////////////
// Set the namespace field in all the entries of the typetable
// This method is required since we need to have a complete AST
// ready to figure out namespace but we start creating the type
// table even simultaneously while parsing. Hence this is required
void TTable::setNameSpace() {
	Entry *curr;
	// Set the 1st element's namespace to global
	_TypeTable[0]._namespace = "global";
	for (size_t index = 0; index < _TypeTable.size(); index++) {
		curr = &_TypeTable[index];
		if (curr->_type == "namespace") {
			nameSpaceHelper(index + 1, curr->_name);
		}
	}
	// Remove the elements with "namespace EOS" type from _TypeTable
	for (size_t index = 0; index < _TypeTable.size(); index++) {
		if (_TypeTable[index]._type == "namespace EOS") {
			_TypeTable.erase(_TypeTable.begin() + index);
			index--;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Helper function for setting namespace field in the typetable entires
void TTable::nameSpaceHelper(size_t begin, std::string NS) {
	Entry *curr;
	int depth = 1;
	for (size_t index = begin; index < _TypeTable.size(); index++) {
		if (depth <= 0) { break; };
		curr = &_TypeTable[index];
		curr->_namespace = NS;
		if (curr->_type == "namespace") { depth++; }
		if (curr->_type == "namespace EOS") { depth--; }
	}
}

#ifdef TEST_TT
///////////////////////////////////
// TEST STUB
void main() {
	TTable table;
	// add entries
	table.addEntry("e1", "namespace");
	table.addEntry("e2", "type1");
	table.addEntry("e3", "type2");
	table.addEntry("e4", "namespace");
	table.addEntry("e5", "type1");

	table.setNameSpace();
	table.setFileName("filename_example");
	table.showTable();
	system("pause");
}

#endif