#include <iostream>

#include "DataStoreQuadFit.h"

int main() 
{   
    const unsigned MAX_LEN = 5;
    const unsigned LEN = 10;

    double arr[10] = {20, 21, 22, 23, 24, 25, 26, 27, 28, 29};

    DataStoreQuadFit dsqf = DataStoreQuadFit(5);
    
    for (unsigned i = 0; i < LEN; i++)
    {
       dsqf.insert(arr[i], i);
    }

    dsqf.QFinit();

    std::cout<< dsqf.QFgetA() <<std::endl;
    std::cout<< dsqf.QFgetB() <<std::endl;
    std::cout<< dsqf.QFgetC() <<std::endl;
    std::cout<< dsqf.QFgetR2() <<std::endl;

    for (DataStoreQuadFit::TemperatureNode* ptr = dsqf.tempList.loopBegin(); ptr != nullptr; ptr = dsqf.tempList.loopNext()) 
    {
        std::cout << ptr->temp  << " " << ptr->time << std::endl;
    }

    dsqf.QFinit();
  
    std::cout<< dsqf.QFgetA() <<std::endl;
    std::cout<< dsqf.QFgetB() <<std::endl;
    std::cout<< dsqf.QFgetC() <<std::endl;
    std::cout<< dsqf.QFgetR2() <<std::endl;
   
    return 0;
}