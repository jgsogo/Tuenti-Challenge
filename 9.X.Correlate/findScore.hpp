#include <math.h>
#include <algorithm>

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) < (Y) ? (Y) : (X))
#define THRESCORR 1e-30

std::vector<double> crosscorr(const double* x, int xSize, const double * y, int ySize, double yMean, double ySumCuadraticDiff)
{
    std::vector<double> xcorr;

    //! Calculate the mean of the two series x[], y[]
    double xMean = std::accumulate(x, x+xSize, 0.0)/xSize;

    //! Calculate the denominator (product of standard deviations)
    double xSumCuadraticDiff = 0.0;
    for (int i = 0; i < xSize; i++) {
        xSumCuadraticDiff += pow(x[i] - xMean, 2);
        }
    
    double denom = sqrt(xSumCuadraticDiff * ySumCuadraticDiff);
    if (denom < THRESCORR){
        xcorr.resize(0);
        return xcorr;
    }

    //! Calculate the correlation series
    xcorr.resize(ySize - xSize + 1);

    for (int delay = 0; delay < xcorr.size(); delay++) {
        double xySum = 0.0;
        for (int i = 0; i < xSize; i++) {
            xySum += (x[i] - xMean) * (y[i + delay ] - yMean);
        }

        xcorr[delay] = xySum / denom;
    }   
    return xcorr;
}

double findScore(const double* wave, int waveSize, const double* pattern, int patternSize){
    double score = 0.0;
    int minSubvectorLength = 2;

    // Precompute data for pattern
    double pMean = std::accumulate(pattern, pattern+patternSize, 0.0)/patternSize;
    double pSumCuadraticDiff = 0.0f;
    for (auto i=0; i<patternSize; ++i) {
        pSumCuadraticDiff += pow(pattern[i]-pMean, 2);
        }
    

    for (int subvectorStart = 0; subvectorStart <= waveSize - minSubvectorLength; subvectorStart++) {
        for (int subvectorLength = minSubvectorLength; subvectorLength <= MIN(waveSize - subvectorStart, patternSize); subvectorLength++) { 
            std::vector<double> xcorrelation = crosscorr(&(wave[subvectorStart]), subvectorLength, pattern, patternSize, pMean, pSumCuadraticDiff);
                
            for (int xcorrelationIndex = 0; xcorrelationIndex < xcorrelation.size(); xcorrelationIndex++) {
                score = MAX(score, xcorrelation[xcorrelationIndex] * subvectorLength);
            }   
            }
    }

    return score;
}
