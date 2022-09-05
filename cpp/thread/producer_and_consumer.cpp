#include<iostream>
#include<vector>
#include<thread>
#include<queue>
#include<mutex>
#include<Windows.h>
#include <condition_variable>

using namespace std;

class demo {
private:
    condition_variable is_full;
    condition_variable is_empty;
    mutex mtx;
    vector<std::thread> tpool;
private:
    queue<int> item_buff;
    const int buffsize;
    static int item_id;
    int c_no, p_no;
private:
    void producItem() {
        while (true) {
            std::unique_lock<mutex> lck(mtx);
            while (item_buff.size() == buffsize) {
                is_full.wait(lck);
            }
            item_id = item_id++ % buffsize;
            item_buff.push(item_id);
            cout << "product item id: " << item_id << endl;
            lck.unlock();
            is_empty.notify_all();
            Sleep(10);
        }
    }

    void consumeItem() {
        while (true) {
            std::unique_lock<mutex> lck(mtx);
            while (item_buff.empty()) {
                cout << "wait product" << endl;
                is_full.notify_one();
                is_empty.wait(lck);
            }
            cout << "customer id : " << this_thread::get_id() << endl;
            cout << "consume Item id: " << item_buff.front() << endl;
            item_buff.pop();
            lck.unlock();
        }
    }

public:
    demo(int size = 10, int pno = 1, int cno = 5) : buffsize(size), p_no(pno), c_no(cno) {};

    void run() {
        productTask();
        customerTask();
        for (auto &i : tpool) {
            i.join();
        }
    }

    void productTask() {
        for (int i = 0; i < p_no; i++) {
            tpool.emplace_back(thread(&demo::producItem, this));
        }
    }

    void customerTask() {
        for (int i = 0; i < c_no; i++) {
            tpool.emplace_back(thread(&demo::consumeItem, this));
        }
    }
};

int demo::item_id = 0;

int main() {
    demo t;
    t.run();
    return 0;
}
