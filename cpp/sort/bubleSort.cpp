#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;


void bubbleSort(vector<int> &vec, int len) {
    // 需要比较len-1次
    for (int i = 0; i < len - 1; i++) {
        // 需要比较进行len - n 次比较  越拍到后面 需要比较的次数越多
        for (int j = 0; j < len - i - 1; j++) {
            if (vec[j] > vec[j + 1])swap(vec[j], vec[j + 1]);
        }
    }
    return;
}



//冒泡排序 始终从0开始比较起
void bubbleSort5(vector<int> &data) {
    for (int i = 0; i < data.size() - 1; i++) {
        for (int j = 0; j < data.size() - i - 1; j++) {
            if (data[j] > data[j + 1])
                swap(data[j], data[j + 1]);
        }
    }
}


int main() {
    vector<int> vec = {4, 2, 6, 7, 1, 1, 3, 5, 8};
    for_each(vec.begin(), vec.end(), [](int i) { cout << i << " "; });
    cout << endl;
    bubbleSort5(vec);

    for_each(vec.begin(), vec.end(), [](int i) { cout << i << " "; });

    cin.get();

    return 0;
}
