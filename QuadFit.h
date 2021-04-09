#ifndef quadFit_h
#define quadFit_h

class QuadFit {
    public:
        
        QuadFit(double* datax, double* datay, unsigned len);
        
        
        double getA();
        double getB();
        double getC();
        double getR2();

        // Approximates a y value from x value based on fitted curve
        double approximateY(double X);
        
        // Approximates a x value from y value based on fitted curve
        double approximateX(double y);
    
    private:
    
        // pointer to input data as well as length
        double* datax;
        double* datay;
        unsigned len;

        // Fits the data, called in constructor
        void init();

        // Stores important data
        double A;
        double B;
        double C;
        double R2;

        // Computes the less important stuff below
        void computeSums();

        // Stores less important data
        double xbar;
        double x2bar;
        double ybar;
        double sxx;
        double sxy;
        double sxx2;
        double sx2x2;
        double sx2y;

        void computeA();
        void computeB();
        void computeC();
        void computeR2();
        

};

#endif