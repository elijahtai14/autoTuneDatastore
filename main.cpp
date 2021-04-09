#include "QuadFit.h"
#include "AutotuneDataStore.h"
#include "iostream"

int main() {
    // x^2 + 2x + 3
    double x[] = {1, 2, 3};
    double y[] = {6, 11, 18};
    unsigned len = 3;

    QuadFit quadFit = QuadFit(x, y, len);
    AutotuneDataStore atds = AutotuneDataStore(10);

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

    double dlist[11] = {5.51, 5.1, 5.2, 5.35, 5.4, 5.5, 5.6, 5.7, 5.8, 5.9, 6.0};
    for (unsigned i = 0; i < 11; i++) 
    {
        atds.append(dlist[i], i);
    }

    // Dump all temperatures
    for (std::list<AutotuneDataStore::TemperatureNode>::iterator it = atds.tempList.begin(); it != atds.tempList.end(); it++)
    {
        std::cout << it -> temp;
        std::cout << " ";
        std::cout << it -> time << std::endl;
    }

    return 0;
}