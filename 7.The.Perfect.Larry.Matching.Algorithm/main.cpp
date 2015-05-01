
#include <iostream>
#include <sstream>
#include <iterator>

/* Tuenti Challenge #7: The Perfect Larry Matching Algorithm
*
*/

#include "Girl.hpp"
#include "GirlVisitor.hpp"

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

    std::map<std::string, pGirl> girls;
    // Read answers and create girls
    std::size_t n_girls, n_friend_line;
    file >> n_girls >> n_friend_line;
    for (auto i = 0; i<n_girls; ++i) {
        auto girl = std::make_shared<Girl>(i);
        girl->parse_line(file);
        girls[girl->label] = girl;
        }
    // Read friendships
    std::string line;
    while(std::getline(file, line)) {
        std::stringstream ss(line);
        // use stream iterators to copy the stream to the vector as whitespace separated strings
        std::istream_iterator<std::string> it(ss);
        std::istream_iterator<std::string> end;
        std::vector<std::string> friends(it, end);
        // Create connections
        for (auto it = friends.begin(); it!=friends.end(); ++it) {
            for (auto it2 = it+1; it2!=friends.end(); ++it2 ) {
                girls[*it]->friends.insert(girls[*it2]);
                girls[*it2]->friends.insert(girls[*it]);
                }
            }
        }
    
    // Compute connected components for the friendship graph     
    GirlVisitor visitor;
    visitor.compute(girls);

    // Count points related to friends
    std::size_t best_match = 0;
    for (auto it = girls.begin(); it!=girls.end(); ++it) {
        auto points = it->second->compute_points(visitor.components, visitor.total_shoppers);
        best_match = (std::max)(best_match, points);
        }
    std::cout << best_match << std::endl;

    /*
    // End of computation - Resumee
    std::cout << std::endl << "------------------------------" << std::endl;
    for (auto it = girls.begin(); it!=girls.end(); ++it) {
        it->second->print(std::cout);
        }
    */
    return 0;
    }



