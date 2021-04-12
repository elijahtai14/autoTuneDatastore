#include <iostream>
#include "DataStoreQuadFit.h"

int main() 
{

    unsigned len = 4;
    DataStoreQuadFit dsqf = DataStoreQuadFit(len);

    // x^2 + 2x + 3
    double x[] = {1, 1.25, 1.5, 2, 3, 4, 5, 6};
    double y[] = {10, 10, 10.5, 11, 18, 19, 19.5, 20};

    for (unsigned i = 0; i < 8; i++) 
    {
        dsqf.append(y[i], x[i]);
    }

    dsqf.QFinit();

    std::cout.precision(5);

    std::cout << dsqf.QFgetA() << std::endl;
    std::cout << dsqf.QFgetB() << std::endl;
    std::cout << dsqf.QFgetC() << std::endl;
    std::cout << dsqf.QFgetR2() << std::endl;

    std::cout << dsqf.QFapproximateY(1)  << std::endl;
    std::cout << dsqf.QFapproximateY(2)  << std::endl;
    std::cout << dsqf.QFapproximateY(3)  << std::endl;
    std::cout << dsqf.QFapproximateY(4)  << std::endl;
    std::cout << dsqf.QFapproximateY(5)  << std::endl;
    std::cout << dsqf.QFapproximateY(6)  << std::endl;

    std::cout << dsqf.QFapproximateX(9)  << std::endl;
    std::cout << dsqf.QFapproximateX(10)  << std::endl;
    std::cout << dsqf.QFapproximateX(11)  << std::endl;
    std::cout << dsqf.QFapproximateX(12)  << std::endl;
    std::cout << dsqf.QFapproximateX(13)  << std::endl;
    std::cout << dsqf.QFapproximateX(14)  << std::endl;
    std::cout << dsqf.QFapproximateX(15)  << std::endl;
    std::cout << dsqf.QFapproximateX(16)  << std::endl;
    std::cout << dsqf.QFapproximateX(17)  << std::endl;
    std::cout << dsqf.QFapproximateX(18)  << std::endl;
    std::cout << dsqf.QFapproximateX(19)  << std::endl;
    std::cout << dsqf.QFapproximateX(20)  << std::endl;
    std::cout << dsqf.QFapproximateX(21)  << std::endl;


    // Dump all temperatures
    for (std::list<DataStoreQuadFit::TemperatureNode>::iterator it = dsqf.tempList.begin(); it != dsqf.tempList.end(); it++)
    {
        std::cout << it -> temp;
        std::cout << " ";
        std::cout << it -> time << std::endl;
    }

    return 0;
}