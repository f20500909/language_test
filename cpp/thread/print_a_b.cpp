#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>
//using namespace std;

std::mutex mut;
std::condition_variable cond_var;
bool flag = true;


void printA() {
    while (1) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::unique_lock<std::mutex> lck(mut);
        cond_var.wait(lck, [] { return flag; });
        std::cout << "thread: " << std::this_thread::get_id() << "   printf: " << "A" << std::endl;
        flag = false;
        cond_var.notify_one();
    }
}

void printA2() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::unique_lock<std::mutex> guard(mut);
        cond_var.wait(guard, [] { return flag; });
        std::cout << " A ";
        flag = false;
        cond_var.notify_all();
    }
}


void printB() {
    while (1) {
        std::unique_lock<std::mutex> lck(mut);
        cond_var.wait(lck, [] { return !flag; });
        std::cout << "thread: " << std::this_thread::get_id() << "   printf: " << "B" << std::endl;
        flag = true;
        cond_var.notify_one();
    }
}

void printB2() {
    while (true) {
        std::unique_lock<std::mutex> lck(mut);
        cond_var.wait(lck, [] { return !flag; });
        std::cout << " B ";
        flag = true;
        cond_var.notify_all();
    }
}


int main() {
    std::thread tA(printA2);
    std::thread tB(printB2);
    tA.join();
    tB.join();
    return 0;
}
