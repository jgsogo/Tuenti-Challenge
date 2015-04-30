
#include <iostream>

/* Tuenti Challenge #5: The One Treasure
*
*   Path search in a graph. Graph is not big enough to use heuristics (max edges = 10000),
*   so we will search the optimal solution using a depth-first-search algorithm over the
*   graph with two optimizations:
*       - search space constrained (some nodes may be deleted from the graph) [disabled]
*       - best path from each node is cached.
*/

#include "Island.hpp"
#include "World.hpp"


// Main program: read file from argument
int main (int argc, char *argv[]) {
    std::string filename = "testInput.sql";
    if (argc == 2) {
        filename = argv[1]; // Override filename if this argument exists.
        }
      
    // Create world        
    World world;
    World::parse(filename, world);
    // Compute constraints related to search space
    std::size_t max_steps = world.constraint_search_space();
    // Search path for my boat
    pBoat me = *world.boats.begin();
    std::vector<pIsland> path;
    std::pair<World::Path, long> best = world.search(me->gold+me->position->cost, me->position, 0, max_steps, world.islands["Raftel"]);
    /*
    std::cout << "Path: ";
    for(auto it = best.first.begin(); it!=best.first.end(); ++it) {
        std::cout << " " << (*it)->id;
        }
    std::cout << std::endl;
    */
    std::cout << best.second << std::endl;
    return 0;
    }



