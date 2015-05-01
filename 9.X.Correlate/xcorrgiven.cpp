#include "findScore.hpp"

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) < (Y) ? (Y) : (X))
#define THRESCORR 1e-30

std::vector<double> crosscorr(const double* x, int xSize, const double * y, int ySize)
{
	std::vector<double> xcorr;

	//! Calculate the mean of the two series x[], y[]
	double xMean = 0.0;
	for (int i = 0; i < xSize; i++) {
	    if (x[i] >= 0.0 && x[i] <= 15.0)
		xMean += x[i] / xSize;
	}

	double yMean = 0.0;
	for (int i = 0; i < ySize; i++) {
	    if (y[i] >= 0.0 && y[i] <= 15.0)
		yMean += y[i] / ySize;
	}

	//! Calculate the denominator (product of standard deviations)
	double xSumCuadraticDiff = 0.0;
	for (int i = 0; i < xSize; i++) {
		xSumCuadraticDiff += pow(x[i] - xMean, 2);
	}
	
	double ySumCuadraticDiff = 0.0;
	for (int i = 0; i < ySize; i++) {
		ySumCuadraticDiff += pow(y[i] - yMean, 2);
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

	for (int subvectorStart = 0; subvectorStart <= waveSize - minSubvectorLength; subvectorStart++) {
		for (int subvectorLength = minSubvectorLength; subvectorLength <= MIN(waveSize - subvectorStart, patternSize); subvectorLength++) { 
			std::vector<double> xcorrelation = crosscorr(&(wave[subvectorStart]), subvectorLength, pattern, patternSize);
				
			for (int xcorrelationIndex = 0; xcorrelationIndex < xcorrelation.size(); xcorrelationIndex++) {
				score = MAX(score, xcorrelation[xcorrelationIndex] * subvectorLength);
			}	
    		}
	}

	return score;
}
