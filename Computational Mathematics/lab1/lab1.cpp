#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

struct Function
{
    double x;
    double y;
};

struct SystemOfEquation
{
    double a;
    double b;
    double c;
    double d;
};

struct RunningCoefficient
{
    double lamda;
    double mu;
};

struct Polynom
{
    double x;
    double cubicSpline;
};

struct Table
{
    int n;
    double maxAbsError;
    double estimatedAbsError;
    double errorRatio;
};

using Spreadsheet = std::vector <Table>;

void createFunction(Function *&sinus, int n);
void createSysOfEquations(SystemOfEquation *&s, Function f[], int n);
void decideSysOfEquations(double *&m, SystemOfEquation s[], int n);
void createPolynom(Polynom *&spline, Function f[], double m[], int n);
double findMaxAbsError(Polynom sp[], int n);
void printTab(Spreadsheet tab);

void printDecisionOfSys(double m[], int n);
void printSysOfEquations(SystemOfEquation s[], int n);
void printPoints(Function sinus[], int n);
void printPolynom(Polynom sp[], int n);

int main()
{
    Spreadsheet tab;
    int lineNumber = 0;
    for (int n = 5; n <= 10240; n *= 2)
    {
        Function *f = new Function[n + 1];
        SystemOfEquation *s = new SystemOfEquation[n + 1];
        double *m = new double[n + 1];
        Polynom *spline = new Polynom[n];

        createFunction(f, n);
        createSysOfEquations(s, f, n);
        decideSysOfEquations(m, s, n);
        createPolynom(spline, f, m, n);

        double maxAbsEr = findMaxAbsError(spline, n);

        if (n == 5)
            tab.push_back({n, maxAbsEr, 0, 0});
        else
        {
            double deltOc = tab[lineNumber].maxAbsError / 16;
            double ratio = tab[lineNumber].maxAbsError / maxAbsEr;
            tab.push_back({n, maxAbsEr, deltOc, ratio});
            lineNumber++;
        }
        delete [] f;
        delete [] s;
        delete [] m;
        delete [] spline;
    }

    printTab(tab);
    return 0;
}

void createFunction(Function *&sinus, int n)
{
    double a = 0, b = acos(-1.0);
    double h = b/n;
    for (int i = 0; i < n; i++)
    {
        sinus[i] = {a, sin(a)};
        a += h;
    }
    sinus[n] = {b, 0};
}

void createSysOfEquations(SystemOfEquation *&s, Function f[], int n)
{
    s[0] = {1, 0, 0, 0};
    s[n] = {1, 0, 0, 0};
    for (int i = 1; i < n; i++)
    {
        double a = (f[i + 1].x - f[i - 1].x) / 3;
        double b = (f[i + 1].x - f[i].x) / 6;
        double c = (f[i].x - f[i - 1].x) / 6;
        double d = (f[i + 1].y - f[i].y)/(f[i + 1].x - f[i].x) - (f[i].y - f[i - 1].y)/(f[i].x - f[i - 1].x);
        s[i] = {a, b, c, d};
    }
}

void decideSysOfEquations(double *&m, SystemOfEquation s[], int n)
{
    RunningCoefficient *r = new RunningCoefficient[n + 1];
    r[0] = {-s[0].b/s[0].a, s[0].d/s[0].a};
    for (int i = 1; i < n + 1; i++)
    {
        double denominator = s[i].a + s[i].c*r[i - 1].lamda;
        double lambda = -s[i].b / denominator;
        double mu = (s[i].d - s[i].c*r[i - 1].mu) / denominator;

        r[i] = {lambda, mu};
    }

    m[n] = r[n].mu;
    for (int i = n - 1; i >= 0; i--)
        m[i] = r[i].lamda*m[i + 1] + r[i].mu;
    
    delete [] r;
}

void createPolynom(Polynom *&spline, Function f[], double m[], int n)
{
    for (int i = 1; i < n + 1; i++)
    {
        double x = (f[i].x + f[i - 1].x)/2;
        double XiX = f[i].x - x;
        double hi = f[i].x - f[i - 1].x;
        double XXi_1 = x - f[i - 1].x;

        double sp = (pow(XiX, 3) - pow(hi, 2)*XiX)*m[i - 1] / (6*hi);
        sp += (pow(XXi_1, 3) - pow(hi, 2)*XXi_1)*m[i] / (6*hi);
        sp += XiX*f[i - 1].y / hi;
        sp += XXi_1*f[i].y / hi;
        spline[i - 1] = {x, sp};
    }
}

double findMaxAbsError(Polynom sp[], int n)
{
    double max = 0;
    for (int i = 0; i < n; i++)
        if (abs(sp[i].cubicSpline - sin(sp[i].x)) > max)
            max = abs(sp[i].cubicSpline - sin(sp[i].x));
    return max;
}

void printTab(Spreadsheet tab)
{
    cout << "-------------------------------------------------------\n";
    cout << "|  n  |     d_Max     |     d_оц      |      K_d      |\n";
    cout << "|-----+---------------+---------------+---------------|\n";
    cout << '|' << setw(5) << tab[0].n;
    cout << '|' << setw(15) << scientific << std::setprecision(2) << tab[0].maxAbsError;
    cout << '|' << "       -       ";
    cout << '|' << "       -       ";
    cout << "|\n";
    for (int i = 1; i < tab.size(); i++)
    {
        cout << scientific << std::setprecision(2);
        cout << "|-----+---------------+---------------+---------------|\n";
        cout << '|' << setw(5) << tab[i].n;
        cout << '|' << setw(15) << tab[i].maxAbsError;
        cout << '|' << setw(15) << tab[i].estimatedAbsError;
        cout << fixed;
        cout << '|' << setw(15) << tab[i].errorRatio;
        cout << "|\n";
    }
    cout << "-------------------------------------------------------\n";
}
////////////////////////////////
void printPolynom(Polynom sp[], int n)
{
    for (int i = 0; i < n; i++)
        cout << "S[" << i << "](" << sp[i].x << ") = " << sp[i].cubicSpline << '\n';
}
void printDecisionOfSys(double m[], int n)
{
    for (int i = 0; i <= n; i++)
        cout << m[i] << '\t';
    cout << endl;
}
void printSysOfEquations(SystemOfEquation s[], int n)
{
    for (int i = 0; i < n + 1; i++)
    {
        cout << "a = " << s[i].a << '\n';
        cout << "\tb = " << s[i].b << '\n';
        cout << "\t\tc = " << s[i].c << '\n';
        cout << "\t\t\td = " << s[i].d << '\n';
    }
}
void printPoints(Function sinus[], int n)
{
    for (int i = 0; i < n + 1; i++)
        cout << i << "\tт. (" << sinus[i].x << "; " << sinus[i].y << ")\n";
}
