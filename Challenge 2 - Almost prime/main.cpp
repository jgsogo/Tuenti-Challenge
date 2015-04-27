

#include <iostream>
#include <fstream>
#include <cstdint>
#include <math.h>
#include <bitset>
#include <vector>
#include <assert.h>

/* Tuenti Challenge #2: Almost prime */

struct prime_numbers {
    // Compute indexes of prime numbers up to N using sieve of Eratosthenes (http://en.wikipedia.org/wiki/Sieve_of_Eratosthenes)
    //  - need to set size to (N+1) in order to access bitset in a natural way.
    static std::vector<bool> indexes(const std::size_t& N) {
        auto ret = std::vector<bool>(N+1, true); // Initialize all to 1's
        auto max = sqrt(static_cast<float>(N));
        for(std::size_t i=2; i<=max; ++i) {
            if (ret[i]) {
                auto j = i*i;
                while(j<=N) {
                    ret[j] = 0;
                    j += i;
                    }
                }
            }
        
        return ret;
        };

    // Compute prime numbers up to last
    static std::vector<std::size_t> list(const std::size_t& first, const std::size_t& last) {
        std::vector<std::size_t> ret;
        auto bits = indexes(last);
        for (std::size_t i=first; i<=last; i++) {
            if (bits[i]) {
                ret.push_back(i);
                }
            }
        return ret;
        };
};


// Compute number of 'almost-primes' contained in the interval [start,end]
std::size_t compute_almost_primes(const std::size_t& start, const std::size_t& end, const std::vector<std::size_t>& prime_numbers) {
    std::size_t ret = 0;
    for (auto it1=prime_numbers.begin(); it1!=prime_numbers.end(); ++it1) {
        if ((*it1 * *it1)> end) break;
        for (auto it2=it1; it2!=prime_numbers.end(); ++it2) {
            auto product = (*it1 * *it2);
            if (product > end) break;
            ret += (product >= start) && (product <= end);
            }
        }
    return ret;
}

// Main program: read file from argument
int main (int argc, char *argv[]) {
    std::string filename = "testInput.sql";
    if (argc == 2) {
        filename = argv[1]; // Override filename if this argument exists.
        }
              
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        return -1;
        }
    
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