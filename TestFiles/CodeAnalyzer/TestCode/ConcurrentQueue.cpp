//////////////////////////////////////////////////////////////////////////////
// ConcurrentQueue.h - Thread-safe Blocking Queue							//
// Version :	  1.3														//
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Parallel Dependency Analysis								//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////

#include <ctime>
#include <thread>
#include <sstream>
#include <iostream>
#include "ConcurrentQueue.h"

static std::mutex _mtx;

void produce(ConcurrentQueue<int>& q) {
	for (int i = 0; i< 20; ++i) {
		_mtx.lock();
		std::cout << "\n Pushing " << i;
		q.enQ(i);
		_mtx.unlock();
	}
}

/* Not a good way to deque but this is just for demonstration */
void consume(ConcurrentQueue<int>& q, int con) {
	std::this_thread::sleep_for(std::chrono::milliseconds(15));	/* This is done so that producer has time to push his threads into ConcurrentQueue */
	while (q.size() > 0) {
		_mtx.lock();
		auto tmp = q.deQ();
		_mtx.unlock();
		std::cout << "\n Consumer : " << con << " Popping : " << tmp;
	}
}

#ifdef TEST_CONCURRENTQUEUE

int main()
{
	ConcurrentQueue<int> q;
	q.setDebugger(true);
	std::cout << "\n TESTING CONCURRENT QUEUES" << "\n ----------------------------";

	std::clock_t timer = std::clock();

	// producer thread
	std::thread prod1(std::bind(produce, std::ref(q)));

	// consumer threads
	std::thread consumer1(std::bind(&consume, std::ref(q), 1));
	std::thread consumer2(std::bind(&consume, std::ref(q), 2));
	std::thread consumer3(std::bind(&consume, std::ref(q), 3));

	prod1.join();
	consumer1.join();
	consumer2.join();
	consumer3.join();
	q.clear();

	std::cout << "\n\n Execution Time : " << (std::clock() - timer) / (double)CLOCKS_PER_SEC << " secs\n ";
	system("pause");
}
#endif // TEST_CONCURRENTQUEUE