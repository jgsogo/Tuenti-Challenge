
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <sstream>
#include <cassert>

/* Tuenti Challenge #5: The One Treasure
*
*   Path search in a graph. Graph is not big enough to use heuristics (max edges = 10000),
*   so we will search the optimal solution using a depth-first-search algorithm over the
*   graph with two optimizations:
*       - search space constrained (some nodes may be deleted from the graph)
*       - best path from each node is cached.
*/

// Structs to store data about the world
struct Connection;
typedef std::shared_ptr<Connection> pConnection;

struct Island {
    std::string id;
    long cost;
    std::vector<pConnection> outgoing;
    std::vector<pConnection> incoming;
    std::map<std::size_t, long> step_cost;
    std::size_t distance_to_end;
    std::size_t distance_to_start;
    bool visited;
    Island() {
        distance_to_end = std::numeric_limits<std::size_t>::max();
        distance_to_start = std::numeric_limits<std::size_t>::max();
        visited = false;
        };
    long get_cost(const std::size_t& step) {
        // At each step the cost of visiting an island is Island::cost, except if there are pirates arriving.
        auto it = step_cost.find(step);
        return (it != step_cost.end()) ? cost + it->second : cost;
        };
    void order_connections();
    void disconnect();
    };
typedef std::shared_ptr<Island> pIsland;

struct Connection {
    pIsland start;
    pIsland end;
    long cost;
    };


struct Boat {
    char id;
    std::string label;
    std::size_t gold;

    pIsland position;
    };
typedef std::shared_ptr<Boat> pBoat;

struct IslandCacheKey {
    std::string id;
    std::size_t step;
    long gold_remaining;
    IslandCacheKey(const std::string& id, const std::size_t& step, const long& gold) : id(id), step(step), gold_remaining(gold) {};
    bool operator<(const IslandCacheKey& other) const {
        return ((id < other.id) || (id==other.id && step<other.step) || (id==other.id && step==other.step && gold_remaining<other.gold_remaining));
        };
    };

struct World {
    std::map<std::string, pIsland> islands;
    std::vector<pBoat> boats;
    static int parse(const std::string& filename, World& world);
    std::size_t constraint_search_space();
    std::vector<pIsland> compute_pirates_path(pIsland end, pBoat);
    std::map<IslandCacheKey, long> best_option_cache;
    void compute_distances_to(pIsland end, std::size_t step);
    void compute_distances_from(pIsland start, std::size_t step);
    long search(long gold, pIsland current, std::size_t step, std::size_t max_steps, pIsland end);
    };


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
    long gold = world.search(me->gold+me->position->cost, me->position, 0, max_steps, world.islands["Raftel"]);
    std::cout << gold << std::endl;
    return 0;
    }


void Island::order_connections() {
    // Order connections from lower cost to higher cost
    auto order_connections = [](const pConnection& lhs, const pConnection& rhs) {
        return lhs->cost < rhs->cost;
        };
    std::sort(outgoing.begin(), outgoing.end(), order_connections);
    std::sort(incoming.begin(), incoming.end(), order_connections);
    }
void Island::disconnect() {
    // Delete all connections and references to adjacent islands.
    for (auto it = outgoing.begin(); it!=outgoing.end(); ++it) {
        std::remove((*it)->end->incoming.begin(), (*it)->end->incoming.end(), *it);
        }
    for (auto it = incoming.begin(); it!=incoming.end(); ++it) {
        std::remove((*it)->start->outgoing.begin(), (*it)->start->outgoing.end(), *it);
        }
    outgoing.clear();
    incoming.clear();
    }


std::size_t World::constraint_search_space() {
    // Automated boats have a deterministic path, it tell us about:
    //  - max_number of steps my boat can spend (I want to reach first)
    //  - cost of staying at an island at any giving step.
    std::size_t max_steps = std::numeric_limits<std::size_t>::max();
    for (auto iboat = boats.begin()+1; iboat!=boats.end(); ++iboat) { // We are guaranteed to have at least one boat
        auto path = compute_pirates_path(islands["Raftel"], *iboat);
        max_steps = (std::min)(max_steps, path.size());
        //std::cout << "Path for '" << (*iboat)->label << "': ";
        //for (auto step = path.begin(); step!=path.end(); ++step) {
        //    std::cout << (*step)->id << " ";
        //    }
        //std::cout << std::endl;
        }
    max_steps -= 1; // No need to reach the starting island ;D
    // Distance from any given island to Raftel.
    compute_distances_to(islands["Raftel"], 0);
    // Distance to any island from the starting point
    compute_distances_from( boats.begin()->get()->position, 0);
    //  If min-distance from start plus min-distance to end island is greater
    //  than max_steps, then this island can't be included in the solution.
    for (auto it = islands.begin(); it!=islands.end(); ) {
        if ( (it->second->distance_to_start + it->second->distance_to_end) > max_steps) {
            it->second->disconnect();
            islands.erase(it++);
            }
        else {
            ++it;
            }
        }    
    return max_steps;
    }

