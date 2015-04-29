
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>


// Structs to store data about the world
struct Connection;
typedef std::shared_ptr<Connection> pConnection;

struct Island {
    std::string id;
    std::size_t cost;
    std::vector<pConnection> outgoing;
    std::vector<pConnection> incoming;
    void order_connections();
    };
typedef std::shared_ptr<Island> pIsland;

struct Connection {
    pIsland start;
    pIsland end;
    std::size_t cost;
    };


struct Boat {
    char id;
    std::string label;
    std::size_t gold;

    pIsland position;
    };
typedef std::shared_ptr<Boat> pBoat;

struct World {
    std::map<std::string, pIsland> islands;
    std::vector<pBoat> boats;
    static int parse(const std::string& filename, World& world);
    std::size_t constraint_search_space();
    std::vector<pIsland> compute_deterministic_path(pIsland end, pBoat);
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

std::size_t World::constraint_search_space() {
    // Automated boats have a deterministic path, it tell us about:
    //  - max_number of steps my boat can spend (I want to reach first)
    for (auto iboat = boats.begin()+1; iboat!=boats.end(); ++iboat) { // We are guaranteed to have at least one boat
        auto path = compute_deterministic_path(islands["Raftel"], *iboat);
        std::cout << "Path for '" << (*iboat)->label << "': ";
        for (auto island: path) {
            std::cout << island->id << " ";
            }
        std::cout << std::endl;
        }
    }

std::vector<pIsland> World::compute_deterministic_path(pIsland end, pBoat boat) {
    std::vector<pIsland> islands;
    islands.push_back(boat->position);
    // Deterministic boats always choose the cheapest connection (odds) or viceversa
    bool is_odd_boat = (boat->id %2 == 1);
    while (boat->position != end) {
        if (is_odd_boat) {
            boat->position = (*boat->position->outgoing.begin())->end;
            }
        else {
            boat->position = (*boat->position->outgoing.rbegin())->end;
            }
        islands.push_back(boat->position);
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
    std::string id1, id2;
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
        file >> id1 >> id2 >> con->cost;
        con->start = world.islands[id1];
        con->end = world.islands[id2];
        con->start->outgoing.push_back(con);
        con->end->incoming.push_back(con);
        }
    // - Read boats
    file >> lines;
    for (auto i = 0; i<lines; ++i) {
        pBoat boat = std::make_shared<Boat>();
        file >> boat->id >> boat->label >> boat->gold >> id1;
        boat->position = world.islands[id1];
        world.boats.push_back(boat);
        }
    file.close();
    }
