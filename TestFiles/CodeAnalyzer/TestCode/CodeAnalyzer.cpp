#include "CodeAnalyzer.h"

using namespace Utilities;
using namespace Scanner;

bool ShowSingleResults = false;
bool SetThreadDebugger = false;

void CodeAnalyzer::setFiles(std::string path, std::vector<std::string> pattern) {
	std::string _path = path;

	DataStore ds;
	// Change this to VERBOSE later
	FileMgr fm(path, ds, true);
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

void CodeAnalyzer::setVerbose(bool paramV) {
	VERBOSE = paramV;
}

void CodeAnalyzer::GenerateFinalTypeTable() {

	// Unfortunately Unable to use ThreadPool here.
	for (auto file : _files) {
		AnalyzeSingleFile asf;
		asf.setVerbose(VERBOSE);
		asf.ShowResult(ShowSingleResults);
		asf.analyze(file);
		AppendTypeTable(asf.getTable());
	}
	
/*
//	Tried to implement multithread, but it throws errors
	ThreadPool<std::string> processor;
	processor.setDebugger(SetThreadDebugger);
	processor.init();
	for (std::string file : _files) {
		WorkItem<std::string> * wi = new WorkItem<std::string>();
		*wi = [=]() {
			AnalyzeSingleFile asf;
			asf.setVerbose(VERBOSE);
			asf.ShowResult(ShowSingleResults);
			asf.analyze(file);
			_PartialTypeTables.push_back(asf.getTable());
			std::string result = "";
			if (VERBOSE) { std::cout << "\n Partial Type Table for file : " << file << " has been generated"; }
			return result;
		};
		processor.doWork(wi);
	}

	for (int i = 0; i < _files.size(); i++) {
		std::cout << processor.result();
	}
	processor.doWork(nullptr);
	processor.wait();

	for (std::vector<Entry>* pTT : _PartialTypeTables) {
		AppendTypeTable(pTT);
	}
*/
}

void CodeAnalyzer::AppendTypeTable(std::vector<Entry>* pTT) {
	for (int i = 0; i < pTT->size(); i++) {
		_FinalTypeTable.insert(std::pair<std::string, Entry>(pTT->at(i)._name, pTT->at(i)));
	}
}

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

void CodeAnalyzer::DependencyAnalysis() {
	// Multithreading
	ThreadPool<std::string> processor;
	processor.setDebugger(SetThreadDebugger);
	processor.init();
	for (std::string file : _files) {
		WorkItem<std::string> * wi = new WorkItem<std::string>();
			*wi = [=]() {
			AnalyzeFileDepedency(file);
			std::string result = "";
			if (VERBOSE) { std::cout << "\n Analysis for file : " << file << " completed"; }
			return result;
		};
		processor.doWork(wi);
	}

	for (int i = 0; i < _files.size(); i++) {
		std::cout << processor.result();
	}
	processor.doWork(nullptr);
	processor.wait();
	
}

void CodeAnalyzer::AnalyzeFileDepedency(std::string file) {
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
		ProcessTok(file, tok);										// Check for match in Final Type Table and perform corresponding action
	} while (in.good());
}

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

void CodeAnalyzer::ProcessTok(std::string filename, std::string token) {
	if (_FinalTypeTable.find(token) == _FinalTypeTable.end()) {
		return;
	}

	if (_FinalTypeTable.count(token) > 1) {
		std::pair<std::multimap<std::string, Entry>::iterator, std::multimap<std::string, Entry>::iterator> iter;
		iter = _FinalTypeTable.equal_range(token);

		for (std::multimap<std::string, Entry>::iterator it = iter.first; it != iter.second; it++) {
			if ((*it).second._type == "function (member)") {
				AppendResult(filename, (*it).second._filename.substr(0, (*it).second._filename.size() - 3) + "h", (*it).second._name+"**");
			}

			if ((*it).second._type != "function") {
				AppendResult(filename, (*it).second._filename, (*it).second._name);
			}
		}
	}
	else {
		std::pair<std::multimap<std::string, Entry>::iterator, std::multimap<std::string, Entry>::iterator> iter;
		iter = _FinalTypeTable.equal_range(token);

		for (std::multimap<std::string, Entry>::iterator it = iter.first; it != iter.second; it++) {
			if ((*it).second._type == "function (member)") {
				AppendResult(filename, (*it).second._filename.substr(0, (*it).second._filename.size() - 3) + "h", (*it).second._name);
			}

			if ((*it).second._type != "function") {
				AppendResult(filename, (*it).second._filename, (*it).second._name);
			}
		}
	}
}

void CodeAnalyzer::AppendResult(std::string& FileName, std::string& DepFileName, std::string& token) {
	size_t index;
	for (index = 0; index < _FinalResults.size(); index++) {
		if (_FinalResults[index].first == FileName) { break; }
	}

	if (index < _FinalResults.size()) {
		size_t fnCheck;
		for (fnCheck = 0; fnCheck < _FinalResults[index].second.size(); fnCheck++) {
			if (_FinalResults[index].second[fnCheck]._FileName == DepFileName) { break; }
		}

		if (fnCheck < _FinalResults[index].second.size()) {
			if ((std::find(_FinalResults[index].second[fnCheck]._tokens.begin(), 
				_FinalResults[index].second[fnCheck]._tokens.end(), token) != 
				_FinalResults[index].second[fnCheck]._tokens.end()) || (std::find(_FinalResults[index].second[fnCheck]._tokens.begin(),
					_FinalResults[index].second[fnCheck]._tokens.end(), token.substr(0, token.size() - 2)) !=
					_FinalResults[index].second[fnCheck]._tokens.end()))
				return;
			_FinalResults[index].second[fnCheck]._tokens.push_back(token);
		}
		else {
			ResultType res;
			res._FileName = DepFileName;
			res._tokens.push_back(token);
			_FinalResults[index].second.push_back(res);
		}
	}
	else {
		std::vector<ResultType> resvec;
		ResultType res;
		res._FileName = DepFileName;
		res._tokens.push_back(token);
		resvec.push_back(res);
		std::make_pair(FileName, res);
		_FinalResults.push_back(std::pair<std::string, std::vector<ResultType>>(FileName, resvec));
	}
}

void CodeAnalyzer::Process(const std::string &path, std::vector<std::string> &pattern, bool paramV) {
	setVerbose(paramV);
	setFiles(path, pattern);
	GenerateFinalTypeTable();
	if (VERBOSE) { ShowFinalTable(); }
	DependencyAnalysis();
	ShowFinalReport();
}

int main() {
	StringHelper::Title("Testing Code Analyzer", '=');
	Timer timer;
	timer.StartClock();
	ShowSingleResults = true;
	SetThreadDebugger = false;
	std::vector<std::string> patterns = { "*.cpp", "*.h" };
	CodeAnalyzer ca;
	ca.Process("../TestFiles/CodeAnalyzer/", patterns, false);
	std::cout << "\n\n Execution Time : " << timer.StopClock() << " ms \n ";
	system("pause");
}