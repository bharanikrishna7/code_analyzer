#ifndef FILEMGR_H
#define FILEMGR_H
//////////////////////////////////////////////////////////////////////////////
// FileMgr.h - find files matching specified patterns on a specified path	//
// Version :	  1.1                                                       //
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
 * ver 1.1 : 25 June 16
 * - Added Support for VERBOSE Mode
 * - Fixed a rare bug which would cause incorrect detection of files
 * 
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

#include "DataStore.h"
#include "FileSystem.h"

#include <iostream>

// FileMgr Class
class FileMgr
{
public:
  using iterator = DataStore::iterator;
  using patterns = std::vector<std::string>;
  /* FileMgr Constructor */
  FileMgr(const std::string& path, DataStore& ds, const bool& paramV) : path_(path), store_(ds), VERBOSE(paramV)
  {
    patterns_.push_back("*.*");
  }
  /* Add patterns to FileMgr */
  void addPattern(const std::string& patt)
  {
    if (patterns_.size() == 1 && patterns_[0] == "*.*")
      patterns_.pop_back();
    patterns_.push_back(patt);
  }
  /* Search for files with specified pattern in the basedir and subdirs */
  void search()
  {
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

  /* Search for files and directories in a directory. Store the files and run find for subdirs */
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
      std::string dpath = fpath + d + "\\";
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
