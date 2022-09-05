/*利用顺序队列，层次建立二叉树*/

#include <iostream>
#include <queue>
using namespace std;
#define MAXSIZE 100
typedef struct Node* Btree;
struct Node   //树的数据结构
{
	int data;
	Node *lchild, *rchild;
};

struct queue  //队列的数据结构
{
	Btree *data[MAXSIZE];
	int front, rear;
};

 

Btree creatbintree()
{
	//1.先将根节点入队，当队列不为空时，循环执行以下操作：
	//2.输入左子树的值，不为空，将其入队
	//3.输入右子树的值，不为空，将其入队
	int a;
	Btree root;
	std::queue<Btree> q;
	cout << "请输入节点值以-1表示空节点：" << endl;
	cin >> a;
	if (a == -1)    //如果第一个节点为空，就直接返回空树
		return NULL;
	else
	{
		root = new Node();
		root->data = a;
		q.push(root);  //根节点入队
	}
	while (q.size()>0)   //当队列不为空
	{
		//先输入左孩子的值，再输入右孩子的值
		Btree p = new Node();
		cin >> a;
		if (a == -1)   //左孩子为空
			p->lchild = NULL;
		else
		{
			p->lchild = new Node();
			p->lchild->data = a;
			q.push(p);
		}
		cin >> a;
		if (a == -1)   //右孩子为空
			p->rchild = NULL;
		else
		{
			p->rchild = new Node();
			p->rchild->data = a;
			q.push(p);
		}

	}
	return root;
}

void showbintree(Btree root)
{
	//1.先将根节点入队，当队列不为空时，循环执行以下操作：
	//2.出队一个元素，访问它
	//3.若左子树不为空，将其入队
	//4.若右子树不为空，将其入队
	std::queue<Btree> Q;
	Btree p;
	if (root == NULL)
		return;
	Q.push(p);
	while (Q.size()>0)
	{
		p = Q.front();
		cout << p->data << ' ';
		if (p->lchild)
			Q.push(p);
		if (p->rchild)
			Q.push(p);
	}
	cout << endl;
}

int main()
{
	Btree root = NULL;
	root = creatbintree();
	showbintree(root);
	cin.get();
	return 0;
}

