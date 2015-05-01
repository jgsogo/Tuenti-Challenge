#include <math.h>
#include <algorithm>

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) < (Y) ? (Y) : (X))
#define THRESCORR 1e-30

double crosscorr(const double* x, int xSize, const double * y, int ySize, double yMean, double ySumCuadraticDiff)
{
 
    //! Calculate the mean of the two series x[], y[]
    double xMean = std::accumulate(x, x+xSize, 0.0)/xSize;

    //! Calculate the denominator (product of standard deviations)
    double xSumCuadraticDiff = 0.0;
    for (int i = 0; i < xSize; ++i) {
        xSumCuadraticDiff += pow(x[i] - xMean, 2);
        }
    
    double denom = sqrt(xSumCuadraticDiff * ySumCuadraticDiff);
    if (denom < THRESCORR){
        return 0.0;
        }

    //! Calculate the correlation series
    double best_xcorr = 0.0;

    for (int delay = 0; delay < (ySize - xSize + 1); ++delay) {
        double xySum = 0.0;
        for (int i = 0; i < xSize; ++i) {
            xySum += (x[i] - xMean) * (y[i + delay ] - yMean);
        }

        best_xcorr = std::max(best_xcorr, xySum / denom);
    }   
    return best_xcorr*xSize;
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
    
    for (int subvectorStart = 0; subvectorStart <= waveSize - minSubvectorLength; ++subvectorStart) {
        int maxSubvectorLength = std::min(waveSize - subvectorStart, patternSize);
        for (int subvectorLength = minSubvectorLength; subvectorLength <= maxSubvectorLength; ++subvectorLength) { 
            double best_xcorr = crosscorr(&(wave[subvectorStart]), subvectorLength, pattern, patternSize, pMean, pSumCuadraticDiff);
            score = std::max(score, best_xcorr);
            }
        }

    return score;
}
