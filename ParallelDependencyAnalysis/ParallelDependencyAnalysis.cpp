//////////////////////////////////////////////////////////////////////////////
// ParallelDependencyAnalysis.h - Create Type Table and Analyze all files	//
// Version :	  1.0														//
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Parallel Dependency Analysis								//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////

#include "ParallelDependencyAnalysis.h"

using PrettyHelper = Utilities::StringHelper;
using namespace Scanner;

////////////////////////////////////////////////
// Will return the vector files when called.
std::vector<std::string> ParallelDependencyAnalysis::returnFiles() {
	return files;
}

////////////////////////////////////////////////
// Will set the VERBOSE variable values
void ParallelDependencyAnalysis::setVerbose(bool paramV) {
	VERBOSE = paramV;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Checks the "path" location for files which match the parrern present in vector "pattern"
// and store those files in "files" vector
void ParallelDependencyAnalysis::setFiles(std::string path, std::vector<std::string> pattern) {
	std::string _path = path;

	DataStore ds;
	FileMgr fm(path, ds);
	for (auto pat : pattern)
	{
		fm.addPattern(pat);
	}
	fm.search();
	for (auto file : ds)
	{
		files.push_back(file);
	}
}

////////////////////////////////////////////////
// Will generate final type table
void ParallelDependencyAnalysis::pass1() {
	GenTypeTable genTT;
	genTT.setVerbose(VERBOSE);
	genTT.setFiles(files);											// set the files vector using "files" in generate type table object
	genTT.getAnalysis(genTT.returnFiles());							// Preform analysis for the file
	genTT.makeDepTable();											// Make final Type Table
	FinTypeTable = *genTT.returnFinTable();							// Copy the final type table's reference from genTT to FinTypeTable
	genTT.showFinTable();
	if (VERBOSE) {
		std::cout << "\n Type Table Creation is complete\n ";
	}
}

///////////////////////////////////////////////////////
// Will append the results to finalResults object
void ParallelDependencyAnalysis::appendResults(std::string _fileNMCur, std::vector<resultType>* results) {
	if (results->size() > 0) {
		finalResults.push_back({ _fileNMCur, *results });
	}
}

///////////////////////////////////////////////////////////////
// Will analyze files using the Type Table for dependencies
void ParallelDependencyAnalysis::pass2() {
	PrettyHelper::Title("\n\n BEGIN ANALYZING FILES FOR DEPENDENCY", '*');
	// Declare Dependency Analysis Class (This will be used to perform DependencyAnalysis for files)
	DependencyAnalysis depAL;
	std::string _file;
	// Detect number of threads in computer so as to use max number of threads.
	size_t _cpuThreads = std::thread::hardware_concurrency();
	if (_cpuThreads == 0) { _cpuThreads = 1; }
	if (VERBOSE) { std::cout << "\n Number of Parallel Thread Queues : " << _cpuThreads << "\n"; }
	// Declare Thread Pool (Right below)
	Task<std::string> processor;
	processor.start(_cpuThreads);
	// Add work items to threadpool which will require DependencyAnalysis processing.
	for (size_t index = 0; index < files.size(); index++) {
		_file = files[index];
		// Sent the Final Type Table reference to the Dependency Analysis Class
		depAL.copyMAP(&FinTypeTable);
		if (VERBOSE) {
			std::cout << "\n MAP reference copy was successful for file - " + _file;
		}
		WorkItem<std::string> *wi = new WorkItem<std::string>;
		*wi = [&]() {
			if (VERBOSE) {
				std::cout << "\n Working on thread " << std::this_thread::get_id();
				std::cout << "\n Processing Thread Execution started for file - " + _file << "\n";
			}
			// Perform Dependency Analysis for the file "_file"
			depAL.processFile(_file);
			// Push the results to the finalResults
			appendResults(_file, depAL.returnResults());
			std::string result = "";
			if (VERBOSE) {
				result = "\n Results of analysis were successful copied to finalResults for file - " + _file + "\n Thread Closed for file -" + _file + "\n";
			}
			return result;
		};
		processor.doWork(wi);
	}
	// Print Results from Task to console
	for (size_t index = 0; index < files.size(); index++) {
		std::cout << processor.result(); 
	}
	processor.doWork(nullptr);
	processor.wait();
}

////////////////////////////////////////////////
// Print the Final Results on Console.
void ParallelDependencyAnalysis::showFinResults() {
	if (finalResults.size() == 0) {
		std::cout << "\n There are no dependencies \n";
		return;
	}
	
	int index = 1;
	//Print the vectors
	for (auto entry : finalResults) {
		PrettyHelper::Title("\n " + std::to_string(index) + " ] " + entry.first, '-');
		index++;
		
		for (auto deps : entry.second) {
			std::cout << std::left << std::setw(11) << "\n FILENAME : " << deps._filename;
			std::cout << std::left << std::setw(11) << "\n Tokens " << ": { ";
			bool first = true;
			for (auto _token : deps._tokens) {
				if (first) {
					std::cout << _token;
					first = false;
				}
				else { std::cout << " , " << _token; }
			}
			std::cout << " }\n";
		}
	}
}

/////////////////////////////////////////////////////////////////////////
// Wrapper method which will call other methods to generate type table,
// perform analysis and display the results on console
void ParallelDependencyAnalysis::PerformAnalysis(std::string path, std::vector<std::string> pattern) {
	// Set Files
	setFiles(path, pattern);

	PrettyHelper::Title("\n BEGIN PARALLEL DEPENDENCY ANALYSIS", '=');
	
	// Begin PASS 1
	if (VERBOSE) { std::cout << "\n\n Pass 1 : Begin"; }
	pass1();
	if (VERBOSE) { std::cout << "\n\n Pass 1 : Success"; }
	// Begin PASS 2
	if (VERBOSE) { std::cout << "\n\n Pass 2 : Begin"; };
	pass2();
	if (VERBOSE) { std::cout << "\n\n Pass 2 : Success"; }
	// Display Final Results on Screen
	if (VERBOSE) { std::cout << "\n\n Disp Results : Begin"; }
	showFinResults();
	if (VERBOSE) { std::cout << "\n\n Disp Results : Success"; }

	std::cout << "\n\n\n\n PARALLEL DEPENDENCY ANALYSIS COMPLETED\n\n ";
}

#ifdef TEST_PARALLELDEPANAL
////////////////////////////////////////////////
// TEST STUB
int main(int argc, char* argv[]) {
	int nuArgs = argc - 1;
	std::string path;
	std::vector<std::string> pattern;

	switch (nuArgs)
	{
	case 0:
		path = "../Test";
		pattern = { "*.h","*.cpp" };
		break;
	case 1:
		path = argv[1];
		pattern = { "*.h","*.cpp" };
		break;
	default:
		path = argv[1];
		for (int index = 2; index < argc; index++)
			pattern.push_back(argv[index]);
		break;
	}

	ParallelDependencyAnalysis ParDepAnal;
	/* Uncomment the below line go in verbose mode
	 * verbose mode will let you visualize the flow of the program ( Amazing for understanding design )
	*/
	//ParDepAnal.setVerbose(true);
	
	// Perform Analysis
	ParDepAnal.PerformAnalysis(path, pattern);

	system("pause");
}

#endif // TEST_PARALLELDEPANAL
