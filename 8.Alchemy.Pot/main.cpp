
#include <iostream>
#include <sstream>
#include <iterator>
#include <fstream>

#include "RecipeBook.hpp"

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
    RecipeBook recipes("book.data");
    recipes.read();

    return 0;
    }



