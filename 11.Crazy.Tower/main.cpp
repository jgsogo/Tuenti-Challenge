
#include <iostream>
#include <sstream>
#include <iterator>

/* Tuenti Challenge #11: Crazy Tower
*
*/

#include "Room.hpp"
#include "Scenario.hpp"


std::size_t solve_scenario(Scenario& scenario, const std::size_t& modulo) {
    return scenario.solve(modulo);
    };


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

    // Read scenarios
    auto scenarios = parse_scenarios("scenarios.txt");

    const std::size_t modulo = 1000000007;

    // Parse input cases
    std::size_t scenario;
    while( file >> scenario) {
        std::cout << "Scenario " << scenario << ": " << std::flush << solve_scenario(scenarios[scenario], modulo) << std::endl;
        }

    return 0;
    }



