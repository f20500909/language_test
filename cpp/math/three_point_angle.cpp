#include <iostream>
#include <cmath>
using namespace std;

/*
get angle ACB, point C is the center point
A(x1,y1)
B(x2,y2)
C(x3,y3)
*/
double get_angle(double x1, double y1, double x2, double y2, double x3, double y3)
{
    double theta = atan2(x1 - x3, y1 - y3) - atan2(x2 - x3, y2 - y3);
    if (theta > M_PI)
        theta -= 2 * M_PI;
    if (theta < -M_PI)
        theta += 2 * M_PI;

    theta = abs(theta * 180.0 / M_PI);
    return theta;
}

int main()
{
    double x1 = 0;
    double y1 = 1;
    double x2 = 1;
    double y2 = 0;
    double x3 = -1;
    double y3 = -2;
    double angle1 = get_angle(x3, y3, x1, y1, x2, y2);
    double angle2 = get_angle(x1, y1, x2, y2, x3, y3);
    double angle3 = get_angle(x2, y2, x3, y3, x1, y1);
    cout << angle2 << endl;
    cout << angle3 << endl;
    cout << angle1 << endl;

    return 0;
}
