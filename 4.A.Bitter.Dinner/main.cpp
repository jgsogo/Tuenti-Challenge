

#include <iostream>
#include <fstream>
#include <cstdint>
#include <sstream>
#include <cassert>
#include <vector>
#include <bitset>
#include <sstream>
#include <numeric>

/* Tuenti Challenge #4: A Bitter Dinner
*
*/

// Base64: snippet from http://www.adp-gmbh.ch/cpp/common/base64.html
static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_decode(std::string const& encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}
// End of Base64: snippet from http://www.adp-gmbh.ch/cpp/common/base64.html


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

long work_case(const std::vector<bool>& bits, std::size_t& position, const std::string& line) {
    std::size_t nbits;
    bool reverse = false;
    bool little_endian = false;
    interpret_case(line, nbits, little_endian, reverse);

    // Build integer
    std::vector<bool> data(bits.begin() + position, bits.begin() + position + nbits);
    long ret = std::accumulate(data.rbegin(), data.rend(), 0, [](long x, long y){return (x<<1) + y;});
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
        
    bitstring = base64_decode(bitstring);
    std::bitset<8> bits_aux;
    std::vector<bool> bits;
    for (auto it=bitstring.begin(); it!=bitstring.end(); ++it) {
        unsigned char byte = *it;
        bits_aux = byte;
        for (std::size_t i=0; i<8; ++i) {
            bits.push_back(bits_aux.at(i));
            }        
        }

    std::size_t position = 0;
    while (file >> line) {
        std::cout << work_case(bits, position, line) << std::endl;
        }
    
    return 0;
}
