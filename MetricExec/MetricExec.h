#ifndef MERTRICEXEC_H
#define MERTRICEXEC_H
//////////////////////////////////////////////////////////////////////////////
// MetricExec.h - analyzes multiple files and provides metrics				//
// Version :	  1.0                                                       //
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Code Parser with AST										//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////
/*
 *  Module Operations:
 *  ------------------
 *  This module will provide functions to analyze multiple files by leveraging
 *  the functions from the Metrics package.
 *
 *  The main funciton of this package will be the Test Suite for OOD Spring 2016
 *  Project 2. When main is disabled this can just be used like any other package
 *  in this solution.
 *
 *  Public Interface:
 *  -----------------
 *  void setFiles(std::string path, std::vector<std::string> pattern);		// Using the path and pattern get file list and store in files
 *  void getAnalysis(std::vector<std::string> _files);						// Perform analysis for the files "_files"
 *  std::vector<std::string> returnFiles();									// Returns the value stored in "files" vector
 *
 *  Required files:
 *  ---------------
 *  MetricExec.h, MetricExec.cpp, Metrics.h, Metrics.cpp, ConfigureParser.h,
 *  ConfigureParser.cpp, Parser.h, Parser.cpp, ActionsAndRules.h, ActionsAndRules.cpp,
 *  SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp, Utilities.h, Utilities.cpp
 *
 *  Build command:
 *  --------------
 *  devenv MetricExec.sln /rebuild debug
 *
 *  Maintenance History:
 *  --------------------
 *  ver 1.0 : 17 March 16
 *  - first release
 *
 */

#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include "../SemiExp/itokcollection.h"
#include "../SemiExp/itokcollection.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../ScopeStack/ScopeStack.h"
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../DataStore/DataStore.h"
#include "../Metrics/Metrics.h"
#include "../FileMgr/FileMgr.h"
#include "../SemiExp/SemiExp.h"
#include "../Parser/Parser.h"
#include "../AST/AST.h"

class MetricExec {
public:
	void setFiles(std::string path, std::vector<std::string> pattern);
	void getAnalysis(std::vector<std::string> _files);
	std::vector<std::string> returnFiles();
private:
	std::vector<std::string> files;
};

#endif