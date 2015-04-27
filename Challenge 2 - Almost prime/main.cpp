

#include <iostream>
#include <fstream>
#include <cstdint>
#include <math.h>
#include <bitset>
#include <vector>
#include <assert.h>

/* Tuenti Challenge #2: Almost prime
*
*  - 
*/



struct prime_numbers {
    // Compute indexes of prime numbers up to N using sieve of Eratosthenes (http://en.wikipedia.org/wiki/Sieve_of_Eratosthenes)
    //  - need to set size to (N+1) in order to access bitset in a natural way.
    template <int N>
    static std::bitset<N+1> indexes() {
        auto ret = std::bitset<N+1>().set();    // Initialize all to 1's
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

    // Compute prime numbers up to N
    template <int N>
    static std::vector<std::size_t> list(const std::size_t& first=1, const std::size_t& last=N) {
        std::vector<std::size_t> ret;
        auto bits = indexes<N>();
        for (std::size_t i=first; i<=last; i++) {
            if (bits[i]) {
                ret.push_back(i);
                }
            }
        return ret;
        };
};


// Compute number of 'almost-primes' contained in the interval [start,end]
uint16_t compute_almost_primes(const uint16_t& start, const uint16_t& end, const std::vector<std::size_t>& prime_numbers) {
    uint16_t ret = 0;
    for (auto it1=prime_numbers.begin(); it1!=prime_numbers.end(); ++it1) {
        for (auto it2=prime_numbers.begin(); it2!=prime_numbers.end(); ++it2) {
            auto product = (*it1 * *it2);
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

    // Compute prime numbers from 2 to 2^8/2 which is the maximum factor we can expect (2*128 = 256).
    auto primes = prime_numbers::list<128>(2, 128);

    uint16_t n_cases, start, end;
    file >> n_cases;
    while(file >> start >> end ) {
        std::cout << start << end;
        //std::cout << compute_almost_primes(start, end, primes) << std::endl;
        }
    return 0;
}