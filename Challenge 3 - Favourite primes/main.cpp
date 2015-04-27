

#include <iostream>
#include <fstream>

#include "xz.h"  //! TODO: XZ embedded in project http://tukaani.org/xz/embedded.html

void log(const std::string& msg) {
    std::cout << msg << std::endl;
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
    
    //XZ format: http://tukaani.org/xz/format.html
    std::string numbers_file = "numbers.txt.xz";
    if (argc >= 3) {
        numbers_file = argv[2]; // Override numbers_file if this argument exists.
        }
    std::ifstream infile(numbers_file, std::ifstream::binary);
    if (!infile) {
        return -1;
        }
    xz_buf buffer;
    infile.seekg (0, infile.end);
    buffer.in_size = infile.tellg();
    infile.seekg (0, infile.beg);
    buffer.in_pos = 0;
    
    // read data as a block:
    unsigned char buffer_stream[buffer.in_size];
    infile.read((char*)(&buffer_stream[0]), buffer.in_size);
    if (!infile) {
        std::cout << "error: only " << infile.gcount() << " could be read";
        return -1;
        }
    infile.close();
    std::cout << "File content (size=" << buffer.in_size << "): " << buffer_stream << std::endl;

    buffer.in = &buffer_stream[0];
    buffer.out = new uint8_t[100];
    buffer.out_pos = 0;
    buffer.out_size = 100;

    xz_dec* handle = xz_dec_init(XZ_SINGLE, 0);
    std::cout << "XZ handle: " << handle << std::endl;
    xz_ret r = xz_dec_run(handle, &buffer);

    std::cout << "RRRRRR: " << r << std::endl;
    std::cout << "buffer(" << buffer.out_pos << "): " << buffer.out << std::endl;

    std::size_t n_cases, start, end;
    file >> n_cases;
    while(file >> start >> end ) {
        //std::cout << start << " " << end << std::endl;
        }

    return 0;
} 