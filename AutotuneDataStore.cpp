#include "AutotuneDataStore.h"

AutotuneDataStore::AutotuneDataStore(unsigned maxLength) 
{
    this -> maxLength = maxLength;
}


unsigned AutotuneDataStore::length() 
{
    return tempList.size();
}

unsigned AutotuneDataStore::getMaxLength() 
{
    return maxLength;
}

bool AutotuneDataStore::append(double temp, double time)
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
            return false;
        }
    }

    // Case 2: If we have extra room, use it!
    filter();
    if (tempList.size() < maxLength) 
    {
        tempList.push_back(tn);
        return true;
    }

    // Case 3: We need replace a value... we erase the minimum two gap to keep the readings evenly spaced
    
    it1 = tempList.end(); it1--;                 // Points to last element
    it2 = tempList.end(); it2--; it2--; it2--;   // Points to two before last element
    it3 = tempList.end(); it3--; it3--;          // Points to element to erase (the element between the minimum two gap)

    // There is only 1 two-gap
    if (tempList.size() <= 3) 
    {
        return false;
    } 

    // Iterate through all the two gaps
    double minTwoGapVal = 5000;
    while(it2 != tempList.begin())
    {
        double twoGapVal = it1 -> temp - it2 -> temp;
        if (twoGapVal < minTwoGapVal)
        {
            minTwoGapVal = twoGapVal;
            it3 = it1;
            it3--;
        }
        it1--;
        it2--;
    }
    
    // Remove the minimum two-gap element, and recalculate maxTemp if we need to
    if (it3 -> temp == maxTemp) 
    {
        maxTemp = -50000;
        for (it1 = tempList.begin(); it1 != tempList.end(); it1++)
        {
            if (it1 -> temp < maxTemp)
            {
                maxTemp = it1 -> temp;
            }
        }
    }
    tempList.erase(it3);

    // Insert the new value
    tempList.push_back(tn);
    return true;
}

// Finds the nearest point to temp and returns its time
double AutotuneDataStore::getTimeFromTemp(double temp)
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
    return time;
}

double AutotuneDataStore::getTempFromTime(double time)
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
    return temp;
}

// TooSmall is defined as being less than 0.63 of the max temperature
bool AutotuneDataStore::isTooSmall (const TemperatureNode& tn) 
{
    return (tn.temp < maxTemp * 0.50);
}

void AutotuneDataStore::filter() 
{

    std::list<TemperatureNode>::iterator new_end = std::remove_if(tempList.begin(), tempList.end(),
        [](const TemperatureNode& tn)
        { 
            return (tn.temp < maxTemp * 0.50); 
        });


    tempList.erase(new_end, tempList.end());
}