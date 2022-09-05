#include<iostream>
#include<vector>

using namespace std;
const int INCRGAP = 2;

void shellSort1(vector<int> &data) {
    int len = data.size();
    int gap, i, j;
    //间隔每次减半
    for (gap = len / 2; gap > 0; gap /= 2) {
        //遍历所有
        for (i = gap; i < len; i++) {
            for (j = i - gap; j >= 0; j -= gap) {
                if (data[j] > data[j + gap])
                    swap(data[j], data[j + gap]);
            }
        }
    }
}


void shellSort2(vector<int> &data) {
    int gap = data.size() >> 1;
    while (gap > 0) {
        for (int i = gap; i < data.size(); i++) {
            for (int j = i - gap; j >= 0; j -= gap) {
                if (data[j] > data[j + gap]) {
                    swap(data[j], data[j + gap]);
                }
            }
        }
        gap = gap >> 1;
    }

}


int main() {
    vector<int> array = {2, 1, 4, 3, 3, 4, 11, 6, 5, 7, 8, 10, 15};
//    shellSort1(array, 13);
//    shellSort2(array, 13);
    shellSort2(array);
    for (auto it: array) {
        cout << it << " ";
    }
    return 0;
}

