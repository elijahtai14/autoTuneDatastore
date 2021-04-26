#include "QuadFit.h"
#include "math.h" 

QuadFit::QuadFit(double* datax, double* datay, unsigned len) 
{
    this -> datax = datax;
    this -> datay = datay;
    this -> len = len;

    init();
}
void QuadFit::init() 
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
    computeSums();
    computeA();
    computeB();
    computeC();
    computeR2();
} 


void QuadFit::computeSums() 
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
    for (unsigned i = 0; i < len; i++) {
        xbar += datax[i];
        x2bar += datax[i] * datax[i];
        ybar += datay[i];
    }

    xbar = xbar / len;
    x2bar = x2bar / len;
    ybar = ybar / len;

    // Compute the rest
    for (unsigned i = 0; i < len; i++) {
        double diffx = datax[i] - xbar;
        double diffy = datay[i] - ybar;
        double diffx2 = datax[i] * datax[i] - x2bar;

        sxx += diffx * diffx;
        sxy += diffx * diffy;
        sxx2 += diffx * diffx2;
        sx2x2 += diffx2 * diffx2;
        sx2y += diffx2 * diffy;
    }
}

void QuadFit::computeA() 
{
    A = (sx2y * sxx - sxy * sxx2) / (sxx * sx2x2 - sxx2 * sxx2);
}

void QuadFit::computeB() 
{
    B = (sxy * sx2x2 - sx2y  * sxx2) / (sxx * sx2x2 - sxx2 * sxx2);
}

void QuadFit::computeC() 
{
    C = ybar - B * xbar - A * x2bar;
}

void QuadFit::computeR2() 
{   
    double numerator = 0;
    double denominator = 0;

    for (unsigned i = 0; i < len; i++) {
        double temp = datay[i] - C - B * datax[i] - A * datax[i] * datax[i];
        numerator += temp * temp;
        denominator += (datay[i] - ybar) * (datay[i] - ybar);
    }
    
    R2 = 1 - (numerator / denominator);
}

double QuadFit::getA() 
{
    return A;
}

double QuadFit::getB() 
{
    return B;
}

double QuadFit::getC() 
{
    return C;
}

double QuadFit::getR2() 
{
    return R2;
}

double QuadFit::approximateY(double X) 
{
    return (A * X * X + B * X + C);
}

double QuadFit::approximateX(double Y) 
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