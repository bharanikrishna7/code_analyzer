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
 * ~ProcessWorkItem();											// Deletes the children thread (actually delete child thread)
 *
 * Required files:
 * ---------------
 * Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp, Threadpool.h, Threadpool.cpp, Utilities.h, Utilities.cpp
 *
 * Build command:
 * --------------
 * devenv Threadpool.sln /rebuild debug
 *
 * Maintenance History:
 * --------------------
 * ver 1.0 : 6 April 16
 * - first release
 *
 * Credits:
 * --------
 * I would like to give credits to Prof. Jim W Fawcett Syracuse University
 * , CST 4-187) for providing a started code and explaining how Parallel 
 * Dependency Analysis project should work.
 *
 * Credits:
 * --------
 * I would like to give credits to Prof. Jim W Fawcett Syracuse University
 * , CST 4-187) for providing a started code and explaining how Parallel 
 * Dependency Analysis project should work.
 *
 */

#include <thread>
#include <functional>
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"

template<typename Result>
using WorkItem = std::function<Result()>;

///////////////////////////////////////////////////////////////////////////////////////////////////
// class to process work items
// - Execute work items Asynchronously
template<typename Result>
class ProcessWorkItem
{
public:
  void start(size_t threadCount);
  void doWork(WorkItem<Result>* pWi);
  Result result();
  void wait();
  ~ProcessWorkItem();
  bool VERBOSE = false;
private:
  std::thread* _pThread;
  BlockingQueue<WorkItem<Result>*> _workItemQueue;
  BlockingQueue<Result> _resultsQueue;
  std::vector<std::thread*> threads;
};

///////////////////////////////////////////////////////
// wait for child thread to terminate 
template<typename Result>
void ProcessWorkItem<Result>::wait()
{
	for (auto thread : threads) {
		thread->join();
	}
}

///////////////////////////////////////////////////////
// enqueue work item
template<typename Result>
void ProcessWorkItem<Result>::doWork(WorkItem<Result>* pWi)
{
  _workItemQueue.enQ(pWi);
}

///////////////////////////////////////////////////////
// retrieve results with blocking call 
template<typename Result>
Result ProcessWorkItem<Result>::result()
{
  return _resultsQueue.deQ();
}

///////////////////////////////////////////////////////
// start child thread that dequeus work items 
template<typename Result>
void ProcessWorkItem<Result>::start(size_t threadCount)
{
	size_t count = 0;
	while (count < threadCount) {
		std::function<void()> threadProc =
			[&]() {
			while (true)
			{
				WorkItem<Result>* pWi = _workItemQueue.deQ();
				if (pWi == nullptr)										
				{
					_workItemQueue.enQ(nullptr);						// nullptr is used for terminating the Threads Processing
					if (VERBOSE) {
						std::cout << "\n  shutting down work item processing";
					}
					return;
				}
				Result result = (*pWi)();
				_resultsQueue.enQ(result);
			}
		};
		_pThread = new std::thread(threadProc);
		threads.push_back(_pThread);
		count++;
	}
}

///////////////////////////////////////////////////////
// clean up heap 
template<typename Result>
ProcessWorkItem<Result>::~ProcessWorkItem()
{
	delete _pThread;
}

#endif