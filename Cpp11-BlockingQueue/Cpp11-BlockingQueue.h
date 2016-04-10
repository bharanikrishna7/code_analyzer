#ifndef CPP11_BLOCKINGQUEUE_H
#define CPP11_BLOCKINGQUEUE_H
///////////////////////////////////////////////////////////////
// Cpp11-BlockingQueue.h - Thread-safe Blocking Queue        //
// ver 1.2                                                   //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015 //
///////////////////////////////////////////////////////////////
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
 * Cpp11-BlockingQueue.h
 *
 * Build Process:
 * --------------
 * devenv Cpp11-BlockingQueue.sln /rebuild debug
 *
 * Maintenance History:
 * --------------------
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

#include <condition_variable>
#include <mutex>
#include <thread>
#include <queue>
#include <string>
#include <iostream>
#include <sstream>

template <typename T>
class BlockingQueue {
public:
  BlockingQueue() {}
  BlockingQueue(BlockingQueue<T>&& bq);
  BlockingQueue<T>& operator=(BlockingQueue<T>&& bq);
  BlockingQueue(const BlockingQueue<T>&) = delete;
  BlockingQueue<T>& operator=(const BlockingQueue<T>&) = delete;
  T deQ();
  void enQ(const T& t);
  T& front();
  void clear();
  size_t size();
private:
  std::queue<T> q_;
  std::mutex mtx_;
  std::condition_variable cv_;
};

//----< move queue constructor >---------------------------------------------
template<typename T>
BlockingQueue<T>::BlockingQueue(BlockingQueue<T>&& bq) // need to lock so can't initialize
{
  std::lock_guard<std::mutex> l(mtx_);
  q_ = bq.q_;
  while (bq.q_.size() > 0)  // clear bq
    bq.q_.pop();
  /* can't copy  or move mutex or condition variable, so use default members */
}

//----< move queue assignment >----------------------------------------------
template<typename T>
BlockingQueue<T>& BlockingQueue<T>::operator=(BlockingQueue<T>&& bq)
{
  if (this == &bq) return *this;
  std::lock_guard<std::mutex> l(mtx_);
  q_ = bq.q_;
  while (bq.q_.size() > 0)  // clear bq
    bq.q_.pop();
  /* can't move assign mutex or condition variable so use target's */
  return *this;
}

//----< remove element from front of queue >---------------------------
template<typename T>
T BlockingQueue<T>::deQ()
{
  std::unique_lock<std::mutex> l(mtx_);
  if(q_.size() > 0)
  {
    T temp = q_.front();
    q_.pop();
    return temp;
  }

  // may have spurious returns so loop on !condition
  while (q_.size() == 0)
    cv_.wait(l, [this] () { return q_.size() > 0; });
  T temp = q_.front();
  q_.pop();
  return temp;
}

//----< push element onto back of queue >------------------------------
template<typename T>
void BlockingQueue<T>::enQ(const T& t)
{
  {
    std::lock_guard<std::mutex> l(mtx_);
    q_.push(t);
  }
  cv_.notify_one();
}

//----< peek at next item to be popped >-------------------------------
template <typename T>
T& BlockingQueue<T>::front()
{
  std::lock_guard<std::mutex> l(mtx_);
  if(q_.size() > 0)
    return q_.front();
  return T();
}

//----< remove all elements from queue >-------------------------------
template <typename T>
void BlockingQueue<T>::clear()
{
  std::lock_guard<std::mutex> l(mtx_);
  while (q_.size() > 0)
    q_.pop();
}

//----< return number of elements in queue >---------------------------
template<typename T>
size_t BlockingQueue<T>::size()
{
  std::lock_guard<std::mutex> l(mtx_);
  return q_.size();
}

#endif
