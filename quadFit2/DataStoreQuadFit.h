#ifndef DataStoreQuadFit_h
#define DataStoreQuadFit_h

#include "SortedLinkedList.h"
#include "SortedLinkedList.cpp"

class DataStoreQuadFit
{   

    public:  
        DataStoreQuadFit(unsigned maxLength);
        
        // All the data for the quad fit is stored in a SortedLinkedList object
        // Each SortedListNode contains a temperature node 
        struct TemperatureNode 
        {
            double temp;
            double time;

            // Overloading some funny operators so that the SortedLinkedList 
            // knows how to keep things sorted knows how to compare TemperatureNodes
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

        // ------------------- Data Store Public Stuff
        SortedLinkedList<TemperatureNode> tempList; 

        bool insert(double temp, double time);

        // ------------------- Quad Fit Public Stuff        
        void QFinit();   // Do the calculations. Must call this before using the other public QF functions.
        
        double QFgetA(); // Getters
        double QFgetB();
        double QFgetC();
        double QFgetR2();

        // Approximates a y (first function) 
        // or x (second function) based on fitted curve
        double QFapproximateY(double X);
        double QFapproximateX(double y);

    private:
        // ------------------- Quad Fit Private Stuff
        double QFcomputeA(double sxx, double sxy, double sxx2, double sx2x2, double sx2y);
        double QFcomputeB(double sxx, double sxy, double sxx2, double sx2x2, double sx2y);
        double QFcomputeC(double A, double B, double xbar, double ybar, double x2bar);
        double QFcomputeR2(double A, double B, double C, double ybar);
            
        // Stores important data, Ax^2 + Bx + C, and the r-squared value
        double A;
        double B;
        double C;
        double R2;
        
        // QF is Initialized?
        bool QFcheckInit();
        bool isInit;
};
       
#endif