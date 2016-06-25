//////////////////////////////////////////////////////////////////////////////
// Threadpool.h - Has vector of threads which will enque work items			//
// Version :	  1.1														//
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Parallel Dependency Analysis								//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////
#include <string>
#include <sstream>
#include <iostream>
#include "ThreadPool.h"

#ifdef TEST_THREADPOOL

#include "../Utilities/Utilities.h"
using namespace Utilities;

////////////////////////////////////////////////////
// demonstrate Threadpool (ProcessWorkItem class)
using WorkResult = std::string;

int main()
{
	StringHelper::Title("Testing ThreadPool Class", '=');
	// Number of Threads to spawn
	int objects = 10;
	std::cout << "\n Main thread id = " << std::this_thread::get_id();
	std::cout << "\n Number of Threads to Spawn = " << objects;
	Timer timer;
	timer.StartClock();
	// Declare and Initialize ThreadPool
	ThreadPool<WorkResult> processor;
	processor.setDebugger(true);
	processor.init();
	// Bunch of Work Items (To keep track of Spawned Threads)
	std::vector<WorkItem<WorkResult>> _WorkItems;
	// Define the Work Item then push it back to WorkItems Vector
	for (int i = 0; i < objects; i++) {
		_WorkItems.push_back([=]() {
			auto tid = std::this_thread::get_id();
			std::stringstream ss;
			ss << tid;
			std::cout << "\n working on thread : " << i;
			return "Hello from work item : " + ss.str();
		});
	}
	// Enqueue the WorkItems to the ThreadPool
	for (int i = 0; i < objects; i++) {
		processor.doWork(&_WorkItems[i]);
	}
	// Print the results of the WorkItem(s) on Console
	for (int i = 0; i < objects; i++) {
		std::cout << "\n " << processor.result();
	}
	// Add a nullptr to ThreadPool to initialize termination sequence
	processor.doWork(nullptr);
	// wait for child thread(s) to complete
	processor.wait();
	std::cout << "\n\n Execution Time : " << timer.StopClock() << " ms\n ";
	system("pause");
}
#endif // TEST_THREADPOOL