//////////////////////////////////////////////////////////////////////////////
// FileMgr.h - find files matching specified patterns on a specified path	//
// Version :	  1.0                                                       //
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Code Parser with AST										//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////

#include "FileMgr.h"
#include "DataStore.h"
#include <iostream>

#ifdef TEST_FILEMGR

int main()
{
  std::cout << "\n  Testing FileMgr";
  std::cout << "\n =================";

  DataStore ds;
  FileMgr fm("..", ds);
  //fm.addPattern("*.h");
  //fm.addPattern("*.c*");
  //fm.addPattern("*.partial");
  fm.search();

  std::cout << "\n\n  contents of DataStore";
  std::cout << "\n -----------------------";
  for (auto fs : ds)
  {
    std::cout << "\n  " << fs;
  }
  std::cout << "\n\n";
  return 0;
}
#endif
