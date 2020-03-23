#include <iostream>
#include <iomanip>
#include <cmath>

struct Point
{
    double x;
    double y;
};

using std::cout;
using std::setw;
using bringOut = double(*)(Point);

void outputDate(Point p, bringOut function, bringOut diffx, bringOut diffy);
Point createNewPoint(Point previous, double alf);
double targetFunction(Point p);
double differentialFuncOnX(Point p);
double differentialFuncOnY(Point p);
bool condition(Point p, double eps);

int main()
{
    double eps = pow(10, -4);
    Point currentPoint = {0, 0};
    double alf = 1;

    cout << "--------------------------------------------------------------------\n";
    cout << "|     x     |     y     |   f(x,y)   |  df(x,y)/dx  |  df(x,y)/dy  |\n";

    while (condition(currentPoint, eps))
    {
        outputDate(currentPoint, targetFunction, differentialFuncOnX, differentialFuncOnY);
        Point nextPoint = createNewPoint(currentPoint, alf);

        if (targetFunction(currentPoint) < targetFunction(nextPoint))
            alf /= 2;
        else
            currentPoint = nextPoint;
    }

    outputDate(currentPoint, targetFunction, differentialFuncOnX, differentialFuncOnY);
    cout << "--------------------------------------------------------------------\n";

    cout << "min F(x) = " << targetFunction(currentPoint) << " при (" << currentPoint.x << "; " << currentPoint.y << ").\n";

    return 0;
}

double differentialFuncOnX(Point p)
{
    return (11.96*p.x*exp(5.98*p.x*p.x + 0.38*p.y*p.y) + 28);
}

double differentialFuncOnY(Point p)
{
    return (0.76*p.y*exp(5.98*p.x*p.x + 0.38*p.y*p.y) + 1.3);
}

Point createNewPoint(Point previous, double alf)
{
    Point newPoint;

    newPoint.x = previous.x - alf*differentialFuncOnX(previous);
    newPoint.y = previous.y - alf*differentialFuncOnY(previous);

    return newPoint;
}
 
double targetFunction(Point p)
{
    return 28*p.x + 1.3*p.y + exp(5.98*p.x*p.x + 0.38*p.y*p.y);
}

void outputDate(Point p, bringOut function, bringOut d_Fxy_on_d_x, bringOut d_Fxy_on_d_y)
{
    cout << "|-----------+-----------+------------+--------------+--------------|\n";
    cout << '|' << setw(11) << p.x;
    cout << '|' << setw(11) << p.y;
    cout << '|' << setw(12) << function(p);
    cout << '|' << setw(14) << d_Fxy_on_d_x(p);
    cout << '|' << setw(14) << d_Fxy_on_d_y(p) << "|\n";
}

bool condition(Point p, double eps)
{
    if (((fabs(differentialFuncOnX(p)) < eps/2) && (fabs(differentialFuncOnY(p)) < eps/2)))
        return false;
    return true;
}