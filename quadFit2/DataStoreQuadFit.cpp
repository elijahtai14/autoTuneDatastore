

#include <cmath> 
#include <iostream>

#include "DataStoreQuadFit.h"

#define DEBUG

#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif


DataStoreQuadFit::DataStoreQuadFit(unsigned maxLength) 
{
    
    tempList.setMax(maxLength); 
    this -> A = 0;
    this -> B = 0;
    this -> C = 0;
    this -> R2 = 0;   
    this -> isInit = false;
}

bool DataStoreQuadFit::insert(double temp, double time)
{
    // TemperatureNode to insert
    TemperatureNode tn; tn.temp = temp; tn.time = time;

    // Filter out TemperatureNodes less than half of the maximum node
    TemperatureNode filter;
    if (tempList.size() > 0) 
    {
        filter = tempList.getMax(); filter.temp = filter.temp * 0.5;
    }
    else
    {
        filter.temp = 0; filter.time = 0;
    }
     
    // Insert
    return tempList.insert(tn, filter);
}


// ------------------------ Quad Fit functions

void DataStoreQuadFit::QFinit() 
{
    /*
     * we are solving f(x) = Ax^2 + Bx + C
     * NOT 
     * Cx^2 + Bx + A.
     * The latter is a stupid convention.
     */

    // Not enough elements to compute. Note: doesn't set isInit to true
    if (tempList.size() < 3) 
    {
        DEBUG_MSG("QFinit() size() To Small");
        return;
    }

    double xbar = 0; double x2bar = 0; double ybar = 0;
    double sxx = 0; double sxy = 0;
    double sxx2 = 0; double sx2x2 = 0; double sx2y = 0;

    // Compute the first 3, sum and then take average
    for (TemperatureNode* it = tempList.loopBegin(); it != nullptr; it = tempList.loopNext())
    {
        double x = it -> time;
        double y = it -> temp;
        xbar += x; x2bar += x * x; ybar += y;
    }

    xbar = xbar / tempList.size();
    x2bar = x2bar / tempList.size();
    ybar = ybar / tempList.size();

    // Compute the rest
    for (TemperatureNode* it = tempList.loopBegin(); it != nullptr; it = tempList.loopNext())
    {
        double x = it -> time;
        double y = it -> temp;
        double diffx = x - xbar;
        double diffy = y - ybar;
        double diffx2 = x * x - x2bar;

        sxx += diffx * diffx;
        sxy += diffx * diffy;
        sxx2 += diffx * diffx2;
        sx2x2 += diffx2 * diffx2;
        sx2y += diffx2 * diffy;
    }

    double a = QFcomputeA(sxx, sxy, sxx2, sx2x2, sx2y);
    double b = QFcomputeB(sxx, sxy, sxx2, sx2x2, sx2y);
    double c = QFcomputeC(a, b, xbar, ybar, x2bar);
    double r2 = QFcomputeR2(a, b, c, ybar);

    A = a; B = b; C = c; R2 = r2;
    isInit = true;
}

double DataStoreQuadFit::QFcomputeA(double sxx, double sxy, double sxx2, double sx2x2, double sx2y) 
{
    return (sx2y * sxx - sxy * sxx2) / (sxx * sx2x2 - sxx2 * sxx2);
}

double DataStoreQuadFit::QFcomputeB(double sxx, double sxy, double sxx2, double sx2x2, double sx2y) 
{
   return (sxy * sx2x2 - sx2y  * sxx2) / (sxx * sx2x2 - sxx2 * sxx2);
}

double DataStoreQuadFit::QFcomputeC(double a, double b, double xbar, double ybar, double x2bar) 
{
    return ybar - b * xbar - a * x2bar;
}

double DataStoreQuadFit::QFcomputeR2(double A, double B, double C, double ybar) 
{   
    double numerator = 0;
    double denominator = 0;

    for (TemperatureNode* it = tempList.loopBegin(); it != nullptr; it = tempList.loopNext())
    {
        double x = it -> time;
        double y = it -> temp;
        double temp = y - C - B * x - A * x * x;
        numerator += temp * temp;
        denominator += (y - ybar) * (y - ybar);
    }
    return 1 - (numerator / denominator);
}

double DataStoreQuadFit::QFgetA() 
{
    if (!QFcheckInit())
    {
        return -1;
    }

    return A;
}

double DataStoreQuadFit::QFgetB() 
{
    if (!QFcheckInit())
    {
        return -1;
    }

    return B;
}

double DataStoreQuadFit::QFgetC() 
{
    if (!QFcheckInit())
    {
        return -1;
    }

    return C;
}

double DataStoreQuadFit::QFgetR2() 
{
    if (!QFcheckInit())
    {
        return -1;
    }

    return R2;
}

double DataStoreQuadFit::QFapproximateY(double X) 
{
    if (!QFcheckInit())
    {
        return -1;
    }

    return (A * X * X + B * X + C);
}

double DataStoreQuadFit::QFapproximateX(double Y) 
{   
    if (!QFcheckInit())
    {
        return -1;
    }

    // Because the inverse of a quadratic does not pass the vertical line test,
    // there may be 1, 2, or 0 solutions.

    double temp = B / (2 * A);

    // Solution 1
    double returnVal1 = (sqrt(((Y - C) / A) + temp * temp) - temp); 

    // Solution 2
    double returnVal2 = (-sqrt(((Y - C) / A) + temp * temp) - temp); 

    
    if (isnan(returnVal1) || isnan(returnVal2)) 
    {
        return (-1);
    }

    // If it is 'u' shaped, then return the not smaller x value
    if (A >= 0) 
    {
        if (returnVal1 > returnVal2) 
        {
            return(returnVal1);
        }
        return(returnVal2);
    }

    // If it is 'n' shaped, then return the not larger x option 
    if (A <= 0) 
    {
        if (returnVal1 < returnVal2) 
        {
            return(returnVal1);
        }
        return(returnVal2);
    }

    return (returnVal1);
}

// It checks if the regression has been calculated.
// If it is already calculated or it is successfully calculated, return true
// If fails to calculate, return false;
bool DataStoreQuadFit::QFcheckInit()
{
    if (!isInit) 
    {
        DEBUG_MSG("QFcheckInit() Initializing...");
        QFinit();
        if (!isInit)
        {
            DEBUG_MSG("QFcheckInit() Error");
            return false;
        }
    }
    DEBUG_MSG("QFcheckInit() Passed");
    return true;
}