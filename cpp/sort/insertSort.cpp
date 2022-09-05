#include<iostream>
#include<vector>

using namespace std;

void insertSort(vector<int> &data) {
    //从第二个元素开始遍历 因为第一个元素插入不了
    for (int i = 1; i < data.size(); i++) {
        //从i-1个开始向前遍历  如果前一个数比后一个数大 就互换
        for (int j = i - 1; j >= 0 && data[j + 1] < data[j]; j--) {
            swap(data[j], data[j + 1]);
        }
    }
}


//插入排序 数组前部是有序的  需要从无序数组中选一个有序的元素插入到有序部分 从而增加有序部分的长度
void insertSort2(vector<int> &data) {
    //前面是有序的 后面是无序的
    for (int i = 1; i < data.size(); i++) { // 有序区间不断扩大
        for (int j = i; j >0; j--) {
            if(data[j-1]>data[j]) swap(data[j],data[j-1]);

        }
    }
}

int main() {

    vector<int> data = {3, 1, 1, 3, 2, 4, 5, 8, 7, 6};
    insertSort2(data);
    for (int i = 0; i < 10; ++i)
        cout << data[i] << " ";
}

