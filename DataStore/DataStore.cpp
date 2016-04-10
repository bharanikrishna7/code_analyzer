//////////////////////////////////////////////////////////////////////////////
// DataStore.h - Store results of file search								//
// Version :	  1.0                                                       //
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Parallel Dependency Analysis								//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////

#include "DataStore.h"
#include <iostream>

#ifdef TEST_DATASTORE
///////////////////////////////////
// TEST STUB
int main()
{
  std::cout << "\n  Testing DataStore";

  DataStore ds;
  ds.save("one");
  ds.save("two");
  ds.save("three");
  DataStore::iterator iter = ds.begin();
  std::cout << "\n  " << (*iter).c_str();

  for (auto item : ds)
  {
    std::cout << "\n  " << item.c_str();
  }
  std::cout << "\n\n";
}

#endif
