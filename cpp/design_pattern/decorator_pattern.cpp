#include <iostream>
#include <mutex>

using namespace std;


class IWomen {
public:
    virtual void lookIntoTheMirror() = 0;
};


class Wife : public IWomen {
public:
    void lookIntoTheMirror() {
        cout << "woman " << endl;
    }
};

class DecoratorWomen : public IWomen {
public:
    DecoratorWomen(IWomen *woman)
            : m_women(woman) {

    }

    void lookIntoTheMirror() {
        m_women->lookIntoTheMirror();
        makeUp();
    }

    virtual void makeUp() = 0; //化妆
protected:
    IWomen *m_women;
};


class Lipstick : public DecoratorWomen {
public:
    Lipstick(IWomen *woman) : DecoratorWomen(woman) {};

    void makeUp() {
        cout << "  do red";
    }
};


class Mascara : public DecoratorWomen {
public:
    Mascara(IWomen *woman) : DecoratorWomen(woman) {};

    void makeUp() {
        cout << "  do yellow ";
    }
};

class Perfume : public DecoratorWomen {
public:
    Perfume(IWomen *woman) : DecoratorWomen(woman) {};

    void makeUp() {
        cout << "  do white ";
    }
};


int main() {
    IWomen *myWife = new Wife();
    Lipstick *lips = new Lipstick(myWife);
    Mascara *mas = new Mascara(lips);
    Perfume *per = new Perfume(mas);
    myWife = per;
    myWife->lookIntoTheMirror();
}
