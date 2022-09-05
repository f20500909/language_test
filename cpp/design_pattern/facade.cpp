#include <iostream>
using namespace std;


class Shopper {
public:
    void doShop() {
        cout << "buy item" << endl;
    }

};

class Cook {
public:
    void doCook() {
        cout << "do cook" << endl;
    }

};


class Clear {
public:
    void doClear() {
        cout << "do clear" << endl;
    }

};

class Team {
public:
    Team() {}

    void doPicnic() {
        cout << "go picnic start..." << endl;
        shopper.doShop();
        cooker.doCook();
        clearer.doClear();
        cout << "picnic end..." << endl;
    }

private:
    Shopper shopper;
    Cook cooker;
    Clear clearer;
};


int main() {
    Team *t = new Team();
    t->doPicnic();
    return 0;
}