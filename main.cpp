#include "QuadFit.h"
#include "AutotuneDataStore.h"
#include "iostream"

int main() {
    // x^2 + 2x + 3
    double x[] = {1, 2, 3};
    double y[] = {6, 11, 18};
    unsigned len = 3;

    QuadFit quadFit = QuadFit(x, y, len);

    std::cout.precision(5);

    std::cout << quadFit.getA() << std::endl;
    std::cout << quadFit.getB() << std::endl;
    std::cout << quadFit.getC() << std::endl;
    std::cout << quadFit.getR2() << std::endl;
    std::cout << quadFit.approximateY(1.0)  << std::endl;
    std::cout << quadFit.approximateY(1.1)  << std::endl;

    std::cout << quadFit.approximateX(2.1)  << std::endl;
    std::cout << quadFit.approximateX(2)  << std::endl;
    std::cout << quadFit.approximateX(1.9)  << std::endl;

    return 0;
}