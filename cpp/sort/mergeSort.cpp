#include<iostream>
#include<vector>
#include <algorithm>

using namespace std;
#define MIN(x, y) ( (x)>(y)?(y):(x))
#define MAX(x, y) ( (x)>(y)?(x):(y))

//归并函数
void Merge(vector<int> &data, int l, int m, int r) {
    int len = r - l + 1; //单元长度 左减右 加1
    int *temp = new int[len]; //新建临时数组
    int i = 0;
    int left = l;
    int right = m + 1;
    while (left <= m && right <= r) temp[i++] = (data[left] >= data[right] ? data[right++] : data[left++]);
    while (left <= m) temp[i++] = data[left++];
    while (right <= r) temp[i++] = data[right++];
    for (int j = 0; j < len; j++) data[l + j] = temp[j];
    delete[] temp;
}


void mergeSort(vector<int> &data, int l, int r) {
    if (l == r) return; // 左右相等 就表示排序完成 所有有序元素是紧邻的
    int m = l + ((r - l) >> 1); //取到中间元素
    mergeSort(data, l, m);  //递归 左边 进行分割
    mergeSort(data, m + 1, r);//递归 右边 进行分割
    Merge(data, l, m, r);// 尾递归 进行排序合并操作
}

void mergeSort2(vector<int> &data, int left, int right) {
    if (left >= right) return;
    int mid = (left + right) >> 1;
    mergeSort2(data, left, mid);
    mergeSort2(data, mid + 1, right);
    Merge(data, left, mid, right);
}

int main() {
    vector<int> data = {3, 1, 2, 4, 5, 8, 7, 6};
    mergeSort2(data, 0, 7);
    for (int i = 0; i < 8; ++i)
        cout << data[i] << " ";
}