long World::search(long gold, pIsland current, std::size_t step, std::size_t max_steps, pIsland end) {
    // DFS-algorithm
    long best_option = std::numeric_limits<long>::min();
    if (!current->visited) {
        // Is is cached?
        auto it_cache = best_option_cache.insert(std::make_pair(IslandCacheKey(current->id, step, gold), 0));
        if (!it_cache.second) {
            return it_cache.first->second;
            }

        // Explore this path        
        long gold_remaining = (std::max)(0L, gold - current->get_cost(step));
        if (current == end) {   // Already at the end island?
            best_option = gold_remaining;
            }
        else {
            // Search best path from my position to the end island.
            if (current->distance_to_end + step <= max_steps) { // If it is possible to arrive to the end.
                // - Option 1) Pillage
                if (current->distance_to_end + step < max_steps) {
                    best_option = (std::max)(best_option, search(gold_remaining+10, current, step+1, max_steps, end));
                    }
                // - Option 2) Move to any of the next islands
                for (auto it = current->outgoing.begin(); it!=current->outgoing.end(); ++it) {
                    current->visited = true;
                    if (gold_remaining!=0/* && (gold_remaining - (*it)->cost) >= 0*/) { //! TODO: Do I have enough gold to travel this route? 
                        auto option_gold = search((std::max)(0L, gold_remaining - (*it)->cost), (*it)->end, step+1, max_steps, end);
                        best_option = (std::max)(best_option, option_gold);
                        }
                    current->visited = false;
                    }
                }
            }

        it_cache.first->second = best_option;        
        }
    return best_option;
    }

void World::compute_distances_from(pIsland start, std::size_t step = 0) {
    // Compute distance from the starting island
    start->distance_to_start = (std::min)(start->distance_to_start, step);
    step +=1;
    for (auto it = start->outgoing.begin(); it!=start->outgoing.end(); ++it) {
        if ( (*it)->end->distance_to_start > step) {
            compute_distances_from( (*it)->end, step);
            }
        }
    }

void World::compute_distances_to(pIsland end, std::size_t step = 0) {
    // DFS strategy, we want to compute the minimum distance to end island.
    end->distance_to_end = (std::min)(end->distance_to_end, step);
    step += 1;
    for (auto it = end->incoming.begin(); it!=end->incoming.end(); ++it) {
        if ( (*it)->start->distance_to_end > step ) {
            compute_distances_to((*it)->start, step);
            }
        }
    }

std::vector<pIsland> World::compute_pirates_path(pIsland end, pBoat boat) {
    std::vector<pIsland> islands;
    islands.push_back(boat->position);
    // Deterministic boats always choose the cheapest connection (odds) or viceversa
    bool is_odd_boat = (boat->id %2 == 1);
    std::size_t step = 0;
    while (boat->position != end) {
        step++;
        if (is_odd_boat) {
            boat->position = (*boat->position->outgoing.begin())->end;
            }
        else {
            boat->position = (*boat->position->outgoing.rbegin())->end;
            }
        islands.push_back(boat->position);
        if (boat->position != end) {
            // Store in island the result of RULE#5 (Raftel doesn't penalize)
            boat->position->step_cost[step] += boat->gold;
            }
        }
    return islands;
    }

int World::parse(const std::string& filename, World& world) {
    // Read data from file
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        return -1;
        }
    std::size_t lines;
    std::string str1, str2;
    // - Read islands
    file >> lines;
    for (auto i = 0; i<lines; ++i) {
        pIsland island = std::make_shared<Island>();
        file >> island->id >> island->cost;
        world.islands.insert(std::make_pair(island->id, island));
        }
    // - Read connections
    file >> lines;
    for (auto i = 0; i<lines; ++i) {
        pConnection con = std::make_shared<Connection>();
        file >> str1 >> str2 >> con->cost;
        con->start = world.islands[str1];
        con->end = world.islands[str2];
        con->start->outgoing.push_back(con);
        con->end->incoming.push_back(con);
        }
    // - Read boats
    file >> lines;
    for (auto i = 0; i<lines; ++i) {
        pBoat boat = std::make_shared<Boat>();
        file >> (boat->id) >> (boat->label) >> (boat->gold) >> str1;
        boat->position = world.islands[str1];
        world.boats.push_back(boat);
        }
    file.close();
    }
