#include <iomanip>
#include <iostream>

using namespace std;

enum RBTColor {
    RED, BLACK
};

class RBTNode {
public:
    RBTColor color;    // 颜色
    int key;            // 关键字(键值)
    RBTNode *left;    // 左孩子
    RBTNode *right;    // 右孩子
    RBTNode *parent; // 父结点

    RBTNode(int value, RBTColor c, RBTNode *p, RBTNode *l, RBTNode *r) :
            key(value), color(c), parent(), left(l), right(r) {}
};

class RBTree {
private:
    RBTNode *mRoot;    // 根结点

public:
    RBTree();

    ~RBTree();

    // 前序遍历"红黑树"
    void preOrder();

    // 中序遍历"红黑树"
    void inOrder();

    // 后序遍历"红黑树"
    void postOrder();

    // (递归实现)查找"红黑树"中键值为key的节点
    RBTNode *search(int key);

    // (非递归实现)查找"红黑树"中键值为key的节点
    RBTNode *iterativeSearch(int key);

    // 查找最小结点：返回最小结点的键值。
    int minimum();

    // 查找最大结点：返回最大结点的键值。
    int maximum();

    // 找结点(x)的后继结点。即，查找"红黑树中数据值大于该结点"的"最小结点"。
    RBTNode *successor(RBTNode *x);

    // 找结点(x)的前驱结点。即，查找"红黑树中数据值小于该结点"的"最大结点"。
    RBTNode *predecessor(RBTNode *x);

    // 将结点(key为节点键值)插入到红黑树中
    void insert(int key);

    // 删除结点(key为节点键值)
    void remove(int key);

    // 销毁红黑树
    void destroy();

    // 打印红黑树
    void print();

private:
    // 前序遍历"红黑树"
    void preOrder(RBTNode *tree) const;

    // 中序遍历"红黑树"
    void inOrder(RBTNode *tree) const;

    // 后序遍历"红黑树"
    void postOrder(RBTNode *tree) const;

    // (递归实现)查找"红黑树x"中键值为key的节点
    RBTNode *search(RBTNode *x, int key) const;

    // (非递归实现)查找"红黑树x"中键值为key的节点
    RBTNode *iterativeSearch(RBTNode *x, int key) const;

    // 查找最小结点：返回tree为根结点的红黑树的最小结点。
    RBTNode *minimum(RBTNode *tree);

    // 查找最大结点：返回tree为根结点的红黑树的最大结点。
    RBTNode *maximum(RBTNode *tree);

    // 左旋
    void leftRotate(RBTNode *&root, RBTNode *x);

    // 右旋
    void rightRotate(RBTNode *&root, RBTNode *y);

    // 插入函数
    void insert(RBTNode *&root, RBTNode *node);

    // 插入修正函数
    void insertFixUp(RBTNode *&root, RBTNode *node);

    // 删除函数
    void remove(RBTNode *&root, RBTNode *node);

    // 删除修正函数
    void removeFixUp(RBTNode *&root, RBTNode *node, RBTNode *parent);

    // 销毁红黑树
    void destroy(RBTNode *&tree);

    // 打印红黑树
    void print(RBTNode *tree, int key, int direction);

#define rb_parent(r)   ((r)->parent)
#define rb_color(r) ((r)->color)
#define rb_is_red(r)   ((r)->color==RED)
#define rb_is_black(r)  ((r)->color==BLACK)
#define rb_set_black(r)  do { (r)->color = BLACK; } while (0)
#define rb_set_red(r)  do { (r)->color = RED; } while (0)
#define rb_set_parent(r, p)  do { (r)->parent = (p); } while (0)
#define rb_set_color(r, c)  do { (r)->color = (c); } while (0)

};

/*
 * 构造函数
 */

RBTree::RBTree() : mRoot(NULL) {
    mRoot = NULL;
}

/*
 * 析构函数
 */

RBTree::~RBTree() {
    destroy();
}

/*
 * 前序遍历"红黑树"
 */

void RBTree::preOrder(RBTNode *tree) const {
    if (tree != NULL) {
        cout << tree->key << " ";
        preOrder(tree->left);
        preOrder(tree->right);
    }
}


