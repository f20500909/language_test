#include <iostream>
#include <thread>
#include <mutex>

using namespace std;
const int count = 5;
int flag = 1;

std::mutex mut;

void fun(const int num, const string &str) {
    for (int i = 0; i < count; i++) {
        while (num != flag) {
            std::this_thread::yield();
        }
        mut.lock();
        for (int j = 0; j < num; ++j) {
            cout << str << endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        flag = (flag == 1 ? 10 : 1);
        mut.unlock();
    }
}




int main() {
    thread child(fun,10,"child");
    fun(100,"father");
    child.join();

    return 0;
}
