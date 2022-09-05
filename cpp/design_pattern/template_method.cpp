#include <iostream>

using namespace std;


class TestPaper {
public:
    void build() {
        setPaperName();
        buildQuest();
    }

    void buildQuest() {
        for (int i = 0; i < 3; i++) {
            cout << "question " << i << endl;
        }
    }

    virtual void setPaperName() = 0;

};

class WhTestPaper : public TestPaper {
public:
    void setPaperName() {
        cout << "student name wangheng" << endl;
    }
};


int main(int argc, char *argv[]) {
    WhTestPaper temp;
    temp.build();

    cin.get();
    return 0;
}