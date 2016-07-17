//////////////////////////////////////////////////////////////////////////////
// CodeAnalyzer.h - Create Type Table and Analyze all files					//
// Version :	  2.0														//
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Parallel Dependency Analysis								//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////

#include "CodeAnalyzer.h"

using namespace Utilities;
using namespace Scanner;

// Parameters to control thread debugger and ShowSingleResults
bool ShowSingleResults = false;
bool SetThreadDebugger = false;

/* Method to set the _files values, using a basedir and patterns
*/
void CodeAnalyzer::setFiles(std::string path, std::vector<std::string> pattern) {
	std::string _path = path;

	DataStore ds;
	FileMgr fm(path, ds, VERBOSE);
	for (auto patt : pattern)
	{
		fm.addPattern(patt);
	}

	fm.search();
	
	for (auto file : ds)
	{
		_files.push_back(file);
	}
}

/* Method to set VERBOSE Mode. True = VERBOSE Mode On, 
  False = VERBOSE Mode Off
*/
void CodeAnalyzer::setVerbose(bool paramV) {
	VERBOSE = paramV;
}

/* Method to Generate Final TypeTable by processing the
  files in _files.
*/
void CodeAnalyzer::GenerateFinalTypeTable() {
	// Unfortunately Unable to use ThreadPool here.
	for (auto file : _files) {
		AnalyzeSingleFile asf;
		asf.setVerbose(VERBOSE);
		asf.ShowResult(ShowSingleResults);
		asf.analyze(file);
		AppendTypeTable(asf.getTable());
	}
}

/* Method to append the processed Partial TypeTable to Final TypeTable */
void CodeAnalyzer::AppendTypeTable(std::vector<Entry>* pTT) {
	for (size_t i = 0; i < pTT->size(); i++) {
		bool notAlnum = true;
		for (size_t j = 0; j < pTT->at(i)._name.size(); j++) {
			if (std::isalnum(pTT->at(i)._name[j])) {
				notAlnum = false;
				break;
			}
		}

		if (notAlnum) {
			continue;
		}

		if (pTT->at(i)._name == "Global" || pTT->at(i)._name == "main") {
			continue;
		}

		bool isPresent = false;
		std::pair<std::multimap<std::string, Entry>::iterator, std::multimap<std::string, Entry>::iterator> iter;
		iter = _FinalTypeTable.equal_range(pTT->at(i)._name);
		for (std::multimap<std::string, Entry>::iterator it = iter.first; it != iter.second; it++) {
			if ((*it).second._name == pTT->at(i)._name && (*it).second._type == pTT->at(i)._type
				&& (*it).second._namespace == pTT->at(i)._namespace && (*it).second._filename == pTT->at(i)._filename) {
				isPresent = true;
				break;
			}
		}

		if (isPresent) {
			continue;
		}

		_FinalTypeTable.insert(std::pair<std::string, Entry>(pTT->at(i)._name, pTT->at(i)));
	}
}

/* Method to Display Final TypeTable on Console
*/
void CodeAnalyzer::ShowFinalTable() {
	StringHelper::Title("Final Type Table", '-');
	std::cout << "\n " << std::left << std::setw(25) << "Name";
	std::cout << std::left << std::setw(20) << "Type" << std::left << std::setw(20) << "Namespace";
	std::cout << std::left << std::setw(15) << "FileName" << std::left << "\n";
	std::cout << " ********************************************************************************************************************\n";

	size_t TotalEntries = 0, UniqueKeys = 0;
	std::string pkey = "";

	for (std::pair<std::string, Entry> entry : _FinalTypeTable) {
		std::cout << " " << std::left << std::setw(25) << entry.second._name;
		std::cout << std::left << std::setw(20) << entry.second._type << std::left << std::setw(20) << entry.second._namespace;
		std::cout << std::left << std::setw(15) << entry.second._filename << std::left << "\n";
		
		TotalEntries++;

		if (pkey != entry.first) {
			pkey = entry.first;
			UniqueKeys++;
		}
	}

	std::cout << "\n\n Total Entries : " << TotalEntries;
	std::cout << "\n Unique Keys : " << UniqueKeys;
}

/* Method to perform Dependency Analysis and generate Final Report
*/
void CodeAnalyzer::DependencyAnalysis() {
	// Multithreading
	ThreadPool<std::pair<std::string, std::vector<ResultType>>> processor;
	processor.setDebugger(SetThreadDebugger);
	processor.init();
	for (std::string file : _files) {
		WorkItem<std::pair<std::string, std::vector<ResultType>>> * wi = new WorkItem<std::pair<std::string, std::vector<ResultType>>>();
			*wi = [=]() {
				std::vector<ResultType> _PartialResult;
				AnalyzeFileDepedency(file, _PartialResult);
				std::string result = "";
				if (VERBOSE) { std::cout << "\n Analysis for file : " << file << " completed"; }
				return std::pair<std::string, std::vector<ResultType>>(file, _PartialResult);
		};
		processor.doWork(wi);
	}

	for (size_t index = 0; index < _files.size(); index++) {
		_FinalResults.push_back(processor.result());
	}
	processor.doWork(nullptr);
	processor.wait();
	
}

