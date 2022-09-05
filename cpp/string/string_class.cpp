/*-------------------------------------
*   日期：2015-03-31
*   作者：SJF0115
*   题目: 实现string类
*   来源：百度
*   博客：
------------------------------------*/
#include <iostream>
#include <cstring>
using namespace std;

class String {
public:
	// 默认构造函数
	String(const char* str = NULL);
	// 复制构造函数
	String(const String &str);
	// 析构函数
	~String();
	// 字符串连接
	String operator+(const String & str);
	// 字符串赋值
	String & operator=(const String &str);
	// 字符串赋值
	String & operator=(const char* str);
	// 判断是否字符串相等
	bool operator==(const String &str);
	// 获取字符串长度
	int length();
	// 求子字符串[start,start+n-1]
	String substr(int start, int n);
	// 重载输出
	friend ostream & operator<<(ostream &o, const String &str);
private:
	char* data;
	int size;
};
// 构造函数
String::String(const char *str) {
	if (str == NULL) {
		data = new char[1];
		data[0] = '\0';
		size = 0;
	}
	else {
		size = strlen(str);
		data = new char[size + 1];
		strcpy(data, str);
	}//else
}

// 复制构造函数
String::String(const String &str) {
	size = str.size;
	data = new char[size + 1];
	strcpy(data, str.data);
}
// 析构函数
String::~String() {
	delete[] data;
}
// 字符串连接
String String::operator+(const String &str) {
	String newStr;
	//释放原有空间
	delete[] newStr.data;
	newStr.size = size + str.size;
	newStr.data = new char[newStr.size + 1];
	strcpy(newStr.data, data);
	strcpy(newStr.data + size, str.data);
	return newStr;
}

// 字符串赋值
String & String::operator=(const String &str) {
	if (data == str.data) {
		return *this;
	}//if
	delete[] data;
	size = str.size;
	data = new char[size + 1];
	strcpy(data, str.data);
	return *this;
}

// 字符串赋值
String& String::operator=(const char* str) {
	if (data == str) {
		return *this;
	}//if
	delete[] data;
	size = strlen(str);
	data = new char[size + 1];
	strcpy(data, str);
	return *this;
}

// 判断是否字符串相等
bool String::operator==(const String &str) {
	return strcmp(data, str.data) == 0;
}


// 获取字符串长度
int String::length() {
	return size;
}
// 求子字符串[start,start+n-1]
String String::substr(int start, int n) {
	String newStr;
	// 释放原有内存
	delete[] newStr.data;
	// 重新申请内存
	newStr.data = new char[n + 1];
	for (int i = 0; i < n; ++i) {
		newStr.data[i] = data[start + i];
	}//for
	newStr.data[n] = '\0';
	newStr.size = n;
	return newStr;
}
// 重载输出
ostream & operator<<(ostream &o, const String &str) {
	o << str.data;
	return o;
}



class MyString2{
private:
    char* _data;
    int _size;

public:
    MyString2(const char* src){
        if(!src){
            //如果为空
            this->_size=1;
            this->_data =new char[1];
            this->_data[0]='0';
        }else{
            this->_size=strlen(src);
            _data= new char [this->_size+1];
            strcpy(_data,src);
        }
    }

    MyString2(MyString2& src){
        this->_size=src._size;
        delete[] this->_data;
        this->_data = new char [this->_size+1];
        strcpy(this->_data,src._data);
    }

    ~ MyString2(){
        this->_size=0;
        delete[] this->_data;
    }

    inline long long len(){ retur this->_size;}

    MyString2& operator=(MyString2& src){
        this->_size = src._size;
        delete[] this->_data;
        this->_data = new char[this->_size+1];
        strcpy(this->_data,src._data);
    }

    bool operator==(MyString2& src){
        if(src._size==1&& this->_size==1) return true;
        return strcmp(src._data,this->_data);
    }
};



int main() {
	String str1("hello ");
	String str2 = "world";
	String str3 = str1 + str2;
	cout << "str1->" << str1 << " size->" << str1.length() << endl;
	cout << "str2->" << str2 << " size->" << str2.length() << endl;
	cout << "str3->" << str3 << " size->" << str3.length() << endl;

	String str4("helloworld");
	if (str3 == str4) {
		cout << str3 << " 和 " << str4 << " 是一样的" << endl;
	}//if
	else {
		cout << str3 << " 和 " << str4 << " 是不一样的" << endl;
	}

	cout << str3.substr(6, 5) << " size->" << str3.substr(6, 5).length() << endl;
	return 0;
}

