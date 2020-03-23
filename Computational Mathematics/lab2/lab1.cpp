#include <iostream>
#include <cmath>

using std::cout;
 
double X_k(double x, double y, double alf);
double Y_k(double x, double y, double alf);
double f(double x, double y);
double diffx(double x, double y);
double diffy(double x, double y);

int main()
{
    double eps = 0.0001;
    double x = 0, y = 0, alf = 1;

    cout << diffx << '\t' << diffy << '\n';

    //do
    //{
    //    cout << x << '\t' << y << '\t' << f(x,y) << '\t' << diffx(x, y) << '\t' << diffy(x, y) << '\n';
    //    double tempX = X_k(x, y, alf);
    //    double tempY = Y_k(x, y, alf);
    //    if (f(x, y) < f(tempX, tempY))
    //        alf /= 2;
    //    x = tempX;
    //    y = tempY;
    //} while (((sqrt(pow(diffx(x, y), 2)) + pow(diffy(x, y), 2)) < eps)) || (((abs(diffx(x, y)) < eps/2)) && (abs(diffy(x, y)) < eps/2)));
}

double diffx(double x, double y)
{
    //double h=pow(10,-7);
    //return (f(x+h,y)-f(x,y))/h;
    return (11.96*y*exp(5.98*x*x + 0.38*y*y) + 28)
}
 
double diffy(double x, double y)
{
    //double h=pow(10,-7);
    //return (f(x,y+h)-f(x,y))/h;
    return (0.76*y*exp(5.98*x*x + 0.38*y*y) + 1.3)
}

double X_k(double x, double y, double alf)
{
    return (x - alf*diffx(x, y));
}
double Y_k(double x, double y, double alf)
{
    return (y - alf*diffy(x, y));
}
 
double f(double x, double y)
{
    double a = 28, b = 1.3, c = 5.98, d = 0.38;
    double f = a*x + b*y + exp(c*x*x + d*y*y);
    return f;
}