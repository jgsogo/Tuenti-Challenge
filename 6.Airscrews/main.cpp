

#include <iostream>
#include <fstream>
#include <stdint.h>
#include <vector>
#include <sstream>
#include <assert.h>

/* Tuenti Challenge #6: Airscrews
*   - We have a MxN matrix (let's keep it generic, but being {M,N}<=5000)
*/

#define MAX_SIZE ((uint16_t)5000)

struct Data {
    uint16_t* data;
    long rows, cols;
    uint16_t col_min, row_min, col_max, row_max;

    Data() {
        data = (uint16_t*)(malloc (sizeof(int) *MAX_SIZE*MAX_SIZE));
        col_min = row_min = 0;
        col_max = row_max = MAX_SIZE;
        }
    ~Data() {
        free(data);
        };
    void set_limits(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
        col_min = x0; row_min = y0; col_max = x1; row_max = y1;
        };
    int parse_file(const std::string& file) {
        std::ifstream datafile(file);
        if (!datafile.is_open()) {
            return -1;
            }
        std::string line;
        std::getline(datafile, line);
        std::stringstream stream(line);
        stream >> rows >> cols;
        for (auto row = 0; row < rows; ++row) {
            std::getline(datafile, line);
            if (row >= row_min && row <= row_max) {
                std::stringstream stream(line);
                std::size_t col = 0;
                while (!stream.eof()) {
                    stream >> data[row*cols + col++];
                    }
                }
            else if (row > row_max) {
                break;
                }
            }
        };
    uint16_t get_value(uint16_t row, uint16_t col) {
        assert(row >= row_min); assert(row <= row_max);
        assert(col >= col_min); assert(col <= col_max);
        return data[row*cols + col];
        };
    long get_sum(uint16_t row0, uint16_t col0, uint16_t k) {
        assert(row0 >=row_min);
        assert(row0+k <= row_max);
        assert(col0 >=col_min);
        assert(col0+k <= col_max);
        long sum = 0;
        for (auto r = 0; r<k; ++r) {
            for (auto c=0; c<k; ++c) {
                sum += get_value(row0+r, col0+c);
                }
            }
        return sum;
        }
    };

struct Case {
    uint16_t col0, row0, col1, row1, k;
    };

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

    // Read cases
    std::vector<Case> cases;
    uint16_t n_cases, y0_min, x0_min, y1_max, x1_max, k;
    y0_min = MAX_SIZE;
    x0_min = MAX_SIZE;
    y1_max = 0;
    x1_max = 0;
    file >> n_cases;
    for (auto i = 0; i<n_cases; ++i) {
        auto it = cases.insert(cases.end(), Case());
        file >> it->row0 >> it->col0 >> it->row1 >> it->col1 >> it->k;
        y0_min = (std::min)(y0_min, it->row0);
        x0_min = (std::min)(x0_min, it->col0);
        y1_max = (std::max)(y1_max, it->row1);
        x1_max = (std::max)(x1_max, it->col1);
        }
    
    // Read sheet.data
    Data data;
    data.set_limits(x0_min, y0_min, x1_max, y1_max);
    data.parse_file("sheet.data");

    // And now start searching

    return 0;
} 