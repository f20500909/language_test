#ifndef __THREAD_SAFE_QUEUE__
#define __THREAD_SAFE_QUEUE__
#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>

template <class T>
class Queue {

public:
	Queue(const size_t size_max);

	bool push(T& data);

	bool pop(T &data);

	// The queue has finished accepting input
	void finished();

	void quit();

	int length();


private
	// Data
	std::queue<T> _queue;
	typename std::queue<T>::size_type _size_max;

	// Thread gubbins
	std::mutex _mutex;
	std::condition_variable _fullQue;
	std::condition_variable _empty;

	// Exit
	// 原子操作
	std::atomic_bool _quit; //{ false };
	std::atomic_bool _finished; // { false };

};

template<class T>
Queue<T>::Queue(const size_t size_max) :_size_max(size_max) {
	_quit = ATOMIC_VAR_INIT(false);
	_finished = ATOMIC_VAR_INIT(false);
}

template<class T>
bool Queue<T>::push(T &data) {
	std::unique_lock<std::mutex> lock(_mutex);
	while (!_quit && !_finished)
	{
		if (_queue.size() < _size_max)
		{
			_queue.push(std::move(data));
			//_queue.push(data);
			_empty.notify_all();
			return true;
		}
		else
		{
			// wait的时候自动释放锁，如果wait到了会获取锁
			_fullQue.wait(lock);
		}
	}

	return false;
}

template<class T>
bool Queue<T>::pop(T &data) {
	std::unique_lock<std::mutex> lock(_mutex);
	while (!_quit)
	{
		if (!_queue.empty())
		{
			//data = std::move(_queue.front());
			data = _queue.front();
			_queue.pop();

			_fullQue.notify_all();
			return true;
		}
		else if (_queue.empty() && _finished)
		{
			return false;
		}
		else
		{
			_empty.wait(lock);
		}
	}
	return false;
}

template<class T>
int Queue<T>::length() {
	return static_cast<int>(_queue.size());
}

template<class T>
void Queue<T>::quit() {
	_quit = true;
	_empty.notify_all();
	_fullQue.notify_all();
}

template<class T>
void Queue<T>::finished() {
	_finished = true;
	_empty.notify_all();
}

int main(){

	int a=1;
	int b=1;
	int c=1;

	Queue<int> q( 4);
	q.push(a);
	q.push(b);
	q.push(c);



	return 0;
}



#endif
