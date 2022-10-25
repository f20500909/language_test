#include <QCoreApplication>
#include <memory>
#include <iostream>
#include <memory>
 
class Point;
typedef std::shared_ptr<Point> ptr_point;
 
class Point 
{
public:
    Point(int _x, int _y) : x(_x), y(_y) {
 
    }
 
    ~Point() {
        std::cout << "~Point()" << std::endl;
    }
 
    ptr_point get_this() {
        return ptr_point(this);
    }
 
private:
    int x;
    int y;
};
 
int main(int argc, char *argv[])
{
 
    {
        Point *raw_pointer = new Point(8, 9);
 
        std::shared_ptr<Point> ptr(raw_pointer);  //[line 1]
        std::shared_ptr<Point> ptr2(raw_pointer); //[line 2]
 
    }
 
    return a.exec();
}