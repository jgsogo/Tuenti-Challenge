
#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "Room.hpp"


struct Scenario {
    std::size_t id;
    std::size_t stamina;

    std::unordered_map<room_id, pRoom> rooms;


    Scenario(const std::size_t& id) : id(id) {};
    };


std::vector<Scenario> parse_scenarios(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        std::cerr << "Cannot parse scenarios file: '" << filename << "'" << std::endl;
        exit(-1);
        }

    std::vector<Scenario> ret;

    std::string line;
    std::getline(file, line);
    auto n_scenarios = std::stoi(line.c_str());
    ret.reserve(n_scenarios);

    auto i_scenario = 0;
    while(std::getline(file, line) && i_scenario++<n_scenarios) {
        auto it =  ret.insert(ret.end(), Scenario(i_scenario));
        
        // Parse scenario i
        std::size_t n_rooms;
        std::stringstream ss(line);
        ss >> it->stamina >> n_rooms;

        // Parse rooms
        std::size_t n_doors, n_keys, stamina;
        std::string name;
        auto i_room = 0;
        while (std::getline(file, line) && --n_rooms>0) {
            std::istringstream ss_room(line);
            ss_room >> name >> n_doors;
            auto it_room = it->rooms.insert(std::make_pair(name, std::make_shared<Room>(name))).first->second;
            
            while(std::getline(file, line) && --n_doors>0) {
                std::istringstream ss_door(line);
                ss_door >> name >> n_keys >> stamina;
                auto it_room_2 = it->rooms.insert(std::make_pair(name, std::make_shared<Room>(name))).first->second;

                // Build stairs
                auto stairs = std::make_shared<Stairs>();
                stairs->stamina = stamina;
                stairs->keys = n_keys;

                // Connect
                it_room->downstairs.insert(std::make_pair(it_room_2->id, stairs));
                it_room_2->upstairs.insert(std::make_pair(it_room->id, stairs));
                }
            }
        }



    return ret;
    };

