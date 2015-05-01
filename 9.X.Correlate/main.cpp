
#include <iostream>
#include <sstream>
#include <iterator>
#include <fstream>
#include <iomanip>
#include <vector>

#include "findScore.hpp"

/* Tuenti Challenge #9: X Correlate
*
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
    const clock_t begin_time = clock();
    for(std::uint16_t i=0; i<10000; ++i)
        findScore(wave, len_wave, pattern, len_pattern);
    std::cout << "Time:" << float( clock () - begin_time ) /  CLOCKS_PER_SEC << std::endl;

    std::cout << std::fixed << std::setprecision(4) << findScore(wave, len_wave, pattern, len_pattern) << std::endl;
    return 0;
    }
