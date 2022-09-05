#include <iostream>
#include <queue>
#include <list>
#include <unordered_map>

using namespace std;

class LRUCache {
private:
    int cap;
    int count;
    unordered_map<int, list<pair<int, int>>::iterator> m;
    list<pair<int, int>> list;

public:
    LRUCache(int capacity) : cap(capacity), count(0) {

    }

    //取数据
    int get(int key) {
        int res = -1;
        auto p = m.find(key);
        if (p != m.end()) {
            //如果能找到数据, p->second 取到对应的list  list->second 取到对应的数据
            res = p->second->second;
            //删除原数据
            list.erase(p->second);
            //将数据添加到链表头部
            list.push_front(make_pair(key, res));
            //更新当前hash所指向的对象
            p->second = list.begin();
        }

        return res;
    }

    void put(int key, int value) {
        //寻找已经有的键值
        auto p = m.find(key);
        if (p != m.end()) {
            //如果已经存在 ，就删除当前位置的数据
            list.erase(p->second);
        } else if (count == cap) {
            //如果不存在 并且容量达到上限
            int delkey = list.back().first;
            //删除最后一个节点
            list.pop_back();
            //从hash表中删除元素
            m.erase(delkey);
        } else {
            //当前元素数量自增
            ++count;
        }
        //数据增加到头
        list.push_front(make_pair(key, value));

        //更新元素头
        m[key] = list.begin();
    }

};


int main() {
    LRUCache cache(2);

    cache.put(1, 1);
    cache.put(2, 2);
    std::cout << cache.get(1) << endl;       // 返回  1
    cache.put(3, 3);    // 该操作会使得密钥 2 作废

    std::cout << cache.get(2) << endl;       // 返回  -1
    cache.put(4, 4);    // 该操作会使得密钥 1 作废


    std::cout << cache.get(1) << endl;       // 返回  -1
    std::cout << cache.get(4) << endl;       // 返回  4
    std::cout << cache.get(2) << endl;       // 返回  -1


    return 0;
}


