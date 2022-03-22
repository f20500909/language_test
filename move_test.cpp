
int main()
{

    // lvalues:
    int i = 42;
    i = 43;      // ok, i is an lvalue
    int *p = &i; // ok, i is an lvalue
    int &foo();
    foo() = 42;       // ok, foo() is an lvalue
    int *p1 = &foo(); // ok, foo() is an lvalue
    // rvalues:
    int foobar();
    int j = 0;
    j = foobar();        // ok, foobar() is an rvalue
    int k = j + 2;       // ok, j+2 is an rvalue
    int *p2 = &foobar(); // error, cannot take the address of an rvalue
    j = 42;              // ok, 42 is an rvalue
}