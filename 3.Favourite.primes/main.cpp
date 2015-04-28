

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <limits>

#include "xz.h"  //! TODO: XZ embedded in project http://tukaani.org/xz/embedded.html
#include <boost/multiprecision/cpp_int.hpp> // We need really big integers

/* Tuenti Challenge #3: Favourite primes
*
*   Implementation on my own, really slow doing factorization :/
*   We could use a third party library like http://sourceforge.net/projects/msieve/
*   that implements GNFS (General Number Field Sieve algorithm).
*/


// Get N first prime numbers: will be called once, no need to optimize
std::vector<std::size_t> compute_prime_numbers(const std::size_t& n) {
    std::vector<std::size_t> primes;
    if (n>=1) {
        primes.push_back(2);
        std::size_t prime_candidate = 3;
        while(primes.size() < n) {
            if (std::all_of(primes.begin(), primes.end(), [prime_candidate](const std::size_t& prime) { return prime_candidate % prime != 0;})) {
                primes.push_back(prime_candidate);
                }
            prime_candidate += 2; // only check odd numbers.
            }
        }
    return primes;
    }


// Count factors for large number. Returns true if the rest is 1 (completely factorized).
bool count_factors(const boost::multiprecision::cpp_int& number, const std::vector<std::size_t>& factors, std::map<std::size_t, std::size_t>& count) {
    boost::multiprecision::cpp_int rest = number;
    for (auto it = factors.rbegin(); it!=factors.rend(); ++it) {
        while(0== boost::multiprecision::integer_modulus(rest, *it)) {
            rest = rest/ *it;
            count[*it] += 1;
            }
        } 
    return rest==1;
    }

// 
void sum_counts(std::map<std::size_t, std::size_t>& accumulator, const std::map<std::size_t, std::size_t>& other) {
    for (auto it = other.begin(); it!= other.end(); ++it) {
        accumulator[it->first] += it->second;
        }
    }

// Main program: reads input file from argument
int main (int argc, char *argv[]) {
    // Input file
    std::string filename = "testInput.sql";
    if (argc >= 2) {
        filename = argv[1]; // Override filename if this argument exists.
        }
              
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        return -1;
        }
    
    // Numbers.txt.xz file
    std::string numbers_filename = "numbers.txt"; // TODO: Use xz-embedded to uncompress original file http://tukaani.org/
    if (argc >= 3) {
        numbers_filename = argv[2]; // Override numbers_file if this argument exists.
        }
    std::ifstream numbers_file(numbers_filename.c_str());
    if (!numbers_file.is_open()) {
        return -1;
        }
    std::vector<std::string> rock_content; // Really big numbers in the rock!.
    std::string line;
    std::size_t max_length = 0;
    while (std::getline(numbers_file, line)) {
        rock_content.push_back(line);
        max_length = (std::max)(max_length, line.length());
        }
    std::map<std::size_t, std::map<std::size_t, std::size_t>> rock_precomputed;

    // Get the 25 first prime numbers
    std::vector<std::size_t> primes = compute_prime_numbers(25);

    // Work over input file
    std::size_t n_cases, start, end;
    file >> n_cases;
    const clock_t begin_time = clock();
    while(file >> start >> end ) {
        // Count factors for the rock_numbers indicated in the input file
        std::map<std::size_t, std::size_t> counter;
        auto it = rock_content.begin() + start;
        auto it_end = rock_content.begin() + end;
        for ( ; it!=it_end; ++it, ++start) {
            auto inserted = rock_precomputed.insert(std::make_pair(start, std::map<std::size_t, std::size_t>()));
            if (inserted.second) {
                boost::multiprecision::cpp_int z(*it); // Create big number
                auto check = count_factors(z, primes, inserted.first->second);
                if (!check) {
                    std::cout << "Number " << z << " is not divisible by factors!" << std::endl;
                    }
                }
            sum_counts(counter, inserted.first->second);
            }
        // Get most common factors and print.
        auto max_count = std::max_element(counter.begin(), counter.end(), [](const std::pair<std::size_t, std::size_t>& lhs, const std::pair<std::size_t, std::size_t>& rhs){ return lhs.second < rhs.second;});
        std::cout << max_count->second;
        for (auto it = counter.begin(); it!=counter.end(); ++it) {
            if (it->second == max_count->second) {
                std::cout << " " << it->first;
                }
            }
        std::cout << std::endl;
        }

    //std::cout << float( clock () - begin_time ) /  CLOCKS_PER_SEC;
    return 0;
} 