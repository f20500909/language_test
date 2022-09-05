#include  <chrono>
#include  <iostream>
#include <iomanip>

using namespace std;

class Timer {
public:
	Timer() = default;
	std::chrono::steady_clock::time_point now() const {
		return std::chrono::steady_clock::now();
	}
	std::chrono::steady_clock::time_point start() {
		return start_time = std::chrono::steady_clock::now();
	}
	std::chrono::steady_clock::time_point stop() {
		return end_time = std::chrono::steady_clock::now();
	}

	template <class T = double>
	T elapsed() const {
		return std::chrono::duration_cast<std::chrono::duration<T>>(
			end_time - start_time)
			.count();
	}

	template <typename F, typename... Args>
	static double profile(F func, Args &&... args) {
		Timer timer;
		timer.start();
		func(std::forward<Args>(args)...);
		timer.stop();
		return timer.elapsed();
	};

private:
	std::chrono::steady_clock::time_point start_time;
	std::chrono::steady_clock::time_point end_time;
};


int main() {


	Timer timer2;
	timer2.start();

	for (int n = 0; n < 1e8; n++)
		 n * n;

	timer2.stop();

	cout << "ÓÃÊ±: " << std::setprecision(3) << timer2.elapsed() << endl;

	cin.get();
	return 0;
}