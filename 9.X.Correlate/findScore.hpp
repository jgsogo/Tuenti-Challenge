#include <math.h>
#include <algorithm>
#include <cassert>

#define THRESCORR 1e-30

struct DataChunk {
    const double* data; 
    double* deviations; // used in patternData.
    int size;
    double sum;  // Sum of all the elements: x1 + x2 + ... xn
    double sum2; // Sum of squares: x1^2 + x2^2 + ... xn^2
    double sum_cuadratic_diff;
    double mean;
    };

std::pair<double, int> crosscorr(const DataChunk& x, const DataChunk& y, int pattern_offset) {
    //! Calculate the denominator (product of standard deviations)
    double denom = sqrt(x.sum_cuadratic_diff * y.sum_cuadratic_diff);
    //! Calculate the correlation series
    std::pair<double, int> best_xcorr = std::make_pair(0.0, 0);

    for (int delay = pattern_offset; delay < (y.size - x.size + 1); ++delay) {
        double xySum = 0.0;
        for (int i = 0; i < x.size; ++i) {
            xySum += (x.data[i] - x.mean) * y.deviations[i+delay];
            }
        if (xySum > best_xcorr.first) {
            best_xcorr = std::make_pair(xySum, delay);
            }
        }
    return std::make_pair(best_xcorr.first/denom, best_xcorr.second);
    }

double findScore(const double* wave, int waveSize, const double* pattern, int patternSize){
    std::pair<double, int> score = std::make_pair(0.0, 0);
    int minSubvectorLength = 2;

    // Precompute data for pattern (it will always be the same)
    DataChunk patternData;
    patternData.mean = std::accumulate(pattern, pattern+patternSize, 0.0)/patternSize;
    patternData.data = pattern;
    patternData.size = patternSize;
    patternData.deviations = new double[patternData.size];
    double pSumCuadraticDiff = 0.0f;
    for (auto i=0; i<patternSize; ++i) {
        pSumCuadraticDiff += pow(pattern[i]-patternData.mean, 2);
        patternData.deviations[i] = pattern[i] - patternData.mean;
        }
    patternData.sum_cuadratic_diff = pSumCuadraticDiff;    
    double xSumCuadraticDiff_THRESHOLD = sqrt(THRESCORR/sqrt(pSumCuadraticDiff));


    /* OPTIMIZACIONES relacionadas con la forma en que la onda es dividida para ser comparada con el patrón:
        - Los bucles se realizan de tal forma que mantenemos el tamaño del trozo de onda constante, así
            podemos limitarnos a actualizar algunos datos en vez de tener que recalcularlos cada vez.
        - [OJO!] ¿El valor medio del trozo de onda está relacionado con este valor de correlación? ¿Puedo descartar
            algún cálculo?
    */
    
    std::pair<int, int> offset = std::make_pair(0,0); // <wave,pattern> offset.
    int maxSubvectorLength = std::min(waveSize-offset.first, patternData.size-offset.second);
    for (int subvectorLength = maxSubvectorLength; subvectorLength>=minSubvectorLength; --subvectorLength) {
        const clock_t begin_time = clock();

        // All these xcorr will be computed for the same chunk length
        DataChunk xData;
        xData.size = subvectorLength;
        xData.sum = std::accumulate(wave+offset.first, wave+subvectorLength+offset.first, 0.0);
        xData.sum2 = std::inner_product(wave+offset.first, wave+subvectorLength+offset.first, wave+offset.first, 0.0);
        double max_expected_xcorr = 0.0;
        //std::cout << "Length: " << subvectorLength << " -> " << minSubvectorLength << " | ";
        for(int subvectorStart=offset.first; subvectorStart + subvectorLength <= waveSize; ++subvectorStart) {
            xData.data = &(wave[subvectorStart]);
            xData.mean = xData.sum/xData.size;
            xData.sum_cuadratic_diff = xData.sum2 - xData.size*xData.mean*xData.mean;
            if (xData.sum_cuadratic_diff > xSumCuadraticDiff_THRESHOLD) {

                auto xcorr = crosscorr(xData, patternData, offset.second);
                
                if (xcorr.first*subvectorLength > (score.first*score.second)) {
                    offset.first = subvectorStart;// - xcorr.second;
                    offset.second = xcorr.second;
                    score = std::make_pair(xcorr.first, subvectorLength);
                    /*
                    std::cout << std::endl << "\tdelay=" << xcorr.second \
                              << "\tstart=" << subvectorStart \
                              << "\tstart-delay=" << subvectorStart - xcorr.second \
                              << "\txcorr=" << xcorr.first \
                              << "\tscore=" << std::fixed << std::setprecision(4) << score.first*score.second;
                    */
                    }
                
                max_expected_xcorr = (xcorr.first + score.first)/2.; // TODO: Guarantees?
                }
                
            // Update values of sum and sum^2
            xData.sum += wave[subvectorStart+subvectorLength] - wave[subvectorStart];
            xData.sum2 += wave[subvectorStart+subvectorLength]*wave[subvectorStart+subvectorLength] - wave[subvectorStart]*wave[subvectorStart];
            }
        //std::cout << " | " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << " | " << std::endl;

        minSubvectorLength = ceil(score.first*score.second); // X-Correlation value cannot be greater than 1 (I have a lower bound for the size of the wave to compare!)
        auto confidence_factor = (1-subvectorLength/(double)patternData.size);
        minSubvectorLength = minSubvectorLength*confidence_factor/max_expected_xcorr;
        }

    return score.first*score.second;
    }
