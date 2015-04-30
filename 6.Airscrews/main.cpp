

#include <iostream>
#include <fstream>
#include <stdint.h>
#include <vector>
#include <sstream>
#include <assert.h>
#include <map>
#include <numeric>

/* Tuenti Challenge #6: Airscrews
*   - We have a MxN matrix (let's keep it generic, but being {M,N}<=5000)
*
*   Optimizations:
*       - Read just the data needed (it is passed in Data::set_limits) [TODO]
*       - Precompute sums of a given size.
*/

#define MAX_SIZE ((uint16_t)5000)

struct DataKey {
    uint16_t row, col, k;
    DataKey(uint16_t row, uint16_t col, uint16_t k) : row(row), col(col), k(k) {};
    bool operator<(const DataKey& other) const {
        return ((row < other.row) || (row==other.row && col<other.col) || (row==other.row && col==other.col && k<other.k));
        };
    bool operator==(const DataKey& other) const {
        return (row == other.row) && (col==other.col) && (k==other.k);
        };
    };

struct Data {
    uint16_t* data;
    long rows, cols; // Total size of data matrix
    uint16_t col_min, row_min, col_max, row_max; // Area of interest regarding our cases.
    // Cache data
    std::map<DataKey, uint64_t> cached;
    std::size_t cache_size;

    Data() {
        data = (uint16_t*)(malloc (sizeof(int) *MAX_SIZE*MAX_SIZE));
        col_min = row_min = 0;
        col_max = row_max = MAX_SIZE;
        cache_size = 0;
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
            // TODO: Advance to row_min
            std::getline(datafile, line);
            //if (row >= row_min && row <= row_max) {
                std::stringstream stream(line);
                std::size_t col = 0;
                // TODO: Read only columns of interest (¡optimization!)
                while (!stream.eof()) {
                    stream >> data[row*cols + col++];
                    }
            //    }
            //else if (row > row_max) {
            //    break;
            //    }
            }
        };
    void precompute_cache(const std::size_t& size = 5) {
        assert(rows%size == 0);
        assert(cols%size == 0);
        cache_size = size;
        cached.clear();        
        for (auto r=0; r<rows/size; r+=size) {
            for (auto c=0; c<cols/size; c+=size) {
                uint64_t sum = 0;
                for (auto i=0; i<size; ++i) {
                    sum += this->get_value(r+i, c, c+size);
                    }                
                cached.insert(std::make_pair(DataKey(r, c, size), sum));
                }
            }
        };
    uint16_t get_value(uint16_t row, uint16_t col) const {
        return data[row*cols + col];
        };
    uint64_t get_value(uint16_t row, uint16_t col_init, uint16_t col_end) const {
        return std::accumulate( &data[row*cols + col_init], &data[row*cols + col_end], 0, std::plus<uint64_t>());
        };
    uint64_t get_sum(uint16_t row0, uint16_t col0, uint16_t k) const {
        auto r_up =  (row0%cache_size) ? cache_size-row0%cache_size : 0;
        auto r_down =  ((row0+k)%cache_size) ? cache_size-(row0+k)%cache_size : 0;

        auto r_left =  (col0%cache_size) ? cache_size-col0%cache_size : 0;
        auto r_right =  ((col0+k)%cache_size) ? cache_size-(col0+k)%cache_size : 0;

        uint64_t sum = 0;
        std::cout << "row0 = " << row0 << ", col0 = " << col0 << ", k = " << k << ", r_up = " << r_up << ", r_down = " << r_down << ", r_left = " << r_left << ", r_right = " << r_right << std::endl;
        // Rows not fitting with cache chunks (up)
        std::cout << "\t for(" << row0 << "; " << row0+r_up << "; ++1) ==> Suma todas las columnas (arriba)" << std::endl;
        for (auto ir = row0; ir<row0+r_up; ++ir) {
            std::cout << ".";
            sum += get_value(ir, col0, col0+k);
            }
        std::cout << "/+" << std::endl;
        std::cout << "\t for(" << row0+k-r_down << "; " << row0+k << "; ++1) ==> Suma todas las columnas (abajo)" << std::endl;
        // Rows not fitting with cache chunks (down)
        for (auto ir = row0+k-r_down; ir<row0+k; ++ir) {
            std::cout << ".";
            sum += get_value(ir, col0, col0+k);
            }
        std::cout << "/+" << std::endl;
        std::cout << "\t for(" << row0+r_up << "; " << row0+k-r_down << "; ++1) ==> Suma los trozos a derecha e izquierda" << std::endl;
        // Cols not fitting with cache chunks (left + right)
        for (auto ir = row0+r_up; ir < row0+k-r_down; ++ir) {
            std::cout << "." << std::flush;
            sum += get_value(ir, col0, col0+r_left);
            std::cout << "." << std::flush;
            sum += get_value(ir, col0+k-r_right, col0+k);
            }
        std::cout << "/+" << std::endl;
        // Cache chunks
        std::cout << "\t for(" << row0+r_up << "; " << row0+k-r_down << "; +=" << cache_size << ")" << std::endl;
        std::cout << "\t for(" << col0+r_left << "; " << col0+k-r_right << "; +=" << cache_size << ")" << std::endl;
        for (auto ir = row0+r_up; ir < row0+k-r_down; ir+=cache_size) {
            for (auto ic = col0+r_left; ic<col0+k-r_right; ic+=cache_size) {
                std::cout << ".";
                sum += cached.find(DataKey(ir, ic, cache_size))->second;
                }
            }
        std::cout << "/+" << std::endl;
        return sum;
        };
    /*
    uint64_t get_sum(uint16_t row0, uint16_t col0, uint16_t k) const {
        auto it = cached.insert(std::make_pair(DataKey(row0, col0, k), uint64_t() ));
        if (!it.second) {
            return it.first->second;
            }
        assert(row0 >=row_min);
        assert(row0+k <= row_max);
        assert(col0 >=col_min);
        assert(col0+k <= col_max);
        uint64_t sum = 0;
        for (auto r = 0; r<k; ++r) {
            sum += get_value(row0+r, col0, col0+k);
            //for (auto c=0; c<k; ++c) {
            //    sum += get_value(row0+r, col0+c);
            //    }
            }

        it.first->second = sum;
        return sum;
        }
    */
    };
    

struct Case {
    uint16_t col0, row0, col1, row1, k;
    uint64_t compute(const Data& data);
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
    
    // Read sheet.data (only relevant one)
    Data data;
    data.set_limits(x0_min, y0_min, x1_max, y1_max);
    data.parse_file("sheet.data");
    data.precompute_cache();

    // And now start searching
    int i = 1;
    for (auto it = cases.begin(); it != cases.end(); ++it) {
        std::cout << "Case " << i++ << ": " << it->compute(data) << std::endl;
        }
    return 0;
} 


uint64_t Case::compute(const Data& data) {
    // flexibility
    uint16_t rows_remain = (row1 - row0) - k*2 - 1;
    uint16_t cols_remain = (col1 - col0) - k*2 - 1;

    uint64_t best_so_far = 0;
    for (auto r = 0; r<=rows_remain; ++r) {
        for (auto c = 0; c<=cols_remain; ++c) {
            auto aspa1 = data.get_sum(row0+r, col0+c, k);
            auto aspa2 = data.get_sum(row0+r+k+1, col0+c+k+1, k);
            best_so_far = (std::max)(best_so_far, aspa1+aspa2);
            }
        }
    return best_so_far;
    }