#ifndef TASK_H
#define TASK_H
//////////////////////////////////////////////////////////////////////////////
// Task.h - This is essentially a wrapper for Threadpool class				//
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
* void start(size_t threadCount);								// This method will process the work items in the vector of threads [Wrapper for Threadpool start method]
* void doWork(WorkItem<Result>* pWi);							// This method will enque the work items in the vector of threads [Wrapper for Threadpool doWork method]
* Result result();												// This method will return result enqued in _ResultsQueue (will pop the result from _ResultsQueue) 
*																   [Wrapper for Threadpool result method]
* void wait();													// This will join the threads to child threads (Wait for all children to be processed) [Wrapper for Threadpool wait method]
* ~ProcessWorkItem();											// Deletes the children thread (actually delete child thread) [Wrapper for Threadpool ~ProcessWorkItem method]
*
* Required files:
* ---------------
* Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp, Threadpool.h, Threadpool.cpp, Utilities.h, Utilities.cpp, Task.h, Task.cpp
*
* Build command:
* --------------
* devenv Task.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 6 April 16
* - first release
*
*/
#include "../ThreadPool/ThreadPool.h"

template<typename Result>
using WorkItem = std::function<Result()>;
using WorkResult = std::string;

///////////////////////////////////////////////////////////////////////////////////////////////////
// Class to wrap threadpool
// - This class wraps threadpool
template<typename Result>
class Task
{
public:
	void start(size_t threadCount);
	void doWork(WorkItem<Result>* pWi);
	Result result();
	void wait();
	~Task() {}
private:
	ProcessWorkItem<WorkResult> task;
};

template<typename Result>
void Task<Result>::start(size_t threadCount) {
	task.start(threadCount);
}

template<typename Result>
void Task<Result>::doWork(WorkItem<Result>* pWi) {
	task.doWork(pWi);
}

template<typename Result>
Result Task<Result>::result() {
	return task.result();
}

template<typename Result>
void Task<Result>::wait() {
	task.wait();
}

#endif // !TASK_H