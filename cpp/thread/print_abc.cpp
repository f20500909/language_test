#include <iostream>
#include <functional>
#include <thread>
#include <mutex>

using namespace std;

std::mutex mut;

int cnt = 0;

void func(char ch, int num) {
    while (num--) {
        while (ch != cnt + 'A') {
            std::this_thread::yield();
        }
        unique_lock<mutex> lk(mut);
        cout << ch;
        cnt = (cnt + 1) % 3;
    }
};




int main() {

    thread t1(func, 'A', 10);
    thread t2(func, 'B', 10);

    func('C', 10);

    t1.join();
    t2.join();


    return 0;
}
