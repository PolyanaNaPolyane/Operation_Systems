#include <iostream>
#include <cmath>

// g(a, b) = (a^2 + b^2) / (a^2 + 2ab + 3b^2 + 4)
double g(double a, double b)
{
    double numerator   = a * a + b * b;
    double denominator = a * a + 2.0 * a * b + 3.0 * b * b + 4.0;
    return numerator / denominator;
}

int main()
{
    char again;
    do
    {
        double s, t;
        std::cout << "Enter s: ";
        std::cin >> s;
        std::cout << "Enter t: ";
        std::cin >> t;

        double result = g(1.2, s) + g(t, s) - g(2.0 * s - 1.0, s * t);

        std::cout << "g(1.2, s) + g(t, s) - g(2s-1, st) = " << result << std::endl;

        std::cout << "Repeat? (y/n): ";
        std::cin >> again;
    } while (again == 'y' || again == 'Y');

    return 0;
}