#include <math.h>
#include <algorithm>
#include <cassert>

#define THRESCORR 1e-30

struct WaveChunkData {
    const double* data; 
    int Size;
    double Sum;  // Sum of all the elements: x1 + x2 + ... xn
    double Sum2; // Sum of squares: x1^2 + x2^2 + ... xn^2
    };

double crosscorr(const WaveChunkData& x, const double * y, int ySize, double yMean, double ySumCuadraticDiff) {
    //! Calculate the mean of the two series x[], y[]
    double xMean = x.Sum/x.Size;

    //! Calculate the denominator (product of standard deviations)
    double xSumCuadraticDiff = x.Sum2 - x.Size*xMean*xMean; // Conseguimos esta expresión desarrollando la expresión original y simplificando.
                                                            //  TODO: OPTIMIZACION: ¿xSumCuadraticDiff es monótona con respecto a xMean?
                                                            //      En su caso podría desechar xMean más bajas que una dada
                                                            //      basándome en el THRESCORR
    
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
    return best_xcorr;
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

    /* OPTIMIZACIONES relacionadas con la forma en que la onda es dividida para ser comparada con el patrón:
        - Los bucles se realizan de tal forma que mantenemos el tamaño del trozo de onda constante, así
            podemos limitarnos a actualizar algunos datos en vez de tener que recalcularlos cada vez.
        - [OJO!] ¿El valor medio del trozo de onda está relacionado con este valor de correlación? ¿Puedo descartar
            algún cálculo?
    */
    int maxSubvectorLength = std::min(waveSize, patternSize);
    for (int subvectorLength = minSubvectorLength; subvectorLength<=maxSubvectorLength; ++subvectorLength) {
        //std::cout << "Length: " << subvectorLength << std::endl;
        // All these xcorr will be computed for the same chunk length
        WaveChunkData xData;
        xData.Size = subvectorLength;
        xData.Sum = std::accumulate(wave, wave+subvectorLength, 0.0);
        xData.Sum2 = std::inner_product(wave, wave+subvectorLength, wave, 0.0);
        double best_xcorr = 0.0;
        for(int subvectorStart=0; subvectorStart <= waveSize - minSubvectorLength; ++subvectorStart) {
            if (subvectorLength <= waveSize - subvectorStart) {
                xData.data = &(wave[subvectorStart]);
                auto xcorr = crosscorr(xData, pattern, patternSize, pMean, pSumCuadraticDiff);
                best_xcorr = std::max(best_xcorr, xcorr) ;
                
                //std::cout << "\t media: " << xData.Sum/xData.Size << "\t" << xcorr << std::endl;
                // Update values of sum and sum^2
                xData.Sum += wave[subvectorStart+subvectorLength] - wave[subvectorStart];
                xData.Sum2 += wave[subvectorStart+subvectorLength]*wave[subvectorStart+subvectorLength] - wave[subvectorStart]*wave[subvectorStart];
                }
            }
        score = std::max(score, best_xcorr*subvectorLength);
        }

    return score;
    }
