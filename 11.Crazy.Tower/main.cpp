
#include <iostream>
#include <sstream>
#include <iterator>

/* Tuenti Challenge #11: Crazy Tower
*
*/

#define VERBOSE 1

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

    // Read cases
    std::vector<std::size_t> cases;
    std::size_t scenario;
    while( file >> scenario) {
        cases.push_back(scenario);
        }

    const std::size_t modulo = 1000000007;

    // Read scenarios up to the one we are going to solve.
    auto scenarios = parse_scenarios("scenarios.txt", *std::max_element(cases.begin(), cases.end()));
 
    solve_scenario(scenarios[9], modulo); return 0; // To debug one case.

    // Parse input cases
    for (auto it = cases.begin(); it!=cases.end(); ++it) {
        std::cout << "Scenario " << (*it) << ": " << std::flush << solve_scenario(scenarios[(*it)], modulo) << std::endl;
        }

    return 0;
    }



