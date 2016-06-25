//////////////////////////////////////////////////////////////////////////////
// ConcurrentQueue.h - Thread-safe Blocking Queue							//
// Version :	  1.3														//
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Parallel Dependency Analysis								//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package contains one thread-safe class: BlockingQueue<T>.
* Its purpose is to support sending messages between threads.
* It is implemented using C++11 threading constructs including
* std::condition_variable and std::mutex.  The underlying storage
* is provided by the non-thread-safe std::queue<T>.
*
* Required Files:
* ---------------
* ConcurrentQueue.h
*
* Build Process:
* --------------
* devenv Cpp11-BlockingQueue.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.3 : 20 June 2016
* - Renamed the class from Cpp11-BlockingQueue.h to ConcurrentQueue.h
* - Trimmed some methods for streamlined code.
* - Added support for VERBOSE Mode.
* ver 1.2 : 27 Feb 2016
* - added front();
* - added move ctor and move assignment
* - deleted copy ctor and copy assignment
* ver 1.1 : 26 Jan 2015
* - added copy constructor and assignment operator
* ver 1.0 : 03 Mar 2014
* - first release
*
* Credits:
* --------
* I would like to give credits to Prof. Jim W Fawcett Syracuse University
* , CST 4-187) for providing a started code and explaining how Parallel
* Dependency Analysis project should work.
*
*/

#ifndef CONCURRENTQUEUE_H
#define CONCURRENTQUEUE_H

#include <mutex>
#include <queue>
#include <thread>
#include <iostream>
#include <condition_variable>

template <typename T>
class ConcurrentQueue {
private:
	std::queue<T> _queue;
	std::mutex _mtx;
	std::condition_variable _cv;
public:
	ConcurrentQueue() {}
	~ConcurrentQueue() {}
	ConcurrentQueue(ConcurrentQueue<T> && bq);
	ConcurrentQueue<T>& operator=(ConcurrentQueue<T>&& bq);
	size_t size();
	void enQ(const T & item);
	T deQ();
	T front();
	void clear();
	bool isEmpty();
	bool DEBUGGER = false;
	void setDebugger(bool paramV);
};

template <typename T>
void ConcurrentQueue<T>::setDebugger(bool paramV) {
	DEBUGGER = paramV;
}

template <typename T>
bool ConcurrentQueue<T>::isEmpty() {
	return !_queue.empty();
}

template <typename T>
ConcurrentQueue<T>::ConcurrentQueue(ConcurrentQueue<T> && bq) {
	if (DEBUGGER) { std::cout << "\n [THREADS DEBUGGER] : Initializing Concurrent Queue (Move Constructor)"; }
	std::lock_guard<std::mutex> lock(_mtx);
	_queue = std::move(bq._queue);
	while (bq._queue.size() > 0) {
		bq._queue.pop();
	}
}

template <typename T>
ConcurrentQueue<T>& ConcurrentQueue<T>::operator=(ConcurrentQueue<T>&& bq) {
	if (DEBUGGER) { std::cout << "\n [THREADS DEBUGGER] : Initializing Concurrent Queue (Move Operator)"; }
	std::lock_guard<std::mutex> lock(_mtx);
	_queue = std::move(bq._queue);
	while (bq._queue.size() > 0) {
		bq._queue.pop();
	}
	return *this;
}

template <typename T>
size_t ConcurrentQueue<T>::size() {
	std::lock_guard<std::mutex> lock(_mtx);
	return _queue.size();
}

template <typename T>
void ConcurrentQueue<T>::enQ(const T & item) {
	if (DEBUGGER) { std::cout << "\n [THREADS DEBUGGER] : Pushing item into Concurrent Queue"; }
	{
		std::lock_guard<std::mutex> lock(_mtx);
		_queue.push(std::move(item));
	}
	_cv.notify_one();
}

template <typename T>
T ConcurrentQueue<T>::deQ() {
	std::unique_lock<std::mutex> lock(_mtx);
	if (_queue.size() == 0) {
		_cv.wait(lock, [&]() {
			return _queue.size();
		});
	}
	if (DEBUGGER) { std::cout << "\n [THREADS DEBUGGER] : Dequeueing item from Concurrent Queue"; }
	T tmp = _queue.front();
	_queue.pop();
	return tmp;
}

template <typename T>
T ConcurrentQueue<T>::front() {
	if (_queue.size() > 0)
		return _queue.front();

	return T();
}

template <typename T>
void ConcurrentQueue<T>::clear() {
	if (DEBUGGER) { std::cout << "\n [THREADS DEBUGGER] : Emptying the Concurrent Queue"; }
	std::lock_guard<std::mutex> lock(_mtx);
	while (_queue.size() > 0) {
		_queue.pop();
	}
}
#endif // ! CONCURRENTQUEUE_H