#ifndef THREADPOOL_H
#define THREADPOOL_H
//////////////////////////////////////////////////////////////////////////////
// Threadpool.h - Has vector of threads which will enque work items			//
// Version :	  1.0														//
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Parallel Dependency Analysis								//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* This module will create a vector of threads which will enque work items.
*
* Public Interface:
* -----------------
* Methods -
* ---------
* void start(size_t threadCount);								// This method will process the work items in the vector of threads
* void doWork(WorkItem<Result>* pWi);							// This method will enque the work items in the vector of threads
* Result result();												// This method will return result enqued in _ResultsQueue (will pop the result from _ResultsQueue)
* void wait();													// This will join the threads to child threads (Wait for all children to be processed)
* ~ThreadPool();												// Deletes the children thread (actually delete child thread)
*
* Required files:
* ---------------
* Cpp11-ConcurrentQueue.h, Cpp11-ConcurrentQueue.cpp, Threadpool.h, Threadpool.cpp, Utilities.h, Utilities.cpp
*
* Build command:
* --------------
* devenv Threadpool.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.1 : 20 June 16
* - Removed _pThread from class since it was not necessary
* - Renamed the class from ProcessWorkItem to  ThreadPool
* - The number of Blocking Queues which are will be initialized
*   depend on the number of threads the Hardware can simultaneously
*   run (number of multi threads). If unable to determine then only
*   one BlockingQueue is spawned.
* - Updated VERBOSE Mode, to show more information.
* ver 1.0 : 6 April 16
* - first release
*
* Credits:
* --------
* I would like to give credits to Prof. Jim W Fawcett Syracuse University
* , CST 4-187) for providing a started code and explaining how Parallel
* Dependency Analysis project should work.
*
* To-Do:
* ------
* Add support for logging all the information to a log file.
*/

#include <algorithm>
#include <functional>
#include "ConcurrentQueue.h"

template<typename Result>
using WorkItem = std::function<Result()>;

///////////////////////////////////////////////////////////////////////////////////////////////////
// class to process work items
// - Execute work items Asynchronously
template<typename Result>
class ThreadPool
{
public:
	void start(int cpuThreads = 1);
	void init();
	void setThreads();
	void doWork(WorkItem<Result>* pWi);
	Result result();
	void wait();
	~ThreadPool() {}
	bool DEBUGGER = false;
	void setDebugger(bool paramV);
private:
	std::thread* _pThread;
	ConcurrentQueue<WorkItem<Result>*> _workItemQueue;
	ConcurrentQueue<Result> _resultsQueue;
	std::vector<std::thread*> _threads;
	int _cpuThreads = 1;
};

template <typename Result>
void ThreadPool<Result>::setDebugger(bool paramV) {
	DEBUGGER = paramV;
	_resultsQueue.setDebugger(DEBUGGER);
}

////////////////////////////////////////////////
// set the thread count for parallel processing
template <typename Result>
void ThreadPool<Result>::setThreads() {
	if (DEBUGGER) { std::cout << "\n [THREADPOOL DEBUGGER] : Determining the number of Parallel Threads System can Run "; }
	_cpuThreads = std::thread::hardware_concurrency();
	if (_cpuThreads == 0) { _cpuThreads = 1; }
	if (DEBUGGER) { std::cout << "\n [THREADPOOL DEBUGGER] : Number of Parallel Threads System can Run : " << _cpuThreads; }
}

///////////////////////////////////////////////////////
// wait for child thread to terminate 
template<typename Result>
void ThreadPool<Result>::wait()
{
	if (DEBUGGER) { std::cout << "\n [THREADPOOL DEBUGGER] : Waiting for Child Threads to Terminate "; }
	for (auto thread : _threads) {
		thread->join();
	}
	if (DEBUGGER) { std::cout << "\n [THREADPOOL DEBUGGER] : All Child Threads Successfully Terminated "; }
}

///////////////////////////////////////////////////////
// enqueue work item
template<typename Result>
void ThreadPool<Result>::doWork(WorkItem<Result>* pWi)
{
	_workItemQueue.enQ(pWi);
}

///////////////////////////////////////////////////////
// retrieve results with blocking call 
template<typename Result>
Result ThreadPool<Result>::result()
{
	return _resultsQueue.deQ();
}

/////////////////////////////
// initialize the ThreadPool
template <typename Result>
void ThreadPool<Result>::init() {
	if (DEBUGGER) { std::cout << "\n [THREADPOOL DEBUGGER] : Initializing ThreadPool "; }
	setThreads();
	start(_cpuThreads);
}

///////////////////////////////////////////////////////
// start child thread that dequeus work items 
template<typename Result>
void ThreadPool<Result>::start(int cpuThreads = 1)
{
	size_t count = 0;
	while (count < cpuThreads) {
		if (DEBUGGER) { std::cout << "\n [THREADPOOL DEBUGGER] : Initializing Thread Queue : " << count; }
		std::function<void()> threadProc =
			[&]() {
			while (true)
			{
				WorkItem<Result>* pWi = _workItemQueue.deQ();
				if (pWi == nullptr)
				{
					_workItemQueue.enQ(nullptr);						// nullptr is used for terminating the Threads Processing
					if (DEBUGGER) { std::cout << "\n [THREADPOOL DEBUGGER] : Shutting down Thread Queue(s) "; }
					return;
				}
				Result result = (*pWi)();
				_resultsQueue.enQ(result);
			}
		};
		;
		_threads.push_back(new std::thread(threadProc));
		count++;
	}
}
#endif