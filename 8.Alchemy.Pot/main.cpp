
#include <iostream>
#include <sstream>
#include <iterator>
#include <fstream>

#include "RecipeBook.hpp"
#include "RecipeTree.hpp"

/* Tuenti Challenge #8: Alchemy Pot
*
*/


// Main program: read file from argument
int main (int argc, char *argv[]) {
    std::string filename = "testInput.sql";
    if (argc == 2) {
        filename = argv[1]; // Override filename if this argument exists.
        }
      
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        std::cerr << "Cannot read Input file: '" << filename << "'" << std::endl;
        return -1;
        }

    
    // Get the recipe book!
    RecipeBook book("book.data");
    book.read();
    // Build the search tree
    RecipeTree tree(book);
    tree.build();

    // Read cases from input file
    std::size_t n_cases;
    std::string line;
    std::getline(file, line); // n_cases
    while(std::getline(file, line)) {
        std::stringstream ss(line);
        std::vector<std::size_t> icase = book.parse_ingredients(line);

        std::cout << std::endl << "----------------------------" << std::endl;
        std::cout << line << std::endl << "Initial items:";
        for (auto i = icase.begin(); i!=icase.end(); ++i) {
            std::cout << " " << *i;
            }
        std::cout << std::endl;

        // Work on this case-inventory
        auto options = tree.retrieve(icase);
        for (auto opt = options.begin(); opt!=options.end(); ++opt) {
            auto opt_value = book.value(*opt);
            std::cout << "\t[" << opt_value << "]";
            for (auto item = opt->begin(); item!=opt->end(); ++item) {
                std::cout << " " << *item;
                }
            std::cout << std::endl;
            }
        }    

    return 0;
    }
