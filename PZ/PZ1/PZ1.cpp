#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>

using namespace std;

void task1() {
    double x;

    cout << "Enter a floating-point number: ";

    if (cin >> x) {
        cout << fixed << setprecision(2);
        cout << "Received number: " << x << '\n';
    }
    else {
        cout << "Invalid input. Please enter a number.\n";
    }
}

void task2_1() {
    const double c = 5.4;
    const double z = cos(c);

    double x;

    if (z < 0) {
        x = exp(-z) + 3 * z;
    }
    else if (z >= 0 && z <= 8) {
        x = log(fabs(z));
    }
    else if (z > 8) {
        x = cos(z) + pow(z, 2);
    }

    const double y = pow(x, 3) * log(pow(x, 2));

    cout << fixed << setprecision(6);
    cout << "c = " << c << '\n';
    cout << "z = cos(c) = " << z << '\n';
    cout << "x = " << x << '\n';
    cout << "y = x^3 * ln(x^2) = " << y << '\n';
}

void task2_2() {
    long long n;
    cout << "Enter an integer n: ";
    cin >> n;

    int lastDigit = llabs(n) % 10;
    int squareLastDigit = 0;

    switch (lastDigit) {
    case 0: squareLastDigit = 0; break;
    case 1: squareLastDigit = 1; break;
    case 2: squareLastDigit = 4; break;
    case 3: squareLastDigit = 9; break;
    case 4: squareLastDigit = 6; break;
    case 5: squareLastDigit = 5; break;
    case 6: squareLastDigit = 6; break;
    case 7: squareLastDigit = 9; break;
    case 8: squareLastDigit = 4; break;
    case 9: squareLastDigit = 1; break;
    }

    cout << "The last digit of the square is: " << squareLastDigit << '\n';
}

void task3_1() {
    int repeat;

    do {
        int n = 0;

        while (n <= 0) {
            cout << "Enter the number of elements n (> 0): ";
            cin >> n;
        }

        long long sumEven = 0;

        for (int i = 1; i <= n; ++i) {
            int x;
            cout << "Enter element " << i << ": ";
            cin >> x;

            if (x % 2 == 0) {
                sumEven += x;
            }
        }

        cout << "Sum of even elements: " << sumEven << endl;

        cout << "Repeat calculation? (1 - yes, 0 - no): ";
        cin >> repeat;

    } while (repeat == 1);
}

void task3_2() {
    int a = 1;
    int b = 3;
    double h = 0.2;

    cout << fixed << setprecision(2);

    cout << "+---------------+--------+" << endl;
    cout << "|   x   \t|  F(x)  |" << endl;
    cout << "+---------------+--------+" << endl;

    for (double x = a; x <= b; x += h) {
        double F = pow(sin(x), 2);
        cout << "| " << x << "\t\t| " << F << "   |" << endl;
    }

    cout << "+---------------+--------+" << endl;
}

int main() {
    task1();
    task2_1();
    task2_2();
    task3_1();
    task3_2();

    return 0;
}