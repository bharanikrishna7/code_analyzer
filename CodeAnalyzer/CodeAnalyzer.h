#ifndef CODEANALYZER_H
#define CODEANALYZER_H
//////////////////////////////////////////////////////////////////////////////
// CodeAnalyzer.h - Create Type Table and Analyze all files					//
// Version :	  2.0														//
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Parallel Dependency Analysis								//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ------------------
* This module will analyze files in a given path by generating a Final Type Table and then
* performing dependency analysis for each of these files.
*
* VERBOSE MODE:
* -------------
* - Set the VERBOSE variable to enable VERBOSE mode
* - Features:
*		> Will print the partial type tables to console
*		> Will print the AST to console
*		> Will print functions to console
*		> Display the progress ( ex: Pass begin, Pass end, etc)
*
* Public Interface:
* -----------------
* Methods :-
* ----------
* setFiles;							// Finds the files with specified patterns in psecified directory
* setVerbose();						// Sets the value of VERBOSE parameter
* GenerateFinalTypeTable();			// Will process all files and generate a final type table (multimap)
* AppendTypeTable();				// Appends the values of the partial type table obtained by processing each
*									// file to final type table
* void ShowFinalTable();			// Display the final type table on console
* DependencyAnalysis();				// Will analyze dependency to generate report(s) for each file
* AnalyzeFileDepedency();			// Will analyze dependency for a file using final type table
* ProcessTok();						// Process Token for denepdency analysis process for a file
* AppendResult();					// Will append the dependency alanysis report of each file to
*									// _FinalResult
* ShowFinalReport();				// Display the final result report on the console
* Process();						// Will conduct the entire Code Analyzer process (Generate Type Table,
*									// Generate Final Result Report & Display Final Result/Report on console)
*
* Required files:
* ---------------
* FileSystem.h, FileSystem.cpp, GenTypeTable.h, GenTypeTable.cpp, ActionsAndRules.h, ActionsAndRules.cpp,
* ConfigureParser.h, ConfigureParser.cpp, Utilities.h, Utilities.cpp, Tokenizer.h, Tokenizer.cpp,
* Metrics.h, Metrics.cpp, Parser.h, Parser.cpp, AST.h, AST.cpp, itokcollection.h, SemiExp.h, SemiExp.cpp,
* TypeTable.h, TypeTable.cpp, DependencyAnalysis.h, DependencyAnalysis.cpp, ParallelDependencyAnalysis.h,
* ParallelDependencyAnalysis.cpp, Threadpool.h, Threadpool.cpp, Task.h, Task.cpp
*
* Build command:
* --------------
* devenv ParallelDependencyAnalysis.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 2.0 : 25 June 16
* - Renamed package from "ParallelDependencyAnalysis" to "CodeAnalyzer"
* - Modified the entire class for better design
* - Now it supports Final Type Table to have multiple entries 
*   with same token (uses multimap, previously unordered_map was used)
* - The ThreadPool now handles number of ThreadPools to initiate/use. Previously
*   this was done in this package.
* - Change is design paradigm while generating final type table. Now thr threads
*   compute results then we obtain them and append them to FinalResult / Final Report.
* - Added more features to VERBOSE Mode.
*
* ver 1.0 : 6 April 16
* - first release
*
* TO-DO:
* ------
* - Some of the files used are temporarily used. In next release these files will
* - be fixed. So the headers will be updated
*/

// Headers which will be updated in next release
#include "../Temp/FileMgr.h"
#include "../Temp/DataStore.h"
#include "../Temp/FileSystem.h"

#include <map>
#include <vector>

#include "../AST/AST.h"
#include "../Parser/Parser.h"
#include "../Parser/TypeTable.h"
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../ThreadPool/ThreadPool.h"
#include "../SemiExpression/SemiExp.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../ThreadPool/ConcurrentQueue.h"
#include "../SemiExpression/ITokCollection.h"
#include "../AnalyzeSingleFile/AnalyzeSingleFile.h"

// ResultType is application specific.

/* ResultType is used for storing dependant filename and 
associated tokens. It will be used for generating dependency 
analysis report
*/
struct ResultType {
	std::string _FileName;
	std::vector<std::string> _tokens;
};

// CodeAnalyzer Class
class CodeAnalyzer {
public:
	void ShowFinalTable();
	void ShowFinalReport();
	void DependencyAnalysis();
	void setVerbose(bool paramV);
	void GenerateFinalTypeTable();
	void AppendTypeTable(std::vector<Entry>* pTT);
	void setFiles(std::string path, std::vector<std::string> pattern);
	void ProcessTok(std::string token, std::vector<ResultType>& _PartialResult);
	void AnalyzeFileDepedency(std::string file, std::vector<ResultType>& _PartialResult);
	void Process(const std::string &path, std::vector<std::string> &pattern, bool paramV);
	void AppendResult(std::vector<ResultType>& _PartialResult, std::string& DepFileName, std::string& token);
	bool VERBOSE = false;
private:
	std::vector<std::string> _files;
	std::multimap<std::string, Entry> _FinalTypeTable;
	std::vector<std::vector<ResultType>*> _PartialTypeTables;
	std::vector<std::pair<std::string, std::vector<ResultType>>> _FinalResults;
};
#endif