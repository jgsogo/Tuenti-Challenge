
#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "Room.hpp"
#include "Hero.hpp"

#if(VERBOSE)
    #define DEBUGGING(msg) msg;
#else
    #define DEBUGGING(msg) void(0);
#endif


struct Scenario {
    std::size_t id;
    std::size_t stamina;

    std::unordered_map<room_id, pRoom> rooms;

    Scenario(const std::size_t& id) : id(id) {};
    std::size_t solve(const std::size_t& modulo) {
        DEBUGGING(std::cout << std::endl << "============ Scenario " << id << " START ============" << std::endl)
        DEBUGGING(rename_rooms())
        Hero hero(stamina, stamina, modulo);
        pRoom start = rooms["start"];
        auto solutions = Hero::visit(start, hero);
        DEBUGGING(std::cout << "Found [" << solutions << "] solutions" << std::endl)
        DEBUGGING(std::cout << std::endl << "============ Scenario " << id << " END ============" << std::endl)
        return solutions;
        };
    void rename_rooms() {
        // In order to debug, its easier to have meaningful names than hashes
        std::size_t i = 0;
        for (auto it = rooms.begin(); it!=rooms.end(); ++it, i++) {
            std::stringstream ss; ss << "room " << i;
            it->second->id = ss.str();
            }
        };
    };


std::vector<Scenario> parse_scenarios(const std::string& filename, const std::size_t& max_to_read = std::numeric_limits<std::size_t>::max()) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        std::cerr << "Cannot parse scenarios file: '" << filename << "'" << std::endl;
        exit(-1);
        }

    std::vector<Scenario> ret;

    std::string line;
    std::getline(file, line);
    auto n_scenarios = std::min(max_to_read, std::stoul(line.c_str()));
    ret.reserve(n_scenarios);

    auto i_scenario = 0;
    while(i_scenario++<n_scenarios && std::getline(file, line)) {
        auto it =  ret.insert(ret.end(), Scenario(i_scenario));
        
        // Parse scenario i
        std::size_t n_rooms;
        std::stringstream ss(line);
        ss >> it->stamina >> n_rooms;

        // Parse rooms
        std::size_t n_doors, n_keys, stamina;
        std::string name;
        auto i_room = 0;
        while (n_rooms--) {
            std::getline(file, line);
            std::istringstream ss_room(line);
            ss_room >> name >> n_doors;
            auto it_room = it->rooms.insert(std::make_pair(name, std::make_shared<Room>(name))).first->second;            
            while(n_doors--) {
                std::getline(file, line);
                std::istringstream ss_door(line);
                ss_door >> name >> n_keys >> stamina;
                auto it_room_2 = it->rooms.insert(std::make_pair(name, std::make_shared<Room>(name))).first->second;

                // Build stairs
                auto stairs = std::make_shared<Stairs>();
                stairs->stamina = stamina;
                stairs->keys = n_keys;
                stairs->down = it_room_2;
                stairs->up = it_room;

                // Connect
                it_room->downstairs.push_back(stairs);
                }
            }
        //if (i_scenario > 3) {break;}
        }

    return ret;
    };


/* Print functions  */
std::ostream& operator<< (std::ostream& os, const Scenario& scenario) {
    os << scenario.stamina << " " << scenario.rooms.size() << "\n";
    for (auto it = scenario.rooms.begin(); it!=scenario.rooms.end(); ++it) {
        os << *(it->second);
        }
    return os;
    };