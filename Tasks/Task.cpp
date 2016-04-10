//////////////////////////////////////////////////////////////////////////////
// Task.h - This is essentially a wrapper for Threadpool class				//
// Version :	  1.0														//
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Parallel Dependency Analysis								//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////
#include <string>
#include <iostream>
#include <conio.h>
#include "Task.h"
#include "../Utilities/Utilities.h"

#ifdef TEST_TASK
////////////////////////////////////////////////////
// demonstrate Task
using Util = Utilities::StringHelper;
using WorkResult = std::string;

int main()
{
	Util::Title("Demostrating Tasks");

	std::cout << "\n  main thread id = " << std::this_thread::get_id();

	// Detect number of threads in computer so as to use max number of threads.
	size_t _cpuThreads = std::thread::hardware_concurrency();
	if (_cpuThreads == 0) { _cpuThreads = 1; }
	std::cout << "\n  Number of Parallel Threads Queues : " << _cpuThreads << "\n";
	ProcessWorkItem<WorkResult> processor; 
	processor.start(_cpuThreads);

	// define 1st work item
	WorkItem<WorkResult> wi1 = []() {
		std::cout << "\n  working on thread " << std::this_thread::get_id();
		return "Hello from work item 1";
	};
	processor.doWork(&wi1);

	// define 2nd work item
	WorkItem<WorkResult> wi2 = []()
	{
		std::cout << "\n  working on thread " << std::this_thread::get_id();
		size_t sum = 0;
		for (size_t i = 0; i <= 10; ++i)
			sum += i;
		return "\nwork item 2 result = " + Utilities::Converter<size_t>::toString(sum);
	};
	processor.doWork(&wi2);

	// define 3rd work item
	WorkItem<WorkResult> wi3 = []() {
		std::cout << "\n  working on thread " << std::this_thread::get_id();
		return "This is work item #3";
	};
	processor.doWork(&wi3);

	// the following calls to result() block until results are enqueued
	std::cout << "\n  " << processor.result();
	std::cout << "\n  " << processor.result();
	std::cout << "\n  " << processor.result();
	processor.doWork(nullptr);

	// wait for child thread to complete

	processor.wait();
	std::cout << "\n\n";
}

#endif // TEST_TASK