#ifndef DataStoreQuadFit_h
#define DataStoreQuadFit_h

#include <list>

class DataStoreQuadFit
{
   public:  

        DataStoreQuadFit(unsigned maxLength);
        
        // ------------------- Data Store Public Stuff
        struct TemperatureNode 
        {
            double temp;
            double time;
        };

        std::list<TemperatureNode> tempList; 

        unsigned length();
        unsigned getMaxLength(); 
        bool append(double temp, double time);
        double getTimeFromTemp(double temp);
        double getTempFromTime(double time);

        // ------------------- Quad Fit Public Stuff        
        void QFinit(); // Do the calculations. Must call this first
        
        double QFgetA();
        double QFgetB();
        double QFgetC();
        double QFgetR2();

        // Approximates a y value from x value based on fitted curve
        double QFapproximateY(double X);
        
        // Approximates a x value from y value based on fitted curve
        double QFapproximateX(double y);

    private:
        // ------------------- Data Store Private Stuff
        unsigned maxLength;
        
        // Filters values out that are less than half the maxTemp
        double maxTemp;
        void filter();

        // ------------------- Quad Fit Private Stuff
        
        // void QFinit(); Fits the data, called in constructor, and calls the following 5 functions in sequence
        void QFcomputeSums();
        void QFcomputeA();
        void QFcomputeB();
        void QFcomputeC();
        void QFcomputeR2();
            

        // Stores important data
        double A;
        double B;
        double C;
        double R2;

        // Stores less important data
        double xbar;
        double x2bar;
        double ybar;
        double sxx;
        double sxy;
        double sxx2;
        double sx2x2;
        double sx2y;
};

#endif