#ifndef AutotuneDataStore_h
#define AutotuneDataStore_h

#include <list>

class AutotuneDataStore 
{
   public:  
        AutotuneDataStore(unsigned maxLength);

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


    private:
        unsigned maxLength;
        double maxTemp;

        void filter();
};

#endif