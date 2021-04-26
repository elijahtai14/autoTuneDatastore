#include <iostream>
#include "SortedLinkedList.h"

struct TemperatureNode 
{
    double temp;
    double time;

    double operator-(TemperatureNode& tn) 
    {return this -> temp - tn.temp;}
    bool operator>(TemperatureNode& tn) 
    {return this -> temp > tn.temp;}
    bool operator<(TemperatureNode& tn) 
    {return this -> temp < tn.temp;}
    bool operator>=(TemperatureNode& tn) 
    {return this -> temp >= tn.temp;}
    bool operator<=(TemperatureNode& tn) 
    {return this -> temp <= tn.temp;}
    bool operator==(TemperatureNode& tn) 
    {return this -> temp == tn.temp;}
};

int main() 
{   
    const unsigned MAX_LEN = 5;
    const unsigned LEN = 10;
    
    TemperatureNode tnArray[LEN]; 
    
    for (unsigned i = 0; i < LEN; i++)
    {
        tnArray[i].temp = i * i;
        tnArray[i].time = i;
    }

    SortedLinkedList<TemperatureNode> sll(MAX_LEN);

    TemperatureNode filter; 
    filter.temp = 10;
    filter.time = 0;

    for (unsigned i = 0; i < LEN; i++) 
    {
        sll.insert(tnArray[i], filter);

        for (TemperatureNode* ptr = sll.loopBegin(); ptr != nullptr; ptr = sll.loopNext())
        {
            std::cout << ptr -> temp << " " << ptr -> time << std::endl; 
        }
        std::cout<<std::endl;
    }

    return 0;
}