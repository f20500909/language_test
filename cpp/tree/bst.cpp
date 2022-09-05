#include<iostream>
#include<iomanip>
#include<stack>
#include<queue>

using namespace std;
typedef int ElemType;

class BTNode   //Binary Tree Node
{
public:
    ElemType data;
    BTNode *lchild;   //左孩子
    BTNode *rchild;   //右孩子
    BTNode(ElemType d, BTNode *left = NULL, BTNode *right = NULL) : data(d), lchild(left), rchild(right) {}
};

class BinarySearchTree {
private:
    BTNode *Root;
    int size;
public:
    BinarySearchTree(int *arr, int len);

    ~BinarySearchTree();

    bool empty() { return Root == NULL; }

    int getSize() { return size; }

    int leaf();

    bool search(ElemType);

    BTNode *parent(ElemType);

    BTNode *predecessor(ElemType);

    //获取后继
    BTNode *successor(ElemType);

    BTNode *minimum();

    BTNode *maximum();

    bool insertNode(ElemType);

    bool deleteNode(ElemType);

    void traverse() { inOrderWithoutRecursion(); }

    void inOrderWithoutRecursion();
};

//构造方法
BinarySearchTree::BinarySearchTree(int *arr, int len) {
    size = 0;
    Root = NULL;
    ElemType data;
    for (int i = 0; i < len; i++) {
        insertNode(arr[i]);
    }
}

//析构方法
BinarySearchTree::~BinarySearchTree() {
    if (!empty()) {
        queue<BTNode *> q;
        q.push(Root);
        BTNode *p = NULL;
        while (!q.empty()) {
            p = q.front();
            q.pop();
            //左孩子不为空，则左孩子入队
            if (p->lchild)
                q.push(p->lchild);
            //右孩子不为空，则右孩子入队
            if (p->rchild)
                q.push(p->rchild);
            //释放内存
            delete p;
        }
    }
}

//求叶子节点数
int BinarySearchTree::leaf() {
    int num = 0;
    //按中序遍历
    if (!empty()) {
        stack<BTNode *> s;
        BTNode *p = Root;
        while (!s.empty() || p) {
            if (p) {
                s.push(p);
                p = p->lchild;
            } else {
                p = s.top();
                s.pop();
                //左右子树均为空，则为叶子节点
                if (p->lchild == NULL && p->rchild == NULL)
                    num++;
                p = p->rchild;
            }
        }
    }
    return num;
}

//查找
bool BinarySearchTree::search(ElemType data) {
    if (!empty()) {
        BTNode *p = Root;
        while (p) {
            if (data == p->data)
                return true;
            else if (data < p->data)
                p = p->lchild;
            else
                p = p->rchild;
        }
    }
    //树空或查找失败
    return false;
}

BTNode *BinarySearchTree::parent(ElemType data) {
    if (!empty()) {
        //根节点的父节点为空
        if (Root->data == data)
            return NULL;
        stack<BTNode *> s;
        BTNode *p = Root;
        while (!s.empty() || p) {
            if (p) {
                if ((p->lchild && p->lchild->data == data) || (p->rchild && p->rchild->data == data))
                    return p;
                s.push(p);
                p = p->lchild;
            } else {//左子树访问完后，访问右子树
                p = s.top();
                s.pop();
                p = p->rchild;
            }
        }
    }
    return NULL;
}

//获取前驱
BTNode *BinarySearchTree::predecessor(ElemType data) {
    BTNode *pcur, *plastVisit;
    pcur = plastVisit = NULL;
    if (!empty()) {
        stack<BTNode *> s;
        pcur = Root;
        while (!s.empty() || pcur) {
            if (pcur) {
                //plastVisit = pcur;
                s.push(pcur);
                pcur = pcur->lchild;
            } else {
                pcur = s.top();
                s.pop();
                if (pcur->data == data)
                    return plastVisit;
                else
                    plastVisit = pcur;
                pcur = pcur->rchild;
            }
        }
    }
    return plastVisit;
}

//获取后继
BTNode *BinarySearchTree::successor(ElemType data) {
    BTNode *pcur = NULL;
    pcur = Root;
    if (!empty()) {
        stack<BTNode *> s;
        while (!s.empty() || pcur) {
            if (pcur) {
                s.push(pcur);
                pcur = pcur->lchild;
            } else {
                pcur = s.top();
                s.pop();
                if (pcur->data == data)
                    return pcur->rchild;
                pcur = pcur->rchild;
            }
        }
    }
    //空树
    return NULL;
}

//获取最小节点
BTNode *BinarySearchTree::minimum() {
    //最小节点在左子树最下边
    if (!empty()) {
        BTNode *p = Root;
        while (p->lchild)
            p = p->lchild;
        return p;
    }
    //树空
    return NULL;
}

//获取最大节点
BTNode *BinarySearchTree::maximum() {
    //最大节点在右子树最下边
    if (!empty()) {
        BTNode *p = Root;
        while (p->rchild)
            p = p->rchild;
        return p;
    }
    //树空
    return NULL;
}

