
#pragma once

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <bitset>
#include <set>
#include <unordered_map>

// Structs and definitions to store data about the world
typedef std::string room_id;

struct Stairs;
typedef std::shared_ptr<Stairs> pStairs;


struct Room {
    bool is_exit;
    room_id id;
    std::unordered_map<room_id, pStairs> downstairs;
    std::unordered_map<room_id, pStairs> upstairs;
    
    Room(const room_id& id) : id(id) {
        is_exit = (id.compare("exit")==0);
        };
    };
typedef std::shared_ptr<Room> pRoom;


struct Stairs {
    int stamina;
    int keys;

    pRoom down;
    pRoom up;

    Stairs() : stamina(0), keys(0) {};
    Stairs(const Stairs& other) : stamina(other.stamina), keys(other.keys) {};

    };


/* Print functions */   
std::ostream& operator<< (std::ostream& os, const Stairs& stairs) {
    os << stairs.down->id << " " << stairs.keys << " " << stairs.stamina;
    return os;
    };
std::ostream& operator<< (std::ostream& os, const pStairs& p) {
    os << *p;
    return os;
    };

std::ostream& operator<< (std::ostream& os, const Room& room) {
    os << room.id << " " << room.downstairs.size() << "\n";
    for (auto it = room.downstairs.begin(); it!=room.downstairs.end(); ++it) {
        os << *(it->second) << "\n";
        }
    return os;
    };
std::ostream& operator<< (std::ostream& os, const pRoom& p) {
    os << *p;
    return os;
    };

