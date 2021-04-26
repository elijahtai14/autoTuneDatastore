#include "DataStoreQuadFit.h"
#include <cmath> 
#include <iostream>

// #define DEBUG

#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

DataStoreQuadFit::DataStoreQuadFit(unsigned maxLength) 
{
    this -> maxLength = maxLength;
    this -> maxTemp = 0;
}


unsigned DataStoreQuadFit::length() 
{
    return tempList.size();
}

unsigned DataStoreQuadFit::getMaxLength() 
{
    return maxLength;
}

bool DataStoreQuadFit::append(double temp, double time)
{
    TemperatureNode tn;
    tn.temp = temp;
    tn.time = time;

    // Update the maximum temperature
    if (tn.temp > maxTemp)
    {
        maxTemp = tn.temp;
    }

    std::list<TemperatureNode>::iterator it1;
    std::list<TemperatureNode>::iterator it2;
    std::list<TemperatureNode>::iterator it3;

    // Case 1: If it is a duplicate of a previous, ignore it!
    for (it1 = tempList.begin(); it1 != tempList.end(); it1++) 
    {
        if (it1 -> temp == tn.temp)
        {
            DEBUG_MSG("not appending: " << it1 -> temp << ", duplicate");
            return false;
        }
    }

    // Case 2: If we have extra room, use it!
    filter();
    if (tempList.size() < maxLength) 
    {
        DEBUG_MSG("appending: " << tn.temp << ", extra room");
        tempList.push_back(tn);
        return true;
    }

    // There is only 1 two-gap
    if (tempList.size() <= 3) 
    {   
        DEBUG_MSG("not appending: " << tn.temp << ", only one two-gap" << std::endl);
        return false;
    } 

    // Case 3: We need replace a value... we erase the minimum two gap to keep the readings evenly spaced
    it1 = tempList.begin();               // Points to two before last element
    it2 = tempList.begin(); it2++; it2++; // Points to last element
    it3 = tempList.begin(); it3++;        // Points to element to erase (the element between the minimum two gap)

    // Iterate through all the two gaps
    double minTwoGapVal = 5000;
    while(it2 != tempList.end())
    {
        double twoGapVal = std:: abs(it2 -> temp - it1 -> temp);
        
        it1++;
        it2++;

        if (twoGapVal < minTwoGapVal)
        {
            minTwoGapVal = twoGapVal;
            it3 = it1;
        }
    }
    
    // recalculate maxTemp if we need to
    if (it3 -> temp == maxTemp) 
    {
        maxTemp = -50000;
        for (it1 = tempList.begin(); it1 != tempList.end(); it1++)
        {
            if (it1 -> temp < maxTemp)
            {
                DEBUG_MSG("appending: reset maxTemp" << std::endl);
                maxTemp = it1 -> temp;
            }
        }
    }

    DEBUG_MSG("appending: " << tn.temp << ", evicting: " << it3 -> temp);

    tempList.erase(it3);

    // Insert the new value
    tempList.push_back(tn);
    return true;
}

// Finds the nearest point to temp and returns its time
double DataStoreQuadFit::getTimeFromTemp(double temp)
{  
    // No elements
    if (tempList.size() == 0) 
    {
        return -1;
    }

    std::list<TemperatureNode>::iterator it; 
    double minTempDiff = 5000;
    double time = -1;

    for (it = tempList.begin(); it != tempList.end(); it++)
    {
        // Diff is absolute value of difference between it -> temp and temp. 
        double diff; if (it -> temp > temp) { diff = it -> temp - temp; } else { diff = temp - it -> temp; }
        if (diff < minTempDiff)
        {
            minTempDiff = diff;
            time = it -> time;
        }
    }

    // Let's also test the quadratic fit to see how they match up
    QFinit();
    double time2 = QFapproximateX(temp);
    DEBUG_MSG("getTimeFromTemp() QuadFit: " << time2 << " DataStore: " << time);

    return time;
}

