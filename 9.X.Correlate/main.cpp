
#include <iostream>
#include <sstream>
#include <iterator>
#include <fstream>
#include <iomanip>
#include <vector>

#include "findScore.hpp"

/* Tuenti Challenge #9: X Correlate
*   
*   Optimizations
*       - Some corrections over the source for performance
*       - Update mean value and sum_squares value instead of computing it all again
*       - Compute the offset between the two series, use it to constraint search space
*       - Compute min-possible length for the wave chunk to get a lower_bound for the search
*
*
*   ..pero SIGUE SIENDO MUY LENTO!!
*       - Intentar acotar la búsqueda en el delay. No permitir que haga todos los delays... sólo ¿cuántos?
*
*   ... la otra opción es tirar de heurísticas; utilizar algún greedy algorithm, un hill climb y listo. Pero
*       he querido centrarme antes en la optimización del algoritmo, no en la búsqueda de otro :/
*/


// Main program: read file from argument
int main (int argc, char *argv[]) {
    std::string filename = "testInput.sql";
    if (argc == 2) {
        filename = argv[1]; // Override filename if this argument exists.
        }
      
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        std::cerr << "Cannot read Input file: '" << filename << "'" << std::endl;
        return -1;
        }

    // Parse file
    std::string line;
    std::getline(file, line);
    std::stringstream ss(line);
    int len_pattern, len_wave;
    ss >> len_pattern >> len_wave;

    // Read pattern
    double* pattern = new double[len_pattern];
    for(auto i = 0; i<len_pattern; ++i) {
        file >> pattern[i];
        }
    std::getline(file, line);

    // Read wave
    double* wave = new double[len_wave];
    for(auto i = 0; i<len_wave; ++i) {
        file >> wave[i];
        }
    
    // Compute
    //const clock_t begin_time = clock();    
    std::cout << std::fixed << std::setprecision(4) << findScore(wave, len_wave, pattern, len_pattern) << std::endl;
    //std::cout << "Time:" << float( clock () - begin_time ) /  CLOCKS_PER_SEC << std::endl;
    return 0;
    }
