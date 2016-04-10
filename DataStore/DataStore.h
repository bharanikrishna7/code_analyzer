#ifndef DATASTORE_H
#define DATASTORE_H
//////////////////////////////////////////////////////////////////////////////
// DataStore.h - Store results of file search								//
// Version :	  1.0                                                       //
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Parallel Dependency Analysis								//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////
/*
 * Module Operations: 
 * ------------------
 * This module provides a datastructure DataStore which will hold vector
 * of strings.
 *
 * Public Interface:
 * -----------------
 * Methods :-
 * save				// Will push back some string to the store
 * begin			// Returns the start position iterator of store vector
 * end				// Returns the end position iterator of store vector
 *
 * Required files:
 * ---------------
 * DataStore.h, Datastore.cpp
 *
 * Build command:
 * --------------
 * devenv DataStore.sln /rebuild debug
 *
 * Maintenance History:
 * --------------------
 * ver 1.0 : 17 March 16
 * - first release
 *
*/
#include <vector>

class DataStore
{
public:
  using iterator = std::vector<std::string>::iterator;
  inline void save(const std::string& filespec) { store.push_back(filespec);  }
  inline void save(const std::string& filename, const std::string& path) {}
  iterator begin() { return store.begin(); }
  iterator end() { return store.end(); }
private:
  std::vector<std::string> store;
};
#endif
