#include <math.h>
#include <algorithm>

#define THRESCORR 1e-30

struct WaveChunkData {
    const double* data; 
    int Size;
    double Sum;  // Sum of all the elements: x1 + x2 + ... xn
    double Sum2; // Sum of squares: x1^2 + x2^2 + ... xn^2
    };

//double crosscorr(const double* x, int xSize, double xSum, const double * y, int ySize, double yMean, double ySumCuadraticDiff) {
double crosscorr(const WaveChunkData& x, const double * y, int ySize, double yMean, double ySumCuadraticDiff) {
    //std::cout << x[0] << "|" << xSize << " >>" << std::endl;
    //! Calculate the mean of the two series x[], y[]
    double xMean = x.Sum/x.Size;

    //! Calculate the denominator (product of standard deviations)
    double xSumCuadraticDiff = x.Sum2 + x.Size*xMean*xMean - 2*xMean*x.Sum;
    
    double denom = sqrt(xSumCuadraticDiff * ySumCuadraticDiff);
    if (denom < THRESCORR){
        return 0.0;
        }

    //! Calculate the correlation series
    double best_xcorr = 0.0;

    for (int delay = 0; delay < (ySize - x.Size + 1); ++delay) {
        double xySum = 0.0;
        for (int i = 0; i < x.Size; ++i) {
            xySum += (x.data[i] - xMean) * (y[i + delay] - yMean);
            }
        best_xcorr = std::max(best_xcorr, xySum / denom);
        }
    //std::cout << " >>>> " << best_xcorr << " *\t" << xSize << " =\t" << best_xcorr*xSize << std::endl;
    return best_xcorr*x.Size;
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
    /*
    for (int subvectorStart = 0; subvectorStart <= waveSize - minSubvectorLength; ++subvectorStart) {
        int maxSubvectorLength = std::min(waveSize - subvectorStart, patternSize);
        for (int subvectorLength = minSubvectorLength; subvectorLength <= maxSubvectorLength; ++subvectorLength) { 
            double best_xcorr = crosscorr(&(wave[subvectorStart]), subvectorLength, pattern, patternSize, pMean, pSumCuadraticDiff);
            score = std::max(score, best_xcorr);
            }
        }
    */

    // It is better to go with a constant length wave and just update sum of its elements (specially for large chunks).
    int maxSubvectorLength = std::min(waveSize, patternSize);
    for (int subvectorLength = minSubvectorLength; subvectorLength<=maxSubvectorLength; ++subvectorLength) {
        // All these xcorr will be computed for the same chunk length
        WaveChunkData xData;
        xData.Size = subvectorLength;
        xData.Sum = std::accumulate(wave, wave+subvectorLength, 0.0);
        xData.Sum2 = std::inner_product(wave, wave+subvectorLength, wave, 0.0);
        for(int subvectorStart=0; subvectorStart <= waveSize - minSubvectorLength; ++subvectorStart) {
            if (subvectorLength <= waveSize - subvectorStart) {
                xData.data = &(wave[subvectorStart]);
                double best_xcorr = crosscorr(xData, pattern, patternSize, pMean, pSumCuadraticDiff);
                score = std::max(score, best_xcorr);
                // Update values
                xData.Sum += wave[subvectorStart+subvectorLength] - wave[subvectorStart];
                xData.Sum2 += wave[subvectorStart+subvectorLength]*wave[subvectorStart+subvectorLength] - wave[subvectorStart]*wave[subvectorStart];
                }
            }
        }

    return score;
    }
