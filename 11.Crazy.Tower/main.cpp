
#include <iostream>
#include <sstream>
#include <iterator>

/* Tuenti Challenge #7: The Perfect Larry Matching Algorithm
*
*/

#include "Room.hpp"
#include "Scenario.hpp"

// Main program: read file from argument
int main (int argc, char *argv[]) {
    std::string filename = "testInput.sql";
    if (argc == 2) {
        filename = argv[1]; // Override filename if this argument exists.
        }
      
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        return -1;
        }



    return 0;
    }