double DataStoreQuadFit::getTempFromTime(double time)
{
    // No elements
    if (tempList.size() == 0) 
    {
        return -1;
    }

    std::list<TemperatureNode>::iterator it; 
    double minTimeDiff = 5000000;
    double temp = -1;

    for (it = tempList.begin(); it != tempList.end(); it++)
    {
        // Diff is absolute value of difference between it -> time and time. 
        double diff; if (it -> time > time) { diff = it -> time - time; } else { diff = time - it -> time; }
        if (diff < minTimeDiff)
        {
            minTimeDiff = diff;
            temp = it -> temp;
        }
    }

    // Let's also test the quadratic fit to see how they match up
    QFinit();
    double temp2 = QFapproximateY(time);
    DEBUG_MSG("getTempFromTime() QuadFit: " << temp2 << " DataStore: " << temp);

    return temp;
}


void DataStoreQuadFit::filter() 
{   
    std::list<TemperatureNode>::iterator it;
    for (it = tempList.begin(); it != tempList.end(); it++)
    {
        if (it -> temp < maxTemp * 0.50)
        {   
            DEBUG_MSG("erasing: " << it -> temp << " < " << maxTemp <<  " / 2");
            tempList.erase(it);
        }
    }
}

// ------------------------ Quad Fit functions

void DataStoreQuadFit::QFinit() 
{   
    // This order CANNOT change. Only separate for debugging/readability
    /*
     * computeR2 relies on all of the previous
     * computeC relies on computeB and computeA
     * computeB and computeA relies on computeSums
     * 
     * we are solving f(x) = Ax^2 + Bx + C
     * NOT 
     * Cx^2 + Bx + A.
     * The latter is a stupid convention.
     */
    QFcomputeSums();
    QFcomputeA();
    QFcomputeB();
    QFcomputeC();
    QFcomputeR2();
} 


void DataStoreQuadFit::QFcomputeSums() 
{
    xbar = 0;
    x2bar = 0;
    ybar = 0;
    sxx = 0;
    sxy = 0;
    sxx2 = 0;
    sx2x2 = 0;
    sx2y = 0;

    // Compute the first 3, sum and then take average

    std::list<TemperatureNode>::iterator it;
    for (it = tempList.begin(); it != tempList.end(); it++)
    {
        double x = it -> time;
        double y = it -> temp;
        xbar += x; x2bar += x * x; ybar += y;
    }

    xbar = xbar / tempList.size();
    x2bar = x2bar / tempList.size();
    ybar = ybar / tempList.size();

    // Compute the rest
    for (it = tempList.begin(); it != tempList.end(); it++) 
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
}

void DataStoreQuadFit::QFcomputeA() 
{
    A = (sx2y * sxx - sxy * sxx2) / (sxx * sx2x2 - sxx2 * sxx2);
}

void DataStoreQuadFit::QFcomputeB() 
{
    B = (sxy * sx2x2 - sx2y  * sxx2) / (sxx * sx2x2 - sxx2 * sxx2);
}

void DataStoreQuadFit::QFcomputeC() 
{
    C = ybar - B * xbar - A * x2bar;
}

void DataStoreQuadFit::QFcomputeR2() 
{   
    double numerator = 0;
    double denominator = 0;

    std::list<TemperatureNode>::iterator it;
    for (it = tempList.begin(); it != tempList.end(); it++)
    {
        double x = it -> time;
        double y = it -> temp;
        double temp = y - C - B * x - A * x * x;
        numerator += temp * temp;
        denominator += (y - ybar) * (y - ybar);
    }
    
    R2 = 1 - (numerator / denominator);
}

double DataStoreQuadFit::QFgetA() 
{
    return A;
}

double DataStoreQuadFit::QFgetB() 
{
    return B;
}

double DataStoreQuadFit::QFgetC() 
{
    return C;
}

double DataStoreQuadFit::QFgetR2() 
{
    return R2;
}

double DataStoreQuadFit::QFapproximateY(double X) 
{
    return (A * X * X + B * X + C);
}

double DataStoreQuadFit::QFapproximateX(double Y) 
{   
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
    if (A >= 0) {
        if (returnVal1 > returnVal2) 
        {
            return(returnVal1);
        }
        return(returnVal2);
    }

    // If it is 'n' shaped, then return the not larger x option 
    if (A <= 0) {
        if (returnVal1 < returnVal2) 
        {
            return(returnVal1);
        }
        return(returnVal2);
    }

    return (returnVal1);
}