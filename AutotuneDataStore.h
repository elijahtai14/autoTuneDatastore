#ifndef AutotuneDataStore_h
#define AutotuneDataStore_h

#include <list>

class AutotuneDataStore 
{

    // I like my own linkedList rather than std::list because we only need limited functionality
    struct TemperatureNode 
    {
        double temp;
        double time;
    };

    std::list<TemperatureNode> tempList;

    public:
        AutotuneDataStore(unsigned maxLength);

        unsigned length();
        unsigned getMaxLength(); 
        bool append(double temp, double time);
        double getTimeFromTemp(double temp);
        double getTempFromTime(double time);


    private:
        unsigned maxLength;
        double maxTemp;

        // Filters out values using the predicate isTooSmall 
        void filter(); bool isTooSmall (const TemperatureNode& tn);

};

#endif