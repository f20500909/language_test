
#include <iostream>
#include <vector>

using namespace std;


void print(const vector<int> &vec) {
    for (auto v:vec) {
        cout << v << " ";
    }
    cout << endl;
}


int partitiona3(vector<int> &vec, int left, int right) {
    int temp = vec[left];
    while (left < right) {
        while (left < right && vec[right] >= temp)right--;
        vec[left] = vec[right];
        while (left < right && vec[left] <= temp)left++;
        vec[right] = vec[left];
    }
    vec[left] = temp;
    return left;
}


// 第二种方法 双指针从头开始，快慢推进，遍历完所有数据
int partition2(vector<int> &vec, int left, int right) {
    int index = left;//用来遍历的指针
    int small = left - 1;//用于比较的指针，大于比较值则不动，小于比较值则交换
    swap(vec[index], vec[right]);//选取第一个作为哨兵比较
    for (; index < right; index++) {//遍历
        if (vec[index] < vec[right]) {//将小的扔到左边,大的不变
            small++;
            swap(vec[small], vec[index]);//交换
        }
    }
    small++;
    swap(vec[small], vec[right]);//交换初始值
    return small;
}


void quickSort(vector<int> &vec, int left, int right) {
    if (left > right) return;
    int index = partitiona3(vec, left, right);
    if (index > left) quickSort(vec, left, index - 1);
    if (index < right) quickSort(vec, index + 1, right);
}

int partition3(vector<int> &data, int left, int right) {
    int val = data[right];
    while (left < right) {
        while (left < right && data[left] <= val) left++;
        data[right] = data[left];
        while (left < right && data[right] >= val) right--;
        data[left] = data[right];
    }
    data[left] = val;
    return left;
}

void quickSort3(vector<int> &data, int left, int right) {
    if (left >= right) return;
    int index = partition3(data, left, right);
    if (index > left) quickSort3(data, left, index - 1);
    if (index < right) quickSort3(data, index + 1, right);
}

int main() {

    vector<int> data;
    int len = 100;
    for(int i=0; i<len; i++){
        data.push_back(rand()%100);
    }

    print(data);

//    bubleSort(data);
    quickSort3(data, 0, data.size() - 1);
//    selectSort(data);

    print(data);
    return 0;
}