#include<vector>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

class Node {
public:
    int key = 0;
    int height = 0;
    Node *left = nullptr;
    Node *right = nullptr;

    Node(int key_t = 0) {
        key = key_t;
        height = 1;
        left = right = nullptr;
    }
};

class AVL {
private:
    Node *header; //header结点并非根结点，header->left指向的才是根结点。

    Node *ll_rotate(Node *y);

    Node *rr_rotate(Node *y);

    Node *lr_rotate(Node *y);

    Node *rl_rotate(Node *y);


    int get_height(Node *node);

    int get_balance(Node *node);

    Node *insert_real(int key, Node *node);

    Node *&find_real(int key, Node *&node);

    Node *erase_real(int key, Node *node);


    int destory(Node *node);

public:
    AVL();

    ~AVL();

    void insert(int key);

    // (递归实现)查找"AVL"中键值为key的节点
    Node *find(int key);

    //(非递归实现)查找"AVL"中键值为key的节点
    Node *loop_find(int key);

    void erase(int key);
};

AVL::AVL() {
    header = new Node(-100);
}

AVL::~AVL() {
    destory(header->left); //将多余的那个节点也删除掉
}

int AVL::destory(Node *p) {
    if (p == nullptr)
        return 0;
    // test++;
    // cout << "test ==  " << test << endl;
    destory(p->left); //注意先后次序，如果先把p销毁，那么就会找不到p->left,p->right
    destory(p->right);
    delete p;
    p = nullptr;
}

void AVL::insert(int key) {
    header->left = insert_real(key, header->left);
}

int AVL::get_height(Node *node) {
    if (nullptr == node) return 0;
    return node->height;
}

int AVL::get_balance(Node *node) {
    if (node == nullptr)
        return 0;
    return get_height(node->left) - get_height(node->right);
}

Node *AVL::insert_real(int key, Node *node) //返回新的根节点，用来更新根节点
{
    if (node == nullptr)
        return new Node(key);

    //递归 判断插到左边还是插入到右边
    if (key < node->key) {
        node->left = insert_real(key, node->left);
    } else if (key > node->key) {
        node->right = insert_real(key, node->right);
    } else return node;

    //当前节点的高度是左右子树高度最大值加一
    node->height = max(get_height(node->left), get_height(node->right)) + 1;
    //因为新加入了一个节点，所以回溯的时候给各个节点高度 +1

    //拿到平衡因子
    int balance = get_balance(node); //左减右

    // 左左失衡
    /*即 "失衡结点" 的左子树比右子树高 2，左孩子（即 x）下的左子树比右子树高 1*/
    if (balance > 1 && get_balance(node->left) > 0)
        return ll_rotate(node);

    //右右失衡
    /*"失衡结点" 的右子树比左子树高 2，右孩子（即 x）下的右子树比左子树高 1。*/
    if (balance < -1 && get_balance(node->right) < 0)
        return rr_rotate(node);

    // 左右失衡
    /*"失衡结点" 的左子树比右子树高 2，左孩子（即 x）下的右子树比左子树高 1。。*/
    if (balance > 1 && get_balance(node->left) < 0)
        return lr_rotate(node);

    // 右左失衡
    /*""失衡结点" 的右子树比左子树高 2，右孩子（即 x）下的左子树比右子树高 1。*/
    if (balance < -1 && get_balance(node->right) > 0)
        return rl_rotate(node);

    return node;
}

//左左失衡
Node *AVL::ll_rotate(Node *y) {
    Node *x = y->left;
    y->left = x->right;
    x->right = y;
    y->height = max(get_height(y->left), get_height(y->right)) + 1;
    x->height = max(get_height(x->left), get_height(x->right)) + 1;
    return x; //返回根节点



}

Node *AVL::rr_rotate(Node *y) {
    Node *x = y->right;
    y->right = x->left;
    x->left = y;
    y->height = max(get_height(y->left), get_height(y->right)) + 1;
    x->height = max(get_height(x->left), get_height(x->right)) + 1;
    return x;
}

Node *AVL::lr_rotate(Node *y) {
    Node *x = y->left;
    y->left = rr_rotate(x);
    return ll_rotate(y);
}

Node *AVL::rl_rotate(Node *y) {
    Node *x = y->right;
    y->right = ll_rotate(x);
    return rr_rotate(y);
}

Node *AVL::find(int key) {
    return find_real(key, header->left);
}

Node *&AVL::find_real(int key, Node *&node) {
    if (node == nullptr)
        return node;
    if (key < node->key)
        find_real(key, node->left);
    else if (key > node->key)
        find_real(key, node->right);
    else // 只剩下相等了
        return node;
}

Node *AVL::loop_find(int key) {
    Node *p = header->left; // p 指向根节点
    while (p && p->key != key) {
        if (key < p->key)
            p = p->left;
        else
            p = p->right;
    }
    return p;
}


Node *AVL::erase_real(int key, Node *node) {
    if (node == nullptr) {
        cout << key << "  not in AVL tree" << endl;
        return node;
    }

    if (key < node->key)
        node->left = erase_real(key, node->left);
    else if (key > node->key)
        node->right = erase_real(key, node->right);
    else {
        if (node->left && node->right) {
            // 找到后继结点
            Node *x = node->right;
            while (x->left)
                x = x->left;

            // 后继直接复制
            node->key = x->key;

            // 转化为删除后继
            node->right = erase_real(x->key, node->right);
        } else {
            Node *t = node;
            node = node->left ? node->left : node->right;
            delete t;
            if (node == nullptr)
                return nullptr;
        }
    }

    node->height = max(get_height(node->left), get_height(node->right)) + 1;

    int balance = get_balance(node);

    // 左左失衡
    if (balance > 1 && get_balance(node->left) >= 0) // 需要加等号
        return ll_rotate(node);

    // 右右失衡
    if (balance < -1 && get_balance(node->right) <= 0) // 需要加等号
        return rr_rotate(node);

    // 左右失衡
    if (balance > 1 && get_balance(node->left) < 0)
        return lr_rotate(node);

    // 右左失衡
    if (balance < -1 && get_balance(node->right) > 0)
        return rl_rotate(node);

    return node;
}

void AVL::erase(int key) {
    header->left = erase_real(key, header->left);
}

int main() {
    AVL avl;

    // test "insert"
    vector<int> intVec{3, 2, 1, 4, 4, 5, 6, 7, 10, 9, 7, 8};

    for (auto i : intVec)
        avl.insert(i);

    //test "find"
    Node *p = nullptr;
    cout << ((p = avl.find(2)) ? p->key : -1) << endl;   //  2
    cout << ((p = avl.find(100)) ? p->key : -1) << endl; // -1

    cout << ((p = avl.loop_find(14)) ? p->key : -1) << endl; // 测试找不到 -1
    cout << ((p = avl.loop_find(5)) ? p->key : -1) << endl;  // 测试找到 5

    // test "erase"
    avl.erase(100);

    avl.erase(9);

    avl.erase(8);

    return 0;
}