

#include <iostream>
#include <fstream>
#include <cstdint>
#include <sstream>
#include <cassert>
#include <vector>
#include <bitset>
#include <sstream>
#include <numeric>
#include <map>
#include <boost/multiprecision/cpp_int.hpp> // We need really big integers

/* Tuenti Challenge #4: A Bitter Dinner
*
*/

// Base64: snippet from http://www.adp-gmbh.ch/cpp/common/base64.html
static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

std::map<char, std::bitset<6>> build_base64_decoder(const std::string& base64chars) {
    std::map<char, std::bitset<6>> decoder;
    for(std::size_t i = 0; i<base64chars.length(); ++i) {
        decoder.insert(std::make_pair(base64chars[i], i));
        }
    return decoder;
    }

std::vector<bool> base64_decode(const std::string& encoded_string) {
    std::vector<bool> bitstring;
    std::map<char, std::bitset<6>> decode_map = build_base64_decoder(base64_chars);
    std::size_t in_len = encoded_string.size();
    std::size_t in_ = 0;
    while(in_len-- && ( encoded_string[in_] != '=')) {
        auto bits = decode_map[encoded_string[in_]]; in_++;
        for (std::size_t i=bits.size(); i>0; --i) {
            bitstring.push_back(bits.at(i-1));
            }
        }
    return bitstring;
    }

// Read length, xxx-endian and reverse from input case description
void interpret_case(const std::string& line, std::size_t& nbits, bool& little_endian, bool& reverse) {
    // Reverse
    reverse = false;
    if (line.substr(line.length()-1) == "R") {
        reverse = true;
        }
    // Little endian
    little_endian = false;
    if (line.substr(line.length()-1 - (reverse?1:0)) == "L") {
        little_endian = true;
        }
    // Number of bits
    nbits = stoi(line.substr(0, line.length() - (reverse?1:0)));
    }

// Build integer for eager CPU
boost::multiprecision::cpp_int work_case(const std::vector<bool>& bits, std::size_t& position, const std::string& line) {
    std::size_t nbits;
    bool reverse = false;
    bool little_endian = false;
    interpret_case(line, nbits, little_endian, reverse);

    // Build integer
    boost::multiprecision::cpp_int ret(0);
    
    if (!little_endian) {
        //  - big-endian
        if (reverse) {
            //  - big-endian and reverse
            for (auto i = position; i<position+nbits; ++i) {
                if (bits.at(i)) {
                    boost::multiprecision::bit_set(ret, i-position);
                    }
                }
            }
        else {
            //  - big-endian
            for (auto i = position; i<position+nbits; ++i) {
                if (bits.at(i)) {
                    boost::multiprecision::bit_set(ret, position+nbits-i-1);
                    }
                }
            }
        }
    else {
        // nbits must be multiple of 8
        auto zeros = 8 - (nbits % 8);
        std::vector<bool> data(bits.begin() + position, bits.begin() + position + nbits);
        data.insert(data.end(), zeros, false);

        // go byte by byte
        auto nbytes = nbits/8;
        std::size_t byte = 0;
        for (auto ibyte=0; ibyte<nbytes; ++ibyte) {
            for (auto ibit=0; ibit<8; ++ibit) {
                if (data[8*ibyte + 7-ibit]) {
                    boost::multiprecision::bit_set(ret, 8*ibyte + ibit);
                    }
                }
            }
        
        if (reverse) {
            }
        else {
            }
        }    
    
    
    position += nbits;
    return ret;
    }


int main (int argc, char *argv[]) {
    std::string filename = "testInput.sql";
    if (argc == 2) {
        filename = argv[1]; // Override filename if this argument exists.
        }
              
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        return -1;
        }
    std::string bitstring;
    std::size_t n_cases;

    // Parse input file
    std::string line;
    std::getline(file, bitstring);
    std::getline(file, line);
    n_cases = std::atoi(line.c_str());
        
    std::vector<bool> bits = base64_decode(bitstring);

    std::size_t position = 0;
    while (file >> line) {
        std::cout << work_case(bits, position, line) << std::endl;
        }
    
    return 0;
}
