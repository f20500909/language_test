#include <iostream>
#include <memory>

using namespace std;

class Parent;

class Child {
public:
		Child() { cout << "construct child  " << endl; }

		~Child() { cout << "destroy child  " << endl; }

		shared_ptr<Parent> myParent;
};


class Parent {
public:
		Parent() { cout << "construct Parent  " << endl; }

		~Parent() { cout << "destroy Parent  " << endl; }

		shared_ptr<Child> myChild;

};

int main() {
	shared_ptr<Child> p1(new Child());
	shared_ptr<Parent> p2(new Parent());
	p1->myParent=p2;
	p2->myChild=p1;


	return 0;

}