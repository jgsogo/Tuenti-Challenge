
#include <iostream>
#include <sstream>
#include <iterator>
#include <fstream>

//#define DEBUG // Define this preprocessor to write debug info to console

#include "RecipeBook.hpp"
#include "RecipeTree.hpp"

/* Tuenti Challenge #8: Alchemy Pot
*
*   Optimizations
*   - Build an indexed search tree for recipes
*   - Cache partial results
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
        t_ingredients icase = book.parse_ingredients(line);

        #ifdef DEBUG
        std::cout << std::endl << "----------------------------" << std::endl;
        std::cout << line << std::endl << "Initial items:";
        for (auto i = icase.begin(); i!=icase.end(); ++i) {
            std::cout << " " << *i;
            }
        std::cout << "\t [" << book.translate_ingredients(icase) << "]" << std::endl;
        #endif

        // Work on this case-inventory
        std::vector<t_ingredients> options;
        options.push_back(icase);
        tree.retrieve(icase, options);
        
        size_t best_option = 0;
        for (auto opt = options.begin(); opt!=options.end(); ++opt) {
            auto opt_value = book.value(*opt);
            best_option = (std::max)(best_option, opt_value);
            #ifdef DEBUG
            std::cout << std::endl << "\t[" << opt_value << "]";
            for (auto item = opt->begin(); item!=opt->end(); ++item) {
                std::cout << " " << *item;
                }
            std::cout << "\t [" << book.translate_ingredients(*opt) << "]" << std::endl;
            #endif
            }
        std::cout << best_option << std::endl;
        }    

    return 0;
    }
