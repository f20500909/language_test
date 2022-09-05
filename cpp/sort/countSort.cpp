#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;

tuple<int, int> getMaxMin(vector<int> &vec) {
    int _max = INT_MIN;
    int _min = INT_MAX;
    for (auto v:vec) {
        _max = max(_max, v);
        _min = min(_min, v);
    }
    return make_tuple(_min, _max);
}

void countSort(vector<int> &arr) {
    int len = arr.size();
    if (len < 1) return;
    auto data = getMaxMin(arr);
    int min = get<0>(data);
    int max = get<1>(data);
    int areaLen = max - min;
    vector<int> count(areaLen + 1, 0);
    vector<int> tmp(arr);
    cout << areaLen << endl;

    for (auto x : arr)
        count[x]++;
    for (int i = 1; i < areaLen; ++i)
        count[i] += count[i - 1];
    for (int i = len - 1; i >= 0; --i) {
        arr[count[tmp[i]] - 1] = tmp[i];
        count[tmp[i]]--;                //注意这里要减1
    }
}

void countSort2(vector<int>& data){
    int len =data.size();
    if(len<1) return ;

    int _max = INT_MIN;
    int _min = INT_MAX;
    for (auto v:data) {
        _max = max(_max, v);
        _min = min(_min, v);
    }
    int areaLen = _max-_min;
    vector<int> count(areaLen+1,0);
    vector<int> temp(data);


}



int main() {
    vector<int> vec = {1, 5, 3, 7, 6, 2, 8, 9, 4, 3, 3};
    countSort(vec);
    for (auto x : vec) cout << x << " ";
    cout << endl;
    return 0;
}