/* Will analyze and generate Dependency Analysis report for a file */
void CodeAnalyzer::AnalyzeFileDepedency(std::string file, std::vector<ResultType>& _PartialResult) {
	Toker toker;
	std::fstream in(file);
	// Check if the filename is valid
	if (!in.good())
	{
		std::cout << "\n can't open file " << file << "\n\n";
		return;
	}

	toker.attach(&in);
	do
	{
		std::string tok = toker.getTok();
		ProcessTok(tok, _PartialResult);										// Check for match in Final Type Table and perform corresponding action
	} while (in.good());
}

/* Method to display Dependency Analysis Final Report on console.
*/
void CodeAnalyzer::ShowFinalReport() {
	StringHelper::Title("\n Result Report", '*');
	std::cout << "\n NOTE\t: Tokens with ** suffix may or may not be dependent. This is because there are multiple definitions with same name in different/same file(s)";
	for (std::pair<std::string, std::vector<ResultType>> record : _FinalResults) {
		StringHelper::Title("\n " + record.first, '-');
		for (ResultType depfiles : record.second) {
			std::cout << "\n File\t: " << depfiles._FileName;
			std::cout << "\n Tokens\t:";
			bool first = true;
			for (std::string token : depfiles._tokens) {
				if (first) {
					std::cout << " " << token;
					first = false;
				}
				else {
					std::cout << " ," << token;
				}
			}
			std::cout << "\n";
		}
		std::cout << "\n\n";
	}
}

/* Method to check if a token exists in a file and if exists, 
  add the result to partial result
*/
void CodeAnalyzer::ProcessTok(std::string token, std::vector<ResultType>& _PartialResult) {
	if (_FinalTypeTable.find(token) == _FinalTypeTable.end()) {
		return;
	}

	if (_FinalTypeTable.count(token) > 1) {
		std::pair<std::multimap<std::string, Entry>::iterator, std::multimap<std::string, Entry>::iterator> iter;
		iter = _FinalTypeTable.equal_range(token);

		for (std::multimap<std::string, Entry>::iterator it = iter.first; it != iter.second; it++) {
			if ((*it).second._type == "function (member)") {
				AppendResult(_PartialResult, (*it).second._filename.substr(0, (*it).second._filename.size() - 3) + "h", (*it).second._name+"**");
			}

			if ((*it).second._type != "function") {
				AppendResult(_PartialResult, (*it).second._filename, (*it).second._name);
			}
		}
	}
	else {
		std::pair<std::multimap<std::string, Entry>::iterator, std::multimap<std::string, Entry>::iterator> iter;
		iter = _FinalTypeTable.equal_range(token);

		for (std::multimap<std::string, Entry>::iterator it = iter.first; it != iter.second; it++) {
			if ((*it).second._type == "function (member)") {
				AppendResult(_PartialResult, (*it).second._filename.substr(0, (*it).second._filename.size() - 3) + "h", (*it).second._name);
			}

			if ((*it).second._type != "function") {
				AppendResult(_PartialResult, (*it).second._filename, (*it).second._name);
			}
		}
	}
}

/* Appends the token and Dependent File Name to Partial Result(s)
*/
void CodeAnalyzer::AppendResult(std::vector<ResultType>& _PartialResult, std::string& DepFileName, std::string& token) {
	size_t index;
	for (index = 0; index < _PartialResult.size(); index++) {
		if (_PartialResult.at(index)._FileName == DepFileName) {
			break;
		}
	}

	if (index < _PartialResult.size()) {
		bool alreadyPresent = false;
		for (size_t i = 0; i < _PartialResult.at(index)._tokens.size(); i++) {
			if (_PartialResult.at(index)._tokens.at(i) == token) {
				alreadyPresent = true;
				break;
			}
		}
		if (!alreadyPresent) {
			_PartialResult.at(index)._tokens.push_back(token);
		}
	}
	else {
		ResultType newRes;
		newRes._FileName = DepFileName;
		newRes._tokens.push_back(token);
		_PartialResult.push_back(newRes);
	}
}

/* Method to Set Files, Generate Final Type Table, Generate Report & 
  Display Final Report on console.
*/
void CodeAnalyzer::Process(const std::string &path, std::vector<std::string> &pattern, bool paramV) {
	setVerbose(paramV);
	setFiles(path, pattern);
	GenerateFinalTypeTable();
	if (VERBOSE) { ShowFinalTable(); }
	DependencyAnalysis();
	ShowFinalReport();
}

/* Test Stub */
#ifdef TEST_CODEANALYZER
int main() {
	StringHelper::Title("Testing Code Analyzer", '=');
	Timer timer;
	timer.StartClock();
	ShowSingleResults = true;
	SetThreadDebugger = true;
	std::vector<std::string> patterns = { "*.cpp", "*.h" };
	CodeAnalyzer ca;
	ca.Process("../TestFiles/CodeAnalyzer/TestCode/", patterns, true);
	std::cout << "\n\n Execution Time : " << timer.StopClock() << " ms \n ";
	system("pause");
}
#endif // TEST_CODEANALYZER
