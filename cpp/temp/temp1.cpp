#include <string>
#include <iostream>
#include <utility>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <numeric>

#include <fstream>

using namespace std;


string slove(string str){
    int len = str.size();
    unordered_map<char,int> hash;
    for(int i=0;i<len/2;i++){
        hash[str[i]]++;
        hash[str[len-1-i]]++;
    }
    if(hash.size() == 1) return "NO";
    for(auto [k,v] : hash){
        if(v%2==1) return "NO";
    }
    return "YES";
}

int main(){
	int len=0;
	cin>>len;
	while(len--){
		string input;
		cin >> input;
		string res = slove(input);
		printf("%s\r\n",res.c_str());
	}
	return 0;
}
