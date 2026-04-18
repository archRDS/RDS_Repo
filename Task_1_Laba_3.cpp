#include <iostream>
#include <iomanip>
#include <cmath>
#include <locale>
using namespace std;

double calculateY(double x){
    if (x > -5 && x <= -3){
        return -0.5 * pow(x + 3, 2) + 2;
    } else if (x > -3 && x <= -2){
        return -x-1;
    } else if (x > -2 && x <= -1){
        return sqrt(1 - pow(x+2, 2));
    } else if (x > -1 && x <= 1){
        return -x - 0.5;
    } else if (x > 1 && x <= 2){
        return -1 - sqrt(1 - pow(x-2, 2));
    } else if (x > 2 && x <= 3){
        return -x;
    } else if (x > 3 && x <= 5){
        return 0.5 * pow(x-3, 2) - 3;
    }
    return 0;
}

int main() {
    double xn = -5.0, xk = 5.0, dx = 0.5;
    cout << "X начальное = -5.0 " << endl;
    cout << "Х конечное = 5.0 " << endl;

    for (double x = xn; x <= xk + 0.001; x += dx){
        double y = calculateY(x);
        cout << "Координаты (X,Y) - " << "X: " << x << " " << "Y: " << setprecision(4) << y << endl; 
    }
}