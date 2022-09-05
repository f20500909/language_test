#include <iostream>
#include <vector>
using namespace std;


int binarySearch(vector<int> data, int len, int key) {
	int low = 0;
	int high = len - 1;
	int mid = 0;
	while (low <= high) {
		mid = (low + high) / 2;
		if (data[mid] == key) return mid;
		if (data[mid] < key)low = mid + 1;
		else high = mid - 1;
	 }
	return -1;
}

int binarySearch2(vector<int> data,int len,int key){
    int left = 0;
    int right = len-1;
    int mid = 0;
    while(left<=right){
        mid = (left+right)>>1;
        if(data[mid] == key) return mid;
        if(data[mid]<key) left=mid+1;
        if(data[mid>key]) right=mid-1;
    }
    return -1;
}

int binarySearch3(vector<int>& data,int len,int key){
    int left = 0;
    int right = len-1;
    int mid ;
    while(left<=right){
        mid = left +  ((right- left )>>1);
        if(data[mid] == key) return mid;
        if(data[mid]>key) right =mid -1;
        if(data[mid]<key) left = mid +1;
    }
    return  -1;
}



int main() {
	vector<int> vec = {1,2,3,4,5,6};

	cout << binarySearch3(vec,vec.size(),4) << endl;
    cout << binarySearch3(vec,vec.size(),0) << endl;
    cout << binarySearch3(vec,vec.size(),1) << endl;
    cout << binarySearch3(vec,vec.size(),6) << endl;

	cin.get();

	return 0;
}