//插入新节点
bool BinarySearchTree::insertNode(ElemType data) {
    /*
     新节点都会被插入到叶子处
     插入一般不会失败，除非是插入了重复节点。
    */
    if (Root == NULL) {
        Root = new BTNode(data);
        size++;
        return true;
    } else {
        BTNode *p = Root;
        while (true) {
            if (data < p->data) {
                //如果有左子树，则继续遍历左子树
                if (p->lchild)
                    p = p->lchild;
                else {//否则，插入节点，下同
                    p->lchild = new BTNode(data);
                    break;
                }
            } else if (data > p->data) {
                if (p->rchild)
                    p = p->rchild;
                else {
                    p->rchild = new BTNode(data);
                    break;
                }
            } else//遇到重复节点
                return false;
        }
        //插入新节点成功，节点总数加一
        size++;
        return true;
    }
}

//删除节点
bool BinarySearchTree::deleteNode(ElemType data) {
    /*
    删除规则
    1.若待删节点无左子树，则用其右子树的根节点替换它。
    2.若待删节点有左子树，则在左子树中寻找中序遍历的最后一个节点，用该节点替换它。
    */
    if (!empty()) {
        //树中无此节点，删除失败
        if (!search(data))
            return false;
        /*
        p:待删结点
        Parent:待删除节点的父节点
        temp:替换节点
        tempp:替换节点的父节点
        */
        BTNode *p, *Parent, *temp, *tempp;
        p = Parent = temp = tempp = NULL;
        //获取待删除节点的父节点
        Parent = parent(data);
        //根据父节点，确定待删结点
        if (Parent->lchild && Parent->lchild->data == data)
            p = Parent->lchild;
        else
            p = Parent->rchild;
        //如果左子树不为空，查找其中序遍历的最后一个节点
        if (p->lchild) {
            temp = p->lchild;
            while (temp->rchild) {
                tempp = temp;
                //不断遍历右子树
                temp = temp->rchild;
            }
            //如果p的左孩子即是替换节点
            if (tempp == NULL)
                p->lchild = temp->lchild;
            else//替换节点的左子树作为其父节点的右子树(这句难以理解，需要多想想)
                tempp->rchild = temp->lchild;
            //替换节点继承待删结点的左右孩子
            temp->lchild = p->lchild;
            temp->rchild = p->rchild;
        } else
            temp = p->rchild;
        //替换节点替换掉待删结点(这也是为什么需要找到待删结点的父节点)
        if (Parent == NULL)  //待删结点恰为根节点
            Root = temp;
        else if (Parent->lchild == p)  //待删结点本身处于左子树
            Parent->lchild = temp;
        else//待删结点本身处于右子树
            Parent->rchild = temp;
        //删除待删结点
        delete p;
        //节点总数减一
        size--;
        return true;
    }
    //树空
    return false;
}

//中序遍历
void BinarySearchTree::inOrderWithoutRecursion() {
    if (!empty()) {
        stack<BTNode *> s;
        BTNode *p = Root;
        while (!s.empty() || p) {
            if (p) {
                s.push(p);
                p = p->lchild;
            } else {
                p = s.top();
                s.pop();
                cout << setw(4) << p->data;
                p = p->rchild;
            }
        }
        cout << endl;
    }
}

int main() {
    cout << "******二叉搜索树***by David***" << endl;
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    BinarySearchTree tree(arr, 9);
    cout << "中序遍历" << endl;
    tree.traverse();
    cout << "树中节点总数 " << tree.getSize() << endl;
    cout << "叶子节点数 " << tree.leaf() << endl;
    BTNode *p = NULL;
    p = tree.minimum();
    p ? cout << "最小节点是 " << p->data << endl : cout << "树空！" << endl;
    p = tree.maximum();
    p ? cout << "最大节点是 " << p->data << endl : cout << "树空！" << endl;
    ElemType data = 2;
    cout << endl << "查找节点 " << data << endl;
    if (tree.search(data)) {
        cout << "节点 " << data << " 查找成功！" << endl;
        p = tree.predecessor(data);
        p ? cout << "节点 " << data << " 的前驱是 " << p->data << endl : cout << "无前驱！" << endl;
        p = tree.successor(data);
        p ? cout << "节点 " << data << " 的后继是 " << p->data << endl : cout << "无后继！" << endl;
    } else
        cout << "节点 " << data << "不在树中！" << endl;
    data = 6;
    cout << endl << "删除节点 " << data << endl;
    if (tree.deleteNode(data)) {
        cout << "删除成功！" << endl;
        cout << "中序遍历" << endl;
        tree.traverse();
        cout << "树中节点总数 " << tree.getSize() << endl;
        cout << "叶子节点数 " << tree.leaf() << endl;
        data = 5;
        cout << endl << "查找节点 " << data << endl;
        if (tree.search(data)) {
            cout << "节点 " << data << " 查找成功！" << endl;
            p = tree.predecessor(data);
            p ? cout << "节点 " << data << " 的前驱是 " << p->data << endl : cout << "无前驱！" << endl;
            p = tree.successor(data);
            p ? cout << "节点 " << data << " 的后继是 " << p->data << endl : cout << "无后继！" << endl;
        } else
            cout << "节点 " << data << "不在树中！" << endl;
    } else
        cout << "删除失败！" << endl;
    cout << endl;
    system("pause");
    return 0;
}


