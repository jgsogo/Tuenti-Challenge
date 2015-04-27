

#include <iostream>
#include <fstream>
#include <cstdint>

// Compute buffer based on spots available.
uint32_t compute_buffer(const uint32_t& case_i) {
    return (case_i >> 1) + (case_i & 1);    // division by 2 (+1 if odd number).
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
    uint32_t n_cases, case_i;   // Possible values according to the challenge: 1 <= N <= 2^32-1
                                //  we don't expect more that 2^32 cases.
    file >> n_cases;
    std::ofstream ofs ("output.txt", std::ofstream::out);
    while(file >> case_i) {
        ofs << compute_buffer(case_i) << std::endl;
        //std::cout << case_i << " - " << compute_buffer(case_i) << " (" << case_i/2.0 << ")" << std::endl;
    }
    return 0;
}