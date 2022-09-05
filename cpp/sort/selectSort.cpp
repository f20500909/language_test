#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;

void print(const vector<int> &vec) {
    for (auto v:vec) {
        cout << v << " ";
    }
    cout << endl;
}

void selectSort(vector<int> &vec) {
    for (int i = 0; i < vec.size(); i++) {
        int minIndex = i;
        for (int j = i; j < vec.size(); j++) {
            // 如果索引值比当前值大  就更新索引值 让索引成为当前id  从而是最小值的索引
            if (vec[j] < vec[minIndex]) minIndex = j;
        }
        swap(vec[minIndex], vec[i]);
    }
}


void selectSort2(vector<int> &data) {
    for (int i = 0; i < data.size(); i++) {
        int index = data.size() - 1;
        for (int j = data.size() - 1; j > i; j--) if (data[index] >= data[j]) { index = j; }
        swap(data[i], data[index]);
    }
}

int main() {
    vector<int> data = {4, 7, 8, 3, 1, 1, 2, 6, 5};
    print(data);
    selectSort2(data);
    print(data);

    return 0;
}