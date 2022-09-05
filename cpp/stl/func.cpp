
#include<iostream>
#include<iomanip>

using namespace std;

typedef struct Node {
    int data;
    struct Node *next;
} node;


int len, num, M, numbers, i = 0, mod, count = 0;
int *a;
node **p, *s;
float ASL = 0, ASL1 = 0;


int ListLength(node *head) {
    int length = 0;
    node *p;
    p = head;
    while (p) {
        length++;
        p = p->next;
    }
    return length;
}

void Insert(node *&head, node *s) {
    node *p;
    p = head;
    while (p->next) {
        p = p->next;
    }
    p->next = s;
}

void Print(node *head) {
    node *p;
    p = head;
    while (p) {
        cout << "->" << p->data;
        p = p->next;
    }
}


void f1() {
    cout << "Input HashTable address length:";
    cin >> len;
    cout << "Input the value of M in Hash function H(k)=k%M:";
    cin >> M;
    a = new int[len];
    cout << "Input the number of data to be hashed into the hash table:";
    cin >> numbers;
    for (i = 0; i < len; i++) {
        a[i] = -1;
    }
    cout << "please input the keys:";
    for (i = 1; i <= numbers; i++) {
        cin >> num;
        mod = num % M;
        if (a[mod] == -1) {
            a[mod] = num;
            ASL += 1;
        } else {
            while (a[(++mod) % len] != -1) {
                count++;
            }
            a[mod] = num;
            ASL += (count + 2);
        }
    }

    for (i = 0; i < len; i++) {
        count = 0;
        if (a[i] == -1)
            ASL1 += 0;
        else {
            int j = i;
            while (a[(++j) % len] != -1) {
                count++;
            }
            ASL1 += (count + 1);
        }
    }
    cout << "the hash table is as followed:" << endl << endl;
    cout << "HashAddress";
    for (i = 0; i < len; i++) {
        cout << setw(4) << i;
    }
    cout << endl;
    cout << "    Key    ";
    for (i = 0; i < len; i++) {
        if (a[i] == -1)
            cout << setw(4) << '\0';
        else
            cout << setw(4) << a[i];
    }
    cout << endl << endl;
    cout
            << "Hash search successfully using linear detection method to resolve conflicts and the average search length ASL is:";
    cout << (float) ASL / numbers << endl;
    cout
            << "Hash search unsuccessfully using linear detection method to resolve conflicts and the average search length ASL is:";
    cout << (float) ASL1 / len << endl;
}


void f2() {
    cout << "input HashTable address length:";
    cin >> len;
    p = new node *[len];
    cout << "Input the value of M in Hash function H(k)=k%M:";
    cin >> M;
    cout << "Input the number of key to be hashed into the hash table:";
    cin >> numbers;
    for (i = 0; i < len; i++) {
        p[i] = NULL;
    }
    cout << "please input the keys:";
    for (i = 0; i < numbers; i++) {
        cin >> num;
        mod = num % M;
        if (p[mod] == NULL) {
            p[mod] = new node;
            p[mod]->data = num;
            p[mod]->next = NULL;
            ASL += 1;
        } else {
            s = new node;
            s->data = num;
            s->next = NULL;
            Insert(p[mod], s);
            ASL += ListLength(p[mod]);
        }
    }
    for (i = 0; i < len; i++) {
        if (p[i] == NULL)
            ASL1 += 0;
        else
            ASL1 += ListLength(p[i]);
    }
    cout << "the hash table is as followed:" << endl << endl;
    for (i = 0; i < len; i++) {
        cout << setiosflags(ios::left) << setw(3) << i;
        if (p[i] == NULL)
            cout << '^' << endl;
        else {
            Print(p[i]);
            cout << endl;
        }
    }
    cout << endl;
    cout
            << "Hash search successfully using chain address method to resolve conflicts and the average search length ASL is:";
    cout << (float) ASL / numbers << endl;
    cout
            << "Hash search unsuccessfully using chain address method to resolve conflicts and the average search length ASL is:";
    cout << (float) ASL1 / len << endl;
}


int main() {
    int choice;
    cout << "1.Linear detection and re hash " << endl;
    cout << "2.separate chaining " << endl;
    cout << "please choose a method to solve the address conflict:";
    cin >> choice;
    system("cls");
    switch (choice) {
        case 1:
            f1();
            break;
        case 2:
            f2();
            break;
        default:
            cout << "input error!" << endl;
            break;
    }
}
