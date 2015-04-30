
#pragma once

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <fstream>

#include "Island.hpp"


struct Boat {
    short int id;
    std::string label;
    long gold;
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
    typedef std::vector<pIsland> Path;
    std::map<std::string, pIsland> islands;
    std::vector<pBoat> boats;
    static int parse(const std::string& filename, World& world);
    std::size_t constraint_search_space();
    std::vector<pIsland> compute_pirates_path(pIsland end, pBoat);
    std::map<IslandCacheKey, std::pair<Path, long>> best_option_cache;
    void compute_distances_to(pIsland end, std::size_t step);
    void compute_distances_from(pIsland start, std::size_t step);
    std::pair<Path, long> search(long gold, pIsland current, std::size_t step, std::size_t max_steps, pIsland end);
    };

/*********** Implementation **************/
std::size_t World::constraint_search_space() {
    // Automated boats have a deterministic path, it tell us about:
    //  - max_number of steps my boat can spend (I want to reach first)
    //  - cost of staying at an island at any giving step.
    std::size_t max_steps = std::numeric_limits<std::size_t>::max();
    for (auto iboat = boats.begin()+1; iboat!=boats.end(); ++iboat) { // We are guaranteed to have at least one boat
        auto path = compute_pirates_path(islands["Raftel"], *iboat);
        max_steps = (std::min)(max_steps, path.size());
        /*
        std::cout << "Path for '" << (*iboat)->label << "': ";
        for (auto step = path.begin(); step!=path.end(); ++step) {
            std::cout << (*step)->id << " ";
            }
        std::cout << std::endl;
        */
        }
    max_steps -= 1; // No need to reach the starting island ;D
    
    // Distance from any given island to Raftel.
    compute_distances_to(islands["Raftel"], 0);
    
    #ifdef OPTIMIZE_GRAPH
    // Distance to any island from the starting point
    compute_distances_from( boats.begin()->get()->position, 0);
    
    //  If min-distance from start plus min-distance to end island is greater
    //  than max_steps, then this island won't ever be part of any solution.
    for (auto it = islands.begin(); it!=islands.end(); ) {
        if ( (it->second->distance_to_start + it->second->distance_to_end) > max_steps) {
            it->second->disconnect();
            islands.erase(it++);
            }
        else {
            ++it;
            }
        }    
    #endif
    return max_steps;
    }

std::pair<World::Path, long> World::search(long gold, pIsland current, std::size_t step, std::size_t max_steps, pIsland end) {
    // DFS-algorithm
    std::pair<Path, long> best_option = std::make_pair(Path(), std::numeric_limits<long>::min());
    //std::vector<pIsland> best_path;
    if (!current->visited) {
        // Is is cached?
        auto it_cache = best_option_cache.insert(std::make_pair(IslandCacheKey(current->id, step, gold), std::make_pair(Path(), 0L)));
        if (!it_cache.second) {
            return it_cache.first->second;
            }

        // Explore this path        
        long gold_remaining = (std::max)(0L, gold - current->get_cost(step));
        if (current == end) {   // Already at the end island?
            best_option.second = gold_remaining;
            }
        else {
            // Search best path from my position to the end island.
            if (current->distance_to_end + step <= max_steps) { // If it is possible to arrive to the end.
                // - Option 1) Pillage
                if (current->distance_to_end + step < max_steps) {
                    auto option_ret = search(gold_remaining+10+current->cost, current, step+1, max_steps, end);
                    if (option_ret.second > best_option.second) {
                        best_option = option_ret;
                        }
                    }
                // - Option 2) Move to any of the next islands
                for (auto it = current->outgoing.begin(); it!=current->outgoing.end(); ++it) {
                    current->visited = true;
                    if (gold_remaining!=0/* && (gold_remaining - (*it)->cost) >= 0*/) { //! TODO: Do I have enough gold to travel this route? 
                        auto option_ret = search((std::max)(0L, gold_remaining - (*it)->cost), (*it)->end, step+1, max_steps, end);
                        if (option_ret.second > best_option.second) {
                            best_option = option_ret;
                            }
                        }
                    current->visited = false;
                    }
                }
            }
        best_option.first.insert(best_option.first.begin(), current);
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

    for (auto it = world.islands.begin(); it!=world.islands.end(); ++it) {
        it->second->order_connections();
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
