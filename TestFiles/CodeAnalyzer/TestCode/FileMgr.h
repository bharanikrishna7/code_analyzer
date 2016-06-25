#ifndef FILEMGR_H
#define FILEMGR_H
//////////////////////////////////////////////////////////////////////////////
// FileMgr.h - find files matching specified patterns on a specified path	//
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
 * This module provices will provide means to iterate through windows
 * directories and retreive the file names (rather path) as per the
 * pattern rules provided by the user and the base directory path.
 *
 * Public Interface:
 * -----------------
 * void addPattern(const std::string& patt){};		// Add search pattern rule
 * void search(){};									// Perform search and store results in the datastore
 * void find(const std::string& path){};			// This is where actual search happens.
 *
 * Required files:
 * ---------------
 * FileMgr.h FileMgr.cpp
 *
 * Build command:
 * --------------
 * devenv FileMgr.sln /rebuild debug
 *
 * Maintenance History:
 * --------------------
 * ver 1.0 : 17 March 16
 * - first release
 *
 * Credits:
 * --------
 * I would like to give credits to Prof. Jim W Fawcett Syracuse University
 * , CST 4-187) for providing a started code and explaining how Parallel 
 * Dependency Analysis project should work.
 *
 */
#include "FileSystem.h"
#include "DataStore.h"
#include <iostream>

class FileMgr
{
public:
  using iterator = DataStore::iterator;
  using patterns = std::vector<std::string>;

  FileMgr(const std::string& path, DataStore& ds, const bool& paramV) : path_(path), store_(ds), VERBOSE(paramV)
  {
    patterns_.push_back("*.*");
  }

  void addPattern(const std::string& patt)
  {
    if (patterns_.size() == 1 && patterns_[0] == "*.*")
      patterns_.pop_back();
    patterns_.push_back(patt);
  }

  void search()
  {
	  if (path_[path_.length() - 1] == '\\')				// it will remove the trailing '\' in the provided file path
		  path_ = path_.substr(0, path_.length() - 1);

	  if (VERBOSE) {
		  std::cout << "\n Searching for files in Path :" << path_;
		  std::cout << "\n\n File List";
		  std::cout << "\n =========";
	  }
	  find(path_);
	  if (VERBOSE) {
		   std::cout << "\n\n";
	  }
  }

  void find(const std::string& path)
  {
    std::string fpath = FileSystem::Path::getFullFileSpec(path);
	

	if (VERBOSE) { std::cout << "\n  ++ " << fpath; }
    
	for (auto patt : patterns_)  
    {                            
      std::vector<std::string> files = FileSystem::Directory::getFiles(fpath, patt);
	  
	  for (auto f : files)				
      {
		  if (VERBOSE) { std::cout << "\n  --   " << f; }
        store_.save(fpath+f);
      }
    }
    std::vector<std::string> dirs = FileSystem::Directory::getDirectories(fpath);
    for (auto d : dirs)
    {
      if (d == "." || d == "..")
        continue;
      std::string dpath = fpath + "\\" + d;  
      find(dpath);
    }
  }
private:
  std::string path_;
  DataStore& store_;
  patterns patterns_;
  bool VERBOSE;
};

#endif
