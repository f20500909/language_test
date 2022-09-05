#include <iostream>
#include <vector>
using namespace std;
struct ListNode {
	int val;
	ListNode* next;
};
void print(ListNode* pHead) {
	ListNode* _p = pHead;
	while (_p != NULL) {
		cout << _p->val<<"  ";
		_p = _p->next;
	}
	cout << endl;

}
ListNode* initListNode(vector<int>& vec) {
	ListNode* demo=new ListNode();
	demo->val = 0;
	demo->next = NULL;
	ListNode* p = demo;
	for (int i = 0; i < vec.size(); i++) {
			ListNode* pNewNode = new ListNode;
			pNewNode->val = vec[i]; // 将新节点的值赋值为i
			pNewNode->next = NULL;
			p->next = pNewNode; // 上一个节点指向这个新建立的节点
			p = p->next;
	}
	return demo;
}

ListNode* ReverseList(ListNode* pHead) {
	ListNode* res=NULL;
	ListNode* pNode = pHead;
	ListNode* pPrev = NULL;
	while (pNode != NULL) {
		ListNode* pNext = pNode->next;//先记录当前节点的下一个节点
		if (pNext == NULL)
			res = pNode;

		pNode->next = pPrev; //让下一个节点等于前一个节点
		pPrev = pNode;//另前节点等于当前节点
		pNode = pNext;//指向下一个节点
	}
	return res;
}

int main() {
	vector<int> vec = { 1,2,3,4,5 };
	ListNode* demo=initListNode(vec);
	print(demo);
	ListNode* res = ReverseList(demo);
	print(res);

	cin.get();

	return 0;
}