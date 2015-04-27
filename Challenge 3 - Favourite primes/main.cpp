

#include <iostream>
#include <fstream>

// Main program: reads input file from argument
int main (int argc, char *argv[]) {
    std::string filename = "testInput.sql";
    if (argc == 2) {
        filename = argv[1]; // Override filename if this argument exists.
        }
              
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        return -1;
        }
    
    //XZ format: http://tukaani.org/xz/format.html

    std::size_t n_cases, start, end, max_number = 0;
    file >> n_cases;
    std::vector<std::pair<std::size_t, std::size_t>> cases;
    while(file >> start >> end ) {
        max_number = (std::max)(max_number, end);
        cases.push_back(std::make_pair(start, end));
        }

    //std::ofstream output("output.txt");
    // Compute prime numbers from 2 to max_number/2 which is the maximum factor we can expect (2*max_number/2 = max_number).
    auto primes = prime_numbers::list(2, max_number/2);
    for (auto it = cases.begin(); it != cases.end(); ++it) {
        std::cout << compute_almost_primes(it->first, it->second, primes) << std::endl;
        //output <<  compute_almost_primes(it->first, it->second, primes) << std::endl;
        }
    //output.close();
    return 0;
}