void RBTree::preOrder() {
    preOrder(mRoot);
}

/*
 * 中序遍历"红黑树"
 */

void RBTree::inOrder(RBTNode *tree) const {
    if (tree != NULL) {
        inOrder(tree->left);
        cout << tree->key << " ";
        inOrder(tree->right);
    }
}


void RBTree::inOrder() {
    inOrder(mRoot);
}

/*
 * 后序遍历"红黑树"
 */

void RBTree::postOrder(RBTNode *tree) const {
    if (tree != NULL) {
        postOrder(tree->left);
        postOrder(tree->right);
        cout << tree->key << " ";
    }
}


void RBTree::postOrder() {
    postOrder(mRoot);
}

/*
 * (递归实现)查找"红黑树x"中键值为key的节点
 */

RBTNode *RBTree::search(RBTNode *x, int key) const {
    if (x == NULL || x->key == key)
        return x;

    if (key < x->key)
        return search(x->left, key);
    else
        return search(x->right, key);
}


RBTNode *RBTree::search(int key) {
    search(mRoot, key);
}

/*
 * (非递归实现)查找"红黑树x"中键值为key的节点
 */

RBTNode *RBTree::iterativeSearch(RBTNode *x, int key) const {
    while ((x != NULL) && (x->key != key)) {
        if (key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    return x;
}


RBTNode *RBTree::iterativeSearch(int key) {
    iterativeSearch(mRoot, key);
}

/*
 * 查找最小结点：返回tree为根结点的红黑树的最小结点。
 */

RBTNode *RBTree::minimum(RBTNode *tree) {
    if (tree == NULL)
        return NULL;

    while (tree->left != NULL)
        tree = tree->left;
    return tree;
}


int RBTree::minimum() {
    RBTNode *p = minimum(mRoot);
    if (p != NULL)
        return p->key;

    return (int) NULL;
}

/*
 * 查找最大结点：返回tree为根结点的红黑树的最大结点。
 */

RBTNode *RBTree::maximum(RBTNode *tree) {
    if (tree == NULL)
        return NULL;

    while (tree->right != NULL)
        tree = tree->right;
    return tree;
}


int RBTree::maximum() {
    RBTNode *p = maximum(mRoot);
    if (p != NULL)
        return p->key;

    return (int) NULL;
}

/*
 * 找结点(x)的后继结点。即，查找"红黑树中数据值大于该结点"的"最小结点"。
 */

RBTNode *RBTree::successor(RBTNode *x) {
    // 如果x存在右孩子，则"x的后继结点"为 "以其右孩子为根的子树的最小结点"。
    if (x->right != NULL)
        return minimum(x->right);

    // 如果x没有右孩子。则x有以下两种可能：
    // (01) x是"一个左孩子"，则"x的后继结点"为 "它的父结点"。
    // (02) x是"一个右孩子"，则查找"x的最低的父结点，并且该父结点要具有左孩子"，找到的这个"最低的父结点"就是"x的后继结点"。
    RBTNode *y = x->parent;
    while ((y != NULL) && (x == y->right)) {
        x = y;
        y = y->parent;
    }

    return y;
}

/*
 * 找结点(x)的前驱结点。即，查找"红黑树中数据值小于该结点"的"最大结点"。
 */

RBTNode *RBTree::predecessor(RBTNode *x) {
    // 如果x存在左孩子，则"x的前驱结点"为 "以其左孩子为根的子树的最大结点"。
    if (x->left != NULL)
        return maximum(x->left);

    // 如果x没有左孩子。则x有以下两种可能：
    // (01) x是"一个右孩子"，则"x的前驱结点"为 "它的父结点"。
    // (01) x是"一个左孩子"，则查找"x的最低的父结点，并且该父结点要具有右孩子"，找到的这个"最低的父结点"就是"x的前驱结点"。
    RBTNode *y = x->parent;
    while ((y != NULL) && (x == y->left)) {
        x = y;
        y = y->parent;
    }

    return y;
}

/*
 * 对红黑树的节点(x)进行左旋转
 *
 * 左旋示意图(对节点x进行左旋)：
 *      px                              px
 *     /                               /
 *    x                               y
 *   /  \      --(左旋)-->           / \                #
 *  lx   y                          x  ry
 *     /   \                       /  \
 *    ly   ry                     lx  ly
 *
 *
 */

void RBTree::leftRotate(RBTNode *&root, RBTNode *x) {
    // 设置x的右孩子为y
    RBTNode *y = x->right;

    // 将 “y的左孩子” 设为 “x的右孩子”；
    // 如果y的左孩子非空，将 “x” 设为 “y的左孩子的父亲”
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;

    // 将 “x的父亲” 设为 “y的父亲”
    y->parent = x->parent;

    if (x->parent == NULL) {
        root = y;            // 如果 “x的父亲” 是空节点，则将y设为根节点
    } else {
        if (x->parent->left == x)
            x->parent->left = y;    // 如果 x是它父节点的左孩子，则将y设为“x的父节点的左孩子”
        else
            x->parent->right = y;    // 如果 x是它父节点的左孩子，则将y设为“x的父节点的左孩子”
    }

    // 将 “x” 设为 “y的左孩子”
    y->left = x;
    // 将 “x的父节点” 设为 “y”
    x->parent = y;
}

/*
 * 对红黑树的节点(y)进行右旋转
 *
 * 右旋示意图(对节点y进行左旋)：
 *            py                               py
 *           /                                /
 *          y                                x
 *         /  \      --(右旋)-->            /  \                     #
 *        x   ry                           lx   y
 *       / \                                   / \                   #
 *      lx  rx                                rx  ry
 *
 */

void RBTree::rightRotate(RBTNode *&root, RBTNode *y) {
    // 设置x是当前节点的左孩子。
    RBTNode *x = y->left;

    // 将 “x的右孩子” 设为 “y的左孩子”；
    // 如果"x的右孩子"不为空的话，将 “y” 设为 “x的右孩子的父亲”
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;

    // 将 “y的父亲” 设为 “x的父亲”
    x->parent = y->parent;

    if (y->parent == NULL) {
        root = x;            // 如果 “y的父亲” 是空节点，则将x设为根节点
    } else {
        if (y == y->parent->right)
            y->parent->right = x;    // 如果 y是它父节点的右孩子，则将x设为“y的父节点的右孩子”
        else
            y->parent->left = x;    // (y是它父节点的左孩子) 将x设为“x的父节点的左孩子”
    }

    // 将 “y” 设为 “x的右孩子”
    x->right = y;

    // 将 “y的父节点” 设为 “x”
    y->parent = x;
}

/*
 * 红黑树插入修正函数
 *
 * 在向红黑树中插入节点之后(失去平衡)，再调用该函数；
 * 目的是将它重新塑造成一颗红黑树。
 *
 * 参数说明：
 *     root 红黑树的根
 *     node 插入的结点        // 对应《算法导论》中的z
 */

void RBTree::insertFixUp(RBTNode *&root, RBTNode *node) {
    RBTNode *parent, *gparent;

    // 若“父节点存在，并且父节点的颜色是红色”
    while ((parent = rb_parent(node)) && rb_is_red(parent)) {
        gparent = rb_parent(parent);

        //若“父节点”是“祖父节点的左孩子”
        if (parent == gparent->left) {
            // Case 1条件：叔叔节点是红色
            {
                RBTNode *uncle = gparent->right;
                if (uncle && rb_is_red(uncle)) {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            }

            // Case 2条件：叔叔是黑色，且当前节点是右孩子
            if (parent->right == node) {
                RBTNode *tmp;
                leftRotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            // Case 3条件：叔叔是黑色，且当前节点是左孩子。
            rb_set_black(parent);
            rb_set_red(gparent);
            rightRotate(root, gparent);
        } else//若“z的父节点”是“z的祖父节点的右孩子”
        {
            // Case 1条件：叔叔节点是红色
            {
                RBTNode *uncle = gparent->left;
                if (uncle && rb_is_red(uncle)) {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            }

            // Case 2条件：叔叔是黑色，且当前节点是左孩子
            if (parent->left == node) {
                RBTNode *tmp;
                rightRotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            // Case 3条件：叔叔是黑色，且当前节点是右孩子。
            rb_set_black(parent);
            rb_set_red(gparent);
            leftRotate(root, gparent);
        }
    }

    // 将根节点设为黑色
    rb_set_black(root);
}

/*
 * 将结点插入到红黑树中
 *
 * 参数说明：
 *     root 红黑树的根结点
 *     node 插入的结点        // 对应《算法导论》中的node
 */

void RBTree::insert(RBTNode *&root, RBTNode *node) {
    RBTNode *y = NULL;
    RBTNode *x = root;

    // 1. 将红黑树当作一颗二叉查找树，将节点添加到二叉查找树中。
    while (x != NULL) {
        y = x;
        if (node->key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    node->parent = y;
    if (y != NULL) {
        if (node->key < y->key)
            y->left = node;
        else
            y->right = node;
    } else
        root = node;

    // 2. 设置节点的颜色为红色
    node->color = RED;

    // 3. 将它重新修正为一颗二叉查找树
    insertFixUp(root, node);
}

/*
 * 将结点(key为节点键值)插入到红黑树中
 *
 * 参数说明：
 *     tree 红黑树的根结点
 *     key 插入结点的键值
 */

void RBTree::insert(int key) {
    RBTNode *z = NULL;

    // 如果新建结点失败，则返回。
    if ((z = new RBTNode(key, BLACK, NULL, NULL, NULL)) == NULL)
        return;

    insert(mRoot, z);
}

/*
 * 红黑树删除修正函数
 *
 * 在从红黑树中删除插入节点之后(红黑树失去平衡)，再调用该函数；
 * 目的是将它重新塑造成一颗红黑树。
 *
 * 参数说明：
 *     root 红黑树的根
 *     node 待修正的节点
 */

void RBTree::removeFixUp(RBTNode *&root, RBTNode *node, RBTNode *parent) {
    RBTNode *other;

    while ((!node || rb_is_black(node)) && node != root) {
        if (parent->left == node) {
            other = parent->right;
            if (rb_is_red(other)) {
                // Case 1: x的兄弟w是红色的
                rb_set_black(other);
                rb_set_red(parent);
                leftRotate(root, parent);
                other = parent->right;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right))) {
                // Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            } else {
                if (!other->right || rb_is_black(other->right)) {
                    // Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。
                    rb_set_black(other->left);
                    rb_set_red(other);
                    rightRotate(root, other);
                    other = parent->right;
                }
                // Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->right);
                leftRotate(root, parent);
                node = root;
                break;
            }
        } else {
            other = parent->left;
            if (rb_is_red(other)) {
                // Case 1: x的兄弟w是红色的
                rb_set_black(other);
                rb_set_red(parent);
                rightRotate(root, parent);
                other = parent->left;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right))) {
                // Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            } else {
                if (!other->left || rb_is_black(other->left)) {
                    // Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。
                    rb_set_black(other->right);
                    rb_set_red(other);
                    leftRotate(root, other);
                    other = parent->left;
                }
                // Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->left);
                rightRotate(root, parent);
                node = root;
                break;
            }
        }
    }
    if (node)
        rb_set_black(node);
}

/*
 * 删除结点(node)，并返回被删除的结点
 *
 * 参数说明：
 *     root 红黑树的根结点
 *     node 删除的结点
 */

void RBTree::remove(RBTNode *&root, RBTNode *node) {
    RBTNode *child, *parent;
    RBTColor color;

    // 被删除节点的"左右孩子都不为空"的情况。
    if ((node->left != NULL) && (node->right != NULL)) {
        // 被删节点的后继节点。(称为"取代节点")
        // 用它来取代"被删节点"的位置，然后再将"被删节点"去掉。
        RBTNode *replace = node;

        // 获取后继节点
        replace = replace->right;
        while (replace->left != NULL)
            replace = replace->left;

        // "node节点"不是根节点(只有根节点不存在父节点)
        if (rb_parent(node)) {
            if (rb_parent(node)->left == node)
                rb_parent(node)->left = replace;
            else
                rb_parent(node)->right = replace;
        } else
            // "node节点"是根节点，更新根节点。
            root = replace;

        // child是"取代节点"的右孩子，也是需要"调整的节点"。
        // "取代节点"肯定不存在左孩子！因为它是一个后继节点。
        child = replace->right;
        parent = rb_parent(replace);
        // 保存"取代节点"的颜色
        color = rb_color(replace);

        // "被删除节点"是"它的后继节点的父节点"
        if (parent == node) {
            parent = replace;
        } else {
            // child不为空
            if (child)
                rb_set_parent(child, parent);
            parent->left = child;

            replace->right = node->right;
            rb_set_parent(node->right, replace);
        }

        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;

        if (color == BLACK)
            removeFixUp(root, child, parent);

        delete node;
        return;
    }

    if (node->left != NULL)
        child = node->left;
    else
        child = node->right;

    parent = node->parent;
    // 保存"取代节点"的颜色
    color = node->color;

    if (child)
        child->parent = parent;

    // "node节点"不是根节点
    if (parent) {
        if (parent->left == node)
            parent->left = child;
        else
            parent->right = child;
    } else
        root = child;

    if (color == BLACK)
        removeFixUp(root, child, parent);
    delete node;
}

/*
 * 删除红黑树中键值为key的节点
 *
 * 参数说明：
 *     tree 红黑树的根结点
 */

void RBTree::remove(int key) {
    RBTNode *node;

    // 查找key对应的节点(node)，找到的话就删除该节点
    if ((node = search(mRoot, key)) != NULL)
        remove(mRoot, node);
}

/*
 * 销毁红黑树
 */

void RBTree::destroy(RBTNode *&tree) {
    if (tree == NULL)
        return;

    if (tree->left != NULL)
        return destroy(tree->left);
    if (tree->right != NULL)
        return destroy(tree->right);

    delete tree;
    tree = NULL;
}


void RBTree::destroy() {
    destroy(mRoot);
}

/*
 * 打印"二叉查找树"
 *
 * key        -- 节点的键值
 * direction  --  0，表示该节点是根节点;
 *               -1，表示该节点是它的父结点的左孩子;
 *                1，表示该节点是它的父结点的右孩子。
 */

void RBTree::print(RBTNode *tree, int key, int direction) {
    if (tree != NULL) {
        if (direction == 0)    // tree是根节点
            cout << setw(2) << tree->key << "(B) is root" << endl;
        else                // tree是分支节点
            cout << setw(2) << tree->key << (rb_is_red(tree) ? "(R)" : "(B)") << " is " << setw(2) << key << "'s " << setw(12) << (direction == 1 ? "right child" : "left child") << endl;

        print(tree->left, tree->key, -1);
        print(tree->right, tree->key, 1);
    }
}


void RBTree::print() {
    if (mRoot != NULL)
        print(mRoot, mRoot->key, 0);
}

int main() {
    int a[] = {1, 4, 3, 6, 9, 7, 2, 5, 8};
    int check_insert = 0;    // "插入"动作的检测开关(0，关闭；1，打开)
    int check_remove = 0;    // "删除"动作的检测开关(0，关闭；1，打开)
    int i;
    int ilen = (sizeof(a)) / (sizeof(a[0]));
    RBTree *tree = new RBTree();

    cout << "== row data: ";
    for (i = 0; i < ilen; i++)
        cout << a[i] << " ";
    cout << endl;

    for (i = 0; i < ilen; i++) {
        tree->insert(a[i]);
        // 设置check_insert=1,测试"添加函数"
        if (check_insert) {
            cout << "== 添加节点: " << a[i] << endl;
            cout << "== 树的详细信息: " << endl;
            tree->print();
            cout << endl;
        }

    }

    cout << "== pre order: ";
    tree->preOrder();

    cout << "\n== in order: ";
    tree->inOrder();

    cout << "\n== after order: ";
    tree->postOrder();
    cout << endl;

    cout << "== min: " << tree->minimum() << endl;
    cout << "== max: " << tree->maximum() << endl;
    cout << "== info : " << endl;
    tree->print();

    // 设置check_remove=1,测试"删除函数"
    if (check_remove) {
        for (i = 0; i < ilen; i++) {
            tree->remove(a[i]);

            cout << "== 删除节点: " << a[i] << endl;
            cout << "== 树的详细信息: " << endl;
            tree->print();
            cout << endl;
        }
    }


    // 销毁红黑树
    tree->destroy();

    return 0;
}
