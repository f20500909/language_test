#include <iostream>
#include <cassert>
using namespace std;

struct Node {
	int val=0;
	Node* next=nullptr;
	Node() {};
	Node(int _val) :val(_val) {};
};


class List {
public:
	Node* pHead;
	int _len=0;


	List(){
		pHead = new Node;
		pHead->next = NULL;
	}

	List(int nums,int initNum=0) {

		Node *p = pHead->next;
		while (p != NULL)
		{
			if (p->next == NULL)
			{
				Node *p1 = new Node;
				p1->date = date;
				p1->next = NULL;
				p->next = p1;
				break;
			}
			p = p->next;
		}

	}


	~List() {
		delete pHead;
	}

	void insert(int val) {
		 Node* temp=new Node();
		 temp->val = val;
	}

	void print() {
		Node* temp=pHead;
		while (temp != NULL) {
			cout << temp->val << " ";
			temp = temp->next;
		}
		cout << endl ;

	}
 
};




int main() {
	List list = List(10,0);
	//list.print();
	cin.get();
	return 0;
}