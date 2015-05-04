
#include <iostream>
#include <sstream>
#include <iterator>

/* Tuenti Challenge #11: Crazy Tower
*
*/

#define VERBOSE 0

#include "Room.hpp"
#include "Scenario.hpp"

#include <boost/thread/thread.hpp>


void solve_scenario(Scenario& scenario, const std::size_t& modulo, std::uint32_t& solution) {
    solution = scenario.solve(modulo);
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
    std::vector<std::pair<std::size_t, std::uint32_t>> cases;
    std::size_t scenario;
    std::size_t max_scenario = 0;
    while( file >> scenario) {
        cases.push_back(std::make_pair(scenario, 0));
        max_scenario = std::max(max_scenario, scenario);
        }

    const std::size_t modulo = 1000000007;

    // Read scenarios up to the one we are going to solve.
    auto scenarios = parse_scenarios("scenarios.txt", max_scenario+1);
 
    // Parallelize
    std::vector<boost::thread*> threads;
    std::size_t init_case = 0;
    for (auto i = init_case; i<max_scenario+1;++i) {
        auto job = new boost::thread(solve_scenario, scenarios[i], modulo, boost::ref(cases[i].second));
        threads.push_back(job);
        }

    // Wait threads to join and dump (in order)
    auto it_case = cases.begin()+init_case;
    for (auto it = threads.begin(); it!=threads.end(); ++it, ++it_case) {
        (*it)->join();
        std::cout << "Scenario " << it_case->first << ": " << std::flush << it_case->second << std::endl;
        }
    return 0;
    }



