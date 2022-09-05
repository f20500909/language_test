// 作者：代码疯子
// 博客：http://www.programlife.net/
// 引用计数 & 写时复制
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char **argv)
{
	string sa = "a";
	string sb = sa;
	string sc = sb;
	printf("sa char buffer address: 0x%08X\n", sa.c_str());
	printf("sb char buffer address: 0x%08X\n", sb.c_str());
	printf("sc char buffer address: 0x%08X\n", sc.c_str());

	sc = "Now writing...";
	printf("After writing sc:\n");
	printf("sa char buffer address: 0x%08X\n", sa.c_str());
	printf("sb char buffer address: 0x%08X\n", sb.c_str());
	printf("sc char buffer address: 0x%08X\n", sc.c_str());

	return 0;
} 
