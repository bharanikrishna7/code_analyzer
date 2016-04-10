#ifndef TT_H
#define TT_H
//////////////////////////////////////////////////////////////////////////////
// TypeTable.h - Provides a structure for Partial Type Table				//
// Version :	  1.0														//
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Parallel Dependency Analysis								//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ------------------
* This module provides structure and methods for partial type table.
*
* Also provides a entry structure which will be used by the partial type table.
*
* Public Interface:
* -----------------
* Structure :-
* ------------
* Entry																					// Structure which helps to hold the partial Type Table objects.
*
* Methods :-
* ----------
* void showTable();																		// Will display the Type Table contents on Console
* void setNameSpace();																	// Will set the namespace field of the objects in the Type Table
* void setFileName(std::string fileNM);													// Will set filename for all the objects in the Type Table.
* void addEntry(std::string name, std::string type);									// Will add object to Type Table (takes only 2 fields : name, type)
* void addEntry(std::string name, std::string type, std::string NS);					// Will add object to Type Table (takes 3 fields : name, type, namespace) ** {future use}
* void nameSpaceHelper(size_t begin, size_t end, std::string NS);						// Helper function for setNameSpace function
* std::vector<Entry> pTypeTable();
*
* Required files:
* ---------------
* Utilities.h, Utilities.cpp, TypeTable.h, TypeTable.cpp
*
* Build command:
* --------------
* devenv TypeTable.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 30 March 16
* - first release
*
* Credits:
* --------
* I would like to give credits to Prof. Jim W Fawcett Syracuse University
* , CST 4-187) for providing a started code and explaining how Parallel 
* Dependency Analysis project should work.
*
*/
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include "../AST/AST.h"
#include "../Utilities/Utilities.h"

struct Entry
{
	std::string _name;
	std::string _namespace;
	std::string _type;
	std::string _filename;

	std::string show()
	{
		std::ostringstream temp;
		temp << "(";
		temp << _name;
		temp << ", ";
		temp << _namespace;
		temp << ", ";
		temp << _type;
		temp << ", ";
		temp << _filename;
		temp << ")";
		return temp.str();
	}
};

// TypeTable Class declaration
class TTable {
public:
	TTable() { addEntry("Global", "namespace", "-"); }
	~TTable() {}
	void showTable();
	void setNameSpace();
	void setFileName(std::string fileNM);
	void addEntry(std::string name, std::string type);
	void addEntry(std::string name, std::string type, std::string NS);
	void nameSpaceHelper(size_t begin, std::string NS);
	std::vector<Entry> pTypeTable();
private:
	std::vector<Entry> _TypeTable;
};

#endif