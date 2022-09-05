#include <iostream>
#include <fstream>
#include <assert.h>
#include <string>
#include <sstream>
#include  <chrono>
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




std::string EOS = "</s>";

bool readWord(std::stringstream& in, std::string& word)  {
	int c;
	std::streambuf& sb = *in.rdbuf();
	word.clear();
	while ((c = sb.sbumpc()) != EOF) {
		if (c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\v' ||
			c == '\f' || c == '\0') {
			if (word.empty()) {
				if (c == '\n') {
					word += EOS;
					return true;
				}
				continue;
			}
			else {
				if (c == '\n')
					sb.sungetc();
				return true;
			}
		}
		word.push_back(c);
	}
	// trigger eofbit
	in.get();
	return !word.empty();
}

void myFun(int lineNum) {
	stringstream in;

	for (size_t i = 0; i < lineNum; i++) {
		in<<"dsf dsf \n sdfdsf dsfdsg fdgdf gfdsgf gdfs gfg f \r\n sdfdsf dsfdsg \r gfdsgf gdfs gfg f \r\n ";
	}

	string word;

	while (readWord(in, word)) {
		//cout << word << endl;
	}
}

void readWithIn(int lineNum) {
	stringstream in;
	for (size_t i = 0; i < lineNum; i++) {
		in << "dsf dsf \n sdfdsf dsfdsg fdgdf gfdsgf gdfs gfg f \r\n sdfdsf dsfdsg \r gfdsgf gdfs gfg f \r\n ";
	}

	string word;
	while (in)
	{
		in >> word;

	}
 }


int main()
{

	Timer timer;
	timer.start();

	myFun(1e5);

	timer.stop();

	cout << "使用readWord 读取完成.... 用时: " <<std::setprecision(3) << timer.elapsed()<< endl;




	Timer timer2;
	timer2.start();

	readWithIn(1e5);

	timer2.stop();

	cout << "使用in      读取完成.... 用时: " << std::setprecision(3) << timer2.elapsed() << endl;




	cin.get();
	return 0;
}