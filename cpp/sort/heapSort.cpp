#include <iostream>
#include <vector>
#include <algorithm>
#include <exception>

using namespace std;

//调整堆 需要传入堆的所有数据 (引用) 数据索引  长度
void heapify(vector<int> &vec, int index, int len) {
    int left = (index << 1) + 1; //左子树
    int right = (index << 1) + 2;//右子树
    int maxId = index;
    if (left < len && vec[left] > vec[maxId]) {
        maxId = left;
    }
    if (right < len && vec[right] > vec[maxId]) {
        maxId = right;
    }

    if (maxId != index) {
        swap(vec[index], vec[maxId]);
        heapify(vec, maxId, len);
    }
}


// 大根堆
void heapify2(vector<int> &data, int index, int len) {
    int left = (index << 1) + 1;
    int right = (index << 1) + 2;
    int maxId = index;
    if (left < len && data[left] > data[maxId]) maxId = left;
    if (right < len && data[right] > data[maxId]) maxId = right;
    if (maxId != index) {
        swap(data[maxId], data[index]);
        heapify(data, maxId, len);
    }
}

void heapSort(vector<int> &vec) {
    int len = vec.size();
    // 从最后一个叶子节点向上 建立堆 ,堆的大小不变
    for (int i = len / 2 - 1; i >= 0; i--) {
        heapify(vec, i, len);
    }
    // 从尾节点开始调整堆，直到堆的大小为1
    for (int i = len - 1; i >= 1; i--) {
        //取到顶端的元素 顶端元素肯定是最大的元素
        swap(vec[0], vec[i]);
        heapify2(vec, 0, i);
    }
}




int main() {
    vector<int> vec = {4, 2, 6, 7, 1, 3, 5, 8};
    for_each(vec.begin(), vec.end(), [](int i) { cout << i << " "; });
    cout << endl;
    heapSort(vec);

    for_each(vec.begin(), vec.end(), [](int i) { cout << i << " "; });
    return 0;
}

