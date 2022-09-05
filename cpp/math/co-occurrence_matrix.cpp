#include <deque>
#include <iostream>
#include <sstream>


void coCoo(std::stringstream& text) {

	int window=4;//window大小
	std::deque<std::string> ids;
	std::string center;
	std::string context;

	text >> context;

	ids.push_back(context);

	while (!text.eof()) {
		text >> center;
		//这行只有一个词，跳过
		if (center.empty()) {
			continue;
		}

		for (int i = 0; i != ids.size(); ++i) {
			std::cout <<  ids[i] <<" "<<center << std::endl;
		}

		//If line breaks, separate context between paragraphs
		//这一行结束,清空ids和flags,重新读取一个词到ids里面
		//peek函数用于读取并返回下一个字符，但并不移动流指针
		if (text.peek() == '\n') {
			ids.clear();
			text >> context;
			ids.push_back(context);
			center = "";
		}
		else {
			//如果超出滑动窗口的上限 window 则移除顶部数据
			if (ids.size() > window-1) {
				ids.pop_front();
			}
			//如果没有到换行,也没有达到滑动窗口的上限,则将id压如ids中;
			ids.push_back(center);
		}
	}
	return;
}

int main() {

	std::stringstream text("这 是 一段 话 用来 测试 生成 共现 矩阵 的\n实际 使用 时 可以 将 stringstream 替换 为 ifstream");
	coCoo(text);

	std::cin.get();
	return 0;
}